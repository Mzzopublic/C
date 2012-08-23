#include <stdio.h>
#include <graphics.h>
#include <dos.h>
/*这是根据给出的圆心坐标和点坐标分别在八个象限画点的子程序*/
void circlePoint( int xCenter, int yCenter, int x, int y )
{
	putpixel( xCenter + x, yCenter + y, YELLOW );
	putpixel( xCenter - x, yCenter + y, YELLOW );
	putpixel( xCenter + x, yCenter - y, YELLOW );
	putpixel( xCenter - x, yCenter - y, YELLOW );
	putpixel( xCenter + y, yCenter + x, YELLOW );
	putpixel( xCenter - y, yCenter + x, YELLOW );
	putpixel( xCenter + y, yCenter - x, YELLOW );
	putpixel( xCenter - y, yCenter - x, YELLOW );
}

void myCircle(int xCenter,int yCenter,int radius)
{
	int x, y, p;
/*初始化各个参数*/
	x = 0;
	y = radius;
	p = 1 - radius;
	circlePoint(xCenter, yCenter, x, y);
/*循环中计算圆上的各点坐标*/
	while( x < y ) {
		x++;
		if( p < 0 )
			p += 2*x+1;
		else
		{
			y--;
			p+=2*(x-y)+1;
		}
		circlePoint( xCenter, yCenter, x, y);
	}
}

void main()
{
	int gdriver=DETECT, gmode;  /*这是用c画图时必须要使用的图像入口*/
	int i;
	int xCenter, yCenter, radius;
	printf("Please input center coordinate :(x,y) ");
	scanf("%d,%d", &xCenter, &yCenter );
	printf("Please input radius : ");
	scanf("%d", &radius );
/*这条语句初始化整个屏幕并把入口传给gdriver,注意引号中是tc中bgi目录的完整路径*/
	registerbgidriver(EGAVGA_driver);
	initgraph(&gdriver, &gmode, "..\\bgi");  
	setcolor( BLUE );
	myCircle(xCenter, yCenter, radius);
	sleep(3);
	closegraph();
	return;
}
