#include <graphics.h>
#include <math.h>
void ellipse1(x0,y0,a,b,dt)
int x0,y0,a,b,dt;
{
	int x,y,n,i;
	float t1,t=0.0;
	t1=dt*0.0174533;
	n=360/dt;
	moveto(x0+a,y0);
	for(i=1;i<n;i++)
	{
		t=t+t1;
		x=x0+a*cos(t);
		y=y0+b*sin(t);
		lineto(x,y);
	}
	lineto(x0+a,y0);
}

void main()
{
	int i,a=200,x=320,y=240;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(8);
	setcolor(2);
	for(i=0;i<=200;i=i+10)
		ellipse1(x,y,a-i,i,10);
	getch();
	closegraph();
}
