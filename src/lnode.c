
#include "lnode.h"

#include <malloc.h>

void lnode_remove(lnode_t * e)
{
	e->prev->next = e->next;
	e->next->prev = e->prev;
	e->next = e;
	e->prev = e;
}
// insert a into b's list, after b
void lnode_insertafter(lnode_t * a, lnode_t * b)
{
	// pull it from its current list
	lnode_remove(a);

	a->prev = b;
	a->next = b->next;
	b->next = a;
	a->next->prev = a;
}
// insert a into b's list, before b
void lnode_insertbefore(lnode_t * a, lnode_t * b)
{
	// pull it from its current list
	lnode_remove(a);

	a->next = b;
	a->prev = b->prev;
	b->prev = a;
	a->prev->next = a;
}

lnode_t * lnode_create(de4_Id type, size_t size)
{
	lnode_t * new_node = calloc(sizeof(lnode_t) + size, 1);
	new_node->next = new_node;
	new_node->prev = new_node;
	new_node->typeid = type;

	return new_node;
}

lnode_t * lnode_destroy(lnode_t * e)
{
	lnode_t * succ = e->next;

	lnode_remove(e);

	free(e);

	if(succ == e)
		return 0;

	return succ;
}

lnode_t * lnode_find(lnode_t * first, de4_Id type)
{
	lnode_foreach(first, node)
	{
		if(node->typeid == type)
		{
			return node;
		}
	}

	return 0;
}
