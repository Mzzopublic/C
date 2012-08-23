#include<stdio.h>
#include<stdlib.h>
#define MAX 100
struct Bag
{	int weight;
int value;
}Bag[MAX];
int a[MAX];
int Value=0;
int Weight;
int comb(int m,int k)
{int i,j;
int wei,val;
for(i=m;i>=k;i--)
{ a[k]=i;
if(k>1)
comb(i-1,k-1);
else
{ wei=0;                    /* 预值0 */ 
val=0;
for(j=a[0];j>0;j--)              /* 每一种组合求它们的重量和价值 */
{ wei=wei+Bag[a[j]].weight;
val=val+Bag[a[j]].value;
}
if(wei<=Weight&&val>Value)    /* 判断是否满足条件进行附值 */
Value=val;
}
}
return Value;
}
void main()
{ int num,subnum;
int l,
clrscr();
printf("输入背包的总个数:"); /* 输入背包的总个数 */
scanf("%d",&num);
printf("输入背包的重量和价值\n",num);  /* 输入背包的重量和价值 */
for(l=1;l<=num;l++)
scanf("%d,%d",&Bag[l].weight,&Bag[l].value);
printf("输入满足要求背包的个数:\n");                    /* 输入要求背包的个数 */
scanf("%d",&subnum);
printf("输入满足条件的重量:\n",subnum);                 /* 输入满足条件的重量*/
scanf("%d",&Weight);
a[0]=subnum;
printf("the max value is:%d",comb(num,subnum));
getch();
}
