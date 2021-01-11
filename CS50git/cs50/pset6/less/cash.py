# Prompt user for an amount of change
# Return the amount of coins through greedy algorithm

from sys import exit
import cs50


def main():

    # Init
    QUARTER = 25
    DIME = 10
    NICKEL = 5
    PENNY = 1
    coins = 0

    # Prompt user change, format
    while True:
        change = cs50.get_float("Change: ")
        if change > 0:
            break
    change = round(change * 100)

    # Perform greedy algorithm
    if change / QUARTER:
        coins += change // QUARTER
        change = change % QUARTER
    if change / DIME:
        coins += change // DIME
        change = change % DIME
    if change / NICKEL:
        coins += change // NICKEL
        change = change % NICKEL
    if change / PENNY:
        coins += change // PENNY
        change = change % PENNY

    # Return coins
    print(f"{coins}")

    exit(0)


if __name__ == "__main__":
    main()