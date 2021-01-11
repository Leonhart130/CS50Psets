# Usage: python crack.py hash
# try up to 53^5 possibilities of encryption
# if success, return password
import sys
import crypt


def main(argv):

    # Verify usage
    if len(argv) != 2:
        print("Usage: python crack.py hash")
        exit(1)

    # Init
    USER_HASH = argv[1]
    SALT = USER_HASH[0:2]
    # Sorted through Robert Lewand's Cryptological Mathematics for faster crack
    LETTERS = " eEtTaAoOiInNsShHrRdDlLcCuUmMwWfFgGyYpPbBvVkKjJxXqQzZ"

    # Perform decryption
    for char5 in LETTERS:
        for char4 in LETTERS:
            for char3 in LETTERS:
                for char2 in LETTERS:
                    for char1 in LETTERS:

                        # Generate password
                        password = f"{char1}{char2}{char3}{char4}{char5}".strip()

                        # Try combination
                        if crypt.crypt(password, SALT) == USER_HASH:
                            print(password)
                            exit(0)

    # Couldn't be cracked
    exit(2)


if __name__ == "__main__":
    main(sys.argv)
