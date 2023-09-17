#ifndef HASHER_INTERFACE_H
#define HASHER_INTERFACE_H

/* 
** A hasher interface is an interface allowing you to hash data (pointed to by data pointers)
** It allows you to hash with OR without a context
*/

typedef struct hasher_interface hasher_interface;
struct hasher_interface
{
	// context for the hasher
	const void* context;

	// the hash functions of the hasher
	union
	{
		// hash function to use when context == NULL
		int (*hash1)(const void* data);

		// hash function to use when context != NULL
		int (*hash2)(const void* context, const void* data);
	};
};

// you must use only this function, with the hasher, to avail the complete functionality of the hasher
int hash_with_hasher(const hasher_interface* hasher, const void* data);

// You may use the macros below to initialize your custom hasher_interface
#define simple_hasher(hash_function_p)               ((const hasher_interface){.context = NULL,      .hasher1 = hash_function_p})
#define contexted_hasher(context_p, hash_function_p) ((const hasher_interface){.context = context_p, .hasher2 = hash_function_p})

#endif