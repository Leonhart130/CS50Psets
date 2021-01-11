# Take for input a credit card number
# Determine the provider
# Perform Luhn's algorithm to verify legimity
from sys import exit
import cs50


def main():

    # Init
    providers = {
        1: "VISA",
        2: "MASTERCARD",
        3: "AMEX"
    }

    # Prompt credit card number
    while True:
        credit_card = cs50.get_float("Number: ")
        if credit_card > 0:
            break

    # float -> int -> str -> list
    card = str(int(credit_card))
    card_arr = []
    for i in range(len(card)):
        card_arr.append(int(card[i]))

    # Perform provider recognition
    provider = get_provider(card_arr)
    if not provider:
        print("INVALID")
        exit(0)

    # Perform Luhn's algorithm and return provider or error
    legitimacy = luhn(card_arr)
    if legitimacy:
        print(providers[provider])
        exit(0)

    print("INVALID")
    exit(1)


# Verify length and first digits to determine provider
def get_provider(arr):

    # Init providers' constant properties
    VISALEN1 = 13
    VISA_MC_LEN2 = 16
    VISADIG = 4
    MCDIG1 = 51
    MCDIG2 = 55
    AMEXLEN = 15
    AMEXDIG1 = 34
    AMEXDIG2 = 37
    first_two = arr[0] * 10 + arr[1]

    # Visa check
    if len(arr) == VISALEN1 or len(arr) == VISA_MC_LEN2:
        if arr[0] == VISADIG:
            return 1

    # Mastercard check
    if len(arr) == VISA_MC_LEN2:
        if first_two >= MCDIG1 and first_two <= MCDIG2:
            return 2

    # American Express check
    if len(arr) == AMEXLEN:
        if first_two == AMEXDIG1 or first_two == AMEXDIG2:
            return 3

    return 0


# Multiply every other digit by 2, starting with the number’s second-to-last digit
# And then add those products’ digits together.
# Add the sum to the sum of the digits that weren’t multiplied by 2.
# If the total’s last digit is 0 (if the total modulo 10 is congruent to 0)
# The credit card is valid
def luhn(arr):

    # Init
    BASE = 10
    i = len(arr) - 2
    j = len(arr) - 1
    luhnValue = 0

    # First iteration from i
    while i >= 0:
        if (arr[i] * 2) >= BASE:
            luhnValue += arr[i] * 2 // BASE
        luhnValue += (arr[i] * 2) % BASE
        i -= 2

    # Second iteration from j
    while j >= 0:
        luhnValue += arr[j]
        j -= 2

    # Return legitimacy
    if not luhnValue % 10:
        return True
    return False


main()