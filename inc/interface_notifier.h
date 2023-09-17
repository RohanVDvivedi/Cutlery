#ifndef INTERFACE_NOTIFIER_H
#define INTERFACE_NOTIFIER_H

/*
	The below interface is intentionally designed to handle elements, after a remove_all_from_* function call of any data structure.
	The remove_all_from_* functions must call it if a notifier_interface is passed.

	This interface can also be used by you to clean up the resources in use by data_p,
	i.e. free memory held by data_p, or notify another resource_p of its deletion.
*/

typedef struct notifier_interface notifier_interface;
struct notifier_interface
{
	// resource that needs to be notified
	void* resource_p;

	// the below function will be called when ever we want some resource to be notified of removal of data_p from a datastructure
	// notify function is designed to mimic insertion function signatures of other data structures of cutlery,
	// this will allow easy assignment of insertion functions (of other data structures) to this functions pointer
	void (*notify)(void* resource_p, const void* data_p);
};

#endif