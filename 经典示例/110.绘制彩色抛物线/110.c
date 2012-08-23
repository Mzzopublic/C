#include <graphics.h>
#include <math.h>

/*画抛物线的子函数spara()*/
/*row,col代表抛物线顶点的坐标，x1,y1是抛物线起点相对顶点的坐标*/
/*t为抛物线绕顶点旋转的角度*/
void spara(row,col,x1,y1,t,color)  
int row,col,x1,y1,t,color;
{   
  int n,dx,dy,x,y,x2,y2;
  double ct,st;
  double f,fx,fy,b,a,rx;
  st=(double)t*3.1415926/180.0;		/*把角度转化为弧度*/
  ct=cos(st); st=sin(st);
  n=abs(x1)+abs(y1); n=n+n;			
  dx=1; dy=1; f=0.0;				/*初始化工作*/
  if (x1>0) dx=-1;
  if (y1>0) dy=-1;
  a=y1; b=x1; b=b*b;
  rx=-a-a; fx=2.0*x1*dx+1.0;
  fx=-a*fx; fy=b;
  if (dy<0) fy=-fy;
  x=x1; y=y1;
  x2=(double)x*ct-(double)y*st+2000.5;
  y2=(double)x*st+(double)y*ct+2000.5;
  x2=x2-2000; y2=y2-2000;
  putpixel(row-y2,col+x2,color);
  while (n>0)						/*具体的运算法则见上面的公式*/
    { n=n-1;
	  if (f>=0.0)
	  { x=x+dx;
	    x2=(double)x*ct-(double)y*st+2000.5;
	    y2=(double)x*st+(double)y*ct+2000.5;
	    x2=x2-2000; y2=y2-2000;
	    putpixel(row-y2,col+x2,color);
	    if (fx>0.0) f=f-fx;
	    else f=f+fx;
	    fx=fx+rx;
	    if (fx==0.0||(fx<0.0&&fx-rx>0.0)||(fx>0.0&&fx-rx<0.0))
	      { dy=-dy; fy=-fy; f=-f;}
	  }
	else
	  { y=y+dy;
	    x2=(double)x*ct-(double)y*st+2000.5;
	    y2=(double)x*st+(double)y*ct+2000.5;
	    x2=x2-2000; y2=y2-2000;
	    putpixel(row-y2,col+x2,color);
	    if (fy>0.0) f=f+fy;
	    else f=f-fy;
	  }
    }
  return;
}

void main()
{
  int i,color;
  int gdriver = DETECT , gmode;
  color = 1;
  registerbgidriver(EGAVGA_driver);
  initgraph(&gdriver,&gmode,"..\\bgi");			/*初始化图形界面*/
  for (i=1;i<=4;i++)							/*先画出四个互成90度的抛物线*/
  {
    spara(200,200,100,100,i*90,color);
    color+=3;
    getch();
  }
  color = 1;
  for (i=1;i<=11;i++)							/*再画12个互成30度的抛物线*/
  {	
    spara(200,200,100,100,i*30,color);
    color++;
  }
  getch();
  closegraph();
  return;
}