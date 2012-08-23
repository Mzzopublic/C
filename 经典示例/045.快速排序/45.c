#include <stdio.h>
#define MAX 255
int R[MAX];
void ShellPass(int d, int n)
{/* 希尔排序中的一趟排序，d为当前增量 */
     int i,j;
     for(i=d+1;i<=n;i++) /* 将R[d+1．．n]分别插入各组当前的有序区 */
       if(R[i]<R[i-d])
       {
         R[0]=R[i];j=i-d; /* R[0]只是暂存单元，不是哨兵 */
         do {/* 查找R[i]的插入位置 */
            R[j+d]=R[j];/* 后移记录 */
            j=j-d; /* 查找前一记录 */
         }while(j>0&&R[0]<R[j]);
         R[j+d]=R[0]; /* 插入R[i]到正确的位置上 */
       } /* endif */
   } /* end of ShellPass */

void  Shell_Sort(int n)
{
    int increment=n; /* 增量初值，不妨设n>0 */
    do {
          increment=increment/3+1; /* 求下一增量 */
	  ShellPass(increment,n); /* 一趟增量为increment的Shell插入排序 */
       }while(increment>1);
} /* ShellSort */


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
	Shell_Sort(n);
	puts("\nThe sequence after shell_sort is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	puts("\n Press any key to quit...");
	getch();
	
}