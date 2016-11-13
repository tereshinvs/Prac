#include <stdio.h>

#define HASH_MOD 5000003

struct node
{
	unsigned long num, sum;
	int alr;
};

struct node data[HASH_MOD];

unsigned long hash(unsigned long t)
{
	return t%HASH_MOD;
}

long add(unsigned long t)
{
	unsigned long thash = hash(t), i;
	for (i = thash; i < HASH_MOD; i++)
		if (!data[i].alr)
		{
			data[i].alr = 1;
			data[i].num = t;
			data[i].sum = 0;
			return i;
		}
	for (i = 0; i < thash; i++)
		if (!data[i].alr)
		{
			data[i].alr = 1;
			data[i].num = t;
			data[i].sum = 0;
			return i;
		}
	return -1;
}

long find(unsigned long t)
{
	unsigned long thash = hash(t), i;
	for (i = thash; i < HASH_MOD; i++)
	{
		if (!data[i].alr)
			break;
		if (data[i].alr && data[i].num == t)
			return i;
	}
	for (i = 0; i < thash; i++)
	{
		if (!data[i].alr)
			break;
		if (data[i].alr && data[i].num == t)
			return i;
	}
	return -1;
}

int main(void)
{
	unsigned long n, i, res1 = 0, res2 = 0, x;
	long pos;
	for (i = 0; i < HASH_MOD; i++)
		data[i].alr = 0;
	scanf("%lu", &n);
	for (i = 0; i < n; i++)
	{
		scanf("%lu", &x);
		pos = find(x);
		if (pos == -1)
			pos = add(x);
		data[pos].sum++;
	}
//	for (i = 0; i < 10; i++)
//		printf("%d %lu %lu\n", data[i].alr, data[i].num, data[i].sum);
	for (i = 0; i < HASH_MOD; i++)
		if (data[i].alr && data[i].sum%2)
		{
			res1 = data[i].num;
			break;
		}
	for (i = 0; i < HASH_MOD; i++)
		if (data[i].alr && data[i].sum%2 && data[i].num != res1)
		{
			res2 = data[i].num;
			break;
		}
	if (res1 < res2)
		printf("%lu %lu\n", res1, res2);
	else
		printf("%lu %lu\n", res2, res1);
	return 0;
}