/* circles */
#include <graphics.h>
#include <math.h>
#define PI 3.1415926
void main()
{
	double a;
	int x,y,r1,rs;
	int gdriver=DETECT,gmode;
	printf("Please input R1(<100) and Rs:\n");
	scanf("%d%d",&r1,&rs);

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setcolor(4);
	outtextxy(80,20,"This program show the Circles picture.");

	for(a=0;a<=2*PI;a+=PI/18)
	{
		x=320+r1*cos(a);
		y=240+r1*sin(a);
		circle(x,y,rs);
	}
	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}