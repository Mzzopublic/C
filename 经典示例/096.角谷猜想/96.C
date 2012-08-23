#include<stdio.h>
void main()
{
    int n=1,count=0;
    clrscr();
    puts("*********************************************************");
    puts("*      >> This program is to verify Jiaogu Guess  <<    *");
    puts("* That is given any natural number, if it is an even,   *");
    puts("* divides 2, if it is an odd, multiple 3 and add 1, the *");
    puts("* result continues to be calculated analogously. After  *");
    puts("* some times, the result is always 1.                   *");
    puts("*********************************************************");
    while(n!=0)
    {
	    printf(" >> Please input a number to verify(0 to quit): ");
	    scanf("%d",&n);      /*输入任一整数*/
	    if(n==0)
		break;
	    printf(" >> ------ Results of verification: ------------\n");
	    do{
	        if(n%2)
	        {
	            n=n*3+1;           /*若为奇数，n乘3加1*/
		    printf(" >> Step No.%d: %d*3+1=%d\n",++count,(n-1)/3,n);
	        }
	        else
	        {
	            n/=2;          /*若为偶数n除以2*/
		    printf(" >> Step No.%d: %d/2=%d\n",++count,2*n,n);
	        }
	    }while(n!=1);             /*n不等于1则继续以上过程*/
	    printf(" >> ---------------------------------------------\n");
    }
    puts("\n Press any key to quit...");
    getch();
}
