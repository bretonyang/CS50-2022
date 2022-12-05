#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars;
    //prompt the user for dollars
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);

    //convert dolllars to cents
    int cents = round(dollars * 100);

    int i, j, k;
    //count number of quarters
    for (i = 0; cents >= 25; i++)
    {
        cents = cents - 25;
    }
    //count number of dimes
    for (j = 0; cents >= 10; j++)
    {
        cents = cents - 10;
    }
    //cont numbers of nickels
    for (k = 0; cents >= 5; k++)
    {
        cents = cents - 5;
    }
    printf("%i\n", i + j + k + cents);
}