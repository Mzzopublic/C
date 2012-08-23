/*
模拟扫雷游戏  
*/
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>

union REGS regs;
int size=15;/*用于表示每个方块的大小(正方形的边长)*/
int pix,piy=50;/*pix,piy是矩阵的偏移量*/
char b[2]="1";/*用于显示方格周围的雷的个数*/
int pan[30][16];/*用于记录盘面的情况：0：没有、9：有雷、1~8：周围雷的个数*/
int pan1[30][16];/*pan1[][]纪录当前的挖雷情况，0：没有操作、1：打开了、2：标记了*/
int tt;/*纪录时间参数*/
int Eflags;/*用于标记鼠标按钮的有效性，0：有效，1：无效，2：这是鼠标的任意键等于重新开始*/

int Msinit();
void Draw(int x,int y,int sizex,int sizey);
void Facedraw(int x,int y,int sizel,int k);
void Dead(int sizel,int x,int y);
void Setmouse(int xmax,int ymax,int x,int y);
int Msread(int *xp,int *yp,int *bup,struct time t1,int k);
void Draw1(int x,int y);
int Open(int x,int y);
float Random();
void Have(int sum,int x,int y,int xx,int yy);
void Help();
void Coread();
void Ddraw2(int x,int y);


/*下面是主函数*/
main()
{
 int mode=VGAHI,devices=VGA;/*图形模式初始化的变量*/
 char ams;                  /*鼠标操作中的标志变量*/
 int xms,yms,bms;           /*鼠标的状态变量*/
 int i,j,k,k1=0;            /*i,j,k是循环变量*/
 int x=9,y=9,flags=0;       /*x,y矩阵的大小*/
 int sum=10;                /*sum 盘面的雷的总数目，是个x,y的函数*/
 int x1=0,y1=0;             /*用于记录光标当前的位置*/
 int x11=0,y11=0;           /*暂时保存鼠标位置的值*/
 int sizel=10;              /*脸的大小*/
 int cflags=1;      /*这是菜单操作标志变量,没有弹出1,弹出0*/
 struct time t1={0,0,0,0};  /*时间结构体，头文件已定义*/
 int co[3];      /*暂时纪录历史纪录*/
 void far *Map;      /*用于保存鼠标图片*/
 char name[3][20];     /*名字字符串，用于记录名字*/
 FILE * p;      /*文件指针用于文件操作*/

 Msinit();                              /*鼠标初始化*/
 /*registerbgidriver(EGAVGA_driver);*/
 initgraph(&devices,&mode,"C:\\tc");      /*图形模式初始化*/

/*为图片指针分配内存*/
if((Map=farmalloc(imagesize(0,0,20,20)))==NULL)/*图片的大小是20*20*/
 {
  printf("Memory ererr!\n");
  printf("Press any key to out!\n");
  exit(1);
 }

 /*用于检验文件是否完整*/
  while((p = fopen("score.dat", "r")) == NULL) /*如果不能打开就新建一个*/
   {
  if((p = fopen("score.dat", "w")) == NULL)/*如果不能新建就提示错误并推出*/
  {
   printf("The file cannot open!\n");
   printf("Presss any key to exit!\n");
   getch();
        exit(1);
  }
  /*写入初始内容*/
  fprintf(p,"%d %d %d,%s\n%s\n%s\n",999,999,999,"xiajia","xiajia","xiajia");
  fclose(p); 
 }
  /*暂时读出历史纪录。并用co[3]纪录*/
  fscanf(p,"%d %d %d,",&co[0],&co[1],&co[2]);
  for(i=0;i<3;i++)
  fscanf(p,"%s\n",name[i]);
  fclose(p);

 Setmouse(1,1,615,460);  /*设置鼠标的活动范围*/
 pix=250-x*size/2;  /*计算地雷按钮矩阵的左边*/

bss:    b[0]='1';        /*游戏开始和重来的切入点*/
       flags=0;  /*测试中这个变量好像没有作用，但是为了逻辑的完整性暂时保留*/
       k=0;
 Eflags=0;
 tt=0;
  cleardevice();/*清屏*/
  setbkcolor(LIGHTCYAN);
       for(i=0;i<30;i++)       /*盘面初始化*/
       for(j=0;j<16;j++)
       {
  pan[i][j]=0;
         pan1[i][j]=0;
 }

 for(;;) /*用于生成雷的盘面情况*/
    {
  i=(int)(x*Random());
      j=(int)(y*Random());
      if(pan[i][j]==0)
         {
   pan[i][j]=9;
   k=k+1;
         }
      if(k==sum) break;
    } 

for(i=0;i<x;i++)/*用于统计各点的周边情况*/
for(j=0;j<y;j++)
  {
  if(pan[i][j]==0)/*只有没有雷的才需要处理*/
    {
      if(i-1>=0 && j-1>=0 && pan[i-1][j-1]==9) pan[i][j]++;
   if(j-1>=0 && pan[i][j-1]==9)     pan[i][j]++;
   if(i+1<x && j-1>=0 && pan[i+1][j-1]==9)  pan[i][j]++;
   if(i-1>=0 && pan[i-1][j]==9)     pan[i][j]++;
   if(i+1<x && pan[i+1][j]==9)     pan[i][j]++;
   if(i-1>=0 && j+1<y && pan[i-1][j+1]==9)  pan[i][j]++;
   if(j+1<y && pan[i][j+1]==9)     pan[i][j]++;
   if(i+1<x && j+1<y && pan[i+1][j+1]==9)   pan[i][j]++;
  }
 }

 if(sum==10)/*画底座*/
 {
  Draw(170,1,162,200);
  setcolor(1);
  outtextxy(175,5,"Game( )");
  setcolor(4);
  outtextxy(175,5,"     G");
 }/*小*/
 else
  if(sum==40)
  {
   Draw(115,1,272,307);
   setcolor(1);
   outtextxy(120,5,"Game(G)");
   setcolor(4);
   outtextxy(120,5,"     G");
  }/*中*/
     else
      if(sum==99)
    {
     Draw(40,1,470,307);
     setcolor(1);
     outtextxy(45,5,"Game(G)");
     setcolor(4);
     outtextxy(45,5,"     G");
    }/*大*/

 setcolor(1);/*显示提示信息*/
 outtextxy(100,350,"  : Help.");
 setcolor(4);
 outtextxy(100,350,"H");
 setcolor(1);
 outtextxy(200,350," /    : exit.");
 setcolor(4);
 outtextxy(200,350,"Q");
 outtextxy(220,350,"ESC");
 setcolor(1);
 outtextxy(330,350,"  : cores.");
 setcolor(4);
 outtextxy(330,350,"R");

 Facedraw(250,30,sizel,2);/*显示控制按钮，脸型*/

 for(i=0;i<x;i++)/*这一段用于描绘盘面*/
 for(j=0;j<y;j++)
 {
  Draw(i*size+pix,j*size+piy,size,size);
 }

 Have(sum,pix+20,30,x,y);/*各项的初始显示*/
 k=0;
 setfillstyle(1, 0);
 bar(460-pix,25,490-pix,40);
 setcolor(4);
 outtextxy(465-pix,30,"000");

/*下面这一段用于接收用户的控制，并做出反应*/
 for(;;)
 {
  ams=Msread(&xms,&yms,&bms,t1,k);/*读取鼠标状态*/

   /*下面一段用于保存并记录鼠标图片*/
  if(flags!=0)
  {
   putimage(x11,y11,Map,COPY_PUT);
  }
  x11=xms;
  y11=yms;
  flags=1;

 /*这里插入菜单操作*/
  if((xms>=pix-10 && xms<=pix+26 && yms>=2 && yms<=14 && bms==1) || ams=='g' || ams=='G')/*点中菜单的判断*/
  {
   Draw(pix-10,14,100,88); /*画菜单*/
   cflags=0;
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,19,"Restar(R)");
   setcolor(4);
   outtextxy(pix-4,19,"       R");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,30,"Easy(E)");
   setcolor(4);
   outtextxy(pix-4,30,"     E");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,41,"Nomall(N)");
   setcolor(4);
   outtextxy(pix-4,41,"       N");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,52,"Hard(H)");
   setcolor(4);
   outtextxy(pix-4,52,"     H");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,63,"Corse(C)");
   setcolor(4);
   outtextxy(pix-4,63,"      C");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,74,"Help(L)");
   setcolor(4);
   outtextxy(pix-4,74,"     L");
   setcolor(1);/*下面显示菜单项*/
   outtextxy(pix-4,85,"Quit(Q)");
   setcolor(4);
   outtextxy(pix-4,85,"     Q");
   bms=0;
  }/*完成菜单的描绘*/

 /*这里是为了完全实现鼠标控制才加入的内容*/
 if(Eflags==2 && (bms!=0 || ams!=-1))
  {
      goto bss;
   }

 /*下面实现菜单的功能，每一项响应一个菜单项*/
 if(cflags==0)
  {
   if((xms>=pix-6 && xms<=pix+85 && yms>=17 && yms<=28 && bms==1)|| (ams=='r' || ams=='R'))/*下面依次响应菜单中各项*/
   {
    cflags=1;
    goto bss;
   }
   else
    if((xms>=pix-6 && xms<=pix+85 && yms>=28 && yms<=39 && bms==1) || ams=='e' || ams=='E')
    {
     cflags=1;
     x=9;
     y=9;
     sum=10;
     pix=250-x*size/2;
     goto bss;
    }
    else
      if((xms>=pix-6 && xms<=pix+85 && yms>=39 && yms<=50 && bms==1) || ams=='n' || ams=='N')
     {
      cflags=1;
      x=16;
      y=16;
      sum=40;
      pix=250-x*size/2;
      goto bss;
     }
     else
       if((xms>=pix-6 && xms<=pix+85 && yms>=50 && yms<=61 && bms==1) || ams=='h' || ams=='H')
      {
       cflags=1;
       x=30;
       y=16;
       sum=99;
       pix=250-x*size/2+25;
       goto bss;
      }
      else
        if((xms>=pix-6 && xms<=pix+85 && yms>=61 && yms<=72 && bms==1) || ams=='c' || ams=='C')
       {
        cflags=1;
        Coread();
       }
        else
          if((xms>=pix-6 && xms<=pix+85 && yms>=72 && yms<=83 && bms==1) || ams=='l' || ams=='L')
       {
        cflags=1;
        Help();
       }
       else
         if((xms>=pix-6 && xms<=pix+85 && yms>=83 && yms<=94 && bms==1) || ams=='q' || ams=='Q' || ams==27)
        {
         cflags=1;
         fclose(p);
         closegraph();
         farfree(Map);
         exit(1);
        }
        else
          if(bms!=0)
         {
          cflags=1;
          goto bss;
         }
  }/*结束菜单功能的实现*/
 /*在笑脸按钮处单击时重新开始*/
   if(cflags!=0 && xms >=250-sizel-2 && yms >= 30-sizel-2 && xms<=250+sizel+2 && yms<=30+sizel+2 && bms==1 )
  {
   goto bss;
  }
 
   if(Eflags==0 && cflags!=0 && xms-pix>=0 && yms-piy >=0 && xms<pix+x*size && yms<piy+y*size )/*111判断*/
  {
   x1=(int)((xms-pix)/size);/*计算鼠标目前的按钮位置*/
   y1=(int)((yms-piy)/size);

   if(ams==-1 && bms==1 && pan1[x1][y1]==0)/*单击鼠标左键*/
   {
    if(k==0)/*k这里判断是否是第一次按下.0:不计时，1:开始计时*/
    {
     gettime(&t1);
     k=1;
    }
    if(pan[x1][y1]!=9)
    {
     Draw1(x1,y1);
     Open(x,y);
    }
    else
    {
     Dead(sizel,x,y);
     k=0;
    }
   }

   if(ams==-1 && bms==2 && pan1[x1][y1]==0)/*单击鼠标右键*/
   {
    delay(100);
    setcolor(10);
    circle(x1*size+pix+7,y1*size+piy+7,4);
    pan1[x1][y1]=2;
    Have(sum,pix+20,30,x,y);
   }
   else
     if(ams==-1 && bms==2 && pan1[x1][y1]==2)/*单击鼠标右键*/
    {
     delay(100);
     Draw(x1*size+pix,y1*size+piy,size,size);
     pan1[x1][y1]=0;
     Have(sum,pix+20,30,x,y);
    }/*相当于鼠标右击*/

  if(ams==-1 && bms==3 && pan1[x1][y1]==1)
   {/*相当于鼠标双击,有效性判断*/
    i=0;
    if(x1-1>=0 && y1-1>=0 && pan1[x1-1][y1-1]==2) i++;
    if(y1-1>=0 && pan1[x1][y1-1]==2)i++;
    if(x1+1<x && y1-1>=0 && pan1[x1+1][y1-1]==2) i++;
    if(x1-1>=0 && pan1[x1-1][y1]==2) i++;
    if(x1+1<x && pan1[x1+1][y1]==2) i++;
    if(x1-1>=0 && y1+1<y && pan1[x1-1][y1+1]==2) i++;
    if(y1+1<y && pan1[x1][y1+1]==2) i++;
    if(x1+1<x && y1+1<y && pan1[x1+1][y1+1]==2) i++;
    if(i==pan[x1][y1])     /*有效*/
       {
      if(x1-1>=0 && y1-1>=0 && pan1[x1-1][y1-1]!=2) 
     {
      if(pan[x1-1][y1-1]!=9) 
       Draw1(x1-1,y1-1);
      else 
       {
        Dead(sizel,x,y); 
         k=0;
       }
     }
      if(y1-1>=0  && pan1[x1][y1-1]!=2)
     {
      if(pan[x1][y1-1]!=9)
       Draw1(x1,y1-1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
       }
     }
      if(x1+1<x && y1-1>=0 && pan1[x1+1][y1-1]!=2) 
     {
      if(pan[x1+1][y1-1]!=9) 
       Draw1(x1+1,y1-1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
       }
     }
      if(x1-1>=0  && pan1[x1-1][y1]!=2) 
     {
      if(pan[x1-1][y1]!=9) 
       Draw1(x1-1,y1);
      else 
        {
         Dead(sizel,x,y);
         k=0;
        }
     }
      if(x1+1<x && pan1[x1+1][y1]!=2) 
     {
      if(pan[x1+1][y1]!=9) 
       Draw1(x1+1,y1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
       }
     }
      if(x1-1>=0 && y1+1<y && pan1[x1-1][y1+1]!=2) 
     {
      if(pan[x1-1][y1+1]!=9)
        Draw1(x1-1,y1+1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
        }
     }
      if(y1+1<y && pan1[x1][y1+1]!=2)
      {
      if(pan[x1][y1+1]!=9) 
       Draw1(x1,y1+1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
       }
     }
      if(x1+1<x && y1+1<y &&pan1[x1+1][y1+1]!=2) 
     {
      if(pan[x1+1][y1+1]!=9) 
       Draw1(x1+1,y1+1);
      else 
       {
        Dead(sizel,x,y);
        k=0;
       }
     }
      Open(x,y);
         }
          }

   /*胜利条件*/
  k1=0;
  for(i=0;i<x;i++)
  for(j=0;j<y;j++)
   if(pan1[i][j]!=1) k1++;

  if(k1==sum)
   {
    for(i=0;i<x;i++)
    for(j=0;j<y;j++)
    if(pan1[i][j]==0) 
     {
      setcolor(10);circle(i*size+pix+7,j*size+piy+7,4);
     }

    Facedraw(250,30,sizel,3);
       
    if(x==9 && tt<co[0])  {j=0;co[0]=tt;i=0;}
    if(x==16 && tt<co[1]) {j=1;co[1]=tt;i=0;}
    if(x==30 && tt<co[2]) {j=2;co[2]=tt;i=0;}

    if(i==0)
    {
     Draw(10,10,300,55);
     setcolor(4);
     outtextxy(20,15,"You break the corse!");
     outtextxy(20,25,"Please input your name(0 - 19):");
     setfillstyle(1, 14);
     bar(20,35,200,50);
     for(i=0;i<19;i++)
     {
      name[j][i]=getch();
      if(name[j][i]==8) i=i-2;
      if(name[j][i]==13) {name[j][i]='\0';break;}
      name[j][i+1]='\0';
      setfillstyle(1, 14);
      bar(20,35,200,50);
      setcolor(1);
      outtextxy(20,40,name[j]);
     }

      if((p = fopen("score.dat", "w")) == NULL)
     {
      printf("The file cannot open!\n");
      exit(1);
     }
      fprintf(p,"%d %d %d,",co[0],co[1],co[2]);
     for(i=0;i<3;i++)
     fprintf(p,"%s\n",name[i]);
     fclose(p);
     Coread();
    }
    getch();
    goto bss;
      }
  }/*结束111的判断*/

  /*下面是一些快捷功能键的定义*/
     if(ams=='q'||ams=='Q'|| ams==27 ) {fclose(p);closegraph();farfree(Map);exit(1);}   /*退出主程序的唯一出口*/
    if(ams=='c'||ams=='C') goto bss;   /*重来*/
    if(ams=='h'||ams=='H') {Help();k=0;}  /*帮助文档*/
    if(ams=='r'||ams=='R') {Coread();k=0;} /*察看成绩*/

  getimage(xms,yms,xms+20,yms+20,Map);
  Ddraw2(xms,yms);
  delay(10);
      }/*结束鼠标循环读取的死循环*/
}
/*++++++++主函数结束++++++++*/

