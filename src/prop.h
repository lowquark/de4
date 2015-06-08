#ifndef PROP_H
#define PROP_H

#include <de4/types.h>

#define PROP_CORE_FLAG 0x10000
#define PROP_VALID_FLAG 0x20000

#define PROP_IS_CORE(id) ((id) & PROP_CORE_FLAG)
#define PROP_IDX(id) ((id) & 0xFFFF)

const de4_PropertyDef * prop_getdefi(de4_State * D, de4_Id propid);
const de4_PropertyDef * prop_getdef(de4_State * D, const char * name);

de4_Id prop_adddef(de4_State * D, const de4_PropertyDef * def);
de4_Id prop_addcoredef(de4_State * D, const de4_PropertyDef * def);
void prop_clearall(de4_State * D);

void * prop_get(de4_State * D, de4_Id eid, de4_Id propid);
void * prop_add(de4_State * D, de4_Id eid, de4_Id propid);
void prop_remove(de4_State * D, de4_Id eid, de4_Id propid);

#endif
