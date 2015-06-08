
#include "state.h"

#include "prop.h"

#include <stdio.h>

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

		if(e->coreflags || vector_size(&e->properties))
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
				const de4_PropertyDef * def = prop_getdefi(D, it.ptr->typeid);

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
