#include <graphics.h>
#include <math.h>
void polygonc();
void main()
{
	int i,r,n,x=320,y=240;
	int gdriver=9,gmode=2;
	float theta,alfa;
	n=30;
	theta=6.0;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(MAGENTA);
	setcolor(WHITE);
	r=160;
	alfa=60.0;
	for(i=0;i<n;i++)
	{
		polygonc(x,y,r,6,alfa);
		r=(int)(0.866*r/cos((30.0-theta)*0.0174533));
		alfa=alfa+theta;
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
