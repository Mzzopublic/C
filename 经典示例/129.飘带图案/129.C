/* linesin */
#include <graphics.h>
#include <math.h>
#define PI 3.1415926
void main()
{
	double a;
	int x1,y1,x2,px=320,py=240;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setcolor(4);
	outtextxy(80,20,"This program show the Linesin picture.");

	for(a=0;a<=PI;a+=PI/380)
	{
		x1=px+280*cos(1.6*a);
		y1=479-((90*sin(8*a))*cos(a/2.5)+py);
		x2=py+280*cos(1.8*a);
		line(x1,y1,x2,y1);
	}
	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}