#include<stdio.h>
#include<cutlery/arraylist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(const void* tsv)
{
	printf("%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_ts_stack(arraylist* stack_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_arraylist(&str, stack_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void push_stack_SAFE(arraylist* stack_p, const void* data_p)
{
	if(is_full_arraylist(stack_p))
		expand_arraylist(stack_p);
	push_back_to_arraylist(stack_p, data_p);
}

void pop_stack_OPTIMUM_MEMORY(arraylist* stack_p)
{
	pop_back_from_arraylist(stack_p);
	if(get_capacity_arraylist(stack_p) > 1.5 * get_element_count_arraylist(stack_p))
		shrink_arraylist(stack_p);
}

int main()
{
	arraylist stack_temp;
	arraylist* stack_p = &stack_temp;
	initialize_arraylist(stack_p, 3);
	
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){1, "one"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){2, "two"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){3, "three"}));
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){4, "four"}));
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){5, "five"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){6, "six"}));
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){7, "seven"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){8, "eight"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){9, "nine"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){10, "ten"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){11, "eleven"}));
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){12, "twelve"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){13, "thirteen"}));
	print_ts_stack(stack_p);

	push_stack_SAFE(stack_p, &((ts){14, "fourteen"}));
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	remove_all_from_arraylist(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	pop_stack_OPTIMUM_MEMORY(stack_p);
	print_ts_stack(stack_p);

	deinitialize_arraylist(stack_p);
	return 0;
}