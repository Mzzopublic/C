/* 函数circle用于判断正整数n的d进制数表示形式是否是回文数 */
int circle(int n, int d)
{
	int s=0,m=n;
	while(m)
	{
		s=s*d+m%d;
		m/=d;
	}
	return s==n;
}
/* main函数用于测试circle函数 */
int num[]={232,27,851};
int scale[]={2,10,16};
main()
{
	int i,j;
	clrscr();
	for(i=0;i<sizeof(num)/sizeof(num[0]);i++)
		for(j=0;j<sizeof(scale)/sizeof(scale[0]);j++)
			if(circle(num[i],scale[j]))
				printf("%d -> (%d) is a Circle Number!\n",num[i],scale[j]);
			else
				printf("%d -> (%d) is not a Circle Number!\n",num[i],scale[j]);
	printf("\n Press any key to quit...\n");
	getch();
}
	