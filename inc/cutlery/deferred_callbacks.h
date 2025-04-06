#ifndef DEFERRED_CALLBACKS_H
#define DEFERRED_CALLBACKS_H

#include<cutlery/cutlery_stds.h>

typedef struct deferred_callback deferred_callback;
struct deferred_callback
{
	void (*deferred_callback_func)(void* resource_p);
	void* resource_p;
};

#define NEW_DEFERRED_CALLS(CAPACITY) \
deferred_callback deferrence_callbacks[CAPACITY] = {}; \
cy_uint deferrence_callbacks_size = 0;

#define DEFER(deferred_callback_func_v, resource_p_v) \
deferrence_callbacks[deferrence_callbacks_size++] = (deferred_callback){(void (*)(void*))deferred_callback_func_v, resource_p_v};

#define CALL_ALL_DEFERRED() \
while(deferrence_callbacks_size > 0) \
{ \
	deferred_callback dc = deferrence_callbacks[--deferrence_callbacks_size]; \
	dc.deferred_callback_func(dc.resource_p); \
}

#endif