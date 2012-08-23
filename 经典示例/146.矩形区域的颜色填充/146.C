#include <stdio.h>
#include <graphics.h>

void main()
{int gd=VGA,gm=VGALO;
 /*registerbgidriver(EGAVGA_driver);*/
 initgraph(&gd,&gm,"e:\\tc\\bgi"); /*设置图形模式*/
 setcolor(YELLOW);
 rectangle(105,105,175,135); /*画正方形*/
 full(120,120,YELLOW); /*调填充函数*/
 getch(); /*等待*/
 closegraph(); /*关闭图形模式*/
}

#define DELAY_TIME 5/*填充点后延长的时间，用来观看填充的过程，单位：毫秒*/

int full(int x,int y,int color1)/*递归的填充函数*/
{int color2,x1,y1;
 x1=x; y1=y;
 if(kbhit())return;
 color2=getpixel(x1,y1); /*读(x,y)点颜色值*/
 if(color2!=color1) /*判断是否与填充色相等*/
 {putpixel(x1,y1,color1); /*画点(x1,y1) */
  delay(DELAY_TIME);
  getch();
  x1++;
  full(x1,y1,color1); /*递归调用*/
 }
 x1=x; y1=y;
 color2=getpixel(x1-1,y1); /*读(x1-1,y1)点颜色值*/
 if(color2!=color1) /*判断是否与填充色相等*/
 {putpixel(x1,y1,color1); /*画点(x1,y1) */
  delay(DELAY_TIME);
  x1--;
  full(x1,y1,color1); /*递归调用*/
 }
 x1=x; y1=y;
 color2=getpixel(x1,y1+1); /*读(x1,y1+1)点颜色值*/
 if(color2!=color1) /*判断是否与填充色相等*/
 {putpixel(x1,y1,color1); /*画点(x1,y1) */
  delay(DELAY_TIME);
  y1++;
  full(x1,y1,color1); /*递归调用*/
 }
 x1=x; y1=y;
 color2=getpixel(x1,y1-1); /*读(x1,y1+1)点颜色值*/
 if(color2!=color1) /*判断是否与填充色相等*/
 {putpixel(x1,y1,color1); /*画点(x1,y1) */
  delay(DELAY_TIME);
  y1--;
  full(x1,y1,color1); /*递归调用*/
 }
 return;
}
