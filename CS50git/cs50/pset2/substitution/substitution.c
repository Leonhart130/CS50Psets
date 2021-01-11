// 18/11/2019
// Substitution

// Take for command-line argument
// A 26 alphabetical characters key
// Prompt user for plain text
// Print ciphered text

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Prototypes
bool checkDuplicate(string key, char letter, int position);
string substitute(string key, string plaintext);

int main(int argc, char *argv[])
{
    // Verify usage
    if (argc != 2)
    {
        fprintf(stdout, "Usage: ./substitution KEY\n");
        return 1;
    }

    // Check key length
    int keyLength = strlen(argv[1]);
    if (keyLength != 26)
    {
        fprintf(stdout, "Usage: Key of 26 characters\n");
        return 1;
    }

    // Check characters
    char *key = malloc(sizeof(char) * keyLength + 1);
    for (int i = 0; i < keyLength; i++)
    {
        // Remind usage if character is not alphabetical
        if (!isalpha(argv[1][i]))
        {
            fprintf(stdout, "Usage: alphabetical characters only\n");
            return 1;
        }
        else
        {
            // Store formatted key
            key[i] = tolower(argv[1][i]);

            // Prevent duplicate on key
            if (!checkDuplicate(key, key[i], i))
            {
                fprintf(stdout, "Character %c is already in key\n", key[i]);
                return 1;
            }
        }
    }
    key[keyLength] = '\0';

    // Prompt user for text
    string plainText = get_string("plaintext: ");

    // Call cipher function
    fprintf(stdout, "ciphertext: %s\n", substitute(key, plainText));

    free(key);

    return 0;
}

bool checkDuplicate(string key, char letter, int position)
{
    for (int i = 0; i < position; i++)
    {
        if (key[i] == letter)
        {
            return false;
        }
    }
    return true;
}

// Cipher user plain text
string substitute(string key, string plaintext)
{
    // Init required variables
    int alphabeticalIndex = 0;
    int asciiMinUpper = 65;
    int asciiMinLower = 97;

    // Cipher alphabetical characters
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                alphabeticalIndex = (int) plaintext[i] - asciiMinUpper;
                plaintext[i] = toupper(key[alphabeticalIndex]);
            }
            else
            {
                alphabeticalIndex = (int) plaintext[i] - asciiMinLower;
                plaintext[i] = tolower(key[alphabeticalIndex]);
            }
        }
    }
    return plaintext;
}