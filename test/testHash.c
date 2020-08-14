#include<hashmap.h>

#include<linkedlist.h>
#include<bst.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;
	int a;
	char* s;

	union
	{
		bstnode embed_node1;
		llnode  embed_node2;
	}embedded_nodes;
};

unsigned int hash_function(const void* data)
{
	return (unsigned int)((((ts*)data)->key)-1);
}

int cmp(const void* data1, const void* data2)
{
	return (((ts*)data1)->key) - (((ts*)data2)->key);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
	{
		printf("(null)");
	}
	else
	{
		printf("%d, %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
	}
}

#include<queue.h>

void sampler_queue_wrapper(const void* data, const void* additional_params)
{
	push_queue((queue*)(additional_params), data);
}

void rehash(hashmap* old_p, hashmap* new_p)
{
	queue q;
	initialize_queue(&q, 30);
	for_each_in_hashmap(old_p, sampler_queue_wrapper, &q);

	while(!isQueueEmpty(&q))
	{
		const void* data = get_top_queue(&q);

		remove_from_hashmap(old_p, data);
		insert_in_hashmap(new_p, data);

		pop_queue(&q);
	}

	deinitialize_queue(&q);
}

const collision_resolution_policy POLICY_USED = ROBINHOOD_HASHING /*ELEMENTS_AS_LINKEDLIST*/ /*ELEMENTS_AS_RED_BLACK_BST*/ /*ELEMENTS_AS_AVL_BST*/;

unsigned int HASH_BUCKETS = 4;

unsigned int NODE_OFFSET = (unsigned long long int)(&(((ts*)0)->embedded_nodes));

int main()
{
	hashmap hashmap_temp;
	hashmap* hashmap_p = &hashmap_temp;

	if(POLICY_USED == ROBINHOOD_HASHING)
	{
		HASH_BUCKETS = 10;
	}

	initialize_hashmap(hashmap_p, POLICY_USED, HASH_BUCKETS, hash_function, cmp, NODE_OFFSET);

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){1, 100, "one"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){2, 200, "two"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){3, 300, "there"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){4, 400, "four"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){5, 500, "five"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){6, 600, "six"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){7, 700, "seven"}));

	print_hashmap(hashmap_p, print_ts);

	printf("We want to update the data with key = 3\n");

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	ts* thr = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){3}));
	printf("-> we find the data in the hashmap\n");
	printf("-> we remove the data that we found, from the hashmap, removed : %d\n", remove_from_hashmap(hashmap_p, thr));
	printf("-> we try to again remove from the hashmap, this time the remove must fail, removed : %d\n", remove_from_hashmap(hashmap_p, thr));
	printf("-> now make actual update to the data\n");
	thr->s = "Three";
	printf("-> insert the data back into the hashmap, inserted : %d\n", insert_in_hashmap(hashmap_p, thr));
	printf("-> Try to again insert the data back into the hashmap, this time it must fail, inserted : %d\n", insert_in_hashmap(hashmap_p, thr));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){8, 800, "eight"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){9, 900, "nine"}));

	print_hashmap(hashmap_p, print_ts);

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

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_hashmap(hashmap_p, print_ts);

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

	print_hashmap(hashmap_p, print_ts);nodes_deleted = 0;

	temp = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){9}));
	if(temp != NULL)
	{
		nodes_deleted += remove_from_hashmap(hashmap_p, temp);
	}

	printf("\nnodes deleted : %d\n\n", nodes_deleted);nodes_deleted = 0;

	print_hashmap(hashmap_p, print_ts);nodes_deleted = 0;

	printf("\nCompleted removing entries\n\n");

	insert_in_hashmap(hashmap_p, &((ts){60, 6000, "sixty"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){70, 7000, "seventy"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){56, 5600, "fifty six"}));

	print_hashmap(hashmap_p, print_ts);

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	ts* svntn = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){70}));
	svntn->a = 70000;
	svntn->s = "seven hundred";

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){80, 8000, "eighty"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){20, 2000, "twenty"}));

	print_hashmap(hashmap_p, print_ts);

	insert_in_hashmap(hashmap_p, &((ts){40, 4000, "forty"}));

	print_hashmap(hashmap_p, print_ts);

	// this is how you update, unless you want to update the key of the object
	// in which case you would have to remove the element completely and reinsert
	svntn = (ts*)find_equals_in_hashmap(hashmap_p, &((ts){70}));
	svntn->a = 7000;
	svntn->s = "seventy";

	print_hashmap(hashmap_p, print_ts);

	printf("Deleting key-value at 80\n");
	ts* temp_80 = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){80}));
	if(temp != NULL)
	{
		remove_from_hashmap(hashmap_p, temp_80);
	}

	print_hashmap(hashmap_p, print_ts);

	printf("Deleting key-value at 60\n");
	ts* temp_60 = (ts*) find_equals_in_hashmap(hashmap_p, &((ts){60}));
	if(temp != NULL)
	{
		remove_from_hashmap(hashmap_p, temp_60);
	}

	print_hashmap(hashmap_p, print_ts);

	printf("Now finding value corresponding to key 20\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){20})));printf("\n\n");

	printf("Now finding value corresponding to key 40\n\n");
	print_ts(find_equals_in_hashmap(hashmap_p, &((ts){40})));printf("\n\n");

	printf("Reinserting key-value at 80\n");
	temp_80->s = "eighty - new";
	insert_in_hashmap(hashmap_p, temp_80);

	printf("Reinserting key-value at 60\n");
	temp_60->s = "sixty - new";
	insert_in_hashmap(hashmap_p, temp_60);

	print_hashmap(hashmap_p, print_ts);

	printf("Inserting key-value at 90\n");
	insert_in_hashmap(hashmap_p, &((ts){90, 9000, "ninety"}));

	print_hashmap(hashmap_p, print_ts);

	printf("\n\nBefore rehashing - 16\n");
	print_hashmap(hashmap_p, print_ts);

	hashmap hashmap_16;
	hashmap* hashmap_16_p = &hashmap_16;
	initialize_hashmap(hashmap_16_p, POLICY_USED, 16, hash_function, cmp, NODE_OFFSET);

	rehash(hashmap_p, hashmap_16_p);

	printf("\n\nAfter rehashing - 16\n");
	print_hashmap(hashmap_16_p, print_ts);

	hashmap hashmap_20;
	hashmap* hashmap_20_p = &hashmap_20;
	initialize_hashmap(hashmap_20_p, POLICY_USED, 20, hash_function, cmp, NODE_OFFSET);

	rehash(hashmap_16_p, hashmap_20_p);

	printf("\n\nAfter rehashing - 20\n");
	print_hashmap(hashmap_20_p, print_ts);

	deinitialize_hashmap(hashmap_p);
	deinitialize_hashmap(hashmap_16_p);
	deinitialize_hashmap(hashmap_20_p);

	return 0;
}