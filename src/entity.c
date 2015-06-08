
#include "entity.h"
#include "state.h"
#include "prop.h"

#include <string.h>
#include <stdlib.h>

void entity_init(de4_State * D, de4_Id eid)
{
	entity_t * e = entity_get(D, eid);

	vector_init(&e->properties);

	*e = (entity_t){
		.name = "entity",
		.coreflags = 0
	};
}
void entity_deinit(de4_State * D, de4_Id eid)
{
	entity_t * e = entity_get(D, eid);
	// iterate through all properties, deinit and free
	if(e->coreflags)
	{
		vector_foreach(&D->corepropdefs, it)
		{
			if(e->coreflags & (1 << it.index))
			{
				if(it.ptr->deinit)
					state_callenv(D, eid, corepropdata(D, it.index, eid), it.ptr->deinit);
			}
		}
	}

	vector_foreach(&e->properties, it)
	{
		de4_PropertyDef * def = prop_getdef(D, it.ptr->typeid);

		if(def && def->deinit)
			state_callenv(D, eid, it.ptr->value, def->deinit);
	}
	vector_deinit(&e->properties);
}

static entity_t * entity_new(de4_State * D, de4_Id * id)
{
	for(uint16_t i = D->last_new_entity + 1 ; i != D->last_new_entity ; i ++)
	{
		if(i >= D->entity_num) i = 0;

		entity_t * e = &D->entities[i];

		if(e->coreflags == 0 && vector_empty(&e->properties))
		{
			// we found a free one
			D->last_new_entity = i;

			*id = i + 1;

			return e;
		}
	}

	return 0;
}

de4_Id de4_newentityi(de4_State * D, const char * name, de4_Id * plist)
{
	de4_Id id = DE4_BADID;

	entity_t * e = entity_new(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		de4_Id * propid = plist;

		while(*propid != DE4_BADID)
		{
			prop_add(D, id, *propid);

			propid ++;
		}
	}

	return id;
}
de4_Id de4_newentity(de4_State * D, const char * name, const char ** plist)
{
	de4_Id id = DE4_BADID;

	entity_t * e = entity_new(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		const char ** pname = plist;

		while(*pname)
		{
			de4_Id pid = de4_propid(D, *pname);

			if(!prop_get(D, id, pid)) prop_add(D, id, pid);

			pname ++;
		}
	}

	return id;
}
de4_Id de4_newentityc(de4_State * D, const char * name, de4_UDFunction ctor, void * ud)
{
	de4_Id id = DE4_BADID;

	entity_t * e = entity_new(D, &id);
	if(e)
	{
		strncpy(e->name, name, DE4_NAMEBYTES);

		D->this_entity = id;
		ctor(D, ud);
		D->this_entity = DE4_BADID;
	}

	return id;
}
