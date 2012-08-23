#include<stdio.h>
void vrNico(int a)
{
    int b,c,d;
    b=a*a*a;                  /*求整数的三次方*/
    printf(" >> %d*%d*%d=%d=",a,a,a,b);
    for(d=0,c=0;c<a;c++)       /*输出数列，首项为a*a-a+1,等差值为2*/
    {
        d+=a*a-a+1+c*2;       /*求数列的前a项的和*/
        printf(c?"+%d":"%d",a*a-a+1+c*2);
    }
    if(d==b)printf(" Satisfy!\n");    /*若条件满足则输出"Y"*/
    else printf(" Dissatisfy!\n");       /*否则输出"N"*/
}


void main()
{

    int n=1;
    clrscr();
    puts("******************************************************");
    puts("*    This program is to verify Theorem of Nicoqish.  *");
    puts("* That is the cube of any integer can be represented *");
    puts("* as the sum of some continue odd numbers.           *");
    puts("* For example, 8^3=512=57+58+59+61+63+65+67+69+71.   *");
    puts("******************************************************");
    while(n!=0)
    {
	    printf(" >> Please input a integer to verify(0 to quit): ");
	    scanf("%d",&n);      /*输入任一整数*/
	    if(n==0)
		break;
	    printf(" >> ------ Results of verification: ------------\n");
	    vrNico(n);           /*调用函数进行验证*/
	    printf(" >> ---------------------------------------------\n");
    }
    puts("\n Press any key to quit...");
    getch();
}
