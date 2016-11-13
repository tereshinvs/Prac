#include <stdio.h>
#include <string.h>

int mat[10][10], n, res[10000000], k = 0, alr[10], res_depth = 0, first_node;
char data[10][20];

void dfs(int v, int depth)
{
	int i;
	alr[v] = 1;
	if (depth == res_depth)
		res[k++] = first_node;
	if (depth > res_depth)
	{
		res_depth = depth;
		res[(k = 1) - 1] = first_node;
	}
	for (i = 0; i < n; i++)
		if (mat[v][i] && !alr[i])
			dfs(i, depth + 1);
	alr[v] = 0;
}

int main(void)
{
	int i, j, flag;
	scanf("%d", &n);
	for (i = 0; i < n; i++)
	{
		scanf("%s", data[i]);
		alr[i] = 0;
	}
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			mat[i][j] = i != j && data[i][strlen(data[i]) - 1] == data[j][0];
	for (first_node = 0; first_node < n; first_node++)
		dfs(first_node, 1);
	printf("%d\n", res_depth);
	for (i = 0; i < n; i++)
	{
		flag = 0;
		for (j = 0; j < k && !flag; j++)
			if (res[j] == i)
				flag = 1;
		if (flag)
			printf("%s\n", data[i]);
	}
	return 0;
}