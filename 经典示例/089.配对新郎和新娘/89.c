#include<stdio.h>
void main()
{
    int x,y,z;
    clrscr();
    puts("****************************************************************");
    puts("*  This program is to solve Problem of Bridegroom and Bride.   *");
    puts("* The Problem is as follows: Someone goes to 3 couples lovers' *");
    puts("* wedding. The bridegrooms are A,B,C and the brides are X,Y,Z. *");
    puts("* He wants to know who marries who and asks them. A says he    *");
    puts("* will marry to X, X says her fiance is C, C says he will marry*");
    puts("* to Z. The man knows that they are all kidding. What they said*");
    puts("* is not true. So try to find who will marry to who?           *");
    puts("****************************************************************");
    puts(" >> The solutions are:");
    printf("---------------------------------------------\n");
    for(x=1;x<=3;x++)          /*穷举x的全部可能配偶*/
        for(y=1;y<=3;y++)      /*穷举y的全部可能配偶*/
            for(z=1;z<=3;z++)    /*穷举z的全部可能配偶*/
                if(x!=1&&x!=3&&z!=3&&x!=y&&x!=z&&y!=z)  /*判断配偶是否满足题意*/
                {
		    printf(" X will marry to %c.\n",'A'+x-1);    /*打印判断结果*/
		    printf(" Y will marry to %c.\n",'A'+y-1);
		    printf(" Z will marry to %c.\n",'A'+z-1);
                }
    printf("---------------------------------------------\n");
    printf(" Press any key to quit...");
    getch();
}
