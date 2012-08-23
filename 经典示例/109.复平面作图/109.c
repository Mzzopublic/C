
#include <stdio.h>
#include <math.h>


void oplot(n,x,y) /*作图函数，这里使用DOS的文本模式画图*/
int n;
double x[],y[];
{
	int i, j;
	char screen[25][80]; /*声明一个字符型数组，用来表示屏幕的输出*/
	memset(screen, ' ', 25*80);  /*将数组整体赋值为空格*/
	/*画x轴*/
	for( i = 0; i <79; i++)
		screen[10][i] = '-';
	screen[10][79] = 'X';
	/*画y轴*/
	for( i = 1; i <25; i++)
		screen[i][40] = '|';
	screen[0][40] = 'Y';
	/*将符合条件的点(x,y)赋值成星号*/
	for( i = 0; i < n; i++)
		screen[(int)(x[i]+10)][(int)(y[i]*2+40)] = '*';
	/*输出数组，在屏幕上画图*/
	for( i = 0; i < 25; i++)
		for( j = 0; j <80; j++)
			printf("%c", screen[i][j] );
}

main()
{
    int points,k;
    double x[50], y[50], angle, portion;
    clrscr();

	points = 40; /*一共画40个点*/
	portion = 4.0 * M_PI / points; /*将720度分成40份。*/
	/*下边是求点的计算*/
	for (k=0; k<points; k++)
	{
		angle=k * portion; /*求出角度*/
		x[k]=2.0+angle*cos(angle); /*x，也就是复数的实部*/
		y[k]=angle*sin(angle); /*y，即复数的虚部*/
	}
    oplot(points,x,y); /*对所求出的点作图*/

    getch();
}