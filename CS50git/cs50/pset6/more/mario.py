# Prompt user for a height
# Print a mario pyramid (more)
from sys import exit
import cs50


def main():
    # Define constants
    MIN = 1
    MAX = 8

    # do while loop, prompt user input
    while True:
        n = cs50.get_int("Height: ")
        if n >= MIN and n <= MAX:
            break

    # Init
    hashes = 1
    spaces = n - 1

    # Print pyramid
    for i in range(n):

        # Print spaces, hashes and new line
        # First row
        print(" " * spaces, end="")
        print("#" * hashes, end="")
        # Second row
        print("  ", end="")
        print("#" * hashes, end="")

        print()

        # Update values
        hashes += 1
        spaces -= 1

    exit(0)


main()
