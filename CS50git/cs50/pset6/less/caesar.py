# Usage: key
# Prompt user for text to cipher
# Cipher through caesar algorithm with key
# Return ciphered text
import sys
import string


def main(argv):

    # Init
    ALPHA = 26
    UPPER = 65
    LOWER = 97
    MOD = 32
    key = 0

    # Verify usage
    if len(sys.argv) != 2:
        print("Usage: key")
        exit(1)
    try:
        key = int(argv[1])
    except ValueError:
        print("Usage: key")
        exit(1)

    # Prompt user text to cipher
    plain_text = input("plaintext: ")

    # Output ciphered text
    print("ciphertext: ", end="")
    for char in plain_text:
        # char -> index -> ciphered index -> ciphered char
        if char.isalpha():
            ciphered_index = (ord(char) % MOD) - 1
            ciphered_index = (ciphered_index + key) % ALPHA
            if char.islower():
                char = chr(ciphered_index + LOWER)
            else:
                char = chr(ciphered_index + UPPER)
        print(char, end="")
    print()

    exit(0)


if __name__ == "__main__":
    main(sys.argv)
