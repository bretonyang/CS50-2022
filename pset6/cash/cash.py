# get a non-negative number
while True:
    cash = input("Change owed: ")

    # check whether cash is number
    try:
        cash = float(cash)
    except ValueError:
        continue

    # check if cash is a non-negative float
    if cash < 0:
        continue

    cash *= 100
    break

count = 0

# handles quaters
while cash >= 25:
    cash -= 25
    count += 1


# handles dimes
while cash >= 10:
    cash -= 10
    count += 1

# handles nickles
while cash >= 5:
    cash -= 5
    count += 1

# handles pennies
while cash >= 1:
    cash -= 1
    count += 1

print(count)