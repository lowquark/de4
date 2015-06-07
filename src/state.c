
#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#define getentity(D, id) (&(D)->entities[id - 1])
#define getentityid(D, e) (((e) - (D)->entities) + 1)

#define corepropdata(D, id, eid) (vector_get(&(D)->coredata, (id)) + (eid))

#ifdef _WIN32
#define aligned_alloc(align, size) _aligned_malloc(size, align)
#define aligned_free(align, size) _aligned_mfree(size, align)
#endif


static void listnode_remove(listnode_t * e)
{
	e->prev->next = e->next;
	e->next->prev = e->prev;
	e->next = e;
	e->prev = e;
}
// insert a into b's list, after b
static void listnode_insert_after(listnode_t * a, listnode_t * b)
{
	// pull it from its current list
	listnode_remove(a);

	a->prev = b;
	a->next = b->next;
	b->next = a;
	a->next->prev = a;
}
// insert a into b's list, before b
static void listnode_insert_before(listnode_t * a, listnode_t * b)
{
	// pull it from its current list
	listnode_remove(a);

	a->next = b;
	a->prev = b->prev;
	b->prev = a;
	a->prev->next = a;
}

static listnode_t * listnode_create(id_t type, size_t size)
{
	listnode_t * new_node = calloc(sizeof(listnode_t) + size, 1);
	new_node->next = new_node;
	new_node->prev = new_node;
	new_node->typeid = type;

	return new_node;
}

static void listnode_destroy(listnode_t * e)
{
	if(e->next == e)
	{
		while(e->next != e)
		{
			listnode_destroy(e->next);
		}
	}
	else
	{
		listnode_remove(e);
	}

	free(e);
}

listnode_t * listnode_find(listnode_t * first, id_t type)
{
	listnode_foreach(first, node)
	{
		if(node->typeid == type)
		{
			return node;
		}
	}

	return 0;
}


de4_State * de4_create(size_t numentities)
{
	de4_State * D = malloc(sizeof(de4_State));

	vector_init(&D->propdefs);
	vector_init(&D->corepropdefs);

	D->entity_num = numentities;
	D->last_new_entity = 0xFFFF;

	D->entities = malloc(sizeof(entity_t) * numentities);
	for(int i = 0 ; i < numentities ; i ++) 
		D->entities[i] = (entity_t){
			.properties = 0,
			.name = "entity",
			.coreflags = 0
		};

	vector_init(&D->coredata);

	D->this_entity = 0;
	D->this_system = 0;
	D->this_property = 0;

	D->error = "";

	return D;
}

#define PROP_CORE_FLAG 0x10000
#define PROP_VALID_FLAG 0x20000

de4_PropertyDef * getdef(de4_State * D, id_t id)
{
	if(id & PROP_CORE_FLAG)
	{
		if(vector_idxok(&D->corepropdefs, id & 0xFFFF))
			return &vector_get(&D->propdefs, (id & 0xFFFF));
	}
	else
	{
		if(vector_idxok(&D->propdefs, id & 0xFFFF))
			return &vector_get(&D->propdefs, (id & 0xFFFF));
	}

	return 0;
}

// sets up the function environment and calls it
void callenv(de4_State * D, de4_Entity this_entity, void * this_prop, de4_Function f)
{
	// no recursion checks necessary, this 
	// function isn't directly accessible 
	// through callbacks
	D->this_entity = this_entity;
	D->this_property = this_prop;
	f(D);
	D->this_property = 0;
	D->this_entity = DE4_BADENTITY;
}

void de4_destroy(de4_State * D)
{
	for(int eid = 1 ; eid <= D->entity_num ; eid ++)
	{
		entity_t * e = getentity(D, eid);
		// iterate through all properties, deinit and free

		if(e->coreflags)
		{
			vector_foreach(&D->corepropdefs, it)
			{
				if(e->coreflags & (1 << it.index))
				{
					if(it.value->deinit)
						callenv(D, eid, corepropdata(D, it.index, eid), it.value->deinit);
				}
			}
		}

		if(e->properties)
		{
			listnode_foreach(e->properties, node)
			{
				de4_PropertyDef * def = getdef(D, node->typeid);

				if(def && def->deinit)
					callenv(D, eid, node->data, def->deinit);
			}
			listnode_destroy(e->properties); // destroy the whole list
		}
	}
	free(D->entities);

	vector_deinit(&D->propdefs);
	vector_deinit(&D->corepropdefs);

	// free every core
	vector_foreach(&D->coredata, it) { free(*it.value); }
	vector_deinit(&D->coredata); // deinitialize list of cores

	free(D);
}
const char * de4_geterror(de4_State * D)
{
	return D->error;
}

