#ifndef DE4_TYPES_H
#define DE4_TYPES_H

#include "de4cfg.h"

#include <stddef.h>

// this needs to be 32 bits or larger
typedef unsigned int de4_Id;
#define DE4_BADID ((de4_Id)0)

typedef struct de4_State de4_State;

typedef void (* de4_Function)(de4_State * D);
typedef void (* de4_Function1)(de4_State * D, void * arg0);
typedef void (* de4_Function2)(de4_State * D, void * arg0, void * arg1);
typedef void (* de4_Function3)(de4_State * D, void * arg0, void * arg1, void * arg2);
typedef void (* de4_Function4)(de4_State * D, void * arg0, void * arg1, void * arg2, void * arg3);

#define DE4_NAMEBYTES (DE4_NAMELEN+1)
typedef char de4_Name[DE4_NAMEBYTES];

typedef struct
{
	de4_Function init, deinit;
	size_t size;
	de4_Name name;
	de4_Id id;
} de4_PropertyDef;

#endif
