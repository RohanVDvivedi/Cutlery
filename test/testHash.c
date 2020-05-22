#include<hashmap.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

typedef struct key ke;
struct key
{
	int k;
};

unsigned long long int hash_function(const void* key)
{
	return (unsigned long long int)((*((int *)key))-1);
}

int key_cmp(const void* key1, const void* key2)
{
	return (((ke*)key1)->k) - (((ke*)key2)->k);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
	{
		printf("(null)");
	}
	else
	{
		printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
	}
}

void print_key(const void* key)
{
	printf("%d hash(%llu)", (*((int*)key)), hash_function(key));
}

void insert_wrapper(const void* key, const void* value, const void* additional_params)
{
	insert_entry_in_hash(((hashmap*)(additional_params)), key, value);
}

void rehash(hashmap* old_p, hashmap* new_p)
{
	for_each_entry_in_hash(old_p, insert_wrapper, new_p);
}

#define POLICY_USED ROBINHOOD_HASHING /*ELEMENTS_AS_LINKEDLIST*/ /*ELEMENTS_AS_RED_BLACK_BST*/ /*ELEMENTS_AS_AVL_BST*/

#define HASH_BUCKETS 4

#if POLICY_USED == ROBINHOOD_HASHING
	#undef HASH_BUCKETS
	#define HASH_BUCKETS 10
#endif

#define USE_STACK_MEMORY
//#define USE_HEAP_MEMORY

int main()
{
	#if defined USE_STACK_MEMORY
		printf("HASHMAP WILL BE CREATED ON STACK MEMORY\n\n");
		hashmap hashmap_temp;
		hashmap* hashmap_p = &hashmap_temp;
		initialize_hashmap(hashmap_p, HASH_BUCKETS, hash_function, key_cmp, POLICY_USED);
	#elif defined USE_HEAP_MEMORY
		printf("HASHMAP WILL BE CREATED ON HEAP MEMORY\n\n");
		hashmap* hashmap_p = get_hashmap(HASH_BUCKETS, hash_function, key_cmp, POLICY_USED);
	#endif

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){1}), &((ts){100, "one"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){2}), &((ts){200, "two"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){3}), &((ts){300, "there"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	update_value_in_hash(hashmap_p, &((ke){3}), &((ts){300, "three"}), NULL);

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){4}), &((ts){400, "four"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){5}), &((ts){500, "five"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){6}), &((ts){600, "six"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){7}), &((ts){700, "seven"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){8}), &((ts){800, "eight"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){9}), &((ts){900, "nine"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("\nStarting to remove few entries\n\n");

	int nodes_deleted = 0;

	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){6}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){2}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){6}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){4}), NULL, NULL);

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_hashmap(hashmap_p, print_key, print_ts);

	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){6}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){7}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){5}), NULL, NULL);
	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){8}), NULL, NULL);

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_hashmap(hashmap_p, print_key, print_ts);nodes_deleted = 0;

	nodes_deleted += delete_entry_from_hash(hashmap_p, &((ke){9}), NULL, NULL);

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_hashmap(hashmap_p, print_key, print_ts);nodes_deleted = 0;

	printf("\nCompleted removing entries\n\n");

	insert_entry_in_hash(hashmap_p, &((ke){60}), &((ts){6000, "sixty"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){70}), &((ts){7000, "seventy"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){56}), &((ts){5600, "fifty six"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	update_value_in_hash(hashmap_p, &((ke){70}), &((ts){70000, "seven hundred"}), NULL);

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){80}), &((ts){8000, "eighty"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){20}), &((ts){2000, "twenty"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	insert_entry_in_hash(hashmap_p, &((ke){40}), &((ts){4000, "forty"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	update_value_in_hash(hashmap_p, &((ke){70}), &((ts){7000, "seventy"}), NULL);

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("Deleting key-value at 80\n");
	delete_entry_from_hash(hashmap_p, &((ke){80}), NULL, NULL);

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("Deleting key-value at 60\n");
	delete_entry_from_hash(hashmap_p, &((ke){60}), NULL, NULL);

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("Now finding value corresponding to key 20\n\n");
	print_ts(find_value_from_hash(hashmap_p, &((ke){20})));printf("\n\n");

	printf("Now finding value corresponding to key 40\n\n");
	print_ts(find_value_from_hash(hashmap_p, &((ke){40})));printf("\n\n");

	printf("Reinserting key-value at 80\n");
	insert_entry_in_hash(hashmap_p, &((ke){80}), &((ts){8000, "eighty - new"}));

	printf("Reinserting key-value at 60\n");
	insert_entry_in_hash(hashmap_p, &((ke){60}), &((ts){6000, "sixty - new"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("Inserting key-value at 90\n");
	insert_entry_in_hash(hashmap_p, &((ke){90}), &((ts){9000, "ninety"}));

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("\n\nBefore rehashing - 16\n");
	print_hashmap(hashmap_p, print_key, print_ts);

	hashmap* hashmap_16 = get_hashmap(16, hash_function, key_cmp, POLICY_USED);
	rehash(hashmap_p, hashmap_16);

	printf("\n\nAfter rehashing - 16\n");
	print_hashmap(hashmap_16, print_key, print_ts);

	hashmap* hashmap_20 = get_hashmap(20, hash_function, key_cmp, POLICY_USED);
	rehash(hashmap_p, hashmap_20);

	printf("\n\nAfter rehashing - 20\n");
	print_hashmap(hashmap_20, print_key, print_ts);

	#if defined USE_STACK_MEMORY
		deinitialize_hashmap(hashmap_p);
	#elif defined USE_HEAP_MEMORY
		delete_hashmap(hashmap_p);
	#endif

	delete_hashmap(hashmap_16);
	delete_hashmap(hashmap_20);

	return 0;
}