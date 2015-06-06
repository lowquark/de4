
#include "event.h"

#include <stdio.h>
#include <stdlib.h>

void eventbuffer_init(eventbuf_t * buf)
{
	free(buf->events);
	buf->events = malloc(sizeof(event_t)*32);
	buf->size = 32;
	buf->front = 0;
	buf->back = 0;
}
void eventbuffer_deinit(eventbuf_t * buf)
{
	for(int i = buf->front ; i != buf->back ; i ++)
		free(buf->events[i].data);
	free(buf->events);
	buf->events = 0;
	buf->size = 0;
	buf->front = 0;
	buf->back = 0;
}

void eventbuffer_push(eventbuf_t * buf, const event_t * event)
{
	if(!buf->events)
	{
		eventbuffer_init(buf);
	}

	event_t newevent = *event;
	newevent.data = malloc(event->size);

	buf->events[buf->back] = newevent;

	buf->back = (buf->back + 1) % buf->size;

	if(buf->back == buf->front)
	{
		size_t new_size = buf->size ? buf->size * 2 : 32;
		event_t * new_events = malloc(sizeof(event_t)*new_size);

		printf("[EB 0x%p] Resizing event buffer to %u elements.\n", buf, new_size);

		for(size_t i = 0;i < buf->size;i ++)
		{
			//printf("Copying %u to %u\n", i, (i + buf->back) % buf->size);
			new_events[i] = buf->events[(i + buf->back) % buf->size];
		}

		buf->front = 0;
		buf->back = buf->size;

		buf->size = new_size;
		free(buf->events);
		buf->events = new_events;
	}
}
bool eventbuffer_peek(const eventbuf_t * buf, event_t * event)
{
	if(buf->back == buf->front)
		return 0;

	*event = buf->events[buf->front];

	return 1;
}
void eventbuffer_pop(eventbuf_t * buf)
{
	if(buf->back != buf->front)
	{
		free(buf->events[buf->front].data);
		buf->events[buf->front].data = 0;

		buf->front = (buf->front + 1) % buf->size;
	}
}
