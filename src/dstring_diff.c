#include<dstring_diff.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

static unsigned int minimum_of(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned int min_a_b = (a < b) ? a : b;
	return (min_a_b < c) ? min_a_b : c;
}

static unsigned int maximum_of(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned int max_a_b = (a > b) ? a : b;
	return (max_a_b > c) ? max_a_b : c;
}

typedef struct array_2d array_2d;
struct array_2d
{
	unsigned int dim_0_size;
	unsigned int dim_1_size;
	unsigned int* holder;
};

static unsigned int* iterator(array_2d* arr, unsigned int i1, unsigned int i0)
{
	if((i1 >= arr->dim_1_size) || (i0 >= arr->dim_0_size))
		return NULL;
	return arr->holder + (i1 * arr->dim_0_size) + i0;
}

unsigned int levenshtein_distance(const dstring* str0, const dstring* str1)
{
	array_2d arr;
	arr.dim_0_size = str0->bytes_occupied + 1;
	arr.dim_1_size = str1->bytes_occupied + 1;
	arr.holder = allocate(DSTRING_mem_alloc, sizeof(unsigned int) * arr.dim_0_size * arr.dim_1_size);

	for(unsigned int i = 0; i <= str1->bytes_occupied; i++)
	{
		for(unsigned int j = 0; j <= str0->bytes_occupied; j++)
		{
			if(i == 0)
				(*(iterator(&arr, i, j))) = j;
			else if(j == 0)
				(*(iterator(&arr, i, j))) = i;
			else
			{
				if(str1->cstring[i - 1] == str0->cstring[j - 1])
					(*(iterator(&arr, i, j))) = (*(iterator(&arr, i - 1, j - 1)));
				else
					(*(iterator(&arr, i, j))) = minimum_of( 1 + (*(iterator(&arr, i - 1, j - 1))),	// replace
															1 + (*(iterator(&arr, i - 1, j    ))),	// insert in str1
															1 + (*(iterator(&arr, i    , j - 1))));	// insert in str0
			}
		}
	}

	unsigned int result = (*(iterator(&arr, str1->bytes_occupied, str0->bytes_occupied)));
	deallocate(DSTRING_mem_alloc, arr.holder, sizeof(unsigned int) * arr.dim_0_size * arr.dim_1_size);
	return result;
}

unsigned int length_of_longest_common_subsequence(const dstring* str0, const dstring* str1)
{
	array_2d arr;
	arr.dim_0_size = str0->bytes_occupied + 1;
	arr.dim_1_size = str1->bytes_occupied + 1;
	arr.holder = allocate(DSTRING_mem_alloc, sizeof(unsigned int) * arr.dim_0_size * arr.dim_1_size);

	for(unsigned int i = 0; i <= str1->bytes_occupied; i++)
	{
		for(unsigned int j = 0; j <= str0->bytes_occupied; j++)
		{
			if(i == 0)
				(*(iterator(&arr, i, j))) = 0;
			else if(j == 0)
				(*(iterator(&arr, i, j))) = 0;
			else
			{
				if(str1->cstring[i - 1] == str0->cstring[j - 1])
					(*(iterator(&arr, i, j))) = 1 + (*(iterator(&arr, i - 1, j - 1)));
				else
					(*(iterator(&arr, i, j))) = maximum_of( (*(iterator(&arr, i - 1, j - 1))),
															(*(iterator(&arr, i - 1, j    ))),
															(*(iterator(&arr, i    , j - 1))));
			}
		}
	}

	unsigned int result = (*(iterator(&arr, str1->bytes_occupied, str0->bytes_occupied)));
	deallocate(DSTRING_mem_alloc, arr.holder, sizeof(unsigned int) * arr.dim_0_size * arr.dim_1_size);
	return result;
}