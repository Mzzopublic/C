/*
原理：

1. 使用叠代公式：
    z[0] = zInit;
    z[k] = z[k-1]*z[k-1] + z[0]
其中z[i]是复数，要使用复数的运算法则。

2. Mandelbort图形集的初始化要求
    -2.25<= Re(zInit) <= 0.75
    -1.25<= Im(zInit) <= 1.25
   其中Re(z)表示z的实部, Im(z)表示z的虚部
*/
#include <graphics.h>

typedef struct { float x, y; } complex; /*定义复数的结构，x表示实部，y表示虚部*/

complex complexSquare( complex c ) 
/*计算复数的平方
(x+yi)^2 = (x^2-y^2) + 2xyi
*/
{
	complex csq;
	csq.x = c.x * c.x - c.y * c.y;
	csq.y = 2 * c.x * c.y;
	return csq;
}

int iterate( complex zInit, int maxIter )
/*叠代计算颜色，maxIter是最多叠代的次数,*/
{
	complex z = zInit;
	int cnt = 0;
	
	/* 当 z*z > 4的时候退出 */
	while((z.x * z.x + z.y * z.y <= 4.0) && (cnt < maxIter))
	{
		/*叠代公式：z[k] = z[k-1]^2 + zInit, cnt是叠代次数*/
		z = complexSquare( z );
		z.x += zInit.x;
		z.y += zInit.y;
		cnt++;
	}
	return cnt;
}

void mandelbrot( int nx, int ny, int maxIter, float realMin, float realMax, float imagMin, float imagMax )
/*画Mandelbrot图形的主程序，参数意义如下：
nx: x轴的最大值
ny: y轴的最大值
maxIter: 叠代的最大次数
realMin: 初值zInit的实部最小值
realMax: 初值zInit的实部最大值
imagMin: 初值zInit的虚部最小值
imagMax: 初值zInit的虚部最大值
*/
{
	float realInc = (realMax - realMin) / nx; /*x轴叠代的步长*/
	float imagInc = (imagMax - imagMin) / ny; /*y轴叠代的步长*/
	complex z; /*初值zInit*/
	int x, y; /*点(x,y)的横纵坐标*/
	int cnt; /*叠代的次数*/
	
	for( x = 0, z.x = realMin; x<nx; x++, z.x += realInc )
	{
		for( y = 0, z.y = imagMin; y < ny; y++, z.y+= imagInc )
		{
			cnt = iterate( z, maxIter ); /*计算叠代次数*/
			if( cnt == maxIter ) /*当叠代最大时，为黑色*/
				putpixel( x, y, BLACK );
			else /*否则将叠代次数作为颜色*/
				putpixel( x, y, cnt  );
		}
	}
}

void main()
{
	int gdriver = 9, gmode=2;
	/*registerbgidriver( EGAVGA_driver );*/
	initgraph( &gdriver, &gmode, "e:\\tc\\bgi");
	mandelbrot( 640, 480, 255, -2.0, 0.55, -1.0, 1.25 );
	getch();
	closegraph();
}