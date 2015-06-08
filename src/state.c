
#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "prop.h"
#include "entity.h"

#ifdef _WIN32
#define memalign(align, size) _aligned_malloc(size, align)
#endif

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
		};
	for(int eid = 1 ; eid <= D->entity_num ; eid ++)
	{
		entity_deinit(D, eid);
	}

	vector_init(&D->coredata);

	D->this_entity = 0;
	D->this_system = 0;
	D->this_property = 0;

	D->error = "";

	return D;
}

void de4_destroy(de4_State * D)
{
	for(int eid = 1 ; eid <= D->entity_num ; eid ++)
	{
		entity_deinit(D, eid);
	}
	free(D->entities);

	prop_clearall(D);

	free(D);
}

// sets up the function environment and calls a function
void state_callenv(de4_State * D, de4_Id this_entity, void * this_prop, de4_Function f)
{
	// no recursion checks necessary, this 
	// function isn't directly accessible 
	// through callbacks
	D->this_entity = this_entity;
	D->this_property = this_prop;
	f(D);
	D->this_property = 0;
	D->this_entity = DE4_BADID;
}
const char * de4_geterror(de4_State * D)
{
	return D->error;
}

