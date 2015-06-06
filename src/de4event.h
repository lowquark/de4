#ifndef DE4_EVENT_H
#define DE4_EVENT_H

#include "vector.h"

typedef struct
{
	size_t size;
	uint16_t id;
	void * data;
} equeue_ent_t;
typedef struct
{
	vector_struct(equeue_ent_t) entries;
	size_t front; // .  |vv|  |  |  |
	size_t back; //  |  |  |  |vv.  .
} equeue_t;

void equeue_init(equeue_t * eq);
void equeue_deinit(equeue_t * eq);

void equeue_push(equeue_t * eq, const void * data, size_t size, uint16_t type);
void * equeue_pop(equeue_t * eq, size_t * size, uint16_t * type);

#endif
