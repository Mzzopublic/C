
/* 一维布朗运动曲线 */

/* 本程序利用分形技术画一维的布朗运动曲线 */
/* BC 3.1编译                               */
/* 其中函数initgraph的第三个参数可能需要修改  */
#include "graphics.h"
#include "math.h"
#include "stdlib.h"
#include "conio.h"
#include "time.h"
#include "stdio.h"
#define  MAX_SIZE 1000
#define MAX_LEVEL 9 /* 递归深度*/
double Delta[MAX_LEVEL];
double Array[MAX_SIZE];
double H;
double Sigma=150.0;
double Gauss(void);
void CreateFractalImage(int y1,int y2);
void MiddlePoint(int p1,int p2,int CurrentLevel);
void DrawFractalImage(void);
/*=============================================================*/
main()
{ int GraphDriver=DETECT;
int GraphMode;
int k,mod;
double TempX,TempY,StartX,StartY;
initgraph(&GraphDriver,&GraphMode,"c:\\tc");
/*===============================================================*/
/*Draw fMB curve 递归画一条分形布朗运动曲线*/
/*==============================================================*/
randomize();
setcolor(GREEN);
H=0.3;
settextstyle(TRIPLEX_FONT,HORIZ_DIR,0);
setusercharsize(2,1,1,1);
outtextxy(450,400,"Wait...");
CreateFractalImage(150,150);
DrawFractalImage();
H=0.7;
CreateFractalImage(400,400);
DrawFractalImage();
getch();
closegraph();
return 0;
}
/*===============================================================*/
void CreateFractalImage(int y1,int y2)
{ int N,i;
N=(int)pow(2.0,(double)MAX_LEVEL);
for(i=0;i<MAX_LEVEL;i++)
Delta[i]=Sigma*pow(0.5,i*H)*sqrt(1.0-pow(2.0,2*H-2));
Array[0]=y1;
Array[N]=y2;
MiddlePoint(0,N,0);
}
/*===============================================================*/
/*Recursive procedure 二分法递归*/
/*===============================================================*/
void MiddlePoint(int p1,int p2,int CurrentLevel)
{ int middle;
middle=(p1+p2)/2;
if(CurrentLevel>MAX_LEVEL) return;
if((middle!=p1) && (middle!=p2))
{ Array[middle]=(Array[p1]+Array[p2])/2.0+Delta[CurrentLevel]*Gauss();
MiddlePoint(p1,middle,CurrentLevel+1);
MiddlePoint(middle,p2,CurrentLevel+1);
}
}
/*===============================================================*/
void DrawFractalImage(void)
{ int i,x,step,number;
number=(int) pow(2.0,(double)MAX_LEVEL);
step=getmaxx()/number*3/2;
moveto(0,(int)Array[0]);
for(i=1,x=step;i<number;x+=step,i++) lineto(x,(int)Array[i]);
}
/*==============================================================*/
double Gauss()
{ double g=0.0;
int RANGE=12000;
int COUNT=50;
int m;
for(m=1;m<=COUNT;m++) g+=(double)random(RANGE);
g=g/COUNT/(RANGE-1);
if(random(RANGE)%2) g=-g;
return g;
} 
