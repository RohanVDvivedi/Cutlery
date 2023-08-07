#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include<arraylist.h>

void print_int(const void* data)
{
	if(data != NULL)
		printf("%d", *((int*)data));
	else
		printf("NULL - INT");
}

void sprint_int(dstring* append_str, const void* data, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	if(data != NULL)
		snprintf_dstring(append_str, "%d", *((int*)data));
	else
		snprintf_dstring(append_str, "NULL - INT");
}

void print_int_arraylist(arraylist* al)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_arraylist(&str, al, sprint_int, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define TOTAL_OPERATIONS_SUPPORTED 14

typedef enum al_op al_op;
enum al_op
{
	PUSH_FRONT = 0,
	PUSH_BACK,		// 1
	POP_FRONT,		// 2
	POP_BACK,		// 3
	GET_NTH_FRONT,	// 4
	GET_NTH_BACK,	// 5
	SET_NTH_FRONT,	// 6
	SET_NTH_BACK,	// 7
	SWAP_FRONT,		// 8
	SWAP_BACK,		// 9
	INSERT_FRONT,	// 10
	INSERT_BACK,	// 11
	REMOVE_FRONT,	// 12
	REMOVE_BACK,	// 13
};

#define INITIAL_TOTAL_SIZE 12
#define NUMBER_OF_OPERATIONS 128
#define ELEMENT_POOL_SIZE 128

int element_pool[ELEMENT_POOL_SIZE];

int non_zero_rand()
{
	int r = 0;
	while((r = rand()) == 0);
	return (r + 13169598) * 13169598;
}

void operate_on_arraylist(arraylist* al, al_op op)
{
	switch(op)
	{
		case PUSH_FRONT :
		{
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_FRONT : %d :: %d\n", *data, push_front_to_arraylist(al, data));
			break;
		}

		case PUSH_BACK :
		{
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_BACK : %d :: %d\n", *data, push_back_to_arraylist(al, data));
			break;
		}

		case POP_FRONT :
		{
			printf("POP_FRONT :: %d\n", pop_front_from_arraylist(al));
			break;
		}

		case POP_BACK :
		{
			printf("POP_BACK :: %d\n", pop_back_from_arraylist(al));
			break;
		}

		case GET_NTH_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = get_from_front_of_arraylist(al, index);
			printf("GET %u TH_FRONT : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case GET_NTH_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = get_from_back_of_arraylist(al, index);
			printf("GET %u TH_BACK : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case SET_NTH_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("SET %u TH_FRONT : %d : %d\n", index, *data, set_from_front_in_arraylist(al, data, index));
			break;
		}

		case SET_NTH_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("SET %u TH_BACK : %d : %d\n", index, *data, set_from_back_in_arraylist(al, data, index));
			break;
		}

		case SWAP_FRONT :
		{
			unsigned int i1 = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.005));
			unsigned int i2 = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.005));
			printf("SWAP_FRONT %u and %u : %d\n", i1, i2, swap_from_front_in_arraylist(al, i1, i2));
			break;
		}

		case SWAP_BACK :
		{
			unsigned int i1 = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.005));
			unsigned int i2 = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.005));
			printf("SWAP_BACK %u and %u : %d\n", i1, i2, swap_from_back_in_arraylist(al, i1, i2));
			break;
		}

		case INSERT_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)(get_element_count_arraylist(al) + 2));
			unsigned int nulls_to_insert = ((unsigned int)(non_zero_rand())) % ((unsigned int)(get_capacity_arraylist(al) - get_element_count_arraylist(al)));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			int res = insert_NULLs_from_front_in_arraylist(al, index, nulls_to_insert);
			for(cy_uint i = 0; i < nulls_to_insert; i++)
				set_from_front_in_arraylist(al, data, index + i);
			printf("INSERT_FRONT %u %u : %d\n", index, nulls_to_insert, res);
			break;
		}

		case INSERT_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)(get_element_count_arraylist(al) + 2));
			unsigned int nulls_to_insert = ((unsigned int)(non_zero_rand())) % ((unsigned int)(get_capacity_arraylist(al) - get_element_count_arraylist(al)));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			int res = insert_NULLs_from_back_in_arraylist(al, index, nulls_to_insert);
			for(cy_uint i = 0; i < nulls_to_insert; i++)
				set_from_back_in_arraylist(al, data, index + i);
			printf("INSERT_BACK %u %u : %d\n", index, nulls_to_insert, res);
			break;
		}

		case REMOVE_FRONT :
		{
			if(get_element_count_arraylist(al) < 10)
			{
				int n = 7;
				while(n > 0)
				{
					if(is_full_arraylist(al))
						expand_arraylist(al);
					push_front_to_arraylist(al, element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE));
					if(is_full_arraylist(al))
						expand_arraylist(al);
					push_back_to_arraylist(al, element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE));
					n--;
				}
				printf("Inserting elements for calling remove front\n");
				print_int_arraylist(al);
				printf("\n\n");
			}
			unsigned int n_at = ((unsigned int)rand()) % (get_element_count_arraylist(al));
			unsigned int element_count_to_remove = ((unsigned int)non_zero_rand()) % (get_element_count_arraylist(al) - n_at);
			int res = remove_elements_from_front_of_arraylist(al, n_at, element_count_to_remove);
			printf("REMOVE_FRONT %u %u : %d\n", n_at, element_count_to_remove, res);
			break;
		}

		case REMOVE_BACK :
		{
			if(get_element_count_arraylist(al) < 10)
			{
				int n = 7;
				while(n > 0)
				{
					if(is_full_arraylist(al))
						expand_arraylist(al);
					push_front_to_arraylist(al, element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE));
					if(is_full_arraylist(al))
						expand_arraylist(al);
					push_back_to_arraylist(al, element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE));
					n--;
				}
				printf("Inserting elements for calling remove back\n");
				print_int_arraylist(al);
				printf("\n\n");
			}
			unsigned int n_at = ((unsigned int)rand()) % (get_element_count_arraylist(al));
			unsigned int element_count_to_remove = ((unsigned int)non_zero_rand()) % (get_element_count_arraylist(al) - n_at);
			int res = remove_elements_from_back_of_arraylist(al, n_at, element_count_to_remove);
			printf("REMOVE_BACK %u %u : %d\n", n_at, element_count_to_remove, res);
			break;
		}
	}
}

