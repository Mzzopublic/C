#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
int binom (int n, int i);
void main (void)
    {
    int int1;
int int2;
//打印两项式
    printf ("NOTE: Entering a figure other than a number will \ncause the program to crash.");
    printf ("\n");
    printf ("Formula computation is in B(n,i) N, 1st integer >= 2nd, I, Integer.\n\n");
    printf (" n!\n");
    printf (" B(n,i)= ----------\n");
    printf ("k!(n-i)!\n\n");
    printf ("Warning: Program has no error checking!\n\n");
    printf ("\nEnter an integer :");
    scanf ("%d", &int1);
    printf ("\n");
    printf ("Enter a second integer :");
    scanf ("%d", &int2);
    printf ("\n");
    printf ("Binomial Coefficiant : %d", binom (int1, int2));
    getch();
}
//算法计算
int binom (int n, int i)
    {
    int n1;
    int n2;
    if (( i ==0 ) || (n== i))
        {
        return 1;
    }
    else
        {
        n1 = binom ( n - 1, i);
        n2 = binom ( n - 1, i -1);
        return n1 + n2;
    }
    }
