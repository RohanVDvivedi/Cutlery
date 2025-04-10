#include<stdio.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/hashmap.h>

#include<cutlery/linkedlist.h>
#include<cutlery/bst.h>

//#define TEST_ROBINHOOD_HASHING
//#define TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD
//#define TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL
#define TEST_ELEMENTS_AS_RED_BLACK_BST
//#define TEST_ELEMENTS_AS_AVL_BST

typedef struct teststruct ts;
struct teststruct
{
	int key;
	int a;
	char* s;

	union
	{
		rbhnode embed_node1;
		llnode  embed_node2;
		bstnode embed_node3;
	}embedded_nodes;
};

cy_uint hash_function(const void* data)
{
	return (cy_uint)((((ts*)data)->key)-1);
}

int cmp(const void* data1, const void* data2)
{
	return (((ts*)data1)->key) - (((ts*)data2)->key);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
		printf("(null)");
	else
		printf("%d, %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void on_remove_all_println_ts_with_node(void* resource_p, const void* tsv)
{
	#if defined TEST_ROBINHOOD_HASHING
		printf("%d %d, %s :: is_free_floating_rbhnode = %d\n", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, is_free_floating_rbhnode(&(((ts*)tsv)->embedded_nodes.embed_node1)));
	#elif defined TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD || defined TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL
		printf("%d %d, %s :: is_free_floating_llnode = %d\n", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, is_free_floating_llnode(&(((ts*)tsv)->embedded_nodes.embed_node2)));
	#elif defined TEST_ELEMENTS_AS_AVL_BST || defined TEST_ELEMENTS_AS_RED_BLACK_BST
		printf("%d %d, %s :: is_free_floating_bstnode = %d\n", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, is_free_floating_bstnode(&(((ts*)tsv)->embedded_nodes.embed_node3)));
	#endif
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	if(tsv == NULL)
		snprintf_dstring(append_str, "(null)");
	else
		snprintf_dstring(append_str, "%d, %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_ts_hashmap(const hashmap* hashmap_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_hashmap(&str, hashmap_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void print_hashmap_bucket_wise(const hashmap* hashmap_p)
{
	printf("printing all elements :: \n");
	for(const void* data = get_first_of_in_hashmap(hashmap_p, FIRST_OF_HASHMAP); data != NULL; data = get_next_of_in_hashmap(hashmap_p, data, ANY_IN_HASHMAP))
	{
		printf("\t");
		print_ts(data);
		printf("\n");
	}
	printf("\n");

	printf("printing all elements bucket_wise :: \n");
	for(unsigned int bucket_index = 0; bucket_index < get_bucket_count_hashmap(hashmap_p); bucket_index++)
	{
		printf("\tbucket_no : %u\n", bucket_index);
		for(const void* data = get_first_of_in_hashmap(hashmap_p, bucket_index); data != NULL; data = get_next_of_in_hashmap(hashmap_p, data, ANY_IN_SAME_BUCKET))
		{
			printf("\t\t");
			print_ts(data);
			printf("\n");
		}
	}
	printf("\n");
}

void print_all_that_equals_from_hashmap(const hashmap* hashmap_p, const void* data)
{
	printf("printing all elements that compare equal to ");
	print_ts(data);
	printf("\n");
	for(const void* d = find_equals_in_hashmap(hashmap_p, data); d != NULL; d = get_next_of_in_hashmap(hashmap_p, d, ANY_THAT_EQUALS))
	{
		printf("\t");
		print_ts(d);
		printf("\n");
	}
	printf("\n");
}

#if defined TEST_ROBINHOOD_HASHING
	#define POLICY_USED ROBINHOOD_HASHING
#elif defined TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD
	#define POLICY_USED ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD
#elif defined TEST_ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL
	#define POLICY_USED ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL
#elif defined TEST_ELEMENTS_AS_RED_BLACK_BST
	#define POLICY_USED ELEMENTS_AS_RED_BLACK_BST
#elif defined TEST_ELEMENTS_AS_AVL_BST
	#define POLICY_USED ELEMENTS_AS_AVL_BST
#else
	#error "INVALID COLLISION RESOLUTION POLICY"
#endif

#if defined TEST_ROBINHOOD_HASHING
	#define INIT_EMBED_NODE {INVALID_INDEX}
	#define HASH_BUCKETS 12
#else
	#define INIT_EMBED_NODE {}
	#define HASH_BUCKETS 4
#endif

int main()
{
	printf("MACRO PARAMS => %d %u\n", POLICY_USED, HASH_BUCKETS);

	hashmap hashmap_temp;
	hashmap* hashmap_p = &hashmap_temp;

	initialize_hashmap(hashmap_p, POLICY_USED, HASH_BUCKETS, &simple_hasher(hash_function), &simple_comparator(cmp), offsetof(ts, embedded_nodes));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){1, 100, "one", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){2, 200, "two", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){3, 300, "there", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){4, 400, "four", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){5, 500, "five", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){6, 600, "six", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){7, 700, "seven", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	printf("We want to update the data with key = 3\n");

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	ts* thr = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){3}));
	if(thr != NULL)
	{
		printf("-> we found the data in the hashmap\n");
		printf("-> we remove the data that we found, from the hashmap, removed : %d\n", remove_from_hashmap(hashmap_p, thr));
		printf("-> we try to again remove from the hashmap, this time the remove must fail, removed : %d\n", remove_from_hashmap(hashmap_p, thr));
		printf("-> now make actual update to the data\n");
		thr->s = "Three";
		printf("-> insert the data back into the hashmap, inserted : %d\n", insert_in_hashmap(hashmap_p, thr));
		printf("-> Try to again insert the data back into the hashmap, this time it must fail, inserted : %d\n\n", insert_in_hashmap(hashmap_p, thr));
	}
	else
	{
		printf("-> we couldn't find the data in the hashmap\n\n");
	}
	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){8, 800, "eight", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){9, 900, "nine", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	print_hashmap_bucket_wise(hashmap_p);

	printf("\nStarting to remove few entries\n\n");

	int nodes_deleted = 0;
	ts* temp = NULL;

	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){6}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){2}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){6}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){4}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}

	print_ts_hashmap(hashmap_p);

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	insert_in_hashmap(hashmap_p, &((ts){15, 333, "333", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){3, 333, "333", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){15, 1515, "1515", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	print_hashmap_bucket_wise(hashmap_p);

	print_all_that_equals_from_hashmap(hashmap_p, &((ts){3}));

	print_all_that_equals_from_hashmap(hashmap_p, &((ts){15}));

	// this is exactly how we remove by finding the object from the hashmap first
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){6}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){7}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){5}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}
	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){8}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_ts_hashmap(hashmap_p);nodes_deleted = 0;

	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){9}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_ts_hashmap(hashmap_p);nodes_deleted = 0;

	printf("\nCompleted removing entries\n\n");

	insert_in_hashmap(hashmap_p, &((ts){60, 6000, "sixty", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){70, 7000, "seventy", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){56, 5600, "fifty six", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	printf("Updating the value of data with key = 70\n\n");
	ts* svntn = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){70}));
	if(svntn != NULL)
	{
		svntn->a = 70000;
		svntn->s = "seven hundred";
	}
	else
	{
		printf("Update failed; key = 70 absent\n\n");
	}

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){80, 8000, "eighty", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){20, 2000, "twenty", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){40, 4000, "forty", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	printf("Updating the value of data with key = 70\n\n");
	svntn = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){70}));
	if(svntn != NULL)
	{
		svntn->a = 7000;
		svntn->s = "seventy";
	}
	else
	{
		printf("Update failed; key = 70 absent\n\n");
	}

	print_ts_hashmap(hashmap_p);

	printf("Now finding value corresponding to key 56\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){56})));printf("\n\n");

	printf("Now finding value corresponding to key 57\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){57})));printf("\n\n");

	printf("Now finding value corresponding to key 60\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){60})));printf("\n\n");

	printf("Now finding value corresponding to key 10\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){10})));printf("\n\n");

	printf("Deleting key-value at 80, if found\n\n");
	ts* temp_80 = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){80}));
	if(temp_80 != NULL)
	{
		remove_from_hashmap(hashmap_p, temp_80);
	}
	else
	{
		printf("key-value at 80 not found\n\n");
	}

	print_ts_hashmap(hashmap_p);

	printf("Deleting key-value at 60, if found\n\n");
	ts* temp_60 = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){60}));
	if(temp_60 != NULL)
	{
		remove_from_hashmap(hashmap_p, temp_60);
	}
	else
	{
		printf("key-value at 60 not found\n\n");
	}

	print_ts_hashmap(hashmap_p);

	printf("Now finding value corresponding to key 20\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){20})));printf("\n\n");

	printf("Now finding value corresponding to key 40\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){40})));printf("\n\n");

	printf("Now finding value corresponding to key 56\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){56})));printf("\n\n");

	printf("Now finding value corresponding to key 57\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){57})));printf("\n\n");

	printf("Now finding value corresponding to key 60\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){60})));printf("\n\n");

	printf("Now finding value corresponding to key 10\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){10})));printf("\n\n");

	if(temp_80 != NULL)
	{
		printf("Reinserting key-value at 80\n");
		temp_80->s = "eighty - new";
		insert_in_hashmap(hashmap_p, temp_80);
	}

	if(temp_60 != NULL)
	{
		printf("Reinserting key-value at 60\n");
		temp_60->s = "sixty - new";
		insert_in_hashmap(hashmap_p, temp_60);
	}

	print_ts_hashmap(hashmap_p);

	printf("Inserting key-value at 90\n");
	insert_in_hashmap(hashmap_p, &((ts){90, 9000, "ninety", INIT_EMBED_NODE}));

	print_ts_hashmap(hashmap_p);

	printf("Now finding value corresponding to key 1\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){1})));printf("\n\n");

	print_hashmap_bucket_wise(hashmap_p);

	print_all_that_equals_from_hashmap(hashmap_p, &((ts){3}));

	print_all_that_equals_from_hashmap(hashmap_p, &((ts){15}));

	printf("\n\nBefore rehashing - 16\n");
	print_ts_hashmap(hashmap_p);

	resize_hashmap(hashmap_p, 16);

	printf("\n\nAfter rehashing - 16\n");
	print_ts_hashmap(hashmap_p);

	resize_hashmap(hashmap_p, 20);

	printf("\n\nAfter rehashing - 20\n");
	print_ts_hashmap(hashmap_p);

	expand_hashmap(hashmap_p, 1.2);

	printf("\n\nAfter rehashing - 20 * 1.2\n");
	print_ts_hashmap(hashmap_p);

	print_hashmap_bucket_wise(hashmap_p);

	print_all_that_equals_from_hashmap(hashmap_p, &((ts){3}));

	resize_hashmap(hashmap_p, 10);

	printf("\n\nAfter rehashing - 10\n");
	print_ts_hashmap(hashmap_p);

	remove_all_from_hashmap(hashmap_p, &((notifier_interface){NULL, on_remove_all_println_ts_with_node}));

	printf("\n\nAfter removing all elements\n");
	print_ts_hashmap(hashmap_p);

	deinitialize_hashmap(hashmap_p);

	return 0;
}