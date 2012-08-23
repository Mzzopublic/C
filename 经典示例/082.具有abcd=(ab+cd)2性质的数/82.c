#include<stdio.h>
void main()
{
    int n,a,b;
    clrscr();
    puts("==========================================================");
    puts("||  This program will find the four figures which have  ||");
    puts("||     the characteristic as follows: abcd=(ab+cd)^2.   ||");
    puts("||            e.g., 3025=(30+25)*(30+25).               ||");
    puts("==========================================================");
    printf("\n >> There are following numbers with satisfied condition:\n\n");
    for(n=1000;n<10000;n++)             /*四位数N的取值范围1000~9999*/
    {
        a=n/100;                        /*截取N的前两位数存于a*/
        b=n%100;                        /*截取N的后两位存于b*/
        if((a+b)*(a+b)==n)      /*判断N是否为符合题目所规定的性质的四位数*/
	    printf(" %d  ",n);
    }
    puts("\n\n >> Press any key to quit...");
    getch();
}
