#include<cutlery_stds.h> 

#include<stdio.h>
#include<stdlib.h>

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

// test for small buffers i.e. supporting only byte copy
//#define MEMORY_OPERATION_SIZE ((sizeof(int) * 2)+ 2)

// test for large buffers i.e. supporting int copy
#define MEMORY_OPERATION_SIZE ((sizeof(int) * 5)+ 2)

#define BUFFER_SIZE (MEMORY_OPERATION_SIZE + sizeof(int))

int main()
{
	{
		char memory_to_0_set[BUFFER_SIZE];
		printf("TEST MEM SET\n\n");
		for(int i = 0; i < sizeof(int); i++)
		{
			init_rand_memory(memory_to_0_set, BUFFER_SIZE);

			printf("\nInitial : \n");
			print_memory(memory_to_0_set, BUFFER_SIZE);

			memory_set(memory_to_0_set + i, '+', MEMORY_OPERATION_SIZE);

			printf("Result : \n");
			print_memory(memory_to_0_set, BUFFER_SIZE);

			printf("\n");
		}
		printf("\n\n");
	}

	{
		char memory_dest[BUFFER_SIZE];
		char memory_src[BUFFER_SIZE];
		printf("TEST MEM MOVE FORWARD COPY\n\n");
		for(int i = 0; i < sizeof(int); i++)
		{
			for(int j = 0; j < sizeof(int); j++)
			{
				init_rand_memory(memory_src + i, MEMORY_OPERATION_SIZE);
				init_rand_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				printf("\nInitial : \n");
				printf("src :");
				print_memory(memory_src + i, MEMORY_OPERATION_SIZE);
				printf("dest:");
				print_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				memory_move(memory_dest + j, memory_src + i, MEMORY_OPERATION_SIZE);

				printf("Result : \n");
				printf("dest:");
				print_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				printf("\n");
			}
		}
		printf("\n\n");
	}

	{
		char memory_src[BUFFER_SIZE];
		char memory_dest[BUFFER_SIZE];
		printf("TEST MEM MOVE BACKWARD COPY\n\n");
		for(int i = 0; i < sizeof(int); i++)
		{
			for(int j = 0; j < sizeof(int); j++)
			{
				init_rand_memory(memory_src + i, MEMORY_OPERATION_SIZE);
				init_rand_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				printf("\nInitial : \n");
				printf("src :");
				print_memory(memory_src + i, MEMORY_OPERATION_SIZE);
				printf("dest:");
				print_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				memory_move(memory_dest + j, memory_src + i, MEMORY_OPERATION_SIZE);

				printf("Result : \n");
				printf("dest:");
				print_memory(memory_dest + j, MEMORY_OPERATION_SIZE);

				printf("\n");
			}
		}
		printf("\n\n");
	}

	{
		char memory_data1[BUFFER_SIZE];
		char memory_data2[BUFFER_SIZE];
		printf("TEST MEM CMP FORWARD COPY\n\n");
		for(int i = 0; i < sizeof(int); i++)
		{
			for(int j = 0; j < sizeof(int); j++)
			{
				init_rand_memory(memory_data1 + i, MEMORY_OPERATION_SIZE);
				init_rand_memory(memory_data2 + j, MEMORY_OPERATION_SIZE);

				memory_move(memory_data1 + j, memory_data2 + i, rand() % MEMORY_OPERATION_SIZE);

				printf("\nInitial : \n");
				printf("data1 :");
				print_memory(memory_data1 + i, MEMORY_OPERATION_SIZE);
				printf("data2:");
				print_memory(memory_data2 + j, MEMORY_OPERATION_SIZE);

				int result = memory_compare(memory_data1 + j, memory_data2 + i, MEMORY_OPERATION_SIZE);

				printf("Result : %d\n", result);

				printf("\n");
			}
		}
		printf("\n\n");
	}

	return 0;
}