#include <stdio.h>
#define MAX 255
int R[MAX];

void Merge(int low,int m,int high)
{/* 将两个有序的子文件R[low..m)和R[m+1..high]归并成一个有序的 */
     /* 子文件R[low..high] */
     int i=low,j=m+1,p=0; /* 置初始值 */
     int *R1; /* R1是局部向量，若p定义为此类型指针速度更快 */
     R1=(int *)malloc((high-low+1)*sizeof(int));
     if(!R1) /* 申请空间失败 */
     {
       puts("Insufficient memory available!");
       return;
     }
     while(i<=m&&j<=high) /* 两子文件非空时取其小者输出到R1[p]上 */
       R1[p++]=(R[i]<=R[j])?R[i++]:R[j++];
     while(i<=m) /* 若第1个子文件非空，则复制剩余记录到R1中 */
       R1[p++]=R[i++];
     while(j<=high) /* 若第2个子文件非空，则复制剩余记录到R1中 */
       R1[p++]=R[j++];
     for(p=0,i=low;i<=high;p++,i++)
       R[i]=R1[p];/* 归并完成后将结果复制回R[low..high] */
} /* end of Merge */


void Merge_SortDC(int low,int high)
{/* 用分治法对R[low..high]进行二路归并排序 */
       int mid;
       if(low<high)
       {/* 区间长度大于1 */
          mid=(low+high)/2; /* 分解 */
	  Merge_SortDC(low,mid); /* 递归地对R[low..mid]排序 */
	  Merge_SortDC(mid+1,high); /* 递归地对R[mid+1..high]排序 */
          Merge(low,mid,high); /* 组合，将两个有序区归并为一个有序区 */
        }
}/* end of Merge_SortDC */


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
	Merge_SortDC(1,n);
	puts("\nThe sequence after merge_sortDC is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	puts("\n Press any key to quit...");
	getch();
	
}