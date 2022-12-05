# loop while height is not in [1, 8]
while True:
    height = input("Height: ")

    if height.isdigit():
        height = int(height)
        if height <= 8 and height >= 1:
            break

# print the pyramid
for i in range(height):
    for j in range(height - i - 1):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print()