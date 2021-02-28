#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

long long int fib_naive(long long int n)
{
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return fib_naive(n - 2) + fib_naive(n - 1);
}

long long int _fib(long long int n, long long int *memo)
{
    if (memo[n] > -1)
        return memo[n];

    if (n == 0)
        return (memo[n] = 0);

    if (n == 1)
        return (memo[n] = 1);

    memo[n - 2] = _fib(n - 2, memo);
    memo[n - 1] = _fib(n - 1, memo);

    return (memo[n] = memo[n - 2] + memo[n - 1]);
}

long long int fib(long long int n)
{
    long long int *memo = (long long int *)malloc((n + 1) * sizeof(long long int));
    if (memo == NULL)
        return 0;

    for (long long int i = 0; i < n + 1; i++)
        memo[i] = -1;

    long long int res = _fib(n, memo);
    free(memo);

    return res;
}

int main(void)
{
    printf("fib_naive: %lld\n", fib_naive(1000));
    printf("fib: %lld\n", fib(1000));
}
