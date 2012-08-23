rest(int a[], int n)
{
	int i,low,high,t;

	for(i=0,low=0,high=n-1;i<=high;)
	{
		if(a[i]>0)
		{
			/*a[i]与a[high]交换，随之high减1*/
			t=a[i];
			a[i]=a[high];
			a[high]=t;
			high--;
		}
		else if(a[i]==0)
			i++; /* 掠过该元素 */
		else
		{
			/*a[i]与a[low]交换，随之low增1, i增1*/
			t=a[i];
			a[i]=a[low];
			a[low]=t;
			low++;
			i++;
		}
	}
}
int s[]={8,4,0,-1,6,0,-5};
main()
{
	int i;
	clrscr();
	printf("\n The arry before rest is:\n");
	for(i=0;i<sizeof(s)/sizeof(s[0]);i++)
		printf("%4d",s[i]);
	rest(s,sizeof(s)/sizeof(s[0]));
	printf("\n The arry after rest is:\n");
	for(i=0;i<sizeof(s)/sizeof(s[0]);i++)
		printf("%4d",s[i]);
	printf("\n Press any key to quit...\n");
	getch();
}