void de4_dump(de4_State * D)
{
	printf("\n---------------[DE4 STATE DUMP]---------------\n");
	printf("\n[Core properties]\n\n");
	vector_foreach(&D->corepropdefs, it)
	{
		printf("  %05X[%s]  init: %p  deinit: %p  size: %u\n", it.value->id, it.value->name, it.value->init, it.value->deinit, it.value->size);
	}
	printf("\n[Properties]\n\n");
	vector_foreach(&D->propdefs, it)
	{
		printf("  %05X[%s]  init: %p  deinit: %p  size: %u\n", it.value->id, it.value->name, it.value->init, it.value->deinit, it.value->size);
	}

	printf("\n[Active entities]\n\n        name.id   [core.x.y.z] (regular.x.y.z)\n\n");
	for(size_t i = 0 ; i < D->entity_num ; i ++)
	{
		entity_t * e = &D->entities[i];

		if(e->coreflags && e->properties)
		{
			printf("  %10s.%04X", e->name, i + 1);

			printf(" [ ");
			if(e->coreflags)
			{
				vector_foreach(&D->corepropdefs, it)
				{
					if(e->coreflags & (1 << it.index))
						printf("%s.", it.value->name);
				}
			}
			printf("\b ] ( ");
			if(e->properties)
			{
				listnode_foreach(e->properties, node)
				{
					de4_PropertyDef * def = getdef(D, node->typeid);

					if(def)
						printf("%s.", def->name);
					else
						printf("UNKNOWN.");
				}
			}
			printf("\b )\n");
		}
	}
	printf("\n");
}

de4_PropId de4_propid(de4_State * D, const char * name)
{
	vector_foreach(&D->corepropdefs, it)
	{
		if(strcmp(name, it.value->name) == 0)
		{
			return it.value->id;
		}
	}
	vector_foreach(&D->propdefs, it)
	{
		if(strcmp(name, it.value->name) == 0)
		{
			return it.value->id;
		}
	}

	return DE4_BADPROPERTY;
}

de4_PropId de4_defproperty(de4_State * D, de4_PropertyDef * p)
{
	if(de4_propid(D, p->name) != DE4_BADPROPERTY)
	{
		return DE4_BADPROPERTY;
	}

	de4_PropertyDef newp = *p;
	newp.id = (vector_size(&D->propdefs) & 0xFFFF) | PROP_VALID_FLAG;

	vector_push(&D->propdefs, newp);

	return newp.id;
}
de4_PropId de4_defcoreproperty(de4_State * D, de4_PropertyDef * p)
{
	if(de4_propid(D, p->name) != DE4_BADPROPERTY)
	{
		return DE4_BADPROPERTY;
	}

	if(p->size <= DE4_CACHELINEBYTES)
	{
		de4_PropertyDef newp = *p;
		newp.id = (vector_size(&D->propdefs) & 0xFFFF) | PROP_CORE_FLAG | PROP_VALID_FLAG;

		vector_push(&D->corepropdefs, newp);

		void * data = memalign(DE4_CACHELINEBYTES, D->entity_num*DE4_CACHELINEBYTES);
		memset(data, 0, DE4_CACHELINEBYTES*D->entity_num);
		vector_push(&D->coredata, data);

		return newp.id;
	}
	else
	{
		return DE4_BADPROPERTY;
	}
}

de4_Entity de4_thisentity(de4_State * D)
{
	return D->this_entity;
}

static entity_t * newentity(de4_State * D, de4_Entity * id)
{
	for(uint16_t i = D->last_new_entity + 1 ; i != D->last_new_entity ; i ++)
	{
		if(i >= D->entity_num) i = 0;

		entity_t * e = &D->entities[i];

		if(e->coreflags == 0 && e->properties == 0)
		{
			// we found a free one
			D->last_new_entity = i;

			*id = i + 1;

			return e;
		}
	}

	return 0;
}

