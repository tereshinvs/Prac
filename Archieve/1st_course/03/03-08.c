#include <stdio.h>

int max(int t1, int t2)
{
	return t1 < t2 ? t2 : t1;
}

struct segment
{
	int left, right;
};

int compare(struct segment *t1, struct segment *t2)
{
	if (t1->left == t2->left)
		return 0;
	if (t1->left > t2->left)
		return 1;
	else
		return -1;
}

void swap(struct segment *t1, struct segment *t2)
{
	struct segment temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

void sort(struct segment *left, struct segment *right)
{
	struct segment *ileft = left, *iright = right, med = *(left + (right - left)/2);
	do
	{
		while (compare(ileft, &med) < 0)
			ileft++;
		while (compare(iright, &med) > 0)
			iright--;
		if (ileft <= iright)
			swap(ileft++, iright--);
	} while (ileft <= iright);
	if (left < iright)
		sort(left, iright);
	if (ileft < right)
		sort(ileft, right);
}

struct segment data[100000];

int main(void)
{
	int n, i;
	struct segment res;
	scanf("%d", &n);
	for (i = 0; i < n; i++)
		scanf("%d%d", &data[i].left, &data[i].right);
	sort(data, data + n - 1);
	res = data[0];
	for (i = 1; i < n; i++)
		if (data[i].left <= res.right)
			res.right = max(data[i].right, res.right);
		else {
			printf("%d %d ", res.left, res.right);
			res = data[i];
		}
	printf("%d %d ", res.left, res.right);
	return 0;
}