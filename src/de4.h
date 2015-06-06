#ifndef DE4_H
#define DE4_H

#include <stdint.h>
#include <stddef.h>

#include "de4cfg.h"

typedef struct de4_State de4_State;
typedef uint32_t de4_PropId;
typedef uint16_t de4_Entity;
typedef uint16_t de4_System;

typedef void (* de4_Function)(de4_State * D);
typedef void (* de4_UDFunction)(de4_State * D, void * ud);

typedef void (* de4_EventHandler)(de4_State * D, void * data, size_t size);

#define DE4_NAMEBYTES (DE4_NAMELEN+1)
typedef struct
{
	de4_Function init, deinit;
	size_t size;
	char name[DE4_NAMEBYTES];
	de4_PropId id;
} de4_PropertyDef;

#define DE4_BADPROPERTY 0
#define DE4_BADENTITY 0

de4_State * de4_create(size_t numentities);
void de4_destroy(de4_State * D);
const char * de4_geterror(de4_State * D);

void de4_dump(de4_State * D);

// looks up the id of the given named property
de4_PropId de4_propid(de4_State * D, const char * name);

// defines a property
de4_PropId de4_defproperty(de4_State * D, de4_PropertyDef * p);
// defines a core property
de4_PropId de4_defcoreproperty(de4_State * D, de4_PropertyDef * p);

// refers to the current entity
de4_Entity de4_thisentity(de4_State * D);

// creates an entity with the supplied component types
de4_Entity de4_newentityi(de4_State * D, const char * name, de4_PropId * plist);
// creates an entity with the supplied component names
de4_Entity de4_newentity(de4_State * D, const char * name, const char ** plist);
// creates an entity by calling the supplied constructor
de4_Entity de4_newentityc(de4_State * D, const char * name, de4_UDFunction ctor, void * ud);

// looks up a property on the current entity
void * de4_propertyi(de4_State * D, de4_PropId id);
// looks up a property by name on the current entity
#define de4_property(D, name)     de4_propertyi((D), de4_propid((D), (name)))

// creates the property with id /id/ on the current entity
void * de4_addpropertyi(de4_State * D, de4_PropId id);
// looks up a property by name on the current entity
#define de4_addproperty(D, name)     de4_addpropertyi((D), de4_propid((D), (name)))
// destroys the property with id /id/ on the current entity
void   de4_removepropertyi(de4_State * D, de4_PropId id);
// destroys the property with name /name/ on the current entity
#define de4_removeproperty(D, name)     de4_removepropertyi((D), de4_propid((D), (name)))

// refers to the current system
de4_System de4_thissystem (de4_State * D);
// refers to the current property
void * de4_thispropertyz  (de4_State * D, size_t * size);

// retrieves the nth property requested for processing by the system
void * de4_systemprop     (de4_State * D, size_t n);

// registers an event handler for a given event type
void de4_subscribe(de4_State * D, uint16_t type, de4_EventHandler h);

// fires an event
void de4_emit(de4_State * D, uint16_t type, void * data, size_t size);

#endif
