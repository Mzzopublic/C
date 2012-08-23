#include<stdio.h>
#define M 255
int a[M];

void main()
{
    int i,j,k,s,n=0;
    clrscr();
    puts("****************************************************************");
    puts("*    This program is to solve Problem of Stamp Combination.    *");
    puts("* The Problem is as follows. John has 4 stamps with value of 3 *");
    puts("* cents and 3 stamps with value of 5 cents. Use one or more of *");
    puts("* these stamps, how many kinds of postages can John provide?   *");
    puts("****************************************************************");
    puts("\n >> The solution is: \n");
    printf("---------------------------------------------\n\n");
    for(i=0;i<=4;i++)           /*i:取三分邮票的张数*/
        for(j=0;j<=3;j++)       /*j:取5分邮票的张数*/
        {
            s=i*3+j*5;        /*计算组成的邮票面值*/
            for(k=0;a[k];k++)     /*查找是否有相同的邮资*/
                if(s==a[k])break;
            if(!a[k]&&s)         /*没有找到相同的邮资则满足要求存入数组*/
            {
                a[k]=s; n++;
            }
        }
    printf(" >> There are %d kinds of postages:\n\n",n);      /*输出结果*/
    for(k=0;a[k];k++)
	printf(" %d",a[k]);
    printf("\n");
    printf("\n---------------------------------------------\n");
    printf("\n Press any key to quit...");
    getch();
}