int main()
{
	// seed the random number generator
	srand(12);
	// srand(time(0));

	// initializide element pool
	for(int i = 0; i < ELEMENT_POOL_SIZE; i++)
		element_pool[i] = i;

	arraylist alist;
	arraylist* al = &alist;
	initialize_arraylist(al, INITIAL_TOTAL_SIZE);

	int num_ops = NUMBER_OF_OPERATIONS;

	int expand_required = 0;

	while(num_ops--)
	{
		int op = rand() % TOTAL_OPERATIONS_SUPPORTED;
		operate_on_arraylist(al, op);
		if(op != 4 && op != 5)	// print complete arraylist only on an update operation
			print_int_arraylist(al);
		printf("\n\n");

		expand_required += is_full_arraylist(al);
		if(expand_required >= 2)
		{
			int has_expanded = expand_arraylist(al);
			if(has_expanded)
				expand_required = 0;
			printf("EXPAND ARRAYLIST : %d\n", has_expanded);
			print_int_arraylist(al);
			printf("\n\n");
		}

		if(num_ops % 50 == 0)
		{
			printf("SHRINK ARRAYLIST : %d\n", shrink_arraylist(al));
			print_int_arraylist(al);
			printf("\n\n");
		}
	}

	// static tests to reserve capacity
	operate_on_arraylist(al, POP_BACK);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, PUSH_FRONT);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, PUSH_FRONT);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, PUSH_FRONT);
	print_int_arraylist(al);
	printf("\n\n");

	printf("RESERVING ARRAYLIST : %d\n", reserve_capacity_for_arraylist(al, 7));
	print_int_arraylist(al);
	printf("\n\n");

	// removing all
	printf("REMOVING ALL\n");
	remove_all_from_arraylist(al);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, INSERT_BACK);
	print_int_arraylist(al);
	printf("\n\n");

	// removing all
	printf("REMOVING ALL\n");
	remove_all_from_arraylist(al);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, INSERT_FRONT);
	print_int_arraylist(al);
	printf("\n\n");

	operate_on_arraylist(al, INSERT_FRONT);
	print_int_arraylist(al);
	printf("\n\n");

	deinitialize_arraylist(al);
	return 0;
}