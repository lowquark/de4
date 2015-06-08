
#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "prop.h"
#include "entity.h"

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

void de4_destroy(de4_State * D)
{
	for(int eid = 1 ; eid <= D->entity_num ; eid ++)
	{
		entity_deinit(D, eid);
	}
	free(D->entities);

	vector_deinit(&D->propdefs);
	vector_deinit(&D->corepropdefs);

	// free every core
	vector_foreach(&D->coredata, it) { free(*it.ptr); }
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
		printf("  %05X[%s]  init: %p  deinit: %p  size: %u\n", it.ptr->id, it.ptr->name, it.ptr->init, it.ptr->deinit, it.ptr->size);
	}
	printf("\n[Properties]\n\n");
	vector_foreach(&D->propdefs, it)
	{
		printf("  %05X[%s]  init: %p  deinit: %p  size: %u\n", it.ptr->id, it.ptr->name, it.ptr->init, it.ptr->deinit, it.ptr->size);
	}

	printf("\n[Active entities]\n\n        name.id   [core.x.y.z] (regular.x.y.z)\n\n");
	for(size_t i = 0 ; i < D->entity_num ; i ++)
	{
		entity_t * e = &D->entities[i];

		if(e->coreflags && vector_size(&e->properties))
		{
			printf("  %10s.%04X", e->name, i + 1);

			printf(" [ ");
			if(e->coreflags)
			{
				vector_foreach(&D->corepropdefs, it)
				{
					if(e->coreflags & (1 << it.index))
						printf("%s.", it.ptr->name);
				}
			}
			printf("\b ] ( ");
			vector_foreach(&e->properties, it)
			{
				de4_PropertyDef * def = prop_getdef(D, it.ptr->typeid);

				if(def)
					printf("%s.", def->name);
				else
					printf("UNKNOWN.");
			}
			printf("\b )\n");
		}
	}
	printf("\n");
}

