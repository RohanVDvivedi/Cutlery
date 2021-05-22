#include<stdio.h>
#include<queue.h>

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
	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_ts_queue(queue* queue_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_queue(&str, queue_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void push_queue_SAFE(queue* queue_p, const void* data_p)
{
	if(is_full_queue(queue_p))
		expand_queue(queue_p);
	push_queue(queue_p, data_p);
}

void pop_queue_OPTIMUM_MEMORY(queue* queue_p)
{
	pop_queue(queue_p);
	if(get_total_size_queue(queue_p) > 1.5 * get_element_count_queue(queue_p))
		shrink_queue(queue_p);
}

int main()
{
	int iter = 0;

	queue queue_temp;
	queue* queue_p = &queue_temp;
	initialize_queue(queue_p, 3);

	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 0

	push_queue_SAFE(queue_p, &((ts){1, "one"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 1

	push_queue_SAFE(queue_p, &((ts){2, "two"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 2

	push_queue_SAFE(queue_p, &((ts){3, "three"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 3

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 4

	push_queue_SAFE(queue_p, &((ts){4, "four"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 5

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 6

	push_queue_SAFE(queue_p, &((ts){5, "five"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 7

	push_queue_SAFE(queue_p, &((ts){6, "six"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 8

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 9

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 10

	push_queue_SAFE(queue_p, &((ts){7, "seven"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 11

	push_queue_SAFE(queue_p, &((ts){8, "eight"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 12

	push_queue_SAFE(queue_p, &((ts){9, "nine"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 14

	push_queue_SAFE(queue_p, &((ts){10, "ten"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 15

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 16

	push_queue_SAFE(queue_p, &((ts){11, "eleven"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 17

	push_queue_SAFE(queue_p, &((ts){12, "twelve"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 18

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 19

	push_queue_SAFE(queue_p, &((ts){13, "thirteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 20

	push_queue_SAFE(queue_p, &((ts){14, "fourteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 21

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 22

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 23

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 24

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 25

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 26

	push_queue_SAFE(queue_p, &((ts){15, "fifteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 27

	push_queue_SAFE(queue_p, &((ts){16, "sixteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 28

	push_queue_SAFE(queue_p, &((ts){17, "seventeen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 29

	push_queue_SAFE(queue_p, &((ts){18, "eighteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 30

	push_queue_SAFE(queue_p, &((ts){19, "nineteen"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 31

	push_queue_SAFE(queue_p, &((ts){20, "twenty"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 32

	push_queue_SAFE(queue_p, &((ts){21, "twenty one"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 33

	push_queue_SAFE(queue_p, &((ts){22, "twenty two"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 34

	push_queue_SAFE(queue_p, &((ts){23, "twenty three"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 35

	push_queue_SAFE(queue_p, &((ts){24, "twenty four"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 35

	push_queue_SAFE(queue_p, &((ts){25, "twenty five"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 36

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 37

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 38

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 39

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 40

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 41

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 42

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 43

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 44

	push_queue_SAFE(queue_p, &((ts){25, "twenty five"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 45

	push_queue_SAFE(queue_p, &((ts){26, "twenty six"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 46

	push_queue_SAFE(queue_p, &((ts){27, "twenty seven"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 47

	push_queue_SAFE(queue_p, &((ts){28, "twenty eight"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 48

	push_queue_SAFE(queue_p, &((ts){29, "twenty nine"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 49

	push_queue_SAFE(queue_p, &((ts){30, "thirty"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 50

	push_queue_SAFE(queue_p, &((ts){31, "thirty one"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 51

	push_queue_SAFE(queue_p, &((ts){32, "thirty two"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 52

	push_queue_SAFE(queue_p, &((ts){33, "thirty three"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 53

	push_queue_SAFE(queue_p, &((ts){34, "thirty four"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 54

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 55

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 56

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 57

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 58

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 59

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 60

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 61

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 62

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 63

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 64

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 65

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 66

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 67

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 68

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 69

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 70

	pop_queue_OPTIMUM_MEMORY(queue_p);
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 71

	push_queue_SAFE(queue_p, &((ts){35, "thirty five"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 72

	push_queue_SAFE(queue_p, &((ts){36, "thirty six"}));
	printf("-> %d\n", iter++);print_ts_queue(queue_p); // 73

	deinitialize_queue(queue_p);

	return 0;
}