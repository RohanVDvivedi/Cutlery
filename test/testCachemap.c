#include<stdio.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/cachemap.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;
	char* s;

	cchnode embed_node;
};

cy_uint hash_function(const void* data)
{
	return (cy_uint)((((ts*)data)->key)-1);
}

int cmp(const void* data1, const void* data2)
{
	return (((ts*)data1)->key) - (((ts*)data2)->key);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
		printf("(null)");
	else
		printf("%d -> %s", ((ts*)tsv)->key, ((ts*)tsv)->s);
}

void on_remove_all_println_ts_with_node(void* resource_p, const void* tsv)
{
	printf("%d -> %s :: is_free_floating_bstnode = %d\n", ((ts*)tsv)->key, ((ts*)tsv)->s, is_free_floating_cchnode(&(((ts*)tsv)->embed_node)));
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	if(tsv == NULL)
		snprintf_dstring(append_str, "(null)");
	else
		snprintf_dstring(append_str, "%d -> %s", ((ts*)tsv)->key, ((ts*)tsv)->s);
}

void print_ts_cachemap(const cachemap* cachemap_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_cachemap(&str, cachemap_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define INIT_EMBED_NODE {}

int main()
{
	cachemap cachemap_temp;
	cachemap* cachemap_p = &cachemap_temp;
	initialize_cachemap(cachemap_p, NULL, NEVER_PINNED, 10, &simple_hasher(hash_function), &simple_comparator(cmp), offsetof(ts, embed_node));

	printf("is_empty = %d\n", is_empty_cachemap(cachemap_p));
	print_ts_cachemap(cachemap_p);

	insert_in_cachemap(cachemap_p, &((ts){1, "one", INIT_EMBED_NODE}));

	print_ts_cachemap(cachemap_p);

	insert_in_cachemap(cachemap_p, &((ts){3, "three", INIT_EMBED_NODE}));

	print_ts_cachemap(cachemap_p);

	insert_in_cachemap(cachemap_p, &((ts){5, "five", INIT_EMBED_NODE}));

	print_ts_cachemap(cachemap_p);

	insert_in_cachemap(cachemap_p, &((ts){7, "seven", INIT_EMBED_NODE}));

	print_ts_cachemap(cachemap_p);

	int res = insert_in_cachemap(cachemap_p, &((ts){3, "three", INIT_EMBED_NODE}));
	printf("duplicate insert = %d\n", res);

	printf("is_empty = %d\n", is_empty_cachemap(cachemap_p));
	print_ts_cachemap(cachemap_p);

	const ts* five = find_equals_in_cachemap(cachemap_p, &((ts){5}));
	bump_element_in_cachemap(cachemap_p, five);

	print_ts_cachemap(cachemap_p);

	insert_in_cachemap(cachemap_p, &((ts){9, "nine", INIT_EMBED_NODE}));

	print_ts_cachemap(cachemap_p);

	ts* eleven = &((ts){11, "eleven", INIT_EMBED_NODE});
	insert_in_cachemap(cachemap_p, eleven);
	bump_element_in_cachemap(cachemap_p, eleven);

	print_ts_cachemap(cachemap_p);

	const ts* seven = find_equals_in_cachemap(cachemap_p, &((ts){7}));
	bump_element_in_cachemap(cachemap_p, seven);

	print_ts_cachemap(cachemap_p);

	// evict top 3 elements from lru
	for(int i = 0; i < 3; i++)
	{
		const ts* e = get_evictable_element_from_cachemap(cachemap_p);
		res = remove_from_cachemap(cachemap_p, e);
		printf("evicted %d for \n", res);
		print_ts(e);
		printf("\n");
		print_ts_cachemap(cachemap_p);
	}

	res = remove_from_cachemap(cachemap_p, eleven);
	printf("remove eleven = %d\n", res);
	res = remove_from_cachemap(cachemap_p, eleven);
	printf("remove eleven again = %d\n", res);

	print_ts_cachemap(cachemap_p);

	resize_cachemap(cachemap_p, 5);

	printf("is_empty = %d\n", is_empty_cachemap(cachemap_p));
	print_ts_cachemap(cachemap_p);

	remove_all_from_cachemap(cachemap_p, &((notifier_interface){NULL, on_remove_all_println_ts_with_node}));

	printf("is_empty = %d\n", is_empty_cachemap(cachemap_p));
	print_ts_cachemap(cachemap_p);

	deinitialize_cachemap(cachemap_p);

	return 0;
}