/*这个函数用于实现鼠标的初始化*/
int Msinit()
{
 int recored;
 regs.x.ax=0;
 int86 (0x33, & regs, & regs);
 recored=regs.x.ax;
 if(recored==0)
   {
  printf("Mouse not found or Mouse driver not installed.\n");
     return 0;
 }
  else
   return recored;
}

/*下面这个子程序是完成描绘一个按钮*/
void Draw(int x,int y,int sizex,int sizey)/* x, y为左上角坐标sizex,sizey为长和宽*/
{
 int sizx=sizex-1;
 int sizy=sizey-1;
 setcolor(15);/*这里最好用白色*/
 line(x,y,x+sizx-1,y);
 line(x,y+1,x+sizx-2,y+1);
 line(x,y,x,y+sizy-1);
 line(x+1,y,x+1,y+sizy-2);
 setcolor(8);/*这里最好用深灰色*/
 line(x+1,y+sizy,x+sizx,y+sizy);
 line(x+2,y+sizy-1,x+sizx,y+sizy-1);
 line(x+sizx-1,y+1,x+sizx-1,y+sizy);
 line(x+sizx,y+2,x+sizx,y+sizy);
 setcolor(7);/*这里最好用灰色*/
 putpixel(x,y+sizy,3);
 putpixel(x+1,y+sizy-1,3);
 putpixel(x+sizx,y,3);
 putpixel(x+sizx-1,y+1,3);
 setfillstyle(1, 7);/*这里最好用灰色,设置填充模式*/
 bar(x+2,y+2,x+sizx-2,y+sizy-2);
}

