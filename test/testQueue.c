#include<queue.h>

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

int main()
{
	queue* queue_p = get_queue(5);
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){1, "one"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){2, "two"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){3, "three"}));
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){4, "four"}));
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){5, "five"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){6, "six"}));
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){7, "seven"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){8, "eight"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){9, "nine"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){10, "ten"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){11, "eleven"}));
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){12, "twelve"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){13, "thirteen"}));
	print_queue(queue_p, print_ts);

	push(queue_p, &((ts){14, "fourteen"}));
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	pop(queue_p);
	print_queue(queue_p, print_ts);

	delete_queue(queue_p);
	return 0;
}