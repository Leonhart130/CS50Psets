// Print and apply Collatz conjecture
#include <stdio.h>
#include <stdlib.h>

int collatz(int n, int steps);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2 || atoi(argv[1]) < 1 || !atoi(argv[1]))
    {
        printf("Usage: positive integer\n");
        return 1;
    }
    int value = atoi(argv[1]);

    int steps = 0;
    collatz(value, steps);
}

int collatz(int n, int steps)
{
    printf("%i\n", n);
    // Base case
    if (n == 1)
    {
        printf("%i steps\n", steps);
        return n;
    }
    steps++;
    // N is even
    if (n % 2 == 0)
        return collatz(n / 2, steps);
    // N is odd
    else
        return collatz(3 * n + 1, steps);
}