#include<cutlery/deferred_callbacks.h>

#include<stdio.h>

void callback_print(void* str)
{
	printf("%s\n", (char*)str);
}

int main()
{
	NEW_DEFERED_CALLS(10);

	DEFER(callback_print, "Hello");

	DEFER(callback_print, "World");

	DEFER(callback_print, "Rohan");

	printf("calling defer-s here\n");
	CALL_ALL_DEFERRED();
}