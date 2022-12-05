#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int count_letters(string array);
int count_words(string array);
int count_sentences(string array);

int main(void)
{
    //Getting user input
    string text = get_string("Text: ");

    //Getting numbers of letters, words, sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    //Calculate the grade
    double L = ((double)letters / words) * 100;
    double S = ((double)sentences / words) * 100;
    double index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    //Print out the result
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

//count letters
int count_letters(string array)
{
    // count number of letters
    int letters = 0;
    for (int i = 0, n = strlen(array); i < n; i++)
    {
        if (isalpha(array[i]))
        {
            letters++;
        }
    }
    return letters;
}

//count words
int count_words(string array)
{
    //count number of words
    int words = 0;
    for (int i = 0, n = strlen(array); i < n; i++)
    {
        if (array[i] == ' ')
        {
            words++;
        }
    }
    return words + 1;
}

//count sentences
int count_sentences(string array)
{
    //count number of sentences
    int sentences = 0;
    for (int i = 0, n = strlen(array); i < n; i++)
    {
        if (array[i] == '.')
        {
            sentences++;
        }
        else if (array[i] == '!')
        {
            sentences++;
        }
        else if (array[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}