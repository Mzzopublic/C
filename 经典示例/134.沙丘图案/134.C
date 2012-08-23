/* Sandhill */
#include <graphics.h>
#include <math.h>
#define PI 3.1415926
void main()
{
	double a,b,c;
	int x,y,py;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(9);
	setcolor(14);
	outtextxy(80,3,"This program show the Sandhill picture.");

	for(py=20;py<=380;py+=3)
	{
		c=(py-20)*4*PI/360;
		b=PI*cos(c);
		for(a=0;a<=6*PI;a+=PI/7)
		{
			x=600/(6*PI)*a+20;
			y=10*sin(a+cos(a)*PI-b)*cos(c)+py;
			if(a==0)
				moveto(x,y);
			else
				lineto(x,y);
		}
	}
	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}