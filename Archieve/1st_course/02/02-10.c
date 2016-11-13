#include <stdio.h>

int sqr(int t)
	{
		return t*t;
	}

int power(int t, int k)
	{
		if (k == 0)
			return 1;
		else
			return k%2 ? sqr(power(t, k/2))*t : sqr(power(t, k/2));
	}

int mod_power(int t, int k, int m)
	{
		if (k == 0)
			return 1%m;
		else
			return k%2 ? ((sqr(mod_power(t, k/2, m))%m)*t)%m : sqr(mod_power(t, k/2, m))%m;
	}

int max(int t1, int t2)
	{
		return t1 > t2 ? t1 : t2;
	}

int main(void)
	{
		int a, b, c, m, res = 0;
		scanf("%d%d%d%d", &a, &b, &c, &m);
		res = max(res, mod_power(a, power(b, c), m));
		res = max(res, mod_power(a, power(c, b), m));
		res = max(res, mod_power(b, power(a, c), m));
		res = max(res, mod_power(b, power(c, a), m));
		res = max(res, mod_power(c, power(a, b), m));
		res = max(res, mod_power(c, power(b, a), m));
		printf("%d\n", res);
		return 0;
	}