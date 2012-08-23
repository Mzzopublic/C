#include<stdio.h>
void main()
{
    long int a,n=0;
    clrscr();
    puts("***********************************************************");
    puts("*      >>      This program is to verify       <<         *");   
    puts("*      >>     odd number's characteristic.     <<         *");
    puts("* That is square of an odd number larger than 1000 minus  *");
    puts("* 1 can be divided exactly by 8.                          *");
    puts("* For example, 2001^2-1=4004000=500500*8.                 *");
    puts("***********************************************************");
    while(n<1001)
    {
    	printf(" >> Please input the range you want to verify: ");
    	scanf("%ld",&n);
    }
    puts(" >> Now start to verify:");
    for(a=1001;a<=n;a+=2)
    {
        printf("%ld:",a);       /*输出奇数本身*/
        printf("(%ld*%ld-1)/8",a,a);      /*输出(奇数的平方减1)/8*/
        printf("=%ld",(a*a-1)/8);        /*输出被8除后的商*/
        printf("+%ld\n",(a*a-1)%8);      /*输出被8除后的余数*/
    }
    puts("\n Press any key to quit...");
    getch();
}
