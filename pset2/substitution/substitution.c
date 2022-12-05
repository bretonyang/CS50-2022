#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    //check if the user type in single command-line argument, and has 26 characters
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters!\n");
        return 1;
    }

    //turn the key to upper case and check it's all alphabetic
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must contain only alphabets!\n");
            return 1;
        }
        else if (islower(argv[1][i]))
        {
            argv[1][i] = toupper(argv[1][i]);
        }
    }

    //check for repetition
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (i != j && argv[1][i] == argv[1][j])
            {
                printf("You should contain each letter exactly once!\n");
                return 1;
            }
        }
    }

    //prompt the user for plaintext and print ciphertext:
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    //encipher the plaintext and print it out
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            //print uppercase letters
            if (isupper(plaintext[i]))
            {
                plaintext[i] = argv[1][plaintext[i] - 'A'];
                printf("%c", plaintext[i]);
            }
            //print lowercase letters
            else if (islower(plaintext[i]))
            {
                plaintext[i] = argv[1][plaintext[i] - 'a'] + 'a' - 'A';
                printf("%c", plaintext[i]);
            }
        }
        //print non alphabetic characters
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}