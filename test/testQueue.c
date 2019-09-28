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
	int iter = 0;

	queue* queue_p = get_queue(5);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 0

	push_queue(queue_p, &((ts){1, "one"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 1

	push_queue(queue_p, &((ts){2, "two"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 2

	push_queue(queue_p, &((ts){3, "three"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 3

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 4

	push_queue(queue_p, &((ts){4, "four"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 5

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 6

	push_queue(queue_p, &((ts){5, "five"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 7

	push_queue(queue_p, &((ts){6, "six"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 8

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 9

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 10

	push_queue(queue_p, &((ts){7, "seven"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 11

	push_queue(queue_p, &((ts){8, "eight"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 12

	push_queue(queue_p, &((ts){9, "nine"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 14

	push_queue(queue_p, &((ts){10, "ten"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 15

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 16

	push_queue(queue_p, &((ts){11, "eleven"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 17

	push_queue(queue_p, &((ts){12, "twelve"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 18

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 19

	push_queue(queue_p, &((ts){13, "thirteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 20

	push_queue(queue_p, &((ts){14, "fourteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 21

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 22

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 23

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 24

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 25

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 26

	push_queue(queue_p, &((ts){15, "fifteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 27

	push_queue(queue_p, &((ts){16, "sixteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 28

	push_queue(queue_p, &((ts){17, "seventeen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 29

	push_queue(queue_p, &((ts){18, "eighteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 30

	push_queue(queue_p, &((ts){19, "nineteen"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 31

	push_queue(queue_p, &((ts){20, "twenty"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 32

	push_queue(queue_p, &((ts){21, "twenty one"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 33

	push_queue(queue_p, &((ts){22, "twenty two"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 34

	push_queue(queue_p, &((ts){23, "twenty three"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 35

	push_queue(queue_p, &((ts){24, "twenty four"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 35

	push_queue(queue_p, &((ts){25, "twenty five"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 36

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 37

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 38

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 39

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 40

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 41

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 42

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 43

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 44

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 45

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 46

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 47

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 48

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 49

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 50

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 51

	delete_queue(queue_p);
	return 0;
}