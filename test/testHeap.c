#include<heap.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
	int index;
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
	return ( (((ke*)key1)->k) - (((ke*)key2)->k) );
}

void print_ts(const void* tsv)
{
	printf(" %d, %s =<HEAP_INDEX>=> %d", ((ts*)tsv)->a, ((ts*)tsv)->s, ((ts*)tsv)->index);
}

void print_key(const void* key)
{
	printf("%d", (*((int*)key)));
}

void change_key(heap* heap_p, unsigned long long int index, int new_key)
{
	if(index <= heap_p->heap_size - 1)
	{
		(*((int*)get_key_bucket_array(&(heap_p->heap_holder), index))) = new_key;
		heapify_at(heap_p, index);
	}
}

void update_index_callback(const void* key, const void* value, unsigned long long int heap_index, const void* additional_params)
{
	((ts*)value)->index = heap_index;
}

int main()
{
	heap* heap_p = get_heap(5, MIN_HEAP, key_cmp, update_index_callback, NULL);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){1}), &((ts){1, "one"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){2}), &((ts){2, "two"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){3}), &((ts){3, "three"}));
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){4}), &((ts){4, "four"}));
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){4}), &((ts){4, "four-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){3}), &((ts){3, "three-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){2}), &((ts){2, "two-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){1}), &((ts){1, "one-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){5}), &((ts){5, "five"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){6}), &((ts){6, "six"}));
	print_heap(heap_p, print_key, print_ts);

	change_key(heap_p, heap_p->heap_size/2, -2000);
	print_heap(heap_p, print_key, print_ts);

	change_key(heap_p, heap_p->heap_size/2, +2000);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){7}), &((ts){7, "seven"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){8}), &((ts){8, "eight"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){14}), &((ts){14, "fourteen"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){13}), &((ts){13, "thirteen"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){12}), &((ts){12, "twelve"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){11}), &((ts){11, "eleven"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){9}), &((ts){9, "nine"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){10}), &((ts){10, "ten"}));
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){2}), &((ts){2, "two-xx"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){1}), &((ts){1, "one-xx"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){5}), &((ts){5, "five-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){6}), &((ts){6, "six-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){11}), &((ts){11, "eleven-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){5}), &((ts){5, "five-xx"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){6}), &((ts){6, "six-xx"}));
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){12}), &((ts){12, "twelve-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){13}), &((ts){13, "thirteen-x"}));
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){14}), &((ts){14, "fourteen-x"}));print_array(&(heap_p->heap_holder), print_key);
	print_heap(heap_p, print_key, print_ts);

	change_key(heap_p, 12, 3);
	print_heap(heap_p, print_key, print_ts);

	change_key(heap_p, 1, 8);
	print_heap(heap_p, print_key, print_ts);

	change_key(heap_p, 5, 2);
	print_heap(heap_p, print_key, print_ts);

	push_heap(heap_p, &((ke){9}), &((ts){9, "nine-x"}));
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	pop_heap(heap_p);
	print_heap(heap_p, print_key, print_ts);

	delete_heap(heap_p);
	return 0;
}