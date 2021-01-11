#include <stdio.h>

int collatz(int n);

int main(int argc, char *argv[])
{
    int c = 155;

    collatz(c);

    return 0;
}

int collatz(int n)
{
    if (n == 1)
    {
        fprintf(stdout, "%i\n", n);
        return n;
    }

    if (n % 2 == 0)
    {
        fprintf(stdout, "%i\n", n);
        return collatz(n / 2);
    }

    fprintf(stdout, "%i\n", n);
    return collatz(3 * n + 1);
}