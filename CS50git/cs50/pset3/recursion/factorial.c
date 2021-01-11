#include <stdio.h>

int factorial(int fact);

int main(int argc, char *argv[])
{
    int n = 5;

    fprintf(stdout, "%i\n", factorial(n));

    return 0;
}

int factorial(int fact)
{
    if (fact == 1)
        return fact;
    return fact * factorial(fact - 1);
}