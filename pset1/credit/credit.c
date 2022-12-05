#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long card_num;

    //prompt user for card number
    do
    {
        card_num = get_long("Number: ");
    }
    while (card_num < 0);

    //calculate even sum
    long check = card_num;
    int sum = 0, i, geteven;
    for (i = 0; check > 9; i++)
    {
        geteven = ((check / 10) % 10) * 2;
        sum = sum + (geteven % 10) + ((geteven / 10) % 10);
        check = check / 100;
    }

    //calculate odd sum and total sum
    long ch = card_num;
    int j, getodd;
    for (j = 0; ch > 0; j++)
    {
        getodd = ch % 10;
        sum = sum + getodd;
        ch = ch / 100;
    }


    int checksum = sum % 10, digits = i + j;
    long start = card_num;

    //get the start
    do
    {
        start = start / 10;
    }
    while (start > 100);

    //print out visa
    if ((checksum == 0) && (digits == 13 || digits == 16) && (start / 10 == 4))
    {
        printf("VISA\n");
    }

    //print out mastercard
    else if ((checksum == 0) && (digits == 16) && (start > 50 && start < 56))
    {
        printf("MASTERCARD\n");
    }

    //print out AMEX
    else if ((checksum == 0) && (digits == 15) && (start == 34 || start == 37))
    {
        printf("AMEX\n");
    }

    //print out invalid
    else
    {
        printf("INVALID\n");
    }

}
