#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node
{
	unsigned long num, sum;
	struct node *left, *right;
};

void add(struct node *v, unsigned long num)
{
	if (v->num == num)
	{
		v->sum++;
		return;
	}
	if (num < v->num)
		if (v->left)
			add(v->left, num);
		else {
			v->left = (struct node *)malloc(sizeof(struct node));
			v->left->num = num;
			v->left->sum = 1;
			v->left->left = NULL;
			v->left->right = NULL;
		}
	else
		if (v->right)
			add(v->right, num);
		else {
			v->right = (struct node *)malloc(sizeof(struct node));
			v->right->num = num;
			v->right->sum = 1;
			v->right->left = NULL;
			v->right->right = NULL;
		}
}

void dfs(struct node *v)
{
	if (v->left)
		dfs(v->left);
	if (v->sum%2)
		printf("%lu ", v->num);
	if (v->right)
		dfs(v->right);
	free(v);
}

unsigned long data[1500000];

void swap(unsigned long *t1, unsigned long *t2)
{
	unsigned long temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

int main(void)
{
	long n, i;
	struct node *root = (struct node *)malloc(sizeof(struct node));
	scanf("%ld", &n);
	for (i = 0; i < n; i++)
		scanf("%lu", &data[i]);
	srand(time(NULL));
	for (i = 0; i < n; i++)
		swap(&data[rand()%n], &data[rand()%n]);
//	for (i = 1; i < n; i++)
//		printf("%lu\n", data[i]);
	root->num = data[0];
	root->sum = 1;
	root->left = NULL;
	root->right = NULL;
	for (i = 1; i < n; i++)
		add(root, data[i]);
	dfs(root);
	return 0;
}