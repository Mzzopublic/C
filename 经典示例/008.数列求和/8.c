#include <stdio.h>
int a[100005];
long long sum=0;
int main()
{
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		cin>>a[i];
		sum+=a[i];
	}
	printf("%d",sum);
	getchar();
	return 0;
}//改成竞赛风格了，不喜勿喷，可以revert
