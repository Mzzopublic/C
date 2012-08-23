#include <stdio.h>
#include <math.h>
#define MAX 50
/* 下面的两个数组可以根据具体要求解的多项式来决定其值*/
static double p[6]={4,-6,3,1,-1,5};	/*表示多项式4x^5 - 6x^4 + 3x^3 + x^2 - x + 5 */
static double q[4]={3,2,-5,1};		/*表示多项式3x^3 + 2x^2 - 5x + 1 */
static double result[9]={0,0,0,0,0,0,0,0,0};		/*存放乘积多项式*/

void npmul(p,m,q,n,s)
int m,n;
double p[],q[],s[];
{
	int i,j;
	for (i=0; i<=m-1; i++)
	for (j=0; j<=n-1; j++)
		s[i+j]=s[i+j]+p[i]*q[j];		/*迭带计算各项系数*/
	return;
}

double compute(s,k,x)					/*计算所给多项式的值*/
double s[];
int k;
float x;
{
	int i;
	float multip = 1;
	double sum = 0;
	for (i=0;i<k;i++)
		multip = multip * x;			/*先求出x的最高次项的值*/
	for (i=k-1;i>=0;i--)
	{
		sum = sum + s[i] * multip;		/*依次从高到低求出相对应次项的值*/
		if (x!=0)
		multip = multip / x;
	}
	return sum;
}

void main()
{
	int i,j,m,n;
	double  px[MAX],qx[MAX],rx[MAX];
	float x;
	clrscr();
	for(i=0;i<MAX;i++)
		rx[i]=0;
	puts("      This is a polynomial multiplication program.");
	puts("It calculate the product of two polynomials: P(x) and Q(x)");
	puts("       P(x)=Pm-1*x^(m-1)+Pm-2*x^(m-2)+...+P1*x+P0");
	puts("       Q(x)=Qn-1*x^(n-1)+Qn-2*x^(n-2)+...+Q1*x+Q0");
	printf("\n >> Please input m (>=1): ");
	scanf("%d",&m);
	printf(" >> Please input P%d, P%d, ... P1, P0 one by one:\n",m-1,m-2);
	for(i=0;i<m;i++)
		scanf("%f",&px[i]);
	printf("\n >> Please input n (>=1): ");
	scanf("%d",&n);
	printf(" >> Please input Q%d, Q%d, ... Q1, Q0 one by one:\n",n-1,n-2);
	for(i=0;i<n;i++)
		scanf("%f",&qx[i]);
	npmul(p,m,q,n,rx);
	printf("\nThe product of two polynomials R(x) is :\n");
	for (i=m+n-1,j=0;i>=1;i--)					/*逐行逐项打印出结果多项式*/
	{
		printf(" (%f*x^%d) + ",rx[m+n-1-i],i-1);
		if(j==2)
		{
			printf("\n");
			j=0;
		}
		else
			j++;
	}
	printf("\n");
	printf("Input the value of x: ");
	scanf("%f",&x);
	printf("\nThe value of the R(%f) is: %13.7f",x,compute(rx,m+n-1,x));
	puts("\n Press any key to quit...");
	getch();
}