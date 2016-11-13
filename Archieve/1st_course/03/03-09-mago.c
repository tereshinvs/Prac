#include <stdio.h>

int calc(int begin, int end, int *s)
{
    int summa = 0, i;
    for (i = begin; i < end; i++)
        summa +=s[i];
    return summa;        
}

int min(int t1, int t2)
{
    return (t1 < t2 ? t1 : t2);
}

void add(int *s, int x)
{  
    int i;
    for (i = x; i >= 0; i--)
        if (s[i] < 9) { s[i]++; return; }
        else s[i] = 0;
    return;
}

int main(void)
{
    char c;
    int n = 0, s[80], sum1 = 0, sum2 = 0, i, z;
    while (1)
    {
        scanf("%c", &c);
        if (c == '\n') break;
        s[n] = c - '0';
        n++;
    }
    if (n % 2)
    {
        for (i = n; i > 0; i--)
            s[i] = s[i - 1];
        n++;
        s[0] = 0;
    }
    sum1 = calc(0, n/2, s);
    sum2 = calc(n/2, n, s);
    //printf("%d %d\n", sum1, sum2);
    if (sum1 > sum2)
    {
        i = n-1;
        while (sum1 != sum2)
        {
            z = min(9 - s[i], sum1 - sum2);
            s[i] += z;
            sum2 +=z;
            i--;
        }
    }
    if (sum1 < sum2)
    {
        for (i = n - 1; i > 0; i--)
        {
            if (s[i] == 0) continue;
            s[i] = 0;
            add(s, i - 1);
            sum1 = calc(0, n/2, s);
            sum2 = calc(n/2, n, s);
            if (sum1 >= sum2) break;
        }
        i = n-1;
        while (sum1 != sum2)
        {
            z = min(9 - s[i], sum1 - sum2);
            s[i] += z;
            sum2 +=z;
            i--;
        }
    }     
    if (s[0] != 0) printf("%d", s[0]);
    for (i = 1; i < n; i++)
        printf("%d", s[i]);
    return 0;
}