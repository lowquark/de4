#ifndef ENTITY_H
#define ENTITY_H

#include "de4.h"
#include "vector.h"
#include "misc.h"

#define entity_get(D, id) (&(D)->entities[id - 1])
#define entity_getid(D, e) (((e) - (D)->entities) + 1)

#define BADENTITY(id) (id == DE4_BADID)

void entity_init(de4_State * D, de4_Id eid);
void entity_deinit(de4_State * D, de4_Id eid);

struct entity
{
	vector_struct(any_t) properties;
	de4_Name name;
	uint32_t coreflags;
};
typedef struct entity entity_t;

#endif
