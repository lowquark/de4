#ifndef STATE_H
#define STATE_H

#include "de4cfg.h"
#include "vector.h"

#include "de4.h"

// this needs to be 32 bits or larger
typedef unsigned int id_t;
#define DE4_BADID ((id_t)0);

typedef struct listnode
{
	struct listnode * next;
	struct listnode * prev;
	id_t typeid;
	uint8_t data[];
} listnode_t;


#define listnode_foreach(first, nodevar) \
	for(listnode_t * nodevar = 0 ; \
		nodevar != (first) ; \
		nodevar = nodevar ? nodevar->next : first)


typedef uint8_t core_data_t[DE4_CACHELINEBYTES];

typedef struct prop
{
	de4_PropertyDef * def;
	struct prop * next;
	struct prop * prev;
	de4_PropId id;
	uint8_t data[];
} prop_t;

typedef struct entity
{
	listnode_t * properties;
	char name[DE4_NAMEBYTES];
	uint32_t coreflags;
} entity_t;

struct de4_State
{
	vector_struct(de4_PropertyDef) propdefs;
	vector_struct(de4_PropertyDef) corepropdefs;

	size_t entity_num;
	size_t last_new_entity;

	entity_t * entities;
	vector_struct(core_data_t *) coredata;

	de4_Entity this_entity;
	de4_System this_system;
	void * this_property;

	const char * error;
};

de4_State * newstate();
void deletestate(de4_State * D);


#endif
