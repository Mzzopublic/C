#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
int brinv(a,n) /*求矩阵的逆矩阵*/
int n; /*矩阵的阶数*/
double a[]; /*矩阵A*/
{
	int *is,*js,i,j,k,l,u,v;
    double d,p;
    is=malloc(n*sizeof(int));
    js=malloc(n*sizeof(int));
    for (k=0; k<=n-1; k++)
	{
		d=0.0;
		for (i=k; i<=n-1; i++)
			/*全选主元，即选取绝对值最大的元素*/
			for (j=k; j<=n-1; j++)
			{
				l=i*n+j; p=fabs(a[l]);
				if (p>d) { d=p; is[k]=i; js[k]=j;}
			}
		/*全部为0，此时为奇异矩阵*/
		if (d+1.0==1.0)
		{
			free(is); free(js); printf(" >> This is a singular matrix, can't be inversed!\n");
			return(0);
		}
		/*行交换*/
		if (is[k]!=k)
			for (j=0; j<=n-1; j++)
			{
				u=k*n+j; v=is[k]*n+j;
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
		/*列交换*/
		if (js[k]!=k)
			for (i=0; i<=n-1; i++)
			{
				u=i*n+k; v=i*n+js[k];
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
		l=k*n+k;
		a[l]=1.0/a[l]; /*求主元的倒数*/
		/* a[kj]a[kk] -> a[kj] */
		for (j=0; j<=n-1; j++)
			if (j!=k)
			{
				u=k*n+j; a[u]=a[u]*a[l];
			}
		/* a[ij] - a[ik]a[kj] -> a[ij] */
		for (i=0; i<=n-1; i++)
			if (i!=k)
				for (j=0; j<=n-1; j++)
					if (j!=k)
					{
						u=i*n+j;
						a[u]=a[u]-a[i*n+k]*a[k*n+j];
					}
		/* -a[ik]a[kk] -> a[ik] */
		for (i=0; i<=n-1; i++)
			if (i!=k)
			{
				u=i*n+k; a[u]=-a[u]*a[l];
			}
	}
    for (k=n-1; k>=0; k--)
	{
		/*恢复列*/
		if (js[k]!=k)
			for (j=0; j<=n-1; j++)
			{
				u=k*n+j; v=js[k]*n+j;
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
	    /*恢复行*/
		if (is[k]!=k)
			for (i=0; i<=n-1; i++)
			{
				u=i*n+k; v=i*n+is[k];
				p=a[u]; a[u]=a[v]; a[v]=p;
			}
	}
    free(is); free(js);
    return(1);
}
print_matrix(a,n)/*打印的方阵a的元素*/
int n; /*矩阵的阶数*/
double a[]; /*矩阵a*/
{
	int i,j;
	for (i=0; i<n; i++)
	{

		for (j=0; j<n; j++)
			printf("%13.7f\t",a[i*n+j]);
		printf("\n");
	}
}
main()
{
	int i,j,n=0;
	double A[MAX],B[MAX],C[MAX];
    static double a[4][4]={ {0.2368,0.2471,0.2568,1.2671},
	{1.1161,0.1254,0.1397,0.1490},
	{0.1582,1.1675,0.1768,0.1871},
	{0.1968,0.2071,1.2168,0.2271}};
    static double b[4][4],c[4][4];
    clrscr();
    puts("**********************************************************");
    puts("*    This program is to inverse a square matrix A(nxn).  *");
    puts("**********************************************************");
    while(n<=0)
    {
    	printf(" >> Please input the order n of the matrix (n>0): ");
    	scanf("%d",&n);
    }

    printf(" >> Please input the elements of the matrix one by one:\n >> ");
    for(i=0;i<n*n;i++)
    {
    	scanf("%lf",&A[i]);
    	B[i]=A[i];
    }
    for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		b[i][j]=a[i][j];

    i=brinv(A,n);

    if (i!=0)
    {
	printf("    Matrix A:\n");
	print_matrix(B,n);
	printf("\n");
	printf("    A's Inverse Matrix A-:\n");
	print_matrix(A,n);
	printf("\n");
	printf("    Product of A and A- :\n");
	MatrixMul(B,A,n,n,n,C);
	print_matrix(C,n);
    }
    printf("\n Press any key to quit...");
    getch();
}
