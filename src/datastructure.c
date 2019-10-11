#include<datastructure.h>

datastructure* get_datastructure(dstype type, int sub_type, unsigned long long int size, int (*key_compare)(const void* key1, const void* key2), unsigned long long int (*hash_function)(const void* key))
{

}

int is_bucketted(datastructure* ds_p)
{

}

int is_unique_keyed(datastructure* ds_p)
{

}

int is_push_pop_type(datastructure* ds_p)
{

}

void insert_entry(datastructure* ds_p, const void* key, const void* value)
{

}

const void* find_value(const datastructure* ds_p, const void* input)
{

}

int update_value(const datastructure* ds_p, const void* input, const void** return_value)
{

}

int delete_entry(datastructure* ds_p, const void* key, const void** return_key, const void** return_value)
{

}

void push(datastructure* ds_p, const void* data_p)
{

}

void push_entry(datastructure* ds_p, const void* key_p, const void* value_p)
{

}

void pop(datastructure* ds_p)
{

}

const void* get_top(datastructure* ds_p)
{

}

void delete_datastructure(datastructure* ds_p)
{

}