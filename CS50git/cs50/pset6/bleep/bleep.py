# Usage: python bleep.py banned.txt
# Prompt user for string to censor
# Output censored text
import sys

banned_words = set()


def main(argv):

    # Verify usage
    if len(argv) != 2:
        print("Usage: python bleep.py banned.txt")
        exit(1)

    # Load set of banned words
    censorship = load(argv[1])
    if censorship == False:
        print(f"Couldn't load {argv[1]}")
        exit(2)

    # Prompt user for text to censor
    user_input = input("What message would you like to censor? ")

    # String -> list
    formatted = user_input.split()

    # Print censored message and include space
    for word in formatted:
        if word.lower() in banned_words:
            word = f"{'*' * len(word)}"
        print(f"{word} ", end="")
    print()

    exit(0)


# Open file and load content to banned_words
def load(text):

    try:
        file = open(text, "r")
    except FileNotFoundError:
        return False

    for line in file:
        banned_words.add(line.rstrip("\n"))
    file.close()

    return True


if __name__ == "__main__":
    main(sys.argv)
