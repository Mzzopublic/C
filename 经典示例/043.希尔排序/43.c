#include <stdio.h>
#define MAX 255
int R[MAX];
void Bubble_Sort(int n)
{ /* R（l..n)是待排序的文件，采用自下向上扫描，对R做冒泡排序 */
     int i,j;
     int exchange; /* 交换标志 */
     for(i=1;i<n;i++){ /* 最多做n-1趟排序 */
       exchange=0; /* 本趟排序开始前，交换标志应为假 */
       for(j=n-1;j>=i;j--) /* 对当前无序区R[i..n]自下向上扫描 */
        if(R[j+1]<R[j]){/* 交换记录 */
          R[0]=R[j+1]; /* R[0]不是哨兵，仅做暂存单元 */
          R[j+1]=R[j];
          R[j]=R[0];
          exchange=1; /* 发生了交换，故将交换标志置为真 */
         }
       if(!exchange) /* 本趟排序未发生交换，提前终止算法 */
	     return;
     }
}

void main()
{
	int i,n;
	clrscr();
	puts("Please input total element number of the sequence:");
	scanf("%d",&n);
	if(n<=0||n>MAX)
	{
		printf("n must more than 0 and less than %d.\n",MAX);
		exit(0);
	}
	puts("Please input the elements one by one:");
	for(i=1;i<=n;i++)
		scanf("%d",&R[i]);
	puts("The sequence you input is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	Bubble_Sort(n);
	puts("\nThe sequence after bubble_sort is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	puts("\n Press any key to quit...");
	getchar();
	getchar();
}