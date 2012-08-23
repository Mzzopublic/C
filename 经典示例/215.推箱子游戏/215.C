/*
推箱子游戏
*/
#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <bios.h>
#include <alloc.h>
/*  定义二维数组ghouse来记录屏幕上各点的状态，
其中：0表示什么都没有，'b'表示箱子，'w'表示墙壁，'m'表示目的地，'i'表示箱子在目的地。 */
char ghouse[20][20];

/*  以下函数为直接写屏函数，很酷的函数哦！是我朋友告诉我的。 */
char far *screen=(char far* )0xb8000000;
void putchxy(int y,int x,char ch,char fc,char bc)
{
  screen[(x*160)+(y<<1)+0]=ch;
  screen[(x*160)+(y<<1)+1]=(bc*16)+fc;
}

/* 定义判断是否胜利的数据结构 */
typedef struct winer {
  int x,y;
  struct winer *p;
}winer;

/* 箱子位置的数据结构 */
typedef struct boxs {
 int x,y;
 struct boxs *next;
}boxs;

/* 在特定的坐标上画墙壁并用数组记录状态的函数 */
void printwall(int x,int y)
{
    putchxy(y-1,x-1,219,MAGENTA,BLACK);
    ghouse[x][y]='w';
}

/* 在特定的坐标上画箱子并用数组记录状态的函数 */
void printbox(int x,int y)
{
   putchxy(y-1,x-1,10,WHITE,BLACK);
   ghouse[x][y]='b';
}

/* 在特定的坐标上画目的地并用数组记录状态的函数 */
void printwhither1(int x,int y,winer  **win,winer **pw)
{
   winer *qw;
   putchxy(y-1,x-1,'*',YELLOW,BLACK);
   ghouse[x][y]='m';
   if(*win==NULL)
   {
     *win=*pw=qw=(winer* )malloc(sizeof(winer));
     (*pw)->x=x;(*pw)->y=y;(*pw)->p=NULL;
   }
   else
   {
     qw=(winer* )malloc(sizeof(winer));
     qw->x=x;qw->y=y;(*pw)->p=qw;(*pw)=qw;qw->p=NULL;
   }
}


/* 在特定的坐标上画目的地并用数组记录状态的函数 */
void printwhither(int x,int y)
{
   putchxy(y-1,x-1,'*',YELLOW,BLACK);
   ghouse[x][y]='m';
}
/* 在特定的坐标上画人的函数 */
void printman(int x,int y)
{
   gotoxy(y,x);
   _AL=02;_CX=01;_AH=0xa;
   geninterrupt(0x10);
}

/* 在特定的坐标上画箱子在目的地上并用数组记录状态的函数 */
void printboxin(int x,int y)
{
  putchxy(y-1,x-1,10,YELLOW,BLACK);
  ghouse[x][y]='i';
}

/* 初始化函数，初始化数组和屏幕 */
void init()
{
  int i,j;
  
   clrscr();
  for(i=0;i<20;i++)
    for(j=0;j<20;j++)
     ghouse[i][j]=0;
      _AL=3;
      _AH=0;
      geninterrupt(0x10);
       gotoxy(40,4);
  printf("Welcome to push box world!");
       gotoxy(40,6);
         printf("Press up,down,left,right to play.");
       gotoxy(40,8);
         printf("Press Esc to quit it.");
       gotoxy(40,10);
  	 printf("Press space to reset the game.");
       gotoxy(40,12);
         printf("April 30th 2004.");
}

/* 第一关的图象初始化 */
winer *inithouse1()
{
	
  int x,y;
  winer *win=NULL,*pw;
  gotoxy(8,2);
  printf("Level No.1");
  for(x=1,y=5;y<=9;y++)
     printwall(x+4,y+10);
  for(y=5,x=2;x<=5;x++)
     printwall(x+4,y+10);
  for(y=9,x=2;x<=5;x++)
     printwall(x+4,y+10);
  for(y=1,x=3;x<=8;x++)
     printwall(x+4,y+10);
  for(x=3,y=3;x<=5;x++)
       printwall(x+4,y+10);
  for(x=5,y=8;x<=9;x++)
       printwall(x+4,y+10);
  for(x=7,y=4;x<=9;x++)
       printwall(x+4,y+10);
  for(x=9,y=5;y<=7;y++)
       printwall(x+4,y+10);
  for(x=8,y=2;y<=3;y++)
       printwall(x+4,y+10);
  printwall(5+4,4+10);
  printwall(5+4,7+10);
  printwall(3+4,2+10);
  printbox(3+4,6+10);
  printbox(3+4,7+10);
  printbox(4+4,7+10);
  printwhither1(4+4,2+10,&win,&pw);
  printwhither1(5+4,2+10,&win,&pw);
  printwhither1(6+4,2+10,&win,&pw);
  printman(2+4,8+10);
return win;
}

