#include <ctime>
#include <iostream>
#include "AliasMethod.h"
using namespace std;

double weights[] = { 1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10 };
#define COUNT 20

int main()
{
	srand(time(0));

	AliasMethod alias(weights, COUNT);

	float rate[COUNT] = {0};
	for (int i = 0; i < 1000000; i++)
	{
		int next = alias.next();
		rate[next]++;
	}

	int sum = 0;
	for (int i = 0; i < COUNT; i++)
		sum += weights[i];

	for (int i = 0; i < COUNT; i++)
		cout << rate[i] * sum / 1000000 << " ";

	char end = getchar();
}
