#include<stack.h>

stack* get_stack(unsigned long long int expected_size)
{
	stack* stack_p = (stack*) calloc(1, sizeof(stack));
	stack_p->stackHolder = get_array(expected_size + 1);
	stack_p->stackSize = 0;
	return stack_p;
}

void push(stack* stack_p, const void* data_p)
{
	if(stack_p->stackSize >= stack_p->stackHolder->total_size)
	{
		expand_array(stack_p->stackHolder);
	}
	set_element(stack_p->stackHolder, data_p, stack_p->stackSize++);
}

void pop(stack* stack_p)
{
	if(stack_p->stackSize > 0)
	{
		set_element(stack_p->stackHolder, NULL, --stack_p->stackSize);
	}
}

const void* get_top(stack* stack_p)
{
	return get_element(stack_p->stackHolder, (stack_p->stackSize)-1);
}

void delete_stack(stack* stack_p)
{
	delete_array(stack_p->stackHolder);
	stack_p->stackHolder = NULL;
	stack_p->stackSize = 0;
	free(stack_p);
}

void print_stack(stack* stack_p, void (*print_element)(const void* data_p))
{
	printf("stack : \n");
	printf("\tstackSize : %llu\n", stack_p->stackSize);
	printf("\tstack array : ");print_array(stack_p->stackHolder, print_element);printf("\n");
	printf("\tthe top element : ");
	if(get_top(stack_p)!=NULL)
	{
		print_element(get_top(stack_p));
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}