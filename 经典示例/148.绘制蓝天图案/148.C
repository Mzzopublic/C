#include <stdio.h>
#include <dos.h>
#include <math.h>

long Addr[768];
int Mode;

Set_Mode (int mode) /*设置显示的模式，参数为INT 10H的子功能号*/
{
    union REGS r;
    r.h.ah=0;
    r.h.al=mode;
    int86 (0x10,&r,&r);
}
Set_Graphics_Mode (unsigned x,unsigned y) /*设置图形模式的分辨率*/
{
    long i;
    if ((x<321)&&(y<201))  {
        Set_Mode (0x13);
        Mode=0x13;
        for (i=0;i<200;i++)  Addr[i]=320*i;
    }
    else if ((x<641)&&(y<401))  {
        Set_Mode (0x5c);
        Mode=0x5c;
        for (i=0;i<400;i++)  Addr[i]=640*i;
    }
    else if ((x<641)&&(y<481))  {
        Set_Mode (0x5d);
        Mode=0x5d;
        for (i=0;i<480;i++)  Addr[i]=640*i;
    }
    else if ((x<801)&&(y<601))  {
        Set_Mode (0x5e);
        Mode=0x5e;
        for (i=0;i<600;i++)  Addr[i]=800*i;
    }
    else if ((x<1025)&&(y<769))  {
        Set_Mode (0x62);
        Mode=0x62;
        for (i=0;i<768;i++)  Addr[i]=1024*i;
    }
    else  {
        Set_Mode (3);
        printf ("Not support this mode .\n");
        getch ();
    }
}
set_pattern () /*设置调色版*/
{
    int i;
    unsigned char pat[256][3];
    struct SREGS inreg;
    union REGS reg;
    pat[0][0]=0;pat[0][1]=0;pat[0][2]=0;
    for (i=1;i<=255;i++)  { /*设置调色版的颜色*/
        pat[i][0]=(unsigned char)((float)(abs(i-127)*63)/127.0+0.5);
        pat[i][1]=(unsigned char)((float)(abs(i-127)*63)/127.0+0.5);
        pat[i][2]=63;
    } 
    reg.x.ax=0x1012; /*设置VIDEO调色版的颜色块*/
    reg.x.bx=0;
    reg.x.cx=256;
    reg.x.dx=FP_OFF(pat);
    inreg.es=FP_SEG(pat);
    int86x (0x10,&reg,&reg,&inreg);
}
plot (int x,int y,unsigned char color) /*使用指定的颜色画点*/
{
    long offset;
    char Page;
    unsigned char far *address;
    switch (Mode)  {
        case 0x13:
            offset=Addr[y]+x;
            address=(unsigned char far *)(0xa0000000L+offset);
            *address=color;
            break;
        case 0x5c:
        case 0x5d:
        case 0x5e:
            offset=Addr[y]+x;
            Page=(offset>>16);
            outportb (0x3c4,0xe);
            outportb (0x3c5,Page^0x02);
            offset=offset&65535;
            address=(unsigned char far *)(0xa0000000L+offset);
            *address=color;
            break;
        case 0x62:
            offset=Addr[y]+x;
            Page=y>>6;
            outportb (0x3c4,0xe);
            outportb (0x3c5,Page^0x02);
            offset=offset&65535;
            address=(unsigned char far *)(0xa0000000L+offset);
            *address=color;
            break;
        default:
            break;
    }
}
get_pixel (int x,int y) /*获得点颜色*/
{
    long offset;
    char Page;
    unsigned char far *address;
    unsigned char color;
    switch (Mode)  {
        case 0x13:
            offset=Addr[y]+x;
            address=(unsigned char far *)(0xa0000000+offset);
            color=*address;
            break;
        case 0x5c:
        case 0x5d:
        case 0x5e:
            offset=Addr[y]+x;
            Page=(offset>>16);
            outportb (0x3c4,0xe);
            outportb (0x3c5,Page^0x02);
            offset=offset&65535;
            address=(unsigned char far *)(0xa0000000L+offset);
            color=*address;
            break;
        case 0x62:
            offset=Addr[y]+x;
            Page=y>>6;
            outportb (0x3c4,0xe);
            outportb (0x3c5,Page^0x02);
            offset=offset&65535;
            address=(unsigned char far *)(0xa0000000L+offset);
            color=*address;
            break;
        default:
            break;
    }
    return (color);
}
randint (unsigned int range) /*生成随机数*/
{
    float sigma=423.1966;
    static double OldRand=0.4231967;
    double temp;
    temp=sigma*OldRand;
    OldRand=temp-(int)temp;
    return (int)(OldRand*(float)range);
} 
void New_Col (int xa,int ya,int x,int y,int xb,int yb) /*生成新的颜色*/
{
    unsigned int color;
    color=abs(xa-xb)+abs(ya-yb);
    color=randint(color<<1)-color;
    color=color+(get_pixel(xa,ya)+get_pixel(xb,yb)+1)>>1;
    if (color<1)  color=1;
    else if (color>255) color=255;
    if ((get_pixel(x,y)==0)) plot (x,y,color);
}
void Sub_Divide (int x1,int y1,int x2,int y2) /*递归填充一块区域*/
{
    int x,y;
    unsigned char color;
    if (!((x2-x1<2)&&(y2-y1<2)))  {
        x=(x1+x2)>>1;
        y=(y1+y2)>>1;
        New_Col (x1,y1,x,y1,x2,y1);
        New_Col (x2,y1,x2,y,x2,y2);
        New_Col (x1,y2,x,y2,x2,y2);
        New_Col (x1,y1,x1,y,x1,y2);
        color=(get_pixel(x1,y1)+get_pixel(x2,y1)+get_pixel(x2,y2)+get_pixel (x1,y2)+2)>>2;
        plot (x,y,color);
        Sub_Divide (x1,y1,x,y);
        Sub_Divide (x,y1,x2,y);
        Sub_Divide (x,y,x2,y2);
        Sub_Divide (x1,y,x,y2);
    }
}
main ()
{
    int x,y;
    x=320;y=200;
    Set_Graphics_Mode (x,y);
    set_pattern ();
    plot (0,0,randint(254)+1);
    plot (x-1,0,randint(254)+1);
    plot (x-1,y-1,randint(254)+1);
    plot (0,y-1,randint(254)+1);
    Sub_Divide (0,0,x-1,y-1);
    getch();
    Set_Mode (0x03);
}
