#include <stdio.h>
#include <stdint.h>

typedef uint8_t  BYTE;

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

int main(int argc, char *argv[])
{


    fprintf(stdout, "%lu\n", sizeof(RGBTRIPLE));

    return 0;
}