/*下面是一些画图程序*/
/*形参x,y笑脸的中心坐标,sizel脸的大小(半径),
k为要描述的脸型，1：哭、2：平常、3：笑*/
void Facedraw(int x,int y,int sizel,int k)
 {Draw(x-sizel-3,y-sizel-3,2*(sizel+4),2*(sizel+4));/*画一个按钮*/
  setcolor(14);
  setfillstyle(1, 14);
  pieslice(x, y, 0, 360, sizel);/*画一个黄色的圆面*/
  setfillstyle(1, 0);
  setcolor(0);
  if(k==1)/*失败*/
  {
  line(x-sizel*7/20,y-sizel*7/20,x-sizel*3/20,y-sizel*3/20);/*画两个差表示眼睛*/
    line(x-sizel*7/20,y-sizel*3/20,x-sizel*3/20,y-sizel*7/20);
    line(x+sizel*3/20,y-sizel*7/20,x+sizel*7/20,y-sizel*3/20);
    line(x+sizel*3/20,y-sizel*3/20,x+sizel*7/20,y-sizel*7/20);
    arc(x,y+sizel,20,160, sizel/2);
 }
  if(k==2)/*平常*/
   {
  bar(x-sizel*7/20,y-sizel*7/20,x-sizel*3/20,y-sizel*3/20);
    bar(x+sizel*3/20,y-sizel*7/20,x+sizel*7/20,y-sizel*3/20);
    arc(x,y,200,340, sizel*3/4);
   }
  if(k==3)/*成功*/
   {
  bar(x-sizel/2,y-sizel/2,x-sizel*3/20,y-sizel*3/20);
    bar(x+sizel*3/20,y-sizel/2,x+sizel/2,y-sizel*3/20);
    line(x-sizel*3/20,y-sizel/2,x+sizel*3/20,y-sizel/2);
    line(x-sizel/2,y-sizel*5/20,x-sizel,y);
    line(x+sizel/2,y-sizel*5/20,x+sizel,y);
    arc(x,y,200,340, sizel*3/4); /*嘴巴*/
   }
}

