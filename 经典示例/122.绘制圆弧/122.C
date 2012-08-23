#include <graphics.h>
void main()
{
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	moveto(160,120);
	linerel(320,0);
	linerel(0,240);
	linerel(-320,0);
	linerel(0,-240);
	getch();
	closegraph();
}