/* diamond */
#include <graphics.h>
#include <math.h>
void main()
{
	float t;
	int x0=320,y0=240;
	int n,i,j,r;
	int x[50],y[50];
	int gdriver=DETECT,gmode;
	printf("Please input n(23<=n<=31) and r(100<=r<=200):\n");
	scanf("%d%d",&n,&r);

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setcolor(4);
	outtextxy(80,20,"This program show the Diamond picture.");
	t=6.28318/n;

	for(i=0;i<n;i++)
	{
		x[i]=x0+r*cos(i*t);
		y[i]=y0+r*sin(i*t);
	}
	for(i=0;i<=n-2;i++)
	{
		for(j=i+1;j<=n-1;j++)
			line(x[i],y[i],x[j],y[j]);
	}
	outtextxy(80,460,"Press any key to quit...");
	getch();
	closegraph();
}