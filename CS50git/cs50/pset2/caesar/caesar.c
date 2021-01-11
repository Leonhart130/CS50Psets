// 18/11/2019
// Caesar

// Take for command-line argument an integer
// Prompt user for a text
// Cipher and return the text

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

string ciphering(int key, string plainText);

int main(int argc, char *argv[])
{
    // Verify usage and store key
    if (argc != 2)
    {
        fprintf(stdout, "Usage: integer\n");
        return 1;
    }
    int key = 0;
    if (!sscanf(argv[1], "%i", &key))
    {
        fprintf(stdout, "Usage: integer\n");
        return 2;
    }

    // Prompt user for plain text
    string plainText = get_string("Text: ");

    // Print ciphered text
    fprintf(stdout, "ciphertext: %s\n", ciphering(key, plainText));

    return 0;
}

// Cipher function
string ciphering(int key, string plainText)
{
    // Init required variables
    int alphabeticalIndex = 0;
    int asciiMinUpper = 65;
    int asciiMinLower = 97;

    // Cipher formula is ci = (pi + k) % 26

    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        if (isalpha(plainText[i]))
        {
            // Cipher uppercase letter
            if (isupper(plainText[i]))
            {
                alphabeticalIndex = (int) plainText[i] - asciiMinUpper;
                alphabeticalIndex = (alphabeticalIndex + key) % 26;
                plainText[i] = alphabeticalIndex + asciiMinUpper;

            }
            // Cipher lowercase letter
            else
            {
                alphabeticalIndex = (int) plainText[i] - asciiMinLower;
                alphabeticalIndex = (alphabeticalIndex + key) % 26;
                plainText[i] = alphabeticalIndex + asciiMinLower;
            }
        }
    }
    return plainText;
}