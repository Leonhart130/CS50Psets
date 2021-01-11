// argv[1] name of forensic image
// Generates recovered JPEG files as 000, 001..
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Verify usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: forensic image\n");
        return 1;
    }

    // Remember filename
    char *infile = argv[1];

    // Open input file, remind usage
    FILE *toRecover = fopen(infile, "r");
    if (toRecover == NULL)
    {
        fprintf(stderr, "Can't open %s\n", infile);
        return 2;
    }

    // Init
    BYTE buffer[512];
    int fileInt = 0;
    const BYTE namelen = 7;
    char *filename = malloc(sizeof(namelen));
    if (filename == NULL)
    {
        fprintf(stderr, "Can't allocate memory for filename\n");
        return 3;
    }

    // Read the file until you reach the first JPEG
    while (!(buffer[0] == 0xff &&
             buffer[1] == 0xd8 &&
             buffer[2] == 0xff &&
             (buffer[3] & 0xf0) == 0xe0))
    {
        fread(&buffer, sizeof(buffer), 1, toRecover);
    }

    // Read the file until the end
    while (!feof(toRecover))
    {
        // Edit filename
        sprintf(filename, "%03i.jpg", fileInt);
        FILE *img = fopen(filename, "w");

        // Write the first buffer and edit fread for next condition
        fwrite(&buffer, sizeof(buffer), 1, img);
        fread(&buffer, sizeof(buffer), 1, toRecover);

        // Write all the content until end of file or end of card
        while (!(buffer[0] == 0xff &&
                 buffer[1] == 0xd8 &&
                 buffer[2] == 0xff &&
                 (buffer[3] & 0xf0) == 0xe0))
        {
            // If end of file, end the loop
            if (feof(toRecover))
            {
                break;
            }
            // Copy paste image to file
            fwrite(&buffer, sizeof(buffer), 1, img);
            fread(&buffer, sizeof(buffer), 1, toRecover);
        }
        // Close file, change name for next file
        fclose(img);
        fileInt += 1;
    }

    free(filename);
    return 0;
}