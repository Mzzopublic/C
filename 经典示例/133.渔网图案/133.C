/* Fishing Net */
#include <graphics.h>
void main()
{
	int x,y,x1,y1,x0=320,y0=50;
	int i,j,n=5,r=20;
	int gdriver=VGA,gmode=VGAHI;
	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(9);
	outtextxy(80,20,"This program show the Fishing Net picture.");

	for(i=0;i<=2*n;i++)
	{
		x1=x0-i*r;
		y1=y0+i*r;
		for(j=0;j<=n-1;j++)
		{
			x=x1+2*j*r;
			y=y1+2*j*r;
			arc(x,y,180,270,r);
			arc(x,y+2*r,0,90,r);
		}
	}
	x1=x0-2*r;
	y1=y0;
	for(i=0;i<=2*n;i++)
	{
		x1=x1+r;
		y1=y1+r;
		for(j=0;j<=n-1;j++)
		{
			x=x1-2*j*r;
			y=y1+2*j*r;
			arc(x,y,90,180,r);
			arc(x-2*r,y,270,360,r);
		}
	}

	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}