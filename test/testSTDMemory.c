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

				memory_move(memory_data1 + i, memory_data2 + j, rand() % MEMORY_OPERATION_SIZE);

				printf("\nInitial : \n");
				printf("data1 :");
				print_memory(memory_data1 + i, MEMORY_OPERATION_SIZE);
				printf("data2:");
				print_memory(memory_data2 + j, MEMORY_OPERATION_SIZE);

				int result = memory_compare(memory_data1 + i, memory_data2 + j, MEMORY_OPERATION_SIZE);

				printf("Result : %d\n", result);

				printf("\n");
			}
		}
		printf("\n\n");
	}

	{
		{
			char c1 = '\xff';
			char c2 = '\x01';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\x01';
			char c2 = '\xff';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\xef';
			char c2 = '\xee';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\xee';
			char c2 = '\xef';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\x21';
			char c2 = '\x22';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\x22';
			char c2 = '\x21';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}

		{
			char c1 = '\x32';
			char c2 = '\x32';
			printf("memory_compare(%hhx::%hhd, %hhx::%hhd) = %d\n", c1, c1, c2, c2, memory_compare(&c1, &c2, 1));
		}
	}

	printf("\n\n");

	printf("memory_reverse_chunks : \n");
	int data[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	printf("before data : ");
	for(int i = 0; i < sizeof(data)/sizeof(int); i++)
		printf("%d ", data[i]);
	printf("\n");
	memory_reverse_chunks(data, sizeof(data), sizeof(int));
	printf("after data : ");
	for(int i = 0; i < sizeof(data)/sizeof(int); i++)
		printf("%d ", data[i]);
	printf("\n");

	return 0;
}