/* 第三关的图象初始化 */
winer *inithouse3()
{int x,y;
 winer *win=NULL,*pw;
 gotoxy(8,3);
  printf("Level No.3");
 for(x=1,y=2;y<=8;y++)
    printwall(x+4,y+10);
 for(x=2,y=2;x<=4;x++)
    printwall(x+4,y+10);
 for(x=4,y=1;y<=3;y++)
    printwall(x+4,y+10);
 for(x=5,y=1;x<=8;x++)
    printwall(x+4,y+10);
 for(x=8,y=2;y<=5;y++)
    printwall(x+4,y+10);
 for(x=5,y=5;x<=7;x++)
    printwall(x+4,y+10);
 for(x=7,y=6;y<=9;y++)
    printwall(x+4,y+10);
 for(x=3,y=9;x<=6;x++)
    printwall(x+4,y+10);
 for(x=3,y=6;y<=8;y++)
    printwall(x+4,y+10);
 printwall(2+4,8+10);
 printwall(5+4,7+10);
 printbox(6+4,3+10);
 printbox(4+4,4+10);
 printbox(5+4,6+10);
  printwhither1(2+4,5+10,&win,&pw);
  printwhither1(2+4,6+10,&win,&pw);
  printwhither1(2+4,7+10,&win,&pw);
 printman(2+4,4+10);
return win;
}

/* 第二关的图象初始化 */
winer *inithouse2()
{int x,y;
 winer *win=NULL,*pw;
 gotoxy(8,2);
  printf("Level No.2");
 for(x=1,y=4;y<=7;y++)
    printwall(x+4,y+10);
 for(x=2,y=2;y<=4;y++)
    printwall(x+4,y+10);
 for(x=2,y=7;x<=4;x++)
    printwall(x+4,y+10);
 for(x=4,y=1;x<=8;x++)
    printwall(x+4,y+10);
 for(x=8,y=2;y<=8;y++)
    printwall(x+4,y+10);
 for(x=4,y=8;x<=8;x++)
    printwall(x+4,y+10);
 for(x=4,y=6;x<=5;x++)
    printwall(x+4,y+10);
 for(x=3,y=2;x<=4;x++)
    printwall(x+4,y+10);
 for(x=4,y=4;x<=5;x++)
    printwall(x+4,y+10);
 printwall(6+4,3+10);
 printbox(3+4,5+10);
 printbox(6+4,6+10);
 printbox(7+4,3+10);
  printwhither1(5+4,7+10,&win,&pw);
  printwhither1(6+4,7+10,&win,&pw);
  printwhither1(7+4,7+10,&win,&pw);
 printman(2+4,6+10);
return win;
}

/* 第四关的图象初始化 */
winer *inithouse4()
{
  int x,y;
  winer *win=NULL,*pw;
  gotoxy(8,2);
  printf("Level No.4");
  for(x=1,y=1;y<=6;y++)
     printwall(x+4,y+10);
  for(x=2,y=7;y<=8;y++)
     printwall(x+4,y+10);
  for(x=2,y=1;x<=7;x++)
     printwall(x+4,y+10);
  for(x=7,y=2;y<=4;y++)
     printwall(x+4,y+10);
  for(x=6,y=4;y<=9;y++)
     printwall(x+4,y+10);
  for(x=3,y=9;x<=5;x++)
     printwall(x+4,y+10);
  for(x=3,y=3;y<=4;y++)
     printwall(x+4,y+10);
  printwall(3+4,8+10);
  printbox(3+4,5+10);
  printbox(4+4,4+10);
  printbox(4+4,6+10);
  printbox(5+4,5+10);
  printbox(5+4,3+10);
  printwhither1(3+4,7+10,&win,&pw);
  printwhither1(4+4,7+10,&win,&pw);
  printwhither1(5+4,7+10,&win,&pw);
  printwhither1(4+4,8+10,&win,&pw);
  printwhither1(5+4,8+10,&win,&pw);
 printman(2+4,2+10);
return win;
}

/* 移动在空地上的箱子到空地上 */
movebox(int x,int y,char a)
{
  switch(a)
  {
    case 'u':ghouse[x-1][y]=0;printf(" ");
      printbox(x-2,y);printman(x-1,y);
             ghouse[x-2][y]='b';break;
    case 'd':ghouse[x+1][y]=0;printf(" ");
      printbox(x+2,y);printman(x+1,y);
             ghouse[x+2][y]='b';break;
    case 'l':ghouse[x][y-1]=0;printf(" ");
      printbox(x,y-2);printman(x,y-1);
             ghouse[x][y-2]='b';break;
    case 'r':ghouse[x][y+1]=0;printf(" ");
      printbox(x,y+2);printman(x,y+1);
             ghouse[x][y+2]='b';break;
    default: break;
  }
}

