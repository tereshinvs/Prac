#include <stdio.h>

int main(void)
	{
		double x1, x2, x3, y1, y2, y3;
		scanf("%lf%lf%lf%lf%lf%lf", &x1, &y1, &x2, &y2, &x3, &y3);
		printf("%.4lf %.4lf\n", (x1 + x2 + x3)/3.0, (y1 + y2 + y3)/3.0);
		return 0;
	}