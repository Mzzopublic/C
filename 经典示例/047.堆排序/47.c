#include <stdio.h>
#define MAX 255
int R[MAX];

void Heapify(int s,int m)
{ /*对R[1..n]进行堆调整，用temp做暂存单元 */
     int j,temp;
     temp=R[s];
     j=2*s;
     while (j<=m)
     {
	  if (R[j]>R[j+1]&&j<m) j++;
	  if (temp<R[j]) break;
	  R[s]=R[j];
          s=j;
          j=j*2;
     }/* end of while */
     R[s]=temp;
} /* end of Heapify */

void BuildHeap(int n)
{ /* 由一个无序的序列建成一个堆 */
   int i;
   for(i=n/2;i>0;i--)
      Heapify(i,n);
}


void Heap_Sort(int n)
{ /* 对R[1..n]进行堆排序，不妨用R[0]做暂存单元 */
    int i;
    BuildHeap(n); /* 将R[1-n]建成初始堆 */
    for(i=n;i>1;i--)
    { /* 对当前无序区R[1..i]进行堆排序，共做n-1趟。 */
    	R[0]=R[1]; R[1]=R[i];R[i]=R[0]; /* 将堆顶和堆中最后一个记录交换 */
    	Heapify(1,i-1); /* 将R[1..i-1]重新调整为堆，仅有R[1]可能违反堆性质 */
    } /* end of for */
} /* end of Heap_Sort */
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
	Heap_Sort(n);
	puts("\nThe sequence after Big heap_sort is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	puts("\n Press any key to quit...");
	getch();
	
}