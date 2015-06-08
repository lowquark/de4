#ifndef STATE_H
#define STATE_H

#include "de4.h"
#include "vector.h"
#include "entity.h"
#include "misc.h"

#define corepropdata(D, id, eid) (vector_get(&(D)->coredata, (id)) + (eid))

// event used internally by de4
typedef struct
{
	enum {
		EVENT_NEWENTITYI,
		EVENT_NEWENTITY,
		EVENT_NEWENTITYC,

		EVENT_ADDPROPERTY,
		EVENT_REMOVEPROPERTY,
 	} type;

	union {

	struct {
		vector_struct(de4_Id) ids;
	} newentityi;
	struct {
		vector_struct(de4_Name) names;
	} newentity;
	struct {
		de4_UDFunction f;
		void * ud;
	} newentityc;

	struct {
		de4_Id id;
		de4_Id eid;
	} addproperty;
	struct {
		de4_Id id;
		de4_Id eid;
	} removeproperty;

	};
} event_t;

typedef uint8_t core_data_t[DE4_CACHELINEBYTES];

typedef struct any any_t;

struct de4_State
{
	vector_struct(de4_PropertyDef) propdefs;
	vector_struct(de4_PropertyDef) corepropdefs;

	size_t entity_num;
	size_t last_new_entity;

	entity_t * entities;
	vector_struct(core_data_t *) coredata;

	de4_Id this_entity;
	de4_Id this_system;
	void * this_property;

	vector_struct(event_t) events; // new entity, new component, ...
	vector_struct(any_t) userevents;

	const char * error;
};

void state_callenv(de4_State * D, de4_Id this_entity, void * this_prop, de4_Function f);

#endif
