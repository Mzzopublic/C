#include<stdio.h>
#define MAX 255
void main()
{
    int i,j,t,k,m,a[MAX];
    long n;
    clrscr();
    puts("     This program will find the Armstrong number.\n");
    printf(" >> Please input the range you want to find (2~n):\n >> ");
    scanf("%ld",&n);
    m=n;
    j=10;
    while(m>=10)
    {
	m=m/10;
	j*=10;
    }
    printf(" >> There are follwing Armstrong number smaller than %d:\n",n);
    for(i=2;i<n;i++)         /*穷举要判定的数i的取值范围2~1000*/
    {
	for(t=0,k=10;k<=j;t++)     /*截取整数i的各位(从高向低位)*/
        {
	    a[t]=(i%k)/(k/10);        /*分别赋于a[0]~a[2}*/
	    k*=10;
	}
	for(m=0,t--;t>=0;t--)
		m+=a[t]*a[t]*a[t];
	if(m==i)
                                       /*判断i是否为阿姆斯特朗数*/
            printf("%5d",i);            /*若满足条件，则输出*/

    }
    printf("\n Press any key to quit...\n");
    getch();
}
