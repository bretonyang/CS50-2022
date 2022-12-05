#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int check_arg(int argc, string argv[]);
string shift_func(int k, string plaintext);


int main(int argc, string argv[])
{
    //checking single command-line argument with only digits
    int argcheck = check_arg(argc, argv);
    if (argcheck == 1)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //convert string into integer
    int K = atoi(argv[1]);

    //get the plaintext
    string plaintext = get_string("plaintext: ");

    //shift the plaintext
    string ciphertext = shift_func(K, plaintext);

    //print out the ciphertext
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}


// check if the argument is single and digits only
int check_arg(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]) == false)
        {
            return 1;
        }
    }
    return 0;
}

//shift the plaintext by k
string shift_func(int k, string plaintext)
{
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isupper(plaintext[i]))
        {
            plaintext[i] = 'A' + ((plaintext[i] - 'A' + k) %  26);
        }
        else if (islower(plaintext[i]))
        {
            plaintext[i] = 'a' + ((plaintext[i] - 'a' + k) % 26);
        }
    }
    return plaintext;
}