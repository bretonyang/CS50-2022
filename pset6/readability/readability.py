from cs50 import get_string

s = get_string("Text: ")

# count letters, words and sentences
letters = sum(char.isalpha() for char in s)
words = 1 + s.count(' ')
sentences = s.count('!') + s.count('?') + s.count('.')


# calculate the grade
L = letters * 100 / words
S = sentences * 100 / words
grade = round(0.0588 * L - 0.296 * S - 15.8)

# print out the result
if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade " + str(grade))