/*死亡后的处理程序*/
/*x,y要判断的点的坐标,sizel为脸的大小*/
void Dead(int sizel,int x,int y)
{
 int i,j;/*循环变量*/
 setcolor(4);
 for(i=0;i<x;i++)
 for(j=0;j<y;j++)
 if(pan[i][j]==9)
 {
  circle(i*size+pix+7,j*size+piy+7,4);
 }/*用红色的圆圈标记有雷的方块*/
 Facedraw(250,30,sizel,1);
 Eflags=1;
}

/*下面的函数用于实现设置鼠标的移动范围*/
/*xmax,ymax为左上角的坐标x,y矩阵的大小*/
void Setmouse(int xmax,int ymax,int x,int y)
{
 int left,right,top,buttom;/*这四个变量用于确定移动的左上角和右下角*/
 left=xmax;
 right=x;
 top=ymax;
 buttom=y;
 regs.x.ax=7;
 regs.x.cx=left;
 regs.x.dx=right;
 int86(0x33,& regs,& regs);
 regs.x.ax=8;
 regs.x.cx=top;
 regs.x.dx=buttom;
 int86(0x33,& regs,& regs);
}

/*下面这个函数用于实现鼠标的读取*/
/*xp,yp,bup分别为鼠标的位置和按键情况,t1,k是时间现实参数,t1为开始时间，k为开始标记*/
int Msread(int *xp,int *yp,int *bup,struct time t1,int k)
{
 int x0=*xp,y0=*yp,bu0=*bup;
 int xnew,ynew,ch;
 int tt1=0;/*用于记录时间差*/
 struct time t2;
 char ttt[4]="000";
 do
  {
  if(kbhit()) return getch();
      regs.x.ax=3;
      int86(0x33,& regs,& regs);
      xnew=regs.x.cx;
     ynew=regs.x.dx;
     *bup=regs.x.bx;
 /*下面用于显示时间*/
  if(k==1)/*如果已经开始就计算时间差*/
        {
   gettime(&t2);
       tt1=(t2.ti_hour- t1.ti_hour)*3600 + (t2.ti_min- t1.ti_min)*60 + t2.ti_sec  - t1.ti_sec;/*有待改进*/
        if(tt1<=999 && tt!=tt1)/*当时间差有改变时，才显示*/
   {
    tt=tt1;
     ttt[0]=(tt-tt%100)/100+48;
    ttt[1]=(tt%100-tt%10)/10 +48;
    ttt[2]=tt%10+48;
    setfillstyle(1, 0);
    bar(460-pix,25,490-pix,40);
    setcolor(4);
    outtextxy(465-pix,30,ttt);
   }
        }
    }
 while(xnew==x0&&ynew==y0&&*bup==bu0);
 *xp=xnew;
 *yp=ynew;
 return -1;/*键盘无输入时返回-1,其次返回键盘值*/
}

