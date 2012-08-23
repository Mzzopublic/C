int a[ ]={1,2,3,4,5};
#define N sizeof a/sizeof a[0]
main()
{
	int j,   /*游标变量*/
	      *p; /*指针变量*/
	clrscr();
	for(j=0;j<N;j++)/*数组名和下标顺序访问数组的元素*/
		printf("a[%d]\t= %d\t",j,a[j]);
	printf("\n");
	for(p=a;p<a+N;p++)/*让指针顺序指向数组的各元素，遍历数组*/
	printf("*p\t= %d\t",*p);
	printf("\n");/*指针与游标变量结合，改变游标变量遍历数组*/
	for(p=a,j=0;p+j<a+N;j++)
		printf("*(p+%d)\t= %d\t",j,*(p+j));
	printf("\n");/*指针与游标变量结合，用指针和下标遍历数组*/
	for(p=a+N-1,j=N-1;j>=0;j--)
		printf("p[-%d]\t= %d\t",j,p[-j]);
	printf("\nPress any key to quit...\n");
	getch();
}