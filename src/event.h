#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
	uint16_t type;
	uint16_t tick;
	size_t size;
	void * data;
} event_t;

typedef struct
{
	event_t * events;
	size_t size;
	size_t back, front;
} eventbuf_t;

extern void eventbuffer_init(eventbuf_t * buf);
extern void eventbuffer_deinit(eventbuf_t * buf);

extern void eventbuffer_push(eventbuf_t * buf, const event_t * event);
extern bool eventbuffer_peek(const eventbuf_t * buf, event_t * event);
extern void eventbuffer_pop(eventbuf_t * buf);

#endif
