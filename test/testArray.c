#include<array.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

int main()
{
	array_p* arr = get_array(3, sizeof(ts));

	delete_array(arr);
	return 0;
}