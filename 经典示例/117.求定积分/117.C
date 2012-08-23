#include "stdio.h"
#include "math.h"


double fsimpf(x) /*要进行计算的被积函数*/
double x;
{
	double y;
    y=log(1.0+x)/(1.0+x*x);
    return(y);
}

double fsimp(a,b,eps) /*辛普森算法*/
double a,b,eps; /*a为积分下限，b为积分上限，eps是希望达到的精度*/
{
    int n,k;
    double h,t1,t2,s1,s2,ep,p,x;
    n=1; h=b-a;
    t1=h*(fsimpf(a)+fsimpf(b))/2.0;  /*用梯形公式求出一个大概的估值*/
    s1=t1;
    ep=eps+1.0;
    while (ep>=eps)
	{
		/*用梯形法则计算*/
		p=0.0;
		for (k=0;k<=n-1;k++)
		{
			x=a+(k+0.5)*h;
			p=p+fsimpf(x);
		}
		t2=(t1+h*p)/2.0;
		/*用辛普森公式求精*/
		s2=(4.0*t2-t1)/3.0;
		ep=fabs(s2-s1);
		t1=t2; s1=s2; n=n+n; h=h/2.0;
	}
    return(s2);
}
main()
{
	double a,b,eps,t;
        a=0.0; b=1.0; eps=0.0000001;
    	clrscr();
	puts("**********************************************************");
	puts("*         This program is to calculat the Value of       *");
	puts("*          a definite integral by Simpson Method.        *");
	puts("**********************************************************");
    	t=fsimp(a,b,eps);
	puts("\n----------------------------------------------------------");
	printf(" >> The result of definite integral is : \n");
	printf(" >> SIGMA(0,1)ln(1+x)/(1+x^2)dx = ");
    	printf("%e\n",t);
    	puts("----------------------------------------------------------");
    	printf("\n Press any key to quit...");
	getch();
}

