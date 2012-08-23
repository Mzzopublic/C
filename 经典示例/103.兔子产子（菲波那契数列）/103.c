#include<stdio.h>
void main()
{
   int n,i,j,un1,un2,un;
   clrscr();
   puts("********************************************************");
   puts("*   This is a program to Calculate Rabbits Numbers.    *");
   puts("* There is a rabbit couple procreats 2 rabbits 1 month,*");
   puts("* and the young rabbits group and can procreats in the *");
   puts("* end of the second month. In this way,how many rabbits*");
   puts("* are there after n generations?                       *");
   puts("********************************************************");
   for(n=2;n<3;)
   {
      printf(" >> Please input number of generations (n>2): ");
      scanf("%d",&n);
      if(n<3) printf("\n >> Input error!\n");     /*控制输入正确的N值*/
   }
   un=un2=1;
   printf(" >> The numbers of rabbits in first %d generation are as follows:\n",n);
   printf(" l\t l\t");

   for(i=3,j=0;i<=n;i++)
   {
      un1=un2;
      un2=un;
      un=un1+un2;      /*利用通项公式求解N项的值*/
      printf(   i%8?" %d\t":"%d\n",un);
   }
   printf("\n");
   printf("\n Press any key to quit...");
   getch();
}
