#include "stdio.h"
#include "math.h"
#include "graphics.h"

double lgam1(x) /*Gamma函数的计算*/
double x;
{ 
	int i;
    double y,t,s,u;
    static double a[11]={ 0.0000677106,-0.0003442342,
		0.0015397681,-0.0024467480,0.0109736958,
		-0.0002109075,0.0742379071,0.0815782188,
		0.4118402518,0.4227843370,1.0};
    if (x<=0.0)
	{ printf("err**x<=0!\n"); return(-1.0);}
    y=x;
    if (y<=1.0)
	{
		t=1.0/(y*(y+1.0)); y=y+2.0;}
    else if (y<=2.0)
	{ 
		t=1.0/y; y=y+1.0;}
    else if (y<=3.0) t=1.0;
    else
	{ 
		t=1.0;
        while (y>3.0)
		{ y=y-1.0; t=t*y;}
	}
    s=a[0]; u=y-2.0;
    for (i=1; i<=10; i++)
		s=s*u+a[i];
    s=s*t;
    return(s);
}  
double lgam2(a,x) /*不完全Gamma函数*/
double a,x;
{ 
	int n;
    double p,q,d,s,s1,p0,q0,p1,q1,qq;
    if ((a<=0.0)||(x<0.0))
	{ if (a<=0.0) printf("err**a<=0!\n");
	if (x<0.0) printf("err**x<0!\n");
	return(-1.0);
	}
    if (x+1.0==1.0) return(0.0);
    if (x>1.0e+35) return(1.0);
    q=log(x); q=a*q; qq=exp(q);
    if (x<1.0+a)
	{ 
		p=a; d=1.0/a; s=d;
        for (n=1; n<=100; n++)
		{ 
			p=1.0+p; d=d*x/p; s=s+d;
			if (fabs(d)<fabs(s)*1.0e-07)
			{ 
				s=s*exp(-x)*qq/lgam1(a);
                return(s);
			}
		}
	}
    else
	{ 
		s=1.0/x; p0=0.0; p1=1.0; q0=1.0; q1=x;
        for (n=1; n<=100; n++)
		{ 
			p0=p1+(n-a)*p0; q0=q1+(n-a)*q0;
            p=x*p0+n*p1; q=x*q0+n*q1;
            if (fabs(q)+1.0!=1.0)
			{ 
				s1=p/q; p1=p; q1=q;
                if (fabs((s1-s)/s1)<1.0e-07)
				{ 
					s=s1*exp(-x)*qq/lgam1(a);
                    return(1.0-s);
				}
                s=s1;
			}
            p1=p; q1=q;
		}
	}
    printf("a too large !\n");
    s=1.0-s*exp(-x)*qq/lgam1(a);
    return(s);
}

double lerrf(x) /*误差函数*/
double x;
{ 
	double y;
    if (x>=0.0)
		y=lgam2(0.5,x*x);
    else
		y=-lgam2(0.5,x*x);
    return(y);
}
double lgass(a,d,x) /*正态分布函数*/
double a,d,x;
{
	double y;
    if (d<=0.0) d=1.0e-10;
    y=0.5+0.5*lerrf((x-a)/(sqrt(2.0)*d));
    return(y);
}

main()
{ 
	int i;
	double j;
    double a, d;

	int gdriver = DETECT, gmode;
	clrscr();
	printf("This program will draw the Normal Distribution Graph.\n");
	printf("Please input the mathematical expectation (Alpha): ");
	scanf("%lf", &a );
	printf("Please input the variance (Sita >0): ");
	scanf("%lf", &d );
	/*registerbgidriver( EGAVGA_driver );*/
	initgraph( &gdriver, &gmode, "e:\\tc\\bgi" );
	
	setbkcolor( BLUE );
	moveto( 50, 430 );
	lineto( 590, 430 );
	outtextxy( 600, 425, "X");
	moveto( 200, 50 );
	lineto( 200, 450 );
	outtextxy( 200, 30, "Y" );
	outtextxy( 185, 435, "O");

	setcolor( RED );
	moveto( 51, 430 - 100 * lgass( a, d, -150.0 ) );
	for( i = 51; i <= 590; i++ )
	{
		j = 430 - 360 * lgass( a, d, (double)(i-200) );
		lineto( i, j );
	}
	getch();
	closegraph();
}