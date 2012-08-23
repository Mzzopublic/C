#include <graphics.h>
#include <math.h>
void polygonc();
void block();

void main()
{
	int i,j,a,length,n,theta,x=100,y=350;
	int gdriver=DETECT,gmode;
	printf("input length, n theta:");
	scanf("%d%d%d",&length,&n,&theta);
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(9);
	setcolor(4);
	a=length/4;
	for(i=1;i<=4;i++)
	{
		for(j=1;j<=4;j++)
		{
			block(x,y,a,n,theta);
			theta=-theta;
			x=x+a;
		}
		x=100;
		y=y-a;
		theta=-theta;
	}
	getch();
	closegraph();
}
void polygonc(x0,y0,r,n,af)
int x0,y0,n,r;
float af;
{
	int x,y,xs,ys,i;
	float dtheta,theta;
	if(n<3)
		return;
	dtheta=6.28318/n;
	theta=af*0.0174533;
	xs=x0+r*cos(theta);
	ys=y0+r*sin(theta);
	moveto(xs,ys);
	for(i=1;i<n;i++)
	{
		theta=theta+dtheta;
		x=x0+r*cos(theta);
		y=y0+r*sin(theta);
		lineto(x,y);
	}
	lineto(xs,ys);
}
void block(x,y,a,n,theta)
int x,y,a,n,theta;
{
	int x0,y0,i,r;
	float t,f,af=45.;
	t=fabs(theta*0.0174533);
	f=1.0/(cos(t)+sin(t));
	r=a/1.414;
	x0=x+0.5*a;
	y0=y+0.5*a;
	for(i=1;i<=n;i++)
	{
		polygonc(x0,y0,r,4,af);
		r=r*f;
		af=af-theta;
	}
}