/* set line style */
#include <graphics.h>
void main()
{
	int i,j,c,x=50,y=50,k=1;
	int gdriver=DETECT,gmode;

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(9);
	setcolor(4);
	for(j=1;j<=2;j++)
	{
		for(i=0;i<4;i++)
		{
			setlinestyle(i,0,k);
			line(50,50+i*50+(j-1)*200,200,200+i*50+(j-1)*200);
			rectangle(x,y,x+210,y+80);
			circle(100+i*50+(j-1)*200,240,100);
		}
		k=3;
		x=50;
		y=250;
	}
	getch();
	closegraph();
}