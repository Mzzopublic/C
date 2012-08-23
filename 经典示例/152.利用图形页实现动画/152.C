#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <math.h>
void main()
{
	void * ptr;
	int gd=9,gm=2;
	float i;
	/*registerbgidriver(EGAVGA_driver);*/
        initgraph(&gd,&gm,"C:\\TC");/*图形初始化*/
	setactivepage(1);/*设置页面1为当前活动页*/
	setcolor(RED);/*设置当前画笔颜色为红色*/
	setfillstyle(1,14);/*设置填充模式为实填充，颜色为黄色*/
	fillellipse(10,10,10,10);/*画一个实心的椭圆*/
	ptr=malloc(imagesize(0,0,20,20));/*动态分配一个动态空间，其中可以存储一个20×20大小的图片*/
	getimage(0,0,20,20,ptr);/*将当前屏幕上的图片写入内存*/
	randomize();/*随机数初始化*/
	setactivepage(0);/*设置页面1为当前活动页*/
	for(i=0;i<3000;i++)
		putpixel(random(640),random(480),random(15));/*在随机的位置以随机的颜色填充点*/
	for(i=0;i<6.28*4;i=i+0.005)
	{
		/*以下两句是将内存中的图形以特定图形块显示在屏幕上*/
		putimage(300+8*(cos(i)+i*sin(i)),240+6*(sin(i)-i*cos(i)),ptr,COPY_PUT);
		putimage(300+8*(cos(i)+(i)*sin(i)),240+6*(sin(i)-(i)*cos(i)),ptr,COPY_PUT);
		if(kbhit()) break;/*获得一个键盘信号停止*/
		delay(10);
	}
	getch();
	closegraph();
}
