#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define queue_struct(T) \
struct \
{ \
	T * data; \
	size_t size; \
	size_t back, front; \
}

#define queue_init(queue) \
	queue->data = 0; \
	queue->size = 0; \
	queue->front = 0; \
	queue->back = 0

#define queue_deinit(queue) \
	free((queue)->data); \
	(queue)->data = 0; \
	(queue)->size = 0; \
	(queue)->front = 0; \
	(queue)->back = 0

#define queue_empty(queue) (queue->back == queue->front)
#define queue_back(queue) (queue->data[queue->back])
#define queue_front(queue) (queue->data[queue->front])

//extern void queue_resize(void * data, 

#define queue_pop(queue) \
	if((queue)->back != (queue)->front) \
	{ \
		free((queue)->data[(queue)->front].data); \
		(queue)->data[(queue)->front].data = 0; \
 \
		(queue)->front = ((queue)->front + 1) % (queue)->size; \
	}

#define queue_

#define queue_push(queue, val) \
	if((queue)->back != (queue)->front) \
	{ \
		free((queue)->data[(queue)->front].data); \
		(queue)->data[(queue)->front].data = 0; \
 \
		(queue)->front = ((queue)->front + 1) % (queue)->size; \
	}

	   /*
	if(!buf->size || ((buf->back + 1) % buf->size) == buf->front)
	{
		size_t new_size = buf->size ? buf->size * 2 : 32;
		event_t * new_events = malloc(sizeof(event_t)*new_size);

		for(size_t i = 0;i < buf->size;i ++)
		{
			//printf("Copying %u to %u\n", i, (i + buf->back) % buf->size);
			new_events[i] = buf->events[(i + buf->back) % buf->size];
		}
	}

	event_t newevent = *event;
	newevent.data = malloc(event->size);

	buf->events[buf->back] = newevent;

	buf->back = (buf->back + 1) % buf->size;


extern void queue_init(eventbuf_t * buf);
extern void queue_deinit(eventbuf_t * buf);

extern void queue_push(eventbuf_t * buf, const event_t * event);
extern bool queue_front(const eventbuf_t * buf, event_t * event);
extern bool queue_back(const eventbuf_t * buf, event_t * event);
extern void queue_pop(eventbuf_t * buf);
	*/

#endif
