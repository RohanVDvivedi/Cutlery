#include<stdio.h>
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
		printf("(null)");
	else
		printf("%d, %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void sprint_ts(dstring* append_str, const void* tsv)
{
	if(tsv == NULL)
		snprintf_dstring(append_str, "(null)");
	else
		snprintf_dstring(append_str, "%d, %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

print_ts_hashmap(hashmap* hashmap_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_hashmap(&str, hashmap_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
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

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){1, 100, "one"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){2, 200, "two"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){3, 300, "there"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){4, 400, "four"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){5, 500, "five"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){6, 600, "six"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){7, 700, "seven"}));

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

	insert_in_hashmap(hashmap_p, &((ts){8, 800, "eight"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){9, 900, "nine"}));

	print_ts_hashmap(hashmap_p);

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

	print_ts_hashmap(hashmap_p);

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

	insert_in_hashmap(hashmap_p, &((ts){60, 6000, "sixty"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){70, 7000, "seventy"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){56, 5600, "fifty six"}));

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

	insert_in_hashmap(hashmap_p, &((ts){80, 8000, "eighty"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){20, 2000, "twenty"}));

	print_ts_hashmap(hashmap_p);

	insert_in_hashmap(hashmap_p, &((ts){40, 4000, "forty"}));

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
	insert_in_hashmap(hashmap_p, &((ts){90, 9000, "ninety"}));

	print_ts_hashmap(hashmap_p);

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

	resize_hashmap(hashmap_p, 10);

	printf("\n\nAfter rehashing - 10\n");
	print_ts_hashmap(hashmap_p);

	deinitialize_hashmap(hashmap_p);

	return 0;
}