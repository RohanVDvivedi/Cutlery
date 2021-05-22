#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include<arraylist.h>

#define TOTAL_OPERATIONS_SUPPORTED 8

typedef enum al_op al_op;
enum al_op
{
	PUSH_FRONT = 0,
	PUSH_BACK,		// 1
	POP_FRONT,		// 2
	POP_BACK,		// 3
	SET_NTH_FRONT,	// 4
	SET_NTH_BACK,	// 5
	GET_NTH_FRONT,	// 6
	GET_NTH_BACK 	// 7
};

#define INITIAL_TOTAL_SIZE 3
#define NUMBER_OF_OPERATIONS 50
#define ELEMENT_POOL_SIZE 50

int element_pool[ELEMENT_POOL_SIZE];

void operate_on_arraylist(arraylist* al, al_op op)
{
	switch(op)
	{
		case PUSH_FRONT :
		{
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_FRONT : %d :: %d\n", *data, push_front(al, data));
			break;
		}

		case PUSH_BACK :
		{
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_BACK : %d :: %d\n", *data, push_back(al, data));
			break;
		}

		case POP_FRONT :
		{
			printf("POP_FRONT :: %d\n", pop_front(al));
			break;
		}

		case POP_BACK :
		{
			printf("POP_BACK :: %d\n", pop_back(al));
			break;
		}

		case GET_NTH_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = get_nth_from_front(al, index);
			printf("GET %u TH_FRONT : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case GET_NTH_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = get_nth_from_back(al, index);
			printf("GET %u TH_BACK : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case SET_NTH_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("SET %u TH_FRONT : %d : %d\n", index, *data, set_nth_from_front(al, data, index));
			break;
		}

		case SET_NTH_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 1) * 1.2));
			const int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("SET %u TH_BACK : %d : %d\n", index, *data, set_nth_from_back(al, data, index));
			break;
		}
	}
}

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
		if(op <= 5)	// print complete arraylist only on an update operation
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

		if(num_ops % 20 == 0)
		{
			printf("SHRINK ARRAYLIST : %d\n", shrink_arraylist(al));
			print_int_arraylist(al);
			printf("\n\n");
		}
	}

	deinitialize_arraylist(al);
	return 0;
}