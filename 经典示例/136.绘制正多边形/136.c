#include <graphics.h>
#include <math.h>

void polygon(x0,y0,a,n,af)
int x0,y0,a,n;
float af;
{
	int x,y,i;
	float dtheta,theta;
	if(n<3)
		return;
	dtheta=6.28318/n;
	theta=af*0.0174533;
	moveto(x0,y0);
	x=x0;
	y=y0;
	for(i=0;i<n;i++)
	{
		x=x+a*cos(theta);
		y=y+a*sin(theta);
		lineto(x,y);
		theta=theta+dtheta;
	}
	lineto(x0,y0);
}
void main()
{
	int i,a=80,x=200,y=100;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"C:\\tc");
	cleardevice();
	setbkcolor(9);
	setcolor(4);
	for(i=3;i<=10;i++)
		polygon(x,y,a,i,0.);
	getch();
	closegraph();
}