/* 移动在目的地上的箱子到空地上 */
moveinbox(int x,int y,char a)
{
  switch(a)
  {
    case 'u':ghouse[x-1][y]='m';printf(" ");
      printbox(x-2,y);printman(x-1,y);
             ghouse[x-2][y]='b';break;
    case 'd':ghouse[x+1][y]='m';printf(" ");
      printbox(x+2,y);printman(x+1,y);
             ghouse[x+2][y]='b';break;
    case 'l':ghouse[x][y-1]='m';printf(" ");
      printbox(x,y-2);printman(x,y-1);
             ghouse[x][y-2]='b';break;
    case 'r':ghouse[x][y+1]='m';printf(" ");
      printbox(x,y+2);printman(x,y+1);
             ghouse[x][y+2]='b';break;
    default: break;
  }
}


 
/* 移动在空地上的箱子到目的地上 */
moveboxin(int x,int y,char a)
{
  switch(a)
  {
    case 'u':ghouse[x-1][y]=0;printf(" ");
      printboxin(x-2,y);printman(x-1,y);
      ghouse[x-2][y]='i';break;
    case 'd':ghouse[x+1][y]=0;printf(" ");
      printboxin(x+2,y);printman(x+1,y);
      ghouse[x+2][y]='i';break;
    case 'l':ghouse[x][y-1]=0;printf(" ");
      printboxin(x,y-2);printman(x,y-1);
      ghouse[x][y-2]='i';break;
    case 'r':ghouse[x][y+1]=0;printf(" ");
      printboxin(x,y+2);printman(x,y+1);
      ghouse[x][y+2]='i';break;
    default: break;
  }
}

/* 移动在目的地上的箱子到目的地 */
moveinboxin(int x,int y,char a)
{
  switch(a)
  {
    case 'u':ghouse[x-1][y]='m';printf(" ");
      printboxin(x-2,y);printman(x-1,y);
      ghouse[x-2][y]='i';break;
    case 'd':ghouse[x+1][y]='m';printf(" ");
      printboxin(x+2,y);printman(x+1,y);
      ghouse[x+2][y]='i';break;
    case 'l':ghouse[x][y-1]='m';printf(" ");
      printboxin(x,y-2);printman(x,y-1);
      ghouse[x][y-2]='i';break;
    case 'r':ghouse[x][y+1]='m';printf(" ");
      printboxin(x,y+2);printman(x,y+1);
      ghouse[x][y+2]='i';break;
    default: break;
  }
}

/* 判断特定的坐标上的状态 */
int judge(int x,int y)
{
  int i;
  switch(ghouse[x][y])
  {
  case 0:   i=1;break;
  case 'w': i=0;break;
  case 'b': i=2;break;
  case 'i': i=4;break;
  case 'm': i=3;break;
  default: break;
  }
return i;
}

