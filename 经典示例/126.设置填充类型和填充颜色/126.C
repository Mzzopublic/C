/* set fill style */
#include <graphics.h>
void main()
{
	int i,c=4,x=5,y=6;
	int gdriver=DETECT,gmode;

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setcolor(c);
	for(i=c;i<c+8;i++)
	{
		setcolor(i);
		rectangle(x,y,x+140,y+104);
		x+=70;
		y+=52;
		setfillstyle(1,i);
		floodfill(x,y,i);
	}
	outtextxy(80,400,"Press any key to quit...");
	getch();
	closegraph();
}