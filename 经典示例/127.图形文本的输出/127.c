/* line */
#include <graphics.h>
void main()
{
	int x;
	int gdriver=DETECT,gmode;

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setcolor(4);
	for(x=160;x<=480;x+=20)
	{
		line(160,240,x,120);
		line(160,240,x,360);
		line(480,240,640-x,360);
		line(480,240,640-x,120);
	}
	outtextxy(80,400,"Press any key to quit...");
	getch();
	closegraph();
}