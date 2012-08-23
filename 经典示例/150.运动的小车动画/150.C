#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<math.h>

#define step 10
#define R 10

main()
{
	int gdriver=9,gmode=2;
	static int startx=5;
	static int starty=100;
	int maxx,l=1,n=1;
	double dalta=20,angle;
	int size;
	void *image;
	/*registerbgidriver( EGAVGA_driver );*/ /*注册vga驱动*/
	initgraph(&gdriver,&gmode,"c:\\tc"); /*初始化图形界面*/
	cleardevice(); /*清除屏幕*/
	setbkcolor(BLUE); /*将背景色设置成蓝色*/
	size=imagesize(startx,starty,startx+60,starty+60); /*计算生成60×60个象素的图需要的字节数*/
	image=(unsigned char *)malloc(size); /*分配内存*/
	maxx=getmaxx(); /*获得屏幕显示x轴的最大值*/
	while(!kbhit()) /*如果没有按键就不停的循环*/
	{
		if(l==1) /*从左到右*/
		{
			n++;
			angle=-1*(n*step)/M_PI*180/R;
			if((int)(-1*angle)%360<dalta)
				angle-=dalta;
			if(n>(maxx-70)/step)
				l=0;
		}
		if(l==0) /*从右到左*/
		{
			--n;
			angle=-1*(n*step)/R/M_PI*180;
			if((int)(-1*angle)%360<dalta)
				angle-=dalta;
			if(n==1)l=1;
		}
		rectangle(startx+n*step,starty,startx+n*step+60,starty+40); /*画车厢*/
		pieslice(startx+n*step+15,starty+50,angle,angle-dalta,10); /*画轮上的小片扇形部分*/
		pieslice(startx+n*step+45,starty+50,angle,angle-dalta,10);
		setcolor(YELLOW); /*设置前景色为黄色*/
		setfillstyle(SOLID_FILL, YELLOW); /*设置填充模式为实填充，颜色为黄色*/
		circle(startx+n*step+15,starty+50,10); /*画车轮*/
		circle(startx+n*step+45,starty+50,10);
		circle(startx+n*step+15,starty+50,3);
		circle(startx+n*step+45,starty+50,3);
		getimage(startx+n*step,starty,startx+n*step+60,starty+60,image); /*获取当前的图片*/
		delay(100); 
		putimage(startx+n*step,starty,image,XOR_PUT); /*使用异或模式将图片显示上去*/
	}
	free(image);
	closegraph();
}
