#include<stdio.h>
#include<cutlery_stds.h>
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

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%d %d, %s", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_smallest(const bst* bst_p)
{
	printf("the value for the smallest key is : ");
	const void* result = find_smallest_in_bst(bst_p);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

void print_largest(const bst* bst_p)
{
	printf("the value for the largest key is : ");
	const void* result = find_largest_in_bst(bst_p);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

void print_succeeding(const bst* bst_p, int k)
{
	printf("the value succeeding %d is : ", k);
	const void* result = find_succeeding_in_bst(bst_p, &k);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

void print_succeeding_equals(const bst* bst_p, int k)
{
	printf("the value succeeding or equals %d is : ", k);
	const void* result = find_succeeding_or_equals_in_bst(bst_p, &k);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

void print_preceding(const bst* bst_p, int k)
{
	printf("the value preceding %d is : ", k);
	const void* result = find_preceding_in_bst(bst_p, &k);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

void print_preceding_equals(const bst* bst_p, int k)
{
	printf("the value preceding or equals %d is : ", k);
	const void* result = find_preceding_or_equals_in_bst(bst_p, &k);
	if(result != NULL)
		print_ts(result);
	else
		printf("NULL");
	printf("\n");
}

int print_accumulate(const void* data, const void* additional_params)
{
	print_ts(data);printf("\n");

	// return of 1 (non-zero) means continue receiving results
	return 1;
	// return of 0 means to stop
	//return 0;
}

void print_all_in_range(const bst* bst_p, int small, int large, sort_direction sd, unsigned int count)
{
	printf("Atleast %u values in range [%d, %d] as %s are : \n", count, small, large, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	unsigned int n = find_all_in_range_in_bst(bst_p, &small, &large, sd, count, print_accumulate, NULL);
	printf("total : %u\n\n", n);
}

void print_all_greater_than_equals(const bst* bst_p, int k, sort_direction sd, unsigned int count)
{
	printf("Atleast %u values in range [%d, infinity) as %s are : \n", count, k, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	unsigned int n = find_all_in_range_in_bst(bst_p, &k, NULL, sd, count, print_accumulate, NULL);
	printf("total : %u\n\n", n);
}

void print_all_lesser_than_equals(const bst* bst_p, int k, sort_direction sd, unsigned int count)
{
	printf("Atleast %u values in range (-infinity, %d] %s are : \n", count, k, (sd == ASCENDING_ORDERED) ? "ASCENDING_ORDERED" : "DESCENDING_ORDERED");
	unsigned int n = find_all_in_range_in_bst(bst_p, NULL, &k, sd, count, print_accumulate, NULL);
	printf("total : %u\n\n", n);
}

void print_all_using_iterator_in_both_directions(const bst* bst_p)
{
	printf("\n\nUsing forward loop :\n");
	for(const void* it = find_smallest_in_bst(bst_p); it != NULL; it = get_inorder_next_of_in_bst(bst_p, it))
	{
		print_ts(it);
		printf("\n");
	}
	printf("\nforward loop end\n\n");

	printf("\nUsing backward loop :\n");
	for(const void* it = find_largest_in_bst(bst_p); it != NULL; it = get_inorder_prev_of_in_bst(bst_p, it))
	{
		print_ts(it);
		printf("\n");
	}
	printf("\nbackward loop end\n\n");
}

void print_ts_bst(bst* bst_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_bst(&str, bst_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define TREE_TYPE_TO_USE 	/*NON_SELF_BALANCING*/ AVL_TREE /*RED_BLACK_TREE*/

#define TEST_REMOVE_ALL_AFTER_INSERTS

int main()
{
	printf("INITIALIZING BALANCED BINARY SEARCH TREE\n");

	bst my_bst;
	bst* bst_p = &my_bst;
	initialize_bst(bst_p, TREE_TYPE_TO_USE, cmp, offsetof(ts, bst_embed_node));
	print_ts_bst(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	printf("COMPLETED INITIALIZING BALANCED BINARY SEARCH TREE\n");

	print_preceding(bst_p, 7);
	print_preceding_equals(bst_p, 7);
	print_succeeding_equals(bst_p, 7);
	print_succeeding(bst_p, 7);

	print_smallest(bst_p);
	print_largest(bst_p);
	printf("\n");

	printf("STARTING TO INSERT NODES\n");

	int error = 0;

	error = insert_in_bst(bst_p, &((ts){2, 2, "ttwwoo"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){1, 1, "oonnee"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){4, 4, "ffoouurr"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){3, 3, "tthhrreeee"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){1}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	// ------- simple tree test case ----

	error = insert_in_bst(bst_p, &((ts){18, 18, "eighteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){17, 17, "seventeen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){16, 16, "sixteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){15, 15, "fifteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_preceding(bst_p, 12);
	print_preceding_equals(bst_p, 12);
	print_succeeding_equals(bst_p, 12);
	print_succeeding(bst_p, 12);

	print_preceding(bst_p, 19);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);
	print_succeeding(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	error = insert_in_bst(bst_p, &((ts){14, 14, "fourteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){13, 13, "thirteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){12, 12, "twelve"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_preceding(bst_p, 11);
	print_preceding_equals(bst_p, 11);
	print_succeeding_equals(bst_p, 11);
	print_succeeding(bst_p, 11);

	print_preceding(bst_p, 12);
	print_preceding_equals(bst_p, 12);
	print_succeeding_equals(bst_p, 12);
	print_succeeding(bst_p, 12);

	print_preceding(bst_p, 15);
	print_preceding_equals(bst_p, 15);
	print_succeeding_equals(bst_p, 15);
	print_succeeding(bst_p, 15);

	print_preceding(bst_p, 19);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);
	print_succeeding(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){11, 11, "eleven"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){10, 10, "ten"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){8, 8, "eight"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){0, 0, "zero"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_preceding(bst_p, 0);
	print_preceding_equals(bst_p, 0);
	print_succeeding_equals(bst_p, 0);
	print_succeeding(bst_p, 0);

	print_preceding(bst_p, 2);
	print_preceding_equals(bst_p, 2);
	print_succeeding_equals(bst_p, 2);
	print_succeeding(bst_p, 2);

	print_preceding(bst_p, 3);
	print_preceding_equals(bst_p, 3);
	print_succeeding_equals(bst_p, 3);
	print_succeeding(bst_p, 3);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){1, 1, "one"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){2, 2, "two"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){3, 3, "three"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = insert_in_bst(bst_p, &((ts){4, 4, "four"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine-lol-2"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){5, 5, "five"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){6, 6, "six"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_preceding(bst_p, 7);
	print_preceding_equals(bst_p, 7);
	print_succeeding_equals(bst_p, 7);
	print_succeeding(bst_p, 7);

	error = insert_in_bst(bst_p, &((ts){7, 7, "seven"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){19, 19,  "nineteen"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	printf("\n");

	printf("Updating key = 19 from 19 19 nineteen to 19 19 LOL\n");
	printf("Demostrating update procedure\n");
	printf("-> First find the data with key = 19 from the bst\n");
	ts* to_update = (void*) find_equals_in_bst(bst_p, &((ts){19}), FIRST_OCCURENCE);
	printf("-> Then we removed the data (node) form the bst : return = %d\n", remove_from_bst(bst_p, to_update));
	printf("-> Trying to remove the same data (node) again from the bst : return = %d\n", remove_from_bst(bst_p, to_update));
	print_ts(to_update);printf("\n");
	to_update->s = "LOL";
	print_ts(to_update);printf("\n");
	printf("-> Inserting the same data (node) with updated value in the bst : return = %d\n", insert_in_bst(bst_p, to_update));
	printf("-> Trying to insert the same data (node) again in the bst : return = %d\n", insert_in_bst(bst_p, to_update));
	print_ts_bst(bst_p);

	print_preceding(bst_p, 19);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);
	print_succeeding(bst_p, 19);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine-lol-0"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	error = insert_in_bst(bst_p, &((ts){9, 9, "nine-lol-1"}));
	printf("node inserted = %d\n", error);
	print_ts_bst(bst_p);

	print_all_lesser_than_equals(bst_p, 3, ASCENDING_ORDERED, 3);
	print_all_lesser_than_equals(bst_p, 3, DESCENDING_ORDERED, 3);
	print_all_lesser_than_equals(bst_p, -1, ASCENDING_ORDERED, 3);
	print_all_in_range(bst_p, 5, 15, ASCENDING_ORDERED, 7);
	print_all_in_range(bst_p, 5, 15, DESCENDING_ORDERED, 7);
	print_all_greater_than_equals(bst_p, 14, ASCENDING_ORDERED, 5);
	print_all_greater_than_equals(bst_p, 14, DESCENDING_ORDERED, 5);
	print_all_greater_than_equals(bst_p, 20, DESCENDING_ORDERED, 5);

	print_all_using_iterator_in_both_directions(bst_p);

	printf("COMPLETED INSERTING NODES\n");

	#ifdef TEST_REMOVE_ALL_AFTER_INSERTS
	{
		printf("REMOVING ALL NODES\n");
		singlylist removed_datas;
		remove_all_from_bst(bst_p, &removed_datas);

		printf("is bst now empty = %d\n", is_empty_bst(bst_p));

		printf("PRINTING ALL REMOVED NODES\n");
		while(!is_empty_singlylist(&removed_datas))
		{
			const ts* tsv = get_head_of_singlylist(&removed_datas);
			remove_head_from_singlylist(&removed_datas);

			print_ts(tsv);
			printf("is its bstnode free floating = %d\n\n", is_free_floating_bstnode(&(tsv->bst_embed_node)));
		}

		printf("COMPLETED REMOVING ALL NODES\n");

		return 0;
	}
	#endif

	printf("STARTING TO REMOVE NODES\n");

	printf("\nFirst we try to remove two nodes which compare equal to  some data (node) already in the bst, but these pointers (data) are not themselves existing in the bst\n");
	printf("As you predict this both operation would Fail, since you need to find or already know exactly, which node from the bst you want to remove\n\n");

	error = remove_from_bst(bst_p, &((ts){-1}));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, &((ts){0}));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	printf("\nFor all the below operations, we first call a find with equal data\n");
	printf("And then on the found node, which exists in the bst, we call remove, and these operations will now succeed\n\n");

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){0}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){1}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){2}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){7}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){14}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){15}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	print_preceding(bst_p, 9);
	print_preceding_equals(bst_p, 9);
	print_succeeding_equals(bst_p, 9);
	print_succeeding(bst_p, 9);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){16}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){13}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){10}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	printf("\nremoving first and last 9\n");

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9}), LAST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){8}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){11}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){12}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){3}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_preceding(bst_p, 0);
	print_preceding_equals(bst_p, 0);
	print_succeeding_equals(bst_p, 0);
	print_succeeding(bst_p, 0);

	print_preceding(bst_p, 3);
	print_preceding_equals(bst_p, 3);
	print_succeeding_equals(bst_p, 3);
	print_succeeding(bst_p, 3);

	print_preceding(bst_p, 6);
	print_preceding_equals(bst_p, 6);
	print_succeeding_equals(bst_p, 6);
	print_succeeding(bst_p, 6);

	print_preceding(bst_p, 9);
	print_preceding_equals(bst_p, 9);
	print_succeeding_equals(bst_p, 9);
	print_succeeding(bst_p, 9);

	print_preceding(bst_p, 15);
	print_preceding_equals(bst_p, 15);
	print_succeeding_equals(bst_p, 15);
	print_succeeding(bst_p, 15);

	print_preceding(bst_p, 19);
	print_preceding_equals(bst_p, 19);
	print_succeeding_equals(bst_p, 19);
	print_succeeding(bst_p, 19);

	print_smallest(bst_p);
	print_largest(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){4}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){5}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){6}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){17}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){18}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){19}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	printf("\nremoving last and first 9\n");

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9}), LAST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){9}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){2}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){3}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	error = remove_from_bst(bst_p, find_equals_in_bst(bst_p, &((ts){4}), FIRST_OCCURENCE));
	printf("node removed = %d\n", error);
	print_ts_bst(bst_p);

	print_smallest(bst_p);
	print_largest(bst_p);

	print_all_using_iterator_in_both_directions(bst_p);

	printf("COMPLETED REMOVING NODES\n");

	return 0;
}