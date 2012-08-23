int a[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
main()
{
	int i;
	clrscr();
	printf("\n The arry is:\n");
	for(i=0;i<sizeof(a)/sizeof(a[0]);i++)
		printf(" %d ",a[i]);
	printf("\n SUM of the arry is: %d\n",sum(a,sizeof(a)/sizeof(a[0])));
	printf("\n Press any key to quit...\n");
	getch();
}
sum(int a[],int n)
{
	if(n<=0)
		return 0;
	return a[n-1]+sum(a,n-1);
}