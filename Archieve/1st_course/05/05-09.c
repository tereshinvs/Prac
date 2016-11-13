#include <stdio.h>

#define INF 100000
#define QUEUE_SIZE 1000000

int res[500][500], alr[500][500], queue[QUEUE_SIZE][2], qi = 0, qk = 0, n, m;

void push(int x, int y) {
	if (alr[x][y])
		return;
	alr[x][y] = 1;
	queue[qk][0] = x;
	queue[qk][1] = y;
	qk++;
}

void try_to_go(int x1, int y1, int x2, int y2) {
	if (x2 < 0 || x2 == n || y2 < 0 || y2 == m)
		return;
	if (res[x2][y2] > res[x1][y1] + 1) {
		res[x2][y2] = res[x1][y1] + 1;
		push(x2, y2);
	}
}

int main(void) {
	int k, i, j, ans, x, y;
	scanf("%d%d%d", &n, &m, &k);
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			res[i][j] = INF;
			alr[i][j] = 0;
		}
	for (i = 0; i < k; i++) {
		scanf("%d%d", &x, &y);
		res[--x][--y] = 0;
		push(x, y);
	}
	while (qi < qk) {
		x = queue[qi][0]; y = queue[qi][1]; qi++;
		try_to_go(x, y, x - 1, y);
		try_to_go(x, y, x + 1, y);
		try_to_go(x, y, x, y - 1);
		try_to_go(x, y, x, y + 1);
	}
	ans = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			if (res[i][j] > ans)
				ans = res[i][j];
	printf("%d\n", ans);
	return 0;
}