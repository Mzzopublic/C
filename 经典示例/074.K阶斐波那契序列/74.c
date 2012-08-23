#include <stdio.h>
#include <stdlib.h>
#define  enoughsize 100  //最大队列长度
typedef struct 
{
	int *base;     //初始化的动态分配存储空间
	int front;      //头指针,若队列不空,指向队列头元素
	int rear;      //尾指针,若队列不空,指向队列尾元素的下一个位置
}SqQueue;
int AddSum(int n,int *q)
{
	int sum=0;
	int i;
	for(i=0;i<n;i++)  sum+=q[i];
	return sum;
}

void main()
{
	SqQueue Q;
	int k,max,i,n,*store;
	printf("请输入斐波那奇的阶数:");
	scanf("%d",&k);
	printf("请输入序列中允许的最大数:");
	scanf("%d",&max);
	Q.base=(int*)malloc(k*sizeof(int));
	store=(int*)malloc(enoughsize*sizeof(int));
	if((!Q.base)||(!store))
	{
		printf("Error!");
		return;
	}
	for(i=0;i<k;i++)
	{
		store[i]=0;
		Q.base[i]=0;
	}
	store[k-1]=1;
	Q.base[k-1]=1;   //初始化fib序列
	store[k]=AddSum(k,Q.base);
	Q.front=0;
	Q.rear=k-1;
	n=k;
	while(store[n]<=max)
	{
		Q.rear=(Q.rear+1)%k;
		Q.base[Q.rear]=store[n];
		n++;
		store[n]=AddSum(k,Q.base);
	}
	printf("The first %d%s%d%c%s",n," numbers are less than ",max,'.',"\n");
	printf("The numbers are:\n");
	for(i=0;i<n;i++)  printf("%d%c",store[i],' ');
	printf("\n");
}
