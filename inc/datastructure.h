#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef enum datastructuretype dstype;
enum datastructuretype
{
	ARRAY,
	LINKEDLIST,
	HASHMAP,
	BALANCEDBST,
	STACK,
	QUEUE,
	HEAP
};

typedef struct datastructure datastructure;
struct datastructure
{
	dstype type;
};

datastructure* get_datastructure(dstype type, int sub_type, unsigned long long int size, int (*key_compare)(const void* key1, const void* key2), unsigned long long int (*hash_function)(const void* key));

// returns true for hashmap, bst, linkedlist if it is bucketted and heap
int is_bucketted(datastructure* ds_p);

// returns true for hashmap, bst, linkedlist if it is bucketted
int is_unique_keyed(datastructure* ds_p);

// returns true for stack, queu and heap
int is_push_pop_type(datastructure* ds_p);

// put a new entry
void insert_entry(datastructure* ds_p, const void* key, const void* value);

// get a new value
const void* find_value(const datastructure* ds_p, const void* input);

// get a new value
int update_value(const datastructure* ds_p, const void* input, const void** return_value);

// returns 1 if the value is found and removed
int delete_entry(datastructure* ds_p, const void* key, const void** return_key, const void** return_value);

// push a new element
void push(datastructure* ds_p, const void* data_p);

// push a new entry
void push_entry(datastructure* ds_p, const void* key_p, const void* value_p);

// pop the element
void pop(datastructure* ds_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the element will be next popped
const void* get_top(datastructure* ds_p);

void delete_datastructure(datastructure* ds_p);

#endif