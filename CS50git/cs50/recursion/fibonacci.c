#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2 || !atoi(argv[1]))
    {
        printf("Usage: integer greater than \n");
        return 1;
    }

    int nthelement = atoi(argv[1]);


}
int fibonacci(int element)
{
    if (element == 0)
        return 0;
    else if (element == 1)
        return 1;



}