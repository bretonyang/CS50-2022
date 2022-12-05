#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    //get height from user
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 0; i  < height; i++)
    {
        //print out space
        for (int j = 0; j < height - i - 1 ; j++)
        {
            printf(" ");
        }
        //print out blocks
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        printf("\n");
    }

}