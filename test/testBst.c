#include<stdio.h>
#include<bst.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;
	int a;
	char* s;

	bstnode bst_embed_node;
};

// note : how we compare the structures only based on their key values
int cmp(const void* data1, const void* data2)
{
	return (((ts*)data1)->key) - (((ts*)data2)->key);
}

void print_ts(const void* tsv)
{
	printf("%d %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_smallest(const bst* bst_p)
{
	printf("the value for the smallest key is : ");
	const void* result = find_smallest(bst_p);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_largest(const bst* bst_p)
{
	printf("the value for the largest key is : ");
	const void* result = find_largest(bst_p);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_succeeding_equals(const bst* bst_p, int k)
{
	printf("the value succeeding or equals %d is : ", k);
	const void* result = find_succeeding_or_equals(bst_p, &k);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_preceding_equals(const bst* bst_p, int k)
{
	printf("the value preceding or equals %d is : ", k);
	const void* result = find_preceding_or_equals(bst_p, &k);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_accumulate(const void* data, const void* additional_params){print_ts(data);printf("\n");}

void print_all_in_range(const bst* bst_p, int small, int large, unsigned int count, sort_direction sd)
{
	printf("Atleast %u values in range [%d, %d] as %s are : \n", count, small, large, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	find_all_in_range(bst_p, &small, &large, count, sd, print_accumulate, NULL);
	printf("\n");
}

void print_all_greater_than_equals(const bst* bst_p, int k, unsigned int count, sort_direction sd)
{
	printf("Atleast %u values in range [%d, infinity) as %s are : \n", count, k, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	find_all_in_range(bst_p, &k, NULL, count, sd, print_accumulate, NULL);
	printf("\n");
}

void print_all_lesser_than_equals(const bst* bst_p, int k, unsigned int count, sort_direction sd)
{
	printf("Atleast %u values in range (-infinity, %d] %s are : \n", count, k, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	find_all_in_range(bst_p, NULL, &k, sd, count, print_accumulate, NULL);
	printf("\n");
}

#define TREE_TYPE_TO_USE 	/*NON_SELF_BALANCING*/ AVL_TREE /*RED_BLACK_TREE*/

int main()
{
	printf("INITIALIZING BALANCED BINARY SEARCH TREE\n");

	bst my_bst;
	bst* bst_p = &my_bst;
	initialize_bst(bst_p, TREE_TYPE_TO_USE, (unsigned long long int)(&(((ts*)0)->bst_embed_node)), cmp);
	print_bst(bst_p, print_ts);

	printf("COMPLETED INITIALIZING BALANCED BINARY SEARCH TREE\n");

	print_smallest(bst_p);
	print_largest(bst_p);

	printf("STARTING TO INSERT NODES\n");

	int error = 0;

	error = insert_in_bst(bst_p, &((ts){18, 18, "eighteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){17, 17, "seventeen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){16, 16, "sixteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){15, 15, "fifteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 12);
	print_succeeding_equals(bst_p, 12);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){14, 14, "fourteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){13, 13, "thirteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){12, 12, "twelve"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 11);
	print_succeeding_equals(bst_p, 11);
	print_preceding_equals(bst_p, 12);
	print_succeeding_equals(bst_p, 12);
	print_preceding_equals(bst_p, 15);
	print_succeeding_equals(bst_p, 15);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){11, 11, "eleven"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){10, 10, "ten"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){8, 8, "eight"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){0, 0, "zero"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 0);
	print_succeeding_equals(bst_p, 0);
	print_preceding_equals(bst_p, 2);
	print_succeeding_equals(bst_p, 2);
	print_preceding_equals(bst_p, 3);
	print_succeeding_equals(bst_p, 3);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){1, 1, "one"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){2, 2, "two"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){3, 3, "three"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){4, 4, "four"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){5, 5, "five"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){6, 6, "six"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 7);
	print_succeeding_equals(bst_p, 7);

	error = insert_in_bst(bst_p, &((ts){7, 7, "seven"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	error = insert_in_bst(bst_p, &((ts){19, 19,  "nineteen"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	printf("\n");

	printf("Updating key = 19 from 19 19 nineteen to 19 19 LOL\n");
	printf("Demostrating update procedure\n");
	printf("-> First find the data with key = 19 from the bst\n");
	ts* to_update = (void*) find_equals_in_bst(bst_p, &((ts){19}));
	printf("-> Then we removed the data (node) form the bst : return = %d\n", remove_from_bst(bst_p, to_update));
	printf("-> Trying to remove the same data (node) again from the bst : return = %d\n", remove_from_bst(bst_p, to_update));
	print_ts(to_update);printf("\n");
	to_update->s = "LOL";
	print_ts(to_update);printf("\n");
	printf("-> Inserting the same data (node) with updated value in the bst : return = %d\n", insert_in_bst(bst_p, to_update));
	printf("-> Trying to insert the same data (node) again in the bst : return = %d\n", insert_in_bst(bst_p, to_update));
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine-lol"}));
	printf("node inserted = %d\n", error);
	print_bst(bst_p, print_ts);

	print_all_lesser_than_equals(bst_p, 3, 3, ASCENDING_ORDERED);
	print_all_lesser_than_equals(bst_p, 3, 3, DESCENDING_ORDERED);
	print_all_in_range(bst_p, 5, 15, 7, ASCENDING_ORDERED);
	print_all_in_range(bst_p, 5, 15, 7, DESCENDING_ORDERED);
	print_all_greater_than_equals(bst_p, 14, 5, ASCENDING_ORDERED);
	print_all_greater_than_equals(bst_p, 14, 5, DESCENDING_ORDERED);return 0;

	printf("COMPLETED INSERTING NODES\n");

	printf("STARTING TO REMOVE NODES\n");

	printf("\nFirst we try to remove two nodes which compare equal to  some data (node) already in the bst, but these pointers (data) are not themselves existing in the bst\n");
	printf("As you predict this both operation would Fail, since you need to find or already know exactly, which node from the bst you want to remove\n\n");

	error = remove_from_bst(bst_p, &((ts){-1}));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, &((ts){0}));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	printf("\nFor all the below operations, we first call a find with equal data\n");
	printf("And then on the found node, which exists in the bst, we call remove, and these operations will now succeed\n\n");

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){0})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){1})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){2})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){7})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){14})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){15})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){16})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){13})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){10})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){8})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){11})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){12})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){3})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_preceding_equals(bst_p, 0);
	print_succeeding_equals(bst_p, 0);
	print_preceding_equals(bst_p, 3);
	print_succeeding_equals(bst_p, 3);
	print_preceding_equals(bst_p, 6);
	print_succeeding_equals(bst_p, 6);
	print_preceding_equals(bst_p, 9);
	print_succeeding_equals(bst_p, 9);
	print_preceding_equals(bst_p, 15);
	print_succeeding_equals(bst_p, 15);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){4})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){5})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){6})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){17})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){18})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){19})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9})));
	printf("node removed = %d\n", error);
	print_bst(bst_p, print_ts);

	print_smallest(bst_p);
	print_largest(bst_p);

	printf("COMPLETED REMOVING NODES\n");

	return 0;
}