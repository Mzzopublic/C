#include<stdio.h>
void verify_four_squares(int number)
{
    int i,j,k,l;
    for(i=1;i<number/2;i++)         /*试探法。试探i,j,k,k的不同值*/
	        for(j=0;j<=i;j++)
	            for(k=0;k<=j;k++)
	                for(l=0;l<=k;l++)
	                    if(number==i*i+j*j+k*k+l*l)    /*若满足定理要求则输出结果*/
	                    {
				printf(" >> %d=%d*%d+%d*%d+%d*%d+%d*%d\n",number,i,i,j,j,k,k,l,l);
				return;
	                    }
}
void main()
{
    int number=1;

    clrscr();
    puts("*****************************************************");
    puts("* This program is to verify Theorem of Four Squares.*");
    puts("* That is all natural numbers can be represented as *");
    puts("* sum of no more than 4 squares of the numbers.     *");
    puts("*****************************************************");
    while(number!=0)
    {
	    printf(" >> Please input a number to verify(0 to quit): ");
	    scanf("%d",&number);      /*输入任一整数*/
	    if(number==0)
		break;
	    printf(" >> ------ Results of verification: ------------\n");
	    verify_four_squares(number);
	    printf(" >> ---------------------------------------------\n");
    }
    puts("\n Press any key to quit...");
    getch();
}