/* 处理按下键盘后,人物移动的主函数 */
move(int x,int y,char a)
{

   switch(a)
   {
     case 'u':if(!judge(x-1,y)) {gotoxy(y,x);break;}
       else if(judge(x-1,y)==1||judge(x-1,y)==3)
        {if(judge(x,y)==3)
  { printwhither(x,y);printman(x-1,y);break;}
  else
  {printf(" ");printman(x-1,y);break;}
        }
       else if(judge(x-1,y)==2)
               { if(judge(x-2,y)==1)
   {movebox(x,y,'u');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y,x-1);
                  }
                else if(judge(x-2,y)==3)
   { moveboxin(x,y,'u');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y,x-1);
                  }
               else gotoxy(y,x);
               break;
               }
              else if(judge(x-1,y)==4)
               { if(judge(x-2,y)==1)
   {moveinbox(x,y,'u');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x-1);
                  }
                else if(judge(x-2,y)==3)
   { moveinboxin(x,y,'u');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x-1);
                  }
               else gotoxy(y,x);
               break;
               }
     case 'd':if(!judge(x+1,y))  {gotoxy(y,x);break;}
       else if(judge(x+1,y)==1||judge(x+1,y)==3)
               {if(judge(x,y)==3)
                { printwhither(x,y);printman(x+1,y);break;}
                else
                {printf(" ");printman(x+1,y);break;}
               }
              else if(judge(x+1,y)==2)
               { if(judge(x+2,y)==1)
    {movebox(x,y,'d');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x+1);
                  }
                 else if(judge(x+2,y)==3)
   {moveboxin(x,y,'d');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x+1);
                  }
               else gotoxy(y,x);
        break;
               }
              else if(judge(x+1,y)==4)
               { if(judge(x+2,y)==1)
    {moveinbox(x,y,'d');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x+1);
                  }
                 else if(judge(x+2,y)==3)
   {moveinboxin(x,y,'d');
    if(judge(x,y)==3) printwhither(x,y);gotoxy(y,x+1);
                  }
               else gotoxy(y,x);
               break;
               }

     case 'l':if(!judge(x,y-1))  {gotoxy(y,x);break;}
       else if(judge(x,y-1)==1||judge(x,y-1)==3)
               {if(judge(x,y)==3)
                { printwhither(x,y);printman(x,y-1);break;}
  else
  {printf(" ");printman(x,y-1);break;}
  }
        else if(judge(x,y-1)==2)
               { if(judge(x,y-2)==1)
   {movebox(x,y,'l');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y-1,x);
                  }
                 else if(judge(x,y-2)==3)
    {moveboxin(x,y,'l');
    if(judge(x,y)==3) printwhither(x,y);  gotoxy(y-1,x);
                  }
               else gotoxy(y,x);
               break;
               }
        else if(judge(x,y-1)==4)
               { if(judge(x,y-2)==1)
   {moveinbox(x,y,'l');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y-1,x);
                  }
                 else if(judge(x,y-2)==3)
    {moveinboxin(x,y,'l');
    if(judge(x,y)==3) printwhither(x,y);  gotoxy(y-1,x);
                  }
               else gotoxy(y,x);
               break;
               }
     case 'r':if(!judge(x,y+1))  {gotoxy(y,x);break;}
       else if(judge(x,y+1)==1||judge(x,y+1)==3)
               {if(judge(x,y)==3)
                 {printwhither(x,y);printman(x,y+1);break;}
               else
                 {printf(" ");printman(x,y+1);break;}
               }
              else if(judge(x,y+1)==2)
               { if(judge(x,y+2)==1)
    {movebox(x,y,'r');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y+1,x);
                  }
                 else if(judge(x,y+2)==3)
   {moveboxin(x,y,'r');
    if(judge(x,y)==3) printwhither(x,y);  gotoxy(y+1,x);
                  }
               else gotoxy(y,x);
               break;
               }
              else if(judge(x,y+1)==4)
               { if(judge(x,y+2)==1)
    {moveinbox(x,y,'r');
    if(judge(x,y)==3) printwhither(x,y); gotoxy(y+1,x);
                  }
                 else if(judge(x,y+2)==3)
   {moveinboxin(x,y,'r');
    if(judge(x,y)==3) printwhither(x,y);  gotoxy(y+1,x);
                  }
               else gotoxy(y,x);
               break;
               }
     default: break;
   }
}

/* 按下空格键后,回到本关开头的函数 */
void reset(int i)
{
          switch(i)
        {
 case 0:  init();
   inithouse1();break;
 case 1:  init();
   inithouse2();break;
        case 2:  init();
   inithouse3();break;
 case 3:  init();
   inithouse4();break;  
 default:break;
 }
}

/* 主函数main */
void main()
{
   int key,x,y,s,i=0;
   winer *win,*pw;
  
   
    _AL=3;_AH=0;
   geninterrupt(0x10);
   init();
   win=inithouse1();

   do{
      _AH=3;
      geninterrupt(0x10);
      x=_DH+1;y=_DL+1;
      while(bioskey(1)==0);
      key=bioskey(0);
      switch(key)
      {
      case 0x4800:move(x,y,'u');break; /* 按下向上键后 */
      case 0x5000:move(x,y,'d');break; /* 按下向下键后 */
      case 0x4b00:move(x,y,'l');break; /* 按下向左键后 */
      case 0x4d00:move(x,y,'r');break; /* 按下向右键后 */
      case 0x3920:reset(i);break;      /* 按下空格键后 */
      default:break;
      }
     s=0;
     pw=win;
     while(pw)
     {
      if(ghouse[pw->x][pw->y]=='m') s++;
      pw=pw->p;
     }
     if(s==0)
     {
 free(win);
 gotoxy(25,2);
 printf("Congratulate! You have passed Level %d!",i+1);
 getch();
        i++;
        switch(i)
        {
        case 1:  init();
   win=inithouse2();break;
 case 2:  init();
   win=inithouse3();break;
 case 3:  init();
   win=inithouse4();break;
 case 4:  gotoxy(8,14);
   printf("Great! You have passed all the levels! Press any key to quit!");
   key=0x011b;getch();break;
 default: break;
 }
     }

   }while(key!=0x011b);

_AL=3;
_AH=0;
geninterrupt(0x10);
}
 
 
