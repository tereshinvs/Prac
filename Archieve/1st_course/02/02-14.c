#include <stdio.h>
#include <math.h>

const double END_FLAG = -1.0, EPS = 10e-6;

double k;

double time_to_sleep(int n)
{
	double i, res = n;
	if (n == 0)
		return 0;
	for (i = k; 1; i *= k, res *= n)
		if (n <= i) return res;
}

double get_next_sleep_time(double cur_time)
{
	double i;
	for (i = k; i <= cur_time; i += k);
	return i;
}

int main(void)
{
	double sleep_time = 0, cur, last = 0, next_sleep, get_up_time;
	int n = 0;
	scanf("%lf%lf", &k, &cur);
	next_sleep = k;
	while (1)               	
	{
		if (fabs(cur - END_FLAG) < EPS)
		{
			printf("%.2lf\n", sleep_time/last*100);
			return 0;
		}
		if (cur < next_sleep)
		{
			n++;
			last = cur;
			scanf("%lf", &cur);
		} else {
			if (n == 0)
			{
				n = 1;
				while (next_sleep <= cur)
					next_sleep += k;
				last = cur;
				scanf("%lf", &cur);
				continue;
			}
			get_up_time = next_sleep + time_to_sleep(n);
			while (cur < get_up_time)
			{
				last = cur;
				scanf("%lf", &cur);
				if (fabs(cur - END_FLAG) < EPS)
				{
					if (get_up_time > last)
						get_up_time = last;
					printf("%.2lf\n", (sleep_time + get_up_time - next_sleep)/last*100);	
					return 0;
				}
			}
			sleep_time += get_up_time - next_sleep;
			next_sleep = get_next_sleep_time(get_up_time);
			if (fabs(cur - get_up_time) < EPS && fabs(cur - next_sleep) < EPS)
			{
				n = 1;
				next_sleep += k;
				last = cur;
				scanf("%lf", &cur);
			} else
				n = 0;
		}
	}
	return 0;                                                          
}