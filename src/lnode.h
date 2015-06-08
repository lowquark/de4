#ifndef LNODE_H
#define LNODE_H

#include "de4.h"

typedef struct lnode
{
	struct lnode * next;
	struct lnode * prev;
	de4_Id typeid;
	uint8_t data[];
} lnode_t;

#define lnode_foreach(first, nodevar) \
	for(lnode_t * nodevar = 0 ; \
		nodevar != (first) ; \
		nodevar = nodevar ? nodevar->next : first)

void lnode_remove(lnode_t * e);
// insert a into b's list, after b
void lnode_insertafter(lnode_t * a, lnode_t * b);
// insert a into b's list, before b
void lnode_insertbefore(lnode_t * a, lnode_t * b);

lnode_t * lnode_create(de4_Id type, size_t size);
lnode_t * lnode_destroy(lnode_t * e);
lnode_t * lnode_find(lnode_t * first, de4_Id type);


#endif
