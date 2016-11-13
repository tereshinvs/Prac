#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

int *arr;
unsigned arr_pos;

void
print_read(int arr_num)
{
    printf("RD %08x\n", arr_pos + arr_num * sizeof(int));
}

void
print_write(int arr_num)
{
    printf("WD %08x\n", arr_pos + arr_num * sizeof(int));
}

void
dumb(int low, int high)
{
    int i = low, j = high, pivot, t;
    if (low + 1 >= high) return;
    t = (low + high) / 2;
    pivot = arr[t];

    print_read(t);

    while (i < j) {

        print_read(i);
        if (arr[i] >= pivot)
            print_read(j - 1);

        if (arr[i] >= pivot && arr[j - 1] <= pivot) {
            t = arr[i];

            print_read(i);

            arr[i] = arr[j - 1];

            print_read(j - 1);
            print_write(i);

            arr[j - 1] = t;

            print_write(j - 1);

            ++i;
            --j;
        } else {

            print_read(i);

            if (arr[i] <= pivot) {
                ++i;
            }

            print_read(j - 1);

            if (arr[j - 1] >= pivot) {
                --j;
            }
        }
    }
    dumb(low, j);
    dumb(j, high);
}

int
main(int argc, char *argv[])
{
    sscanf(argv[1], "%x", &arr_pos);
    int n;
    sscanf(argv[2], "%d", &n);

    arr = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    dumb(0, n);

    free(arr);

    return 0;
}
