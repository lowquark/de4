
#include "prop.h"

#include "state.h"
#include "entity.h"
#include "vector.h"

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#define memalign(align, size) _aligned_malloc(size, align)
#endif

const de4_PropertyDef * prop_getdefi(de4_State * D, de4_Id propid)
{
	if(propid & PROP_CORE_FLAG)
	{
		if(vector_idxok(&D->corepropdefs, PROP_IDX(propid)))
			return &vector_get(&D->propdefs, PROP_IDX(propid));
	}
	else
	{
		if(vector_idxok(&D->propdefs, PROP_IDX(propid)))
			return &vector_get(&D->propdefs, PROP_IDX(propid));
	}

	return 0;
}
const de4_PropertyDef * prop_getdef(de4_State * D, const char * name)
{
	vector_foreach(&D->corepropdefs, it)
	{
		if(strcmp(name, it.ptr->name) == 0)
		{
			return it.ptr;
		}
	}
	vector_foreach(&D->propdefs, it)
	{
		if(strcmp(name, it.ptr->name) == 0)
		{
			return it.ptr;
		}
	}

	return 0;
}

de4_Id prop_adddef(de4_State * D, const de4_PropertyDef * def)
{
	de4_PropertyDef newp = *def;
	newp.id = (vector_size(&D->propdefs) & 0xFFFF) | PROP_VALID_FLAG;

	vector_push(&D->propdefs, newp);

	return newp.id;
}
de4_Id prop_addcoredef(de4_State * D, const de4_PropertyDef * def)
{
	if(def->size <= DE4_CACHELINEBYTES)
	{
		de4_PropertyDef newp = *def;
		newp.id = (vector_size(&D->propdefs) & 0xFFFF) | PROP_CORE_FLAG | PROP_VALID_FLAG;

		vector_push(&D->corepropdefs, newp);

		void * data = memalign(DE4_CACHELINEBYTES, D->entity_num*DE4_CACHELINEBYTES);
		memset(data, 0, DE4_CACHELINEBYTES*D->entity_num);
		vector_push(&D->coredata, data);

		return newp.id;
	}
	else
	{
		return DE4_BADID;
	}
}

void prop_clearall(de4_State * D)
{
	vector_deinit(&D->propdefs);
	vector_deinit(&D->corepropdefs);

	// free every core
	vector_foreach(&D->coredata, it) { free(*it.ptr); }
	vector_deinit(&D->coredata); // deinitialize list of cores
}

// retrieves component data for an entity
void * prop_get(de4_State * D, de4_Id eid, de4_Id id)
{
	assert(eid <= D->entity_num && eid != DE4_BADID);

	entity_t * e = entity_get(D, eid);

	if(id & PROP_CORE_FLAG)
	{
		// grab the core data
		int coreid = id & 0xFFFF;

		// does this core property exist?
		if(!vector_idxok(&D->coredata, coreid)) return 0;

		// if the flag is set,
		if(e->coreflags & (1 << coreid))
			return vector_get(&D->coredata, coreid) + eid;
		else
			return 0;
	}
	else
	{
		// find the entity data
		vector_foreach(&e->properties, it)
		{
			if(it.ptr->typeid == id) return it.ptr->value;
		}

		return 0;
	}
}

// blindly attaches component data to an entity
void * prop_add(de4_State * D, de4_Id eid, de4_Id propid)
{
	assert(eid <= D->entity_num && eid != DE4_BADID);

	//printf("prop_add(0x%04X, 0x%08X)\n", eid, propid);

	entity_t * e = entity_get(D, eid);
	const de4_PropertyDef * def = prop_getdefi(D, propid);
	if(!def) return 0;

	void * data = 0;

	if(propid & PROP_CORE_FLAG)
	{
		int coreid = propid & 0xFFFF;

		// mark the entity
		e->coreflags |= (1 << coreid);

		data = corepropdata(D, coreid, eid);
	}
	else
	{
		data = calloc(1, def->size);

		// look for an open slot
		int found = 0;
		vector_foreach(&e->properties, it)
		{
			if(!it.ptr->value)
			{
				found = 1;
				*it.ptr = (any_t){propid, data};
				break;
			}
		}

		if(!found)
		{
			any_t val = { propid, data };
			vector_push(&e->properties, val);
		}
	}

	if(def->init) state_callenv(D, eid, data, def->init);

	return data;
}

// finds and removes component data from an entity
void prop_remove(de4_State * D, de4_Id eid, de4_Id propid)
{
	assert(eid <= D->entity_num && eid != DE4_BADID);

	//printf("prop_remove(0x%04X, 0x%08X)\n", eid, propid);

	entity_t * e = entity_get(D, eid);
	const de4_PropertyDef * def = prop_getdefi(D, propid);
	if(!def) return;

	if(propid & PROP_CORE_FLAG)
	{
		int coreid = propid & 0xFFFF;

		// unmark the entity
		e->coreflags &= ~(1 << coreid);

		void * data = corepropdata(D, coreid, eid);
		if(def->deinit) state_callenv(D, eid, data, def->deinit);
	}
	else
	{
		vector_foreach(&e->properties, it)
		{
			if(it.ptr->typeid == propid)
			{
				if(def->deinit) state_callenv(D, eid, it.ptr->value, def->deinit);
				free(it.ptr->value);
				it.ptr->value = 0;
				it.ptr->typeid = DE4_BADID;
				break;
			}
		}
	}
}
