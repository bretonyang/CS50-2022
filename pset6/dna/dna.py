# A program for identifying to whom a sequence of DNA belongs (recursive version)

import sys
import csv


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    data = []

    # read csv file into data
    with open(sys.argv[1], "r") as csv_file:
        reader = csv.DictReader(csv_file)

        # get the name of each column
        headers = reader.fieldnames

        # store databases into a list of dicts
        for row in reader:
            # convert str into int
            for i in range(1, len(headers)):
                row[headers[i]] = int(row[headers[i]])

            data.append(row)

    # read dna datas into a list
    with open(sys.argv[2], "r") as dna_file:
        dna = dna_file.read()

    # print the person's name if matched else print "No match"
    result = find_match(data, headers, dna)
    print(result)
    sys.exit(0)


def find_match(data, headers, dna):

    candidate = data.copy()

    # loop through each column (i.e STRs)
    for i in range(1, len(headers)):

        # change the recursion limit for longer dna sequences (Be aware that this might create stackoverflow)
        sys.setrecursionlimit(100000)

        # calculate STR counts
        count_STR = calculate(headers[i], dna, 0, 0)

        # loop through every remaining candidate
        j = 0
        while j < len(candidate):
            # eliminate incorrect candidates
            if candidate[j][headers[i]] != count_STR:
                del candidate[j]
                continue
            j += 1

    # return "No match" if no candidate is matched
    if len(candidate) == 0:
        return "No match"

    # return name of the candidate if there's only one candidate left
    if len(candidate) == 1:
        return candidate[0][headers[0]]


def calculate(STR, dna, index, max_count):
    # recursion base case
    if index > len(dna) - len(STR):
        return max_count

    count = 0
    for i in range(index, 1 + len(dna) - len(STR), len(STR)):
        # increment the count if the sequence is repeated
        if dna[i:i+len(STR)] == STR:
            count += 1
        # break otherwise
        else:
            break
    # update max_count
    max_count = max(count, max_count)
    index += 1
    # recursive case
    return calculate(STR, dna, index, max_count)


if __name__ == "__main__":
    main()
