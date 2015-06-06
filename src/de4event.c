
#include "de4event.h"

#define INITIAL_SIZE 32

void equeue_init(equeue_t * eq)
{
	eq->data = malloc(INITIAL_SIZE);
	eq->nbytes = INITIAL_SIZE;
	eq->front = 0;
	eq->back = 0;
}
void equeue_deinit(equeue_t * eq)
{
	free(eq->data);
	eq->data = 0;
	eq->front = 0;
	eq->back = 0;
}

typedef struct
{
	size_t size;
	uint16_t type;
	uint8_t data[];
} equeue_entry_t;

void equeue_push(equeue_t * eq, const void * data, size_t size, uint16_t type)
{
	if(eq->back + size > eq->nbytes)
	{
		eq->data = realloc(eq->data, eq->nbytes * 2);
	}

	if(
}
void * equeue_pop(equeue_t * eq, size_t * size, uint16_t * type)
{
	
}
