/* set color */
#include <graphics.h>
void main()
{
	int cb,cf;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	for(cb=0;cb<=15;cb++)
	{
		setbkcolor(cb);
		for(cf=0;cf<=15;cf++)
		{
			setcolor(cf);
			circle(100+cf*25,240,100);

		}
		getch();
	}
	getch();
	closegraph();
}