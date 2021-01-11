# Usage: key
# Prompt user for text to cipher
# Cipher through vigenere algorithm with key
# Return ciphered text
import sys
import string


def main(argv):

    # Init
    ALPHA = 26
    UPPER = 65
    LOWER = 97
    MOD = 32

    # Verify usage
    if len(sys.argv) != 2:
        print("Usage: python vigenere.py k")
        exit(1)
    if argv[1].isalpha():
        key = argv[1]
    else:
        print("Usage: python vigenere.py k")
        exit(1)

    # Prompt user text to cipher
    plain_text = input("plaintext: ")

    # Init ciphering variables
    N = len(key)
    i = 0

    # Output ciphered text
    print("ciphertext: ", end="")
    for char in plain_text:

        # Reset key loop
        if i == N:
            i = 0

        # chars -> indices -> ciphered index -> ciphered char
        if char.isalpha():
            # Retrieve indices and calculate new index
            key_index = ord(key[i]) % MOD - 1
            char_index = ord(char) % MOD - 1
            ciphered_index = (key_index + char_index) % ALPHA
            # Output ciphered character
            if char.islower():
                char = chr(ciphered_index + LOWER)
            else:
                char = chr(ciphered_index + UPPER)
            i += 1

        print(char, end="")
    print()

    exit(0)


if __name__ == "__main__":
    main(sys.argv)