static void * getprop(de4_State * D, de4_Entity eid, de4_PropId id)
{
	if(id & PROP_CORE_FLAG)
	{
		// grab the core data
		int coreid = id & 0xFFFF;

		// does this core property exist?
		if(!vector_idxok(&D->coredata, coreid)) return 0;

		// if the flag is set,
		if(getentity(D, eid)->coreflags & (1 << coreid))
			return vector_get(&D->coredata, coreid) + eid;
		else
			return 0;
	}
	else
	{
		// find the entity data in the (circular) linked list

		entity_t * entity = getentity(D, eid);

		if(!entity) return 0;

		listnode_t * found = listnode_find(entity->properties, id);

		if(!found) return 0;

		return found->data;
	}

	return 0;
}
/*
static void * addprop(de4_State * D, de4_Entity eid, de4_PropId id)
{
	printf("addprop(0x%04X, 0x%08X)\n", eid, id);

	entity_t * e = getentity(D, eid);

	de4_PropertyDef * def = 0;

	void * data = 0;

	if(id & PROP_CORE_FLAG)
	{
		def = &vector_get(&D->corepropdefs, id & 0xFFFF);

		int coreid = id & 0xFFFF;

		// does this core property exist?
		if(!vector_idxok(&D->coredata, coreid)) return 0;

		// mark the entity
		e->coreflags |= (1 << coreid);

		data = corepropdata(D, coreid, eid);
	}
	else
	{
		def = &vector_get(&D->propdefs, id & 0xFFFF);

		// allocate a new property
		prop_t * newp = calloc(1, sizeof(prop_t) + def->size);
		newp->id = def->id;
		newp->def = def;

		// add it as the new first
		prop_t * first = e->properties;

		if(first)
		{
			// linked list magic
			newp->next = first;
			newp->prev = first->prev;
			first->prev = newp;
			newp->prev->next = newp;
		}
		else
		{
			// sole entry, points to self
			e->properties = newp;
			newp->next = newp;
			newp->prev = newp;
		}

		data = newp->data;
	}

	if(def->init) callenv(D, eid, data, def->init);

	return data;
}
static void removeprop(de4_State * D, de4_Entity eid, de4_PropId id)
{
	if(id & PROP_CORE_FLAG)
	{
		int coreid = id & 0xFFFF;

		// does this core property exist?
		if(!vector_idxok(&D->coredata, coreid)) return;

		// unmark the entity
		getentity(D, eid)->coreflags &= ~(1 << coreid);
	}
	else
	{
		// find the entity data in the (circular) linked list

		prop_t * first = getentity(D, eid)->properties;

		if(!first) return; // entity doesn't have any non-core properties

		prop_t * p = first;
		do
		{
			// id matches?

			if(p->id == id)
			{
				entity_t * e = getentity(D, eid);

				// root node?
				if(e->properties == p)
				{
					// sole entry?
					if(p->next == p)
						e->properties = 0;
					else
						e->properties = p->next;
				}

				// bypass it
				p->prev->next = p->next;
				p->next->prev = p->prev;

				// time to kill
				de4_PropertyDef * def = &vector_get(&D->propdefs, id & 0xFFFF);

				if(def->deinit) callenv(D, eid, p->data, def->deinit);

				free(p);

				return;
			}

			p = p->next;
		} while(p != first);
	}
}

de4_Entity de4_newentityi(de4_State * D, const char * name, de4_PropId * plist)
{
	de4_Entity id = DE4_BADENTITY;

	entity_t * e = newentity(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		de4_PropId * propid = plist;

		while(*propid != DE4_BADPROPERTY)
		{
			addprop(D, id, *propid);

			propid ++;
		}
	}

	return id;
}
de4_Entity de4_newentity(de4_State * D, const char * name, const char ** plist)
{
	de4_Entity id = DE4_BADENTITY;

	entity_t * e = newentity(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		const char ** pname = plist;

		while(*pname)
		{
			de4_PropId pid = de4_propid(D, *pname);

			if(!getprop(D, id, pid)) addprop(D, id, pid);

			pname ++;
		}
	}

	return id;
}
de4_Entity de4_newentityc(de4_State * D, const char * name, de4_UDFunction ctor, void * ud)
{
	de4_Entity id = DE4_BADENTITY;

	entity_t * e = newentity(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		D->this_entity = id;
		ctor(D, ud);
		D->this_entity = DE4_BADENTITY;
	}

	return id;
}

#define BADPROP(id) (id == DE4_BADPROPERTY || !(id & PROP_VALID_FLAG))
#define BADENTITY(e) (e == DE4_BADENTITY)

void * de4_propertyi(de4_State * D, de4_PropId id)
{
	if(BADPROP(id))
		return 0;
	if(BADENTITY(D->this_entity))
		return 0;

	return getprop(D, D->this_entity, id);
}

void * de4_addpropertyi(de4_State * D, de4_PropId id)
{
	if(BADPROP(id))
		return 0;
	if(BADENTITY(D->this_entity))
		return 0;

	void * existing = getprop(D, D->this_entity, id);

	if(!existing)
		return addprop(D, D->this_entity, id);
	else
		return existing;
}
void de4_removepropertyi(de4_State * D, de4_PropId id)
{
	if(BADPROP(id))
		return;
	if(BADENTITY(D->this_entity))
		return;

	removeprop(D, D->this_entity, id);
}
*/
