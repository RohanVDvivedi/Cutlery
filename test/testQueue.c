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

	push_queue(queue_p, &((ts){25, "twenty five"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 45

	push_queue(queue_p, &((ts){26, "twenty six"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 46

	push_queue(queue_p, &((ts){27, "twenty seven"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 47

	push_queue(queue_p, &((ts){28, "twenty eight"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 48

	push_queue(queue_p, &((ts){29, "twenty nine"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 49

	push_queue(queue_p, &((ts){30, "thirty"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 50

	push_queue(queue_p, &((ts){31, "thirty one"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 51

	push_queue(queue_p, &((ts){32, "thirty two"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 52

	push_queue(queue_p, &((ts){33, "thirty three"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 53

	push_queue(queue_p, &((ts){34, "thirty four"}));
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 54

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 55

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 56

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 57

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 58

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 59

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 60

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 61

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 62

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 63

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 64

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 65

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 66

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 67

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 68

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 69

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 70

	pop_queue(queue_p);
	printf("-> %d\n", iter++);print_queue(queue_p, print_ts); // 71

	delete_queue(queue_p);
	return 0;
}