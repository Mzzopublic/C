#include<stdio.h>
#include<dos.h>
#include<graphics.h>
#include<fcntl.h>
#include<time.h>

#define N1 64
#define N2 32
#define N4 16
#define N8 8
#define N16 4
#define END 0

void interrupt(* handler)( );
void interrupt music( );
int handle,control;

enum NOTES
{
	C10=131,D10=147,E10=165,F10=175,G10=196,A10=220,B10=247,
	C0=262, D0=296, E0=330, F0=349, G0=392, A0=440, B0=494,
	C1=523, D1=587, E1=659, F1=698, G1=784, A1=880, B1=988,
	C2=1047, D2=1175, E2=1319, F2=1397, G2=1568, A2=1760, B2=1796
}song[]={
	E1,N4,E1,N8,E1,N8,F1,N4,G1,N4,F1,N4,F1,N4,E1,N4,D1,
  N4,C1,N4,C1,N4,D1,N4,E1,N4,E1,N4,D1,N4,D1,N4,E1,N4,
   E1,N8,E1,N8,F1,N4,G1,N4,G1,N4,F1,N4,E1,N4,D1,N4,C1,
   N4,C1,N4,D1,N4,E1,N4,D1,N4,D1,N4,C1,N4,D1,N4,D1,N8,
   D1,N8,E1,N4,C1,N4,D1,N4,E1,N8,F1,N8,E1,N4,C1,N4,D1,
   N4,E1,N8,F1,N8,E1,N4,C1,N4,C1,N4,D1,N4,G0,N4,E1,N4,
   E1,N4,E1,N8,F1,N4,G1,N4,G1,N4,F1,N4,E1,N4,D1,N4,C1,
   N4,C1,N4,D1,N4,E1,N4,E1,N4,D1,N4,C1,N4,D1,N4,
   END,END
   };

main()
{
	int gdriver=VGA,gmode=1,i;
	/*registerbgidriver( EGAVGA_driver );*/
	initgraph (&gdriver,&gmode,"c:\\tc");
	handler=getvect(0x1c); /*获取0x1c中断向量*/
	setvect(0x1c,music); /*将music函数写入到0x1c中断向量中去*/
	cleardevice( ); /*清除屏幕*/
	setbkcolor(BLACK); /*将背景色设置成黑色*/
	setcolor(YELLOW); /*前景色设置为黄色*/
	setfillstyle(SOLID_FILL,RED); /*对封闭区域使用填充，填充色为红色*/
	sector(200,150,50,120,70,50); /*画一个扇形*/
	bar3d(400,150,500,200,10,5); /*画一个3维立方体*/
	setactivepage(1); /*将活动页切换到第一个页面*/
	sector(200,150,50,170,70,50);
	bar3d(400,200,500,250,10,5);
	for(i=0;i<100;i++)
	{
		while(kbhit()) goto end; /*当用户按下一个键时，退出*/
		setvisualpage(1); /*显示第一个页面*/
		delay((1000-i)*10);
		setvisualpage(0); /*显示第0个页面*/
		delay((1000-i)*10);
	}
end:;
	outportb(0x61,control&0xfe); /*关闭PC扬声器*/
	setvect(0x1c,handler); /*将0x1c中断向量置成系统原有的处理例程*/
	cleardevice();
	closegraph();
}

void interrupt music( )
{
	static int flag=0,note=0,fre,dur=8;
	flag++;
	fre=song[note];
	dur=song[note+1];
	if(/*flag>(int)dur*2/5*/fre)
	{
		flag=0;
		outportb(0x43,0xb6); /*打开计数器*/
		fre=(unsigned)(1193180L/fre); /*计算频率*/
		outportb(0x42,(char)fre); /*将频率写入计时器*/
		outportb(0x42,(char)(fre>>8));
		control=inportb(0x61); /*从扬声器端口读出控制信息*/
		outportb(0x61,(control)|0x3); /*写入扬声器，使之发声。*/
		note=note+2;
		if(note>=134)note=0;
	}
	handler();
}
