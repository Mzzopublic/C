#include "stdio.h"
#define MAX 255
void MatrixMul(a,b,m,n,k,c)  /*实矩阵相乘*/
int m,n,k; /*m:矩阵A的行数, n:矩阵B的行数, k:矩阵B的列数*/
double a[],b[],c[]; /*a为A矩阵, b为B矩阵, c为结果，即c = AB */
{
	int i,j,l,u;
	/*逐行逐列计算乘积*/
	for (i=0; i<=m-1; i++)
		for (j=0; j<=k-1; j++)
		{
			u=i*k+j; c[u]=0.0;
			for (l=0; l<=n-1; l++)
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
		}
	return;
}



main()
{
	int i,j,m,n,k;
	double A[MAX];
	double B[MAX];
	double C[MAX];
	for(i=0;i<MAX;i++)
		C[i]=1.0;
	clrscr();
	puts("This is a real-matrix-multiplication program.\n");
	puts("It calculate the two matrixes C(m*k)=A(m*n)B(n*k).\n");
	printf(" >> Please input the number of rows in A, m= ");
	scanf("%d",&m);
	printf(" >> Please input the number of cols in A, n= ");
	scanf("%d",&n);
	printf(" >> Please input the number of cols in B, k= ");
	scanf("%d",&k);
	printf(" >> Please input the %d elements in A one by one:\n",m*n);
	for(i=0;i<m*n;i++)
		scanf("%lf",&A[i]);
	printf(" >> Please input the %d elements in B one by one:\n",n*k);
	for(i=0;i<n*k;i++)
		scanf("%lf",&B[i]);

	MatrixMul(A,B,m,n,k,C); /*计算C的结果*/
	/*格式化输出结果*/
	printf("\n >> The result of C(%d*%d)=A(%d*%d)B(%d*%d) is:\n",m,k,m,n,n,k);
	for (i=0; i<m; i++)
	{
		for (j=0; j<k; j++)
			printf("%10.5f    ",C[i*k+j]);
		printf("\n");
	}
	printf("\n Press any key to quit...\n");
	getch();
	return 0;
}
