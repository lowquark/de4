
#include "de4.h"

#include "state.h"
#include "prop.h"
#include <stdlib.h>

#define BADPROP(id) (id == DE4_BADID || !(id & PROP_VALID_FLAG))
#define BADENTITY(id) (id == DE4_BADID)

de4_Id de4_thisentity(de4_State * D)
{
	return D->this_entity;
}

void * de4_propertyi(de4_State * D, de4_Id id)
{
	if(BADPROP(id))
		return 0;
	if(BADENTITY(D->this_entity))
		return 0;

	return prop_get(D, D->this_entity, id);
}
void * de4_addpropertyi(de4_State * D, de4_Id id)
{
	if(BADPROP(id))
		return 0;
	if(BADENTITY(D->this_entity))
		return 0;

	event_t event = {
		.type = EVENT_ADDPROPERTY,
		.addproperty = {
			.id = id,
			.eid = D->this_entity
		}
	};
	vector_push(&D->events, event);

	void * existing = prop_get(D, D->this_entity, id);

	if(!existing)
		return prop_add(D, D->this_entity, id);
	else
		return existing;
}
void de4_removepropertyi(de4_State * D, de4_Id id)
{
	if(BADPROP(id))
		return;
	if(BADENTITY(D->this_entity))
		return;

	prop_remove(D, D->this_entity, id);
}

size_t de4_allpass(de4_State * D, de4_Function f)
{
	size_t n = 0;
	entity_t * e = D->entities;
	for(de4_Id eid = 0 ; eid < D->entity_num ; ++ eid, ++ e)
	{
		if(D->entities[eid].coreflags || !vector_empty(&e->properties))
		{
			++ n;
			D->this_entity = eid + 1;
			f(D);
		}
	}
	D->this_entity = DE4_BADID;

	return n;
}
size_t de4_pass1(de4_State * D, de4_Id id_0, de4_Function f)
{
	if(id_0 == DE4_BADID)
		return 0;

	size_t n = 0;

	if(PROP_IS_CORE(id_0))
	{
		uint32_t flag = (1 << PROP_IDX(id_0));
		for(de4_Id eid = 0 ; eid < D->entity_num ; eid ++)
		{
			if(D->entities[eid].coreflags & flag)
			{
				n ++;
				D->this_entity = eid + 1;
				f(D);
			}
		}
	}
	else
	{
		for(de4_Id eid = 0 ; eid < D->entity_num ; eid ++)
		{
			vector_foreach(&D->entities[eid].properties, it)
			{
				if(it.index == id_0)
				{
					n ++;
					D->this_entity = eid + 1;
					f(D);
					break;
				}
			}
		}
	}

	return n;
}
size_t de4_pass2(de4_State * D, de4_Id id_0, de4_Id id_1, de4_Function f)
{
	return 0;
}
size_t de4_pass3(de4_State * D, de4_Id id_0, de4_Id id_1, de4_Id id_2, de4_Function f)
{
	return 0;
}
