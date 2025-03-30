#include<cutlery/cutlery_stds.h> 

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
				printf("data2 :");
				print_memory(memory_data2 + j, MEMORY_OPERATION_SIZE);

				int result = memory_compare(memory_data1 + i, memory_data2 + j, MEMORY_OPERATION_SIZE);

				printf("Result : %d\n", result);

				printf("\n");
			}
		}
		printf("\n\n");
	}

	{
		#define SWAP_BUFFER_SIZE 18
		char memory_data1[SWAP_BUFFER_SIZE] = "Rohan V Dvivedi**";
		char memory_data2[SWAP_BUFFER_SIZE] = "Devashree Joshi++";
		printf("TEST MEM swap for %p %p\n\n", memory_data1, memory_data2);
		printf("\nInitial : \n");
		printf("data1 :");
		printf("%s\n", memory_data1);
		printf("data2 :");
		printf("%s\n", memory_data2);
		printf("\n");
		memory_swap(memory_data1, memory_data2, SWAP_BUFFER_SIZE);
		printf("\nLater : \n");
		printf("data1 :");
		printf("%s\n", memory_data1);
		printf("data2 :");
		printf("%s\n", memory_data2);
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

	{
		printf("memory_reverse_chunks : \n");
		unsigned long long int data[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		printf("before data : ");
		for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
			printf("%llx ", data[i]);
		printf("\n");
		memory_reverse_chunks(data, sizeof(data), sizeof(data[0]));
		printf("after data  : ");
		for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
			printf("%llx ", data[i]);
		printf("\n");
	}

	printf("\n\n");

	{
		for(cy_uint right_rotate_amount = 0; right_rotate_amount <= 14; right_rotate_amount++)
		{
			printf("memory_right_rotate(%"PRIu_cy_uint") : \n", right_rotate_amount);
			unsigned long long int data[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
			printf("before data : ");
			for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
				printf("%llx ", data[i]);
			printf("\n");
			memory_right_rotate(data, sizeof(data), sizeof(data[0]) * right_rotate_amount);
			printf("after data  : ");
			for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
				printf("%llx ", data[i]);
			printf("\n\n");
		}
	}

	printf("\n\n");

	{
		for(cy_uint left_rotate_amount = 0; left_rotate_amount <= 14; left_rotate_amount++)
		{
			printf("memory_left_rotate(%"PRIu_cy_uint") : \n", left_rotate_amount);
			unsigned long long int data[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
			printf("before data : ");
			for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
				printf("%llx ", data[i]);
			printf("\n");
			memory_left_rotate(data, sizeof(data), sizeof(data[0]) * left_rotate_amount);
			printf("after data  : ");
			for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
				printf("%llx ", data[i]);
			printf("\n\n");
		}
	}

	printf("\n\n");

	{
		#define BYTES_SIZE 10
		char bytes[BYTES_SIZE];

		printf("memory = %p, %d\n", bytes, BYTES_SIZE);

		printf("%p is contained %d\n", bytes-5, memory_contains(bytes, BYTES_SIZE, bytes-5));
		printf("%p is contained %d\n", bytes-1, memory_contains(bytes, BYTES_SIZE, bytes-1));
		printf("%p is contained %d\n", bytes+5, memory_contains(bytes, BYTES_SIZE, bytes+5));
		printf("%p is contained %d\n", bytes+10, memory_contains(bytes, BYTES_SIZE, bytes+10));
		printf("%p is contained %d\n", bytes+15, memory_contains(bytes, BYTES_SIZE, bytes+15));

		printf("memory = %p, %d\n", bytes, 0);
		printf("%p is contained %d\n", bytes, memory_contains(bytes, 0, bytes));
	}

	printf("\n\n");

	{
		#define BYTES_SIZE 10
		char bytes[BYTES_SIZE];
		printf("memory = %p, %d\n", bytes, BYTES_SIZE);
		printf("2 -> %p\n", memory_get_first_aigned_in_region(bytes, BYTES_SIZE, 2));
		printf("5 -> %p\n", memory_get_first_aigned_in_region(bytes, BYTES_SIZE, 5));
		printf("12 -> %p\n", memory_get_first_aigned_in_region(bytes, BYTES_SIZE, 12));
		printf("24 -> %p\n", memory_get_first_aigned_in_region(bytes, BYTES_SIZE, 24));

		char* b = (char* )(-1) - 9;
		int b_size = 10;
		printf("memory = %p, %d\n", b, b_size);
		printf("2 -> %p\n", memory_get_first_aigned_in_region(b, b_size, 2));
		printf("5 -> %p\n", memory_get_first_aigned_in_region(b, b_size, 5));
		printf("12 -> %p\n", memory_get_first_aigned_in_region(b, b_size, 12));
		printf("16 -> %p\n", memory_get_first_aigned_in_region(b, b_size, 16));
		printf("24 -> %p\n", memory_get_first_aigned_in_region(b, b_size, 24));
	}

	printf("\n\n");

	return 0;
}