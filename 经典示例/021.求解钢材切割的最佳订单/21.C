#include <stdio.h>
#define N 20
#define DELTA 2
int bestlen;
int bestsele[N];
int sele[N];
int n;
int orderlen[N];
int total;
main()
{
	int i;
	clrscr();
	printf("\n Please enter total length of the steel:\n");/* 输入钢材总长 */
	scanf("%d",&total);
	printf("\n Please enter number of order:\n");  /* 输入定单数 */
	scanf("%d",&n);
	printf("\n Please enter the orders:\n"); /* 输入各定单 */
	for(i=0;i<n;i++)
		scanf("%d",&orderlen[i]);
	bestlen=0;	/*最佳解用料的初值 */
	for(i=0;i<n;i++)
		sele[i]=bestsele[i]=0;	/*置当前选择和最佳选择初值 */
	try();	/* 调用函数求解 */
	for(i=0;i<n;i++) /* 输出结果 */
		if(bestsele[i])
			printf("order %d length = %d\n",i+1,orderlen[i]);
	printf("\n Press any key to quit...");
	getch();
}
try()
{
	int i,len;
	for(len=i=0;i<n;i++)	/* 求当前选中的用料量 */
		if(sele[i])
			len+=orderlen[i]+DELTA;
	if(len-DELTA<=total)	/* 注意最后一段可能不需要切割 */
	{
		if(bestlen < len)
		{
			/* 找到一个更好的解 */
			bestlen = len;
			for(i=0;i<n;i++)
				bestsele[i]=sele[i];
		}
		for(i=0;i<n;i++) /* 对所有未选定单逐一作选中尝试循环 */
			if(!sele[i])
			{
				sele[i]=1;	/* 做选中尝试*/
				try();
				sele[i]=0;
			}
	}
}

