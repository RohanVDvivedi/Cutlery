#include<heap.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

typedef struct key ke;
struct key
{
	int k;
};

unsigned long long int hash_function(const void* key)
{
	return (*((unsigned long long int *)key))-1;
}

int key_cmp(const void* key1, const void* key2)
{
	return (((ke*)key1)->k) - (((ke*)key2)->k);
}

void print_ts(const void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_key(const void* key)
{
	printf("%d", (*((int*)key)));
}


int main()
{
	heap* heap_p = get_heap(5, MIN_HEAP);
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){1}), &((ts){1, "one"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){2}), &((ts){2, "two"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){3}), &((ts){3, "three"}));
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){4}), &((ts){4, "four"}));
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){5}), &((ts){5, "five"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){6}), &((ts){6, "six"}));
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){7}), &((ts){7, "seven"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){8}), &((ts){8, "eight"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){9}), &((ts){9, "nine"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){10}), &((ts){10, "ten"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){11}), &((ts){11, "eleven"}));
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){12}), &((ts){12, "twelve"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){13}), &((ts){13, "thirteen"}));
	print_heap(heap_p, print_ts);

	push(heap_p, &((ke){14}), &((ts){14, "fourteen"}));
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	pop(heap_p);
	print_heap(heap_p, print_ts);

	delete_heap(heap_p);
	return 0;
}