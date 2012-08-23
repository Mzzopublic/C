/*
     VGA256.c -- VGA 256 色编程
*/
#include "dos.h"
#include "conio.h"
#include "stdio.h"

void InitScr();
void RstScr();
void PutPoint(int x, int y, int Color);
void Rect(int x1, int y1, int x2, int y2, int Color);
void LineV(int x1, int y1, int x2, int y2, int Color);

int main()
{
    int x1, y1, x2, y2, i, j;
    x1 = y1 = 0;
    x2 = 319;
    y2 = 199;
    InitScr();
    for (i = 0; i < 256; i++)
       LineV(i, 0, i, 199, i);
    for( i = 18; i < 100; i++)
       Rect(x1++, y1++, x2--, y2--, i);
    for( i = 18; i < 50; i++)
       Rect(x1--, y1--, x2++, y2++, i);
    getch();
    RstScr();
}

void InitScr()
{
    union REGS In;
    In.x.ax = 0x13;                 /*进入13H模式  */
    int86(0x10, &In, &In);
}

void RstScr()
{
    union REGS In;
    In.x.ax = 0x03;             /* 退出13H模式 */
    int86(0x10, &In, &In);
}

/* 直接写视频缓冲区 */
void PutPoint(int x, int y, int Color)   /* 画点函数 */
{
   char far *p;
   p = (char far *) (0x0a0000000L);
   * (x+y*320+p) = Color;
}

/* 利用VGA BIOS中断在屏幕上画点, 速度慢
void PutPoint(int x, int y, int Color)
{
   union REGS  In;
   In.h.ah = 0x0C;
   In.h.al = Color;
   In.x.cx = x;
   In.x.dx = y;
   In.h.bh = 0;
   int86(0x10, &In, &In);
}
*/

void LineV(int x1, int y1, int x2, int y2, int Color)  /* 画一垂直线 */
{
   int i;
   for (i = 0; i < 199; i++)
      PutPoint(x1, i, Color);
}

void Rect(int x1, int y1, int x2, int y2, int Color)  /* 画一矩形*/
{
   int i;
   for(i = x1; i <= x2; i++)
   {
      PutPoint(i, y1, Color);
      PutPoint(i, y2, Color);
   }
   for(i = y1; i <= y2; i++)
   {
      PutPoint(x1, i, Color);
      PutPoint(x2, i, Color);
   }
}
