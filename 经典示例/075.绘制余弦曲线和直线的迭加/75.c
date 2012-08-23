#include<stdio.h>
#include<math.h>
void main()
{
    double y;
    int x,m,n,yy;
    clrscr();
    puts("========This program shows the curve of cos(x) and a line.========");
    puts("        ******* f(x)=cos(x)    +++++++ g(x)=45*(y-1)+31");

    for(yy=0;yy<=20;yy++) /*对于第一个y坐标进行计算并在一行中打印图形*/
    {
        y=0.1*yy;                       /*y：屏幕行方向坐标*/
        m=acos(1-y)*10;       /*m: cos(x)曲线上y点对应的屏幕列坐标*/
        n=45*(y-1)+31;        /*n: 直线上y点对应的列坐标*/
        for(x=0;x<=62;x++)              /*x: 屏幕列方向坐标*/
            if(x==m&&x==n) printf("+");  /*直线与cos(x)相交时打印"+"*/
            else if(x==n) printf("+");   /*打印不相交时的直线图形*/
            else if(x==m||x==62-m) printf("*");  /*打印不相交时的cos(x)图形*/
            else  printf(" ");                  /*其它情况打印空格*/
        printf("\n");
    }
    puts(" Press any key to quit...");
    getch();
}
