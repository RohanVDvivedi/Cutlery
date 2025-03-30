#include<unistd.h>
#include<stdio.h>
#include<string.h>

#include<cutlery/dstring_base64.h>

dstring read_all_from_stdin()
{
	#define BUFFER_SIZE 4096
	dstring res;
	init_empty_dstring(&res, 0);
	while(1)
	{
		char buffer[BUFFER_SIZE];
		unsigned int buffer_size = read(0, buffer, BUFFER_SIZE);
		if(buffer_size == 0)
			break;
		concatenate_dstring(&res, &get_dstring_pointing_to(buffer, buffer_size));
	}
	return res;
}

int read_from_stdin_and_encode()
{
	dstring in = read_all_from_stdin();

	dstring res;
	init_empty_dstring(&res, 0);

	int return_val = 0;

	if(base64_encode(&in, &res) == 0)
	{
		printf("not encodable\n");
		return_val = -1;
	}
	else
		printf_dstring(&res);

	deinit_dstring(&res);
	deinit_dstring(&in);
	return return_val;
}

int read_from_stdin_and_decode()
{
	dstring in = read_all_from_stdin();

	dstring res;
	init_empty_dstring(&res, 0);

	int return_val = 0;

	if(base64_decode(&in, &res) == 0)
	{
		printf("not decodable\n");
		return_val = -1;
	}
	else
		printf_dstring(&res);

	deinit_dstring(&res);
	deinit_dstring(&in);
	return return_val;
}

int main(int argc, char** argv)
{
	if(argc == 1)
		return read_from_stdin_and_encode();
	else if(argc == 2 && 0 == strcmp("--decode", argv[1]))
		return read_from_stdin_and_decode();

	printf("usage error you can either do :\n `cat file | base64`\n or :\n `cat file | base64 --decode`\n");
	return -1;
}