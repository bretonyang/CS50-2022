# a program to check whether a card number is valid via Luhn’s algorithm.

from cs50 import get_string
import sys


card = get_string("Number: ")
length = len(card)

# Luhn's algorithm
even_sum = 0
odd_sum = 0
valid = False
for i in range(length - 2, -1, -2):
    tmp = str(2 * int(card[i]))
    # add those products’ digits together
    for j in range(len(tmp)):
        even_sum += int(tmp[j])

for i in range(length - 1, -1, -2):
    odd_sum += int(card[i])

# validate the card
if (even_sum + odd_sum) % 10 == 0:
    valid = True

if valid:
    # check the beginning of the card number
    begin = int(card[0:2])
    # VISA card
    if card[0] == '4' and (length == 13 or length == 16):
        print("VISA")
        sys.exit(0)

    # American Express
    elif begin in {34, 37} and length == 15:
        print("AMEX")
        sys.exit(0)

    # MasterCard
    elif length == 16 and begin in {51, 52, 53, 54, 55}:
        print("MASTERCARD")
        sys.exit(0)

print("INVALID")
sys.exit(0)
