#ifndef HEAP_H
#define HEAP_H

#include<arraylist.h>
#include<index_accessed_interface.h>

#include<heap_info.h>

typedef struct hpnode hpnode;
struct hpnode
{
	// an embedded heap node is used to define the index of that element in the heap
	cy_uint heap_index;
};

// you can use heap, without embedded a hpnode
// pass this as the node_offset when you are not using the embedded node
#define NO_HEAP_NODE_OFFSET INVALID_INDEX

/*
**
**	This is a n-ary heap data structures, organized as a complete n-ary tree on an array
**	Here n refer's to the degree of the heap
**	A degree of 2 makes it a binary heap
**	A degree of n implies that each element/node of the heap has atmost n children
**	Hence, the degree must be non zero, i.e. > 0
**
*/

#define BINARY_HEAP_DEGREE 2

typedef struct heap heap;
struct heap
{
	// stores whether, type = MIN_HEAP or MAX_HEAP AND int compare(const void*, const void*) function to be used
	heap_info info;

	// this is the degree of this heap, it must be non zero
	// each element in the heap may have atmost degree number of children
	cy_uint degree;

	// defines the address of the data, with respect to the heap node
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// to store pointers to the elements of the heap, its main container
	arraylist heap_holder;
};

// initializes heap and it will depend on initialize_array to give necessary memory to manage internal element contents
// the initialize_heap* functions may fail if the initial memory allocation fails
// in case of a failure, the heap will still be initialized to capacity 0
int initialize_heap(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, int (*compare)(const void* data1, const void* data2), cy_uint node_offset);
int initialize_heap_with_allocator(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, int (*compare)(const void* data1, const void* data2), cy_uint node_offset, memory_allocator mem_allocator);
int initialize_heap_with_memory(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, int (*compare)(const void* data1, const void* data2), cy_uint node_offset, const void* data_ps[]);

// always initialize your hpnode before using it
void initialize_hpnode(hpnode* node_p);

// a free floating hpnode is the one that does not exist in any heap
int is_free_floating_hpnode(const hpnode* node_p);

// push a new data element to the heap
// push returns 1, if data_p is successfully pushed
// push returns 0 and fails if the heap container is full
// if push fails you must expand_heap before pushing any new elements
// O(log(N)) operation
int push_to_heap(heap* heap_p, const void* data);

// pushes all the elements from the iai_p from start_index to last_index (both inclusive) to the heap_p
// returns 1, if the operation was successfull
// returns 0, if the total_size of the heap was smaller than size necessary
// upon the return of this function, either all the elements are inserted or no elements are inserted
// O(N) operation
int push_all_to_heap(heap* heap_p, index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index);

// pop the top element from the heap
// pop returns 1, if an element is successfully popped
// pop returns 0 and fails if the heap container is already empty and does not have any elements to pop
// O(log(N)) operation
int pop_from_heap(heap* heap_p);

// returns pointer to the top data element of the heap, returns NULL if the heap is empty
// O(1) operation
const void* get_top_of_heap(const heap* heap_p);

// the below function is to be called when you doubt if the heap properties are being violated at the given index
// or because you changed the attributes of the data, which changed its comparison ourput with respect to other elements in the heap
// if your doubt turns out to be true and the heap property is being violated,
// then this function would restore the heap property for that index, assuming that all other elements are still holding the same values while the heap was valid
// O(log(N)) operation
void heapify_at(heap* heap_p, cy_uint index);

// this function can be called only if you are using the embedded hpnode in your data
// it figures out the heap_index from the hpnode of the data and 
// then internally calls heapify_at()
// please do not use this function if you are not using hpnode in your datastructure
// this functions fails with 0 returns, if the element provided does not exist in this heap
// else if the element was removed it will return 1
// O(log(N)) operation
void heapify_for(heap* heap_p, const void* data);

// the below function is to be called when you doubt if the heap properties are being violated at multiple indices of the heap
// or because you changed the attributes of multiple data elements, which changed its ordering
// if your doubt turns out to be true and the heap property is being violated at multiple indices of heap,
// then this function would restore the heap property throughout the heap
// O(N) operation
void heapify_all(heap* heap_p);

// this function removes an element from the heap, at a particular index
// it returns 0, if no element was removed, else it returns 1
// this functions fails with 0 returns, if the index provided is greater than the elements in the heap
// i.e. fails if index is out of bounds, else if the element was removed it will return 1
// O(log(N)) operation
int remove_at_index_from_heap(heap* heap_p, cy_uint index);

// this function can be called only if you are using the embedded hpnode in your data
// it figures out the heap_index from the hpnode of the data and 
// then internally calls remove_at_index_from_heap()
// please do not use this function if you are not using hpnode in your datastructure
// this functions fails with 0 returns, if the element provided does not exist in this heap
// else if the element was removed it will return 1
// O(log(N)) operation
int remove_from_heap(heap* heap_p, const void* data);

// removes all the elements from heap heap_p
// after a call to this function the heap contains 0 elements (get_element_count() == 0)
// it does not affect the capacity of the heap
void remove_all_from_heap(heap* heap_p);

void deinitialize_heap(heap* heap_p);

// returns capacity of the heap, i.e. the maximum number of elements that can be accomodated in heap
cy_uint get_capacity_heap(const heap* heap_p);

// returns the number of elements inside the heap
cy_uint get_element_count_heap(const heap* heap_p);

// returns 1, if the heap container is full, i.e. it needs to be expanded to push any futher elements
// else it return 0, this means there is still space in the container to push elements
int is_full_heap(const heap* heap_p);

// returns 1, if the heap is empty, i.e. does not have any elements to pop
// else it return 0
int is_empty_heap(const heap* heap_p);

// expand the heap container, returns 1 on successfull expansion of the heap container
// else returns 0
int expand_heap(heap* heap_p);

// shrink the heap container, returns 1 on successfull shrink of the heap container
// else returns 0
// you may call this function at regular intervals, to keep the heap size in check
int shrink_heap(heap* heap_p);

// expands heap to atleast the capacity of atleast_capacity
// returns 1, only if the heap was expanded
int reserve_capacity_for_heap(heap* heap_p, cy_uint atleast_capacity);

// perform an operation on all the key value pairs for the heap
void for_each_in_heap(const heap* heap_p, void (*operation)(void* data, cy_uint heap_index, const void* additional_params), const void* additional_params);

// serializes the array, and appends the serialized form to the dstring
void sprint_heap(dstring* append_str, const heap* heap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

#endif