void Draw1(int x,int y)/*这个函数用于在x,y的位置描绘开后的情况*/
{
 setfillstyle(1, 7);
 bar(x*size+pix,y*size+piy,x*size+size+pix-1,y*size+size+piy-1);
 
 /*在四个角上画上点标记*/
 /*putpixel(x*size+pix,y*size+piy,8);
 putpixel(x*size+size+pix-1,y*size+size+piy-1,8);
 putpixel(x*size+pix,y*size+size+piy-1,8);
 putpixel(x*size+size+pix-1,y*size+piy,8);*/

 setcolor(8);
 setlinestyle(1, 0, 1);
 rectangle(x*size+pix,y*size+piy,x*size+size+pix-1,y*size+size+piy-1);
 setlinestyle(0, 0, 1);

 if(pan[x][y]!=0)/*没有雷的话不做显示*/
    {
  b[0]=pan[x][y]+48;
       if(pan[x][y]!=7)/*防止字的颜色和背景颜色相同*/
  setcolor(pan[x][y]);
  else setcolor(9);
  outtextxy (x*size+pix+3,y*size+piy+3,b);
    }
 pan1[x][y]=1;/*在pan1[][]中标记这一点的挖雷情况*/
}

/*自开函数*/
int Open(int x,int y)/*函数中引入pan2[][]的目的是为了增加自开的效率,但是还是有不足之处*/
{
 int i,j,k;
 int pan2[30][16]={0};/*pan2[][],临时纪录盘面的自开情况,0没有自开过,1:已经自开过(30 这里有危险的技术性问题!!!)*/
 for(;;)
 {
  k=0;/*是一个标记变量*/
  for(i=0;i<x;i++)
  for(j=0;j<y;j++)
      {
   if (pan[i][j]==0 && pan1[i][j]==1 && pan2[i][j]==0)/*如果满足下面条件,没有雷,已经打开,没有自开过就继续*/
   {
    pan2[i][j]=1;/*标记自开过*/
     k=k+1;
   /*开周围的点*/
    if(i-1>=0 && j-1>=0 && pan1[i-1][j-1]==0) Draw1(i-1,j-1); /*左上方*/
    if(j-1>=0 && pan1[i][j-1]==0) Draw1(i,j-1);       /*上方*/
    if(i+1<x && j-1>=0 && pan1[i+1][j-1]==0) Draw1(i+1,j-1);  /*右上方*/
    if(i-1>=0 && pan1[i-1][j]==0) Draw1(i-1,j);          /*左方*/
    if(i+1<x && pan1[i+1][j]==0) Draw1(i+1,j);              /*右方*/
    if(i-1>=0 && j+1<y && pan1[i-1][j+1]==0) Draw1(i-1,j+1);  /*左下方*/
    if(j+1<y && pan1[i][j+1]==0) Draw1(i,j+1);              /*下方*/
    if(i+1<x && j+1<y && pan1[i+1][j+1]==0) Draw1(i+1,j+1);   /*右下方*/
   }
       }
  if(k==0) return 0;/*没有新的自开点时退出*/
 }
}

