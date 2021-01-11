#include <stdio.h>
#include <math.h>

int main(void)
{
    int voter_count = 14;

    double majority = floor((float) voter_count / 2) + 1;

    fprintf(stdout, "%.1f\n", majority);
    return 0;
}