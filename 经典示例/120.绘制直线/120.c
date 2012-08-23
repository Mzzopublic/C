#include <graphics.h>
void main()
{
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	line(160,120,480,120);
	line(480,120,480,360);
	line(480,360,160,360);
	line(160,360,160,120);
	getch();
	closegraph();
}