float Random()/*随即数生成函数,加入了时间变量目的在于加强它的随机性*/
{float aa;
 int i,j;
 struct time t;
 gettime(&t);
 i=t.ti_min+t.ti_hour+t.ti_sec+t.ti_hund;
 for(j=0;j<i;j++)
 rand();
 aa=rand()/32767.0;
 return aa;/*返回产生的随机数(0-1)*/
}

/*下面是一个显示剩余雷数目的函数*/
void Have(int sum,int x,int y,int xx,int yy)/*游戏中雷的总数目不能多余99颗,sum 为雷的总数目,x,y为显示的地点*/
{
 int i,j,k=0;
 char a[3]="1";
 for(i=0;i<xx;i++)
 for(j=0;j<yy;j++)
    {if (pan1[i][j]==2) k++;}/*统计一共标记了多少*/
 k=sum-k;/*剩下了多少*/
 if(k<0) k=0;
 a[0]=(k-(k%10))/10+48;/*字符转换*/
 a[1]=k%10+48;
 setfillstyle(1, 0);
 bar(x-5,y-5,x+20,y+10);/*在原点涂黑*/
 setcolor(4);
 outtextxy(x,y,a);/*显示数目*/
}

/*下面是帮助函数*/
void Help()
{
 Draw(40,1,560,160);
 setcolor(1);
 outtextxy(300,10,"Help");
 setcolor(4);
 outtextxy(50,30,"Welcome to play this game. The mouse is supplied.");
 outtextxy(50,40,"Please press left button of the mouse where there is no boom.");
 outtextxy(50,50,"If you think there is a boom please press right button.");
 outtextxy(50,60,"You can choose the size of the game with press the right buttons.");
 outtextxy(50,70,"'Big' means 99 booms,'Normal' means 40 booms,'Small' means 10 booms.");
 outtextxy(50,80,"You can press the laugh-face-button or press C to play again.");
 outtextxy(50,90,"When you fail or success you can press any key to play again. ");
 outtextxy(50,100,"Whenever you want to quit this game please press Q. ");
 outtextxy(50,110,"If you find out all booms then you are winning. ");
 setcolor(9);
 outtextxy(50,140,"Press any key to play again.");
 Eflags=2;
}

