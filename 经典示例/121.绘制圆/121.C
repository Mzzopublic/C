#include <graphics.h>
void main()
{
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	moveto(160,120);
	lineto(480,120);
	lineto(480,360);
	lineto(160,360);
	lineto(160,120);
	getch();
	closegraph();
}