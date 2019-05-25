#include<hashmap.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

unsigned long long int hash_function(void* key)
{
	return (*((unsigned long long int *)key))-1;
}

void print_ts(void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_key(void* key)
{
	printf("%lld", (*((unsigned long long int*)key)));
}

int main()
{
	hashmap* hashmap_p = get_hashmap(4, hash_function);

	print_hashmap(hashmap_p, print_key, print_ts);

	unsigned long long int key = 1;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){100, "one"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 2;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){200, "two"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 3;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){300, "there"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 4;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){400, "four"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 5;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){500, "five"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 6;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){600, "six"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 7;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){700, "seven"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 8;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){800, "eight"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 9;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){900, "nine"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("\n\nprinting by using get methods\n\n");

	for(unsigned long long int i = 1; i <= 9; i++)
	{
		print_bucket(get(hashmap_p, &i, sizeof(unsigned long long int)), print_key, print_ts);
	}

	key = 2;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	key = 6;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	key = 4;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 7;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	key = 8;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 2;
	remove_bucket(hashmap_p, &key, sizeof(unsigned long long int));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 60;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){6000, "sixty"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 70;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){7000, "seventy"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 80;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){8000, "eighty"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 20;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){2000, "twenty"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	key = 40;
	put(hashmap_p, get_bucket(&key, sizeof(unsigned long long int), &((ts){4000, "forty"}), sizeof(ts)));

	print_hashmap(hashmap_p, print_key, print_ts);

	printf("\n\nprinting by using get methods\n\n");

	for(unsigned long long int i = 1; i <= 100; i++)
	{
		bucket* bucket_p = get(hashmap_p, &i, sizeof(unsigned long long int));
		if(bucket_p != NULL)
		{
			print_bucket(get(hashmap_p, &i, sizeof(unsigned long long int)), print_key, print_ts);
		}
	}

	delete_hashmap(hashmap_p);

	return 0;
}