void Coread()/*这个函数用于读取纪录并显示*/
{ 
  int co[3],i;
  char name[3][20];
  char cco[3][4];
  FILE * p;
  for(i=0;i<3;i++)
       cco[i][3]='\0';

  if((p = fopen("score.dat", "rb")) == NULL)
     {
  printf("The file cannot open!\n");
       exit(1);
 }

  fscanf(p,"%d %d %d,",&co[0],&co[1],&co[2]);
  for(i=0;i<3;i++)
     fscanf(p,"%s\n",name[i]);
  fclose(p);
  Draw(10,20,320,90);
  setcolor(1);
  outtextxy(140,30,"HighCore");
  setcolor(2);
  outtextxy(20,50,"Easy");
  outtextxy(20,60,"Normal");
  outtextxy(20,70,"Hard");

  for(i=0;i<3;i++)
 {
  cco[i][0]=(co[i]-co[i]%100)/100+48;
   cco[i][1]=(co[i]%100-co[i]%10)/10 +48;
   cco[i][2]=co[i]%10+48;
 }

  setcolor(3);
  outtextxy(100,50,cco[0]);
  outtextxy(100,60,cco[1]);
  outtextxy(100,70,cco[2]);

  setcolor(5);
  outtextxy(150,50,name[0]);
  outtextxy(150,60,name[1]);
  outtextxy(150,70,name[2]);  

  setcolor(9);
  outtextxy(30,85,"Press any key to play again.");
  Eflags=2;
}

/*这个函数用来画鼠标光标,x,y,为左上角坐标，大小为20*20*/
void Ddraw2(int x,int y)
{
 /*这个地方暂时使用简易光标，程序成熟以后使用复杂的*/
 setcolor(1);
 line(x,y,x,y+18);
 line(x+1,y,x+1,y+18);
 line(x,y,x+12,y+12);
 line(x+1,y,x+13,y+12);
 line(x,y+18,x+4,y+14);
 line(x,y+17,x+4,y+13);
 line(x+4,y+14,x+6,y+20);
 line(x+4,y+13,x+6,y+19);
 line(x+6,y+20,x+10,y+18);
 line(x+6,y+19,x+10,y+17);
 line(x+10,y+18,x+8,y+12);
 line(x+10,y+17,x+8,y+11);
 line(x+8,y+12,x+12,y+12);
 line(x+8,y+11,x+12,y+11);
}

/*直接编译运行*/
 
 
