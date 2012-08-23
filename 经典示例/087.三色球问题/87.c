#include<stdio.h>
void main()
{
    int i,j,count=0;
    clrscr();
    puts("****************************************************************");
    puts("*     This program is to solve Problem of Three Color Ball.    *");
    puts("* The Problem is as follows: There are 12 balls in the pocket. *");
    puts("* Amony them, 3 balls are red,3 balls are white and 6 balls    *");
    puts("* are black. Now take out any 8 balls from the pocket,how      *");
    puts("* many color combinations are there?                           *");
    puts("****************************************************************");
    puts(" >> The solutions are:");
    printf("  No.     RED BALL  WHITE BALL   BLACK BALL\n");
    printf("-----------------------------------------------------\n");
    for(i=0;i<=3;i++)               /*循环控制变量i控制任取红球个数0￣3*/
        for(j=0;j<=3;j++)           /*循环控制变量j控制任取白球个数0￣3*/
            if((8-i-j)<=6)
		printf(" %2d    |     %d     |    %d    |     %d\n",++count,i,j,8-i-j);

    printf("-----------------------------------------------------\n");
    printf(" Press any key to quit...");
    getch();
}
