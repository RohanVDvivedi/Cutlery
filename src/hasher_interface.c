#include<hasher_interface.h>

int hash_with_hasher(const hasher_interface* hasher, const void* data)
{
	if(hasher->context == NULL)
		return hasher->hash1(data);
	else
		return hasher->hash2(hasher->context, data);
}