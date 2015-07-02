#ifndef DE4_H
#define DE4_H

#include <stdint.h>
#include <stddef.h>

#include "de4cfg.h"
#include "types.h"

// create and destroy an instance of the engine
de4_State * de4_create(size_t numentities);
void de4_destroy(de4_State * D);

size_t de4_entitynum(de4_State * D);

// looks up the id of the given named property
de4_Id de4_propertyid(de4_State * D, const char * name);

// defines a property
de4_Id de4_defproperty(de4_State * D, de4_PropertyDef * p);
// defines a core property
de4_Id de4_defcoreproperty(de4_State * D, de4_PropertyDef * p);

// refers to the current entity
de4_Id de4_thisentity(de4_State * D);
// refers to the current property data
void * de4_thisproperty(de4_State * D);

// creates an entity with the supplied component types
de4_Id de4_newentityi(de4_State * D, const char * name, de4_Id * plist);
// creates an entity with the supplied component names
de4_Id de4_newentity(de4_State * D, const char * name, const char ** plist);
// creates an entity by calling the supplied constructor
//de4_Id de4_newentityc(de4_State * D, const char * name, de4_Function1 ctor, void * ud);

// looks up a property on the current entity
void * de4_propertyi(de4_State * D, de4_Id id);
// looks up a property by name on the current entity
#define de4_property(D, name)     de4_propertyi((D), de4_propid((D), (name)))

// creates the property with id /id/ on the current entity
void * de4_addpropertyi(de4_State * D, de4_Id id);
// looks up a property by name on the current entity
#define de4_addproperty(D, name)     de4_addpropertyi((D), de4_propid((D), (name)))
// destroys the property with id /id/ on the current entity
void   de4_removepropertyi(de4_State * D, de4_Id id);
// destroys the property with name /name/ on the current entity
#define de4_removeproperty(D, name)     de4_removepropertyi((D), de4_propid((D), (name)))

size_t de4_pass1(de4_State * D, de4_Id id_0, de4_Function1 f);
size_t de4_pass2(de4_State * D, de4_Id id_0, de4_Id id_1, de4_Function2 f);
size_t de4_pass3(de4_State * D, de4_Id id_0, de4_Id id_1, de4_Id id_2, de4_Function3 f);
size_t de4_pass4(de4_State * D, de4_Id id_0, de4_Id id_1, de4_Id id_2, de4_Id id_3, de4_Function4 f);

/*
// registers an event handler for a given event type
void de4_subscribe(de4_State * D, uint16_t type, de4_EventHandler h);
*/

// fires an event
void de4_emit(de4_State * D, uint16_t type, void * data, size_t size);

#endif
