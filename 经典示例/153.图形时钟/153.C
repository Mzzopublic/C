#include<math.h>
#include<dos.h>
#include<graphics.h>
#define CENTERX 320   /*表盘中心位置*/
#define CENTERY 175
#define CLICK 100  /*喀嗒声频率*/
#define CLICKDELAY 30 /*喀嗒声延时*/
#define HEBEEP 10000 /*高声频率*/
#define LOWBEEP 500 /*低声频率*/
#define BEEPDELAY 200 /*报时声延时*/

/*表盘刻度形状*/
int Mrk_1[8]={-5,-160,5,-160,5,-130,-5,-130, };
int Mrk_2[8]={-5,-160,5,-160,2,-130,-2-130, };

/*时针形状*/
int HourHand[8]={-3,-100,3,-120,4, 10,-4,10};

/*分针形状*/
int MiHand[8]={-3,-120,3,-120,4, 10,-4,10};

/*秒针形状*/
int SecHand[8]={-2,-150,2,-150,3, 10,-3,10};

/*发出喀嗒声*/
void Click()
{
	sound(CLICK);
	delay(CLICKDELAY);
	nosound();
}

/*高声报时*/
void  HighBeep()
{
	sound(HEBEEP);
	delay(BEEPDELAY);
	nosound;
}

/*低声报时*/
void LowBeep()
{
	sound(LOWBEEP);
}

/*按任意角度画多边形*/
void DrawPoly(int *data,int angle,int color)
{
	int usedata[8];
	float sinang,cosang;
	int i;
	sinang=sin((float)angle/180*3.14);
	cosang=cos((float)angle/180*3.14);
	for(i=0;i<8;i+=2)
	{
		usedata[i]  =CENTERX+ cosang*data[i]-sinang*data[i+1]+.5;
		usedata[i+1]=CENTERY+sinang*data[i]+cosang*data[i+1]+.5;
	}
	setfillstyle(SOLID_FILL,color);
	fillpoly(4,usedata);
}

/*画表盘*/
void DrawClock(struct time *cutime)
{
	int ang;
	float hourrate,minrate,secrate;
	
	setbkcolor(BLACK);
	cleardevice();
	setcolor(WHITE);
	
	/* 画刻度*/
	for(ang=0;ang<360;ang+=90)
	{
		DrawPoly(Mrk_1,ang,WHITE);
		DrawPoly(Mrk_2,ang+30,WHITE);
		DrawPoly(Mrk_2,ang+60,WHITE);
	}
	secrate=(float)cutime->ti_sec/60;
	minrate=((float)cutime->ti_min+secrate)/60;
	hourrate=(((float)cutime->ti_hour/12)+minrate)/12;
	ang=hourrate*360;
	DrawPoly(HourHand,ang,YELLOW);/*画时针*/
	ang=minrate*360;
	DrawPoly(MiHand,ang, GREEN);/*画分针*/
	ang=secrate*360;
	DrawPoly(SecHand,ang, RED);/*画秒针*/
}
main()
{
	int gdriver=EGA,
		gmode=EGAHI;
	int curpage;
	struct time curtime ,newtime ;
	initgraph(&gdriver,&gmode,"c:\\tc");
	setbkcolor(BLUE);
	cleardevice();
	gettime(&curtime);
	curpage=0;
	DrawClock(&curtime);
	while(1)
	{
		if(kbhit())
			break; /*按任意键退出*/
		gettime(&newtime); /*检测系统时间*/
		if(newtime.ti_sec!=curtime.ti_sec)/*每1秒更新一次时间*/
		{
			if(curpage==0)
				curpage=1;
			else
				curpage=0;
			curtime=newtime;
			/*设置绘图页*/
			setactivepage(curpage);
			/*在图页上画表盘*/
			DrawClock(&curtime);
			/*设置绘图页为当前可见页*/
			setvisualpage(curpage);
			/*0分0秒高声报时*/
			if(newtime.ti_min==0&&newtime.ti_sec==0)
				HighBeep();
			/* 59分55至秒时低声报时*/
			else if(newtime.ti_min==59&&
				newtime.ti_sec<=59)
				LowBeep();/*其他时间只发出喀嗒声*/
			else
				Click();
		}
	}
	closegraph();
}