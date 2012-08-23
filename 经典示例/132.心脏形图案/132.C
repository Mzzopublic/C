/* Heart */
#include <graphics.h>
#include <math.h>
#define PI 3.1415926
void main()
{
	double a;
	int x,y,y1,r,r1;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	printf("Please input Radus(<80): ");
	scanf("%d",&r);
	cleardevice();
	setbkcolor(9);
	setcolor(4);
	outtextxy(80,20,"This program show the Kidney picture.");

	y1=240-r;
	for(a=0;a<=2*PI;a+=PI/27)
	{
		x=320+r*cos(a);
		y=240+r*sin(a);
		r1=sqrt((x-320)*(x-320)+(y-y1)*(y-y1));
		circle(x,y,r1);
	}
	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}