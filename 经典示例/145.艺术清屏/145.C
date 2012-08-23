#include<stdio.h>
#include<dos.h>
#include<conio.h>

void goto_xy(int x,int y);
void dcls(int x1,int x2,int y1,int y2);
void bcls(int x1,int x2,int y1,int y2);
void recls(int x1,int x2,int y1,int y2);
void zcls(int x1,int x2,int y1,int y2);
void puta(void);

/*--------------演示程序---------------------*/
main()
{
    puta();
    getch();
    dcls(0,4,0,79);
    getch();
    puta();
    getch();
    bcls(0,25,0,79);
    getch();
    puta();
    getch();
    zcls(0,25,0,79);
    getch();
}
/*********center clear screen（中心清屏）***********/
void zcls(int x1,int x2,int y1,int y2)
{
    int x00,y00,x0,y0,i,d;
    if((y2-y1)>(x2-x1)){
	d=(x2-x1)/2;
	x0=(x1+x2)/2;
	y0=y1+d;
	y00=y2-d;
	for(i=0;i<(d+1);i++)
	    recls((x0-i),(x00+i),(y0-i),(y00+i));
    }
    else{
	d=(y2-y1)/2;
	y0=(y1+y2)/2;
	x0=x1+d;
	x00=x2-d;
	for(i=0;i<d+1;i++)
	    recls(x0-i,x00+i,y0-i,y00+i);
    }
}

/************* clear rectangle side（矩形边清屏）***********************/

void recls(int x1,int x2,int y1,int y2)
{
    int i,j;
    for(i=y1;i<y2;i++){
	goto_xy(x1,i);
	putchar(' ');
	goto_xy(x2,i);
	putchar(' ');
    }
    for(j=x1;j<x2;j++){
	goto_xy(i,y1);
	putchar(' ');
	goto_xy(j,y2);
	putchar(' ');
    }
}

/*****************close screen clear*****闭幕式清屏*******************/

void bcls(int x1,int x2,int y1,int y2)
{
    int t,s,j;
    t=y1;
    s=y2;
    for(t=y1;t<(y1+y2)/2;t++,s--)
    for(j=x1;j<x2;j++){
	goto_xy(j,t);
	putchar(' ');
	goto_xy(j,s);
	putchar(' ');
	delay(1);
    }
}
/******************bottom screen clear（自下清屏）********************/

void dcls(int x1,int x2,int y1,int y2)
{
    int t,s,j,i;
    t=s=(y1+y2)/2;
    for(j=x2;j>x1;j--)
    for(i=y1;i<y2;i++){
	goto_xy(j,i);
	putchar(' ');
	delay(1);
    }
}
/******************设置光标子函数******************/

void goto_xy(int x,int y)
{
    union REGS r;
    r.h.ah=2;
    r.h.dl=y;
    r.h.dh=x;
    r.h.bh=0;
    int86(0x10,&r,&r);
}

/**********************在屏幕上打出一连串的@字母用于演示程序******************/

void puta(void)
{
    int i,j;
    for(i=0;i<24;i++){
	for(j=0;j<79;j++){
	    goto_xy(i,j);
	    printf("@");
	}
    }
}

