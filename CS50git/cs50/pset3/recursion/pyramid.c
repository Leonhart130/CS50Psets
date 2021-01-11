#include <stdio.h>
#include <cs50.h>

void draw(int height);

int main(int argc, char *argv[])
{
    int height = get_int("Height: ");

    draw(height);

    return 0;
}

void draw(int height)
{
    if (height == 0)
        return;

    draw(height - 1);

    for(int i = 0; i < height; i++)
    {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");


}