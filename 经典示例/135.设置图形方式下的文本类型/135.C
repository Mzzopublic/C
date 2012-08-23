/* text */
#include <graphics.h>
#include <dos.h>
void main()
{
	int i,t,x=300,y=50;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	setbkcolor(9);
	setcolor(4);
	printf("Please input delay time (1-10): ");
	scanf("%d",&t);
	outtextxy(80,20,"This program show the Text in graphic mode.");

	for(i=1;i<=10;i++)
	{
		x=x-15;
		y=y+15;
		settextstyle(1,0,i);
		/*cleardevice();*/
		outtextxy(x,y,"Hello!");
		delay(1000*t);
	}
	settextstyle(1,0,1);
	outtextxy(80,420,"Press any key to quit...");
	getch();
	closegraph();
}