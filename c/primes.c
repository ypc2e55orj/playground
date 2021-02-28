#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t u64;

u64 sqrt_u64(u64 n)
{
    double x = n;
    double l = 0;
    while (x != l)
    {
        l = x;
        x = (x + n / x) / 2;
    }

    return x;
}

u64 *get_primes(u64 n, u64 less_than, u64 *size)
{
    u64 *ns = (u64 *)malloc(less_than * sizeof(u64));
    if (ns == NULL)
    {
        return NULL;
    }

    u64 *ps = (u64 *)malloc(n * sizeof(u64));
    if (ps == NULL)
    {
        free(ns);
        return NULL;
    }

    u64 root = sqrt_u64(less_than);
    *size = 0;

    for (u64 i = 2; i < less_than; i++)
    {
        ns[i] = i;
    }

    for (u64 i = 2; i < less_than; i++)
    {
        if (!ns[i])
        {
            continue;
        }

        ps[(*size)++] = ns[i];
        if (n == *size)
        {
            break;
        }

        if (ns[i] <= root)
        {
            for (u64 j = i; i * j < less_than; j++)
            {
                ns[i * j] = 0;
            }
        }
    }

    free(ns);
    return ps;
}

int main(int argc, char *argv[])
{
    int n, less_than;
    u64 *primes, size;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <n> <less_than>\n", argv[0]);
        return 1;
    }

    n = abs(atoi(argv[1]));
    less_than = abs(atoi(argv[2]));

    if (n == 0 || less_than < 3 || less_than <= n)
    {
        return 1;
    }

    primes = get_primes(n, less_than, &size);
    if (primes == NULL)
    {
        return 1;
    }

    if (size < n)
    {
        free(primes);
        return 1;
    }

    printf("%ld\n", primes[n - 1]);

    free(primes);
    return 0;
}
