#include<stdlib.h>
#include<stdio.h>
int comp(const void *j,const void *i);
void p(int p,int b[],char n[]);
void main()
{
    static char n[]={'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
    int a[53],b1[13],b2[13],b3[13],b4[13];
    int b11=0,b22=0,b33=0,b44=0,t=1,m,flag,i;
    clrscr();
    puts("******************************************************");
    puts("*      This is an Automatic Dealing Card program!    *");
    puts("*  In one game, 52 cards are divided into 4 groups.  *");
    puts("******************************************************");
    printf(" >> ----- Each person's cards are as follows. -------");
    while(t<=52)           /*控制发52张牌*/
    {     m=random(52);      /*产生0到51之间的随机数*/
          for(flag=1,i=1;i<=t&&flag;i++)   /*查找新产生的随机数是否已经存在*/
            if(m==a[i]) flag=0;      /*flag=1:产生的是新的随机数
                                        flag=0:新产生的随机数已经存在*/
          if(flag)
          {
             a[t++]=m;       /*如果产生了新的随机数，则存入数组*/
             if(t%4==0) b1[b11++]=a[t-1];        /*根据t的模值，判断当前*/
             else if(t%4==1) b2[b22++]=a[t-1];   /*的牌应存入哪个数组中*/
             else if(t%4==2) b3[b33++]=a[t-1];
             else if(t%4==3) b4[b44++]=a[t-1];
          }
      }
      qsort(b1,13,sizeof(int),comp);      /*将每个人的牌进行排序*/
      qsort(b2,13,sizeof(int),comp);
      qsort(b3,13,sizeof(int),comp);
      qsort(b4,13,sizeof(int),comp);
      p(1,b1,n); p(2,b2,n); p(3,b3,n); p(4,b4,n);     /*分别打印每个人的牌*/
      printf(" >> ----------- Press any key to quit... ------------");
      getch();
}
void p(int p,int b[],char n[])
{
   int i;
   printf("\n   Person No.%d   \006 ",p);        /*打印黑桃标记*/
   for(i=0;i<13;i++)        /*将数组中的值转换为相应的花色*/
      if(b[i]/13==0) printf(" %c",n[b[i]%13]);      /*该花色对应的牌*/
   printf("\n                 \003 ");        /*打印红桃标记*/
   for(i=0;i<13;i++)
      if((b[i]/13)==1) printf(" %c",n[b[i]%13]);
   printf("\n                 \004 ");        /*打印方块标记*/
   for(i=0;i<13;i++)
      if(b[i]/13==2) printf(" %c",n[b[i]%13]);
   printf("\n                 \005 ");        /*打印梅花标记*/
   for(i=0;i<13;i++)
      if(b[i]/13==3||b[i]/13==4) printf(" %c",n[b[i]%13]);
   printf("\n");
}
int comp(const void *j,const void *i)     /*qsort调用的排序函数*/
{
   return(*(int*)i-*(int*)j);
}
