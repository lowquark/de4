#ifndef PROP_H
#define PROP_H

#include "de4.h"

#define PROP_CORE_FLAG 0x10000
#define PROP_VALID_FLAG 0x20000

#define PROP_IS_CORE(id) ((id) & PROP_CORE_FLAG)
#define PROP_IDX(id) ((id) & 0xFFFF)

de4_PropertyDef * prop_getdef(de4_State * D, de4_Id propid);

void * prop_get(de4_State * D, de4_Id eid, de4_Id propid);
void * prop_add(de4_State * D, de4_Id eid, de4_Id propid);
void prop_remove(de4_State * D, de4_Id eid, de4_Id propid);

#endif
