// Take for input a hash (with salt)
// try to crack a 5-letter alphabetical password
#include <stdio.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char *argv[])
{
    // Verify usage
    if (argc != 2)
    {
        printf("Usage: hash (with salt)\n");
        return 1;
    }

    // Init
    char *hash = argv[1];
    char salt[2];
    memcpy(salt, hash, 2);
    char password[6] = "\0\0\0\0\0\0";

    // Sorted through Robert Lewand's Cryptological Mathematics for faster crack
    char letters[53] = "\0etaoinshrdlcumwfgypbvkjxqzETAOINSHRDLCUMWFGYPBVKJXQZ";
    // 26 + 26 + 1 "\0" = 53
    const int letters_count = 53;


    // First try to match hash with 53 tries before adding another character
    // Then try 53^2 times before adding another character, etc...
    for (int fifthChar = 0; fifthChar < letters_count; fifthChar++)
    {
        for (int fourthChar = 0; fourthChar < letters_count; fourthChar++)
        {
            for (int thirdChar = 0; thirdChar < letters_count; thirdChar++)
            {
                for (int secondChar = 0; secondChar < letters_count; secondChar++)
                {
                    for (int firstChar = 1; firstChar < letters_count; firstChar++)
                    {
                        // Update password's characters
                        password[0] = letters[firstChar];
                        password[1] = letters[secondChar];
                        password[2] = letters[thirdChar];
                        password[3] = letters[fourthChar];
                        password[4] = letters[fifthChar];

                        // If hash matches, print password and return
                        if (!strcmp(crypt(password, salt), hash))
                        {
                            printf("%s\n", password);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 2;
}