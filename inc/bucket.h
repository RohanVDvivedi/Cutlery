#ifndef BUCKET_H
#define BUCKET_H

typedef struct bucket bucket;
struct bucket
{
	// the key of the bucket
	const void* key;

	// value of the bucket
	const void* value;
};

// get a new bucket
const bucket* get_bucket(const void* key, const void* value);

// delete a bucket pointed to by bucket_p
void delete_bucket(bucket* bucket_p);

#endif