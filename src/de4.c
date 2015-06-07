
#include "de4.h"

#include "state.h"

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
		vector_struct(de4_PropId) ids;
	} newentityi;
	struct {
		vector_struct(de4_Name) names;
	} newentity;
	struct {
		de4_UDFunction f;
		void * ud;
	} newentityc;

	struct {
		de4_PropId id;
		de4_Entity eid;
	} addproperty;
	struct {
		de4_PropId id;
		de4_Entity eid;
	} removeproperty;

	};
} de4_event_t;


