#include <stdio.h>
#define MAX 255
int R[MAX];
void Insert_Sort(int n)
{ /* 对数组R中的记录R[1..n]按递增序进行插入排序  */
    int i,j;
    for(i=2;i<=n;i++) /* 依次插入R[2]，…，R[n] */
      if(R[i]<R[i-1])
      {/* 若R[i]大于等于有序区中所有的R，则R[i] */
                              /* 应在原有位置上 */
        R[0]=R[i];j=i-1; /* R[0]是哨兵，且是R[i]的副本 */
        do{ /* 从右向左在有序区R[1．．i-1]中查找R[i]的插入位置 */
         R[j+1]=R[j]; /* 将关键字大于R[i]的记录后移 */
         j--;
         }while(R[0]<R[j]);  /* 当R[i]≥R[j]时终止 */
        R[j+1]=R[0]; /* R[i]插入到正确的位置上 */
       }
}

main()
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
	Insert_Sort(n);
	puts("\nThe sequence after insert_sort is:");
	for(i=1;i<=n;i++)
		printf("%4d",R[i]);
	puts("\n Press any key to quit...");
	getchar();
	getchar();
}
