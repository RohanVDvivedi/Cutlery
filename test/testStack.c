#include<stack.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(const void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

#define USE_STACK_MEMORY
//#define USE_HEAP_MEMORY

int main()
{
	#if defined USE_STACK_MEMORY
		printf("STACK WILL BE CREATED ON STACK MEMORY\n\n");
		stack stack_temp;
		stack* stack_p = &stack_temp;
		initialize_stack(stack_p, 5);
	#elif defined USE_HEAP_MEMORY
		printf("STACK WILL BE CREATED ON HEAP MEMORY\n\n");
		stack* stack_p = get_stack(5);
	#endif
	
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){1, "one"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){2, "two"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){3, "three"}));
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){4, "four"}));
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){5, "five"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){6, "six"}));
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){7, "seven"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){8, "eight"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){9, "nine"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){10, "ten"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){11, "eleven"}));
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){12, "twelve"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){13, "thirteen"}));
	print_stack(stack_p, print_ts);

	push_stack(stack_p, &((ts){14, "fourteen"}));
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	pop_stack(stack_p);
	print_stack(stack_p, print_ts);

	#if defined USE_STACK_MEMORY
		deinitialize_stack(stack_p);
	#elif defined USE_HEAP_MEMORY
		delete_stack(stack_p);
	#endif

	return 0;
}