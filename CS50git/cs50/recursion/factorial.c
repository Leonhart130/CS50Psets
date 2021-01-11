// Return factorial value
#include <stdio.h>
#include <stdlib.h>

int factorial(int n);

int main(int argc, char *argv[])
{
    // Verify correct usage
    if (!atoi(argv[1]))
    {
        printf("Not a number\n");
        return 1;
    }
    int factorialValue = atoi(argv[1]);

    printf("%i\n", factorial(factorialValue));

}

int factorial(int n)
{
    // Base case
    if (n == 1)
        return 1;

    return n * factorial(n - 1);
}