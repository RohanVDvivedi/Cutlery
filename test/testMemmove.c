#include<stdio.h>
#include<stdlib.h>

#include<cutlery_stds.h>

void init_rand_memory(char* mem, int size)
{
	while(size-- > 0)
		*(mem++) = (rand() % 26) + 'A';
}

void print_memory(char* mem, int size)
{
	printf("@ %p %d\n", mem, size);
	while(size-- > 0)
		printf("%c ", (*(mem++)));
	printf("\n");
}

int main()
{
	const int test_iter = 8;
	const int test_memory_size = 22;

	char test_memory_dest_backward[test_memory_size + test_iter];
	char test_memory_src[test_memory_size + test_iter];
	char test_memory_dest_forward[test_memory_size + test_iter];

	for(int i = 0; i < test_iter; i++)
	{
		for(int j = 0; j < test_iter; j++)
		{
			init_rand_memory(test_memory_src + i, test_memory_size);
			init_rand_memory(test_memory_dest_forward + j, test_memory_size);

			printf("\nInitial : \n");
			printf("src:");
			print_memory(test_memory_src + i, test_memory_size);
			printf("dest:");
			print_memory(test_memory_dest_forward + j, test_memory_size);

			memory_move(test_memory_dest_forward + j, test_memory_src + i, test_memory_size);

			printf("Result : \n");
			printf("dest:");
			print_memory(test_memory_dest_forward + j, test_memory_size);
		}
	}

	for(int i = 0; i < test_iter; i++)
	{
		for(int j = 0; j < test_iter; j++)
		{
			init_rand_memory(test_memory_src + i, test_memory_size);
			init_rand_memory(test_memory_dest_backward + j, test_memory_size);

			printf("\nInitial : \n");
			printf("src:");
			print_memory(test_memory_src + i, test_memory_size);
			printf("dest:");
			print_memory(test_memory_dest_backward + j, test_memory_size);

			memory_move(test_memory_dest_backward + j, test_memory_src + i, test_memory_size);

			printf("Result : \n");
			printf("dest:");
			print_memory(test_memory_dest_backward + j, test_memory_size);
		}
	}

	return 0;
}