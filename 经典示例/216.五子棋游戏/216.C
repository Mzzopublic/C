/*
五子棋
*/

#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<bios.h>
#include<conio.h>

#define LEFT 0x4b00 
#define RIGHT 0x4d00 
#define DOWN 0x5000 
#define UP 0x4800
#define ESC 0x011b 
#define SPACE 0x3920 

#define BILI 20 
#define JZ 4 
#define JS 3 
#define N 19 

int box[N][N];
int step_x,step_y ;
int key ;
int flag=1 ;

void draw_box();
void draw_cicle(int x,int y,int color);
void change();
void judgewho(int x,int y);
void judgekey();
int judgeresult(int x,int y);
void attentoin();

void attention()
{
    char ch ;
    window(1,1,80,25);
    textbackground(LIGHTBLUE);
    textcolor(YELLOW);
    clrscr();
    gotoxy(15,2);
    printf("游戏操作规则：");
    gotoxy(15,4);
    printf("Play Rules:");
    gotoxy(15,6);
    printf("1、按左右上下方向键移动棋子");
    gotoxy(15,8);
    printf("1. Press Left,Right,Up,Down Key to move Piece");
    gotoxy(15,10);
    printf("2、按空格确定落棋子");
    gotoxy(15,12);
    printf("2. Press Space to place the Piece");
    gotoxy(15,14);
    printf("3、禁止在棋盘外按空格");
    gotoxy(15,16);
    printf("3. DO NOT press Space outside of the chessboard");
    gotoxy(15,18);
    printf("你是否接受上述的游戏规则（Y/N）");
    gotoxy(15,20);
    printf("Do you accept the above Playing Rules? [Y/N]:");
    while(1)
    {
        gotoxy(60,20);
        ch=getche();
        if(ch=='Y'||ch=='y')
        break ;
        else if(ch=='N'||ch=='n')
        {
        	window(1,1,80,25);
    		textbackground(BLACK);
    		textcolor(LIGHTGRAY);
        	clrscr();
        	exit(0);
	}
        gotoxy(51,12);
        printf(" ");
    }
}
void draw_box()
{
    int x1,x2,y1,y2 ;
    setbkcolor(LIGHTBLUE);
    setcolor(YELLOW);
    gotoxy(7,2);
    printf("Left, Right, Up, Down KEY to move, Space to put, ESC-quit.");
    for(x1=1,y1=1,y2=18;x1<=18;x1++)
    line((x1+JZ)*BILI,(y1+JS)*BILI,(x1+JZ)*BILI,(y2+JS)*BILI);
    for(x1=1,y1=1,x2=18;y1<=18;y1++)
    line((x1+JZ)*BILI,(y1+JS)*BILI,(x2+JZ)*BILI,(y1+JS)*BILI);
    for(x1=1;x1<=18;x1++)
    for(y1=1;y1<=18;y1++)
    box[x1][y1]=0 ;
}

void draw_circle(int x,int y,int color)
{
    setcolor(color);
    setlinestyle(SOLID_LINE,0,1);
    x=(x+JZ)*BILI ;
    y=(y+JS)*BILI ;
    circle(x,y,8);
}

void judgekey()
{
    int i ;
    int j ;
    switch(key)
    {
        case LEFT :

        if(step_x-1<0)
        break ;
        else
        {
            for(i=step_x-1,j=step_y;i>=1;i--)
            if(box[i][j]==0)
            {
		draw_circle(step_x,step_y,LIGHTBLUE);
                break ;
            }
            if(i<1)break ;
            step_x=i ;
            judgewho(step_x,step_y);
            break ;
        }
        case RIGHT :

        if(step_x+1>18)
        break ;
        else
        {
            for(i=step_x+1,j=step_y;i<=18;i++)
            if(box[i][j]==0)
            {
		draw_circle(step_x,step_y,LIGHTBLUE);
                break ;
            }
            if(i>18)break ;
            step_x=i ;
            judgewho(step_x,step_y);
            break ;
        }
        case DOWN :

        if((step_y+1)>18)
        break ;
        else
        {
            for(i=step_x,j=step_y+1;j<=18;j++)
            if(box[i][j]==0)
            {
		draw_circle(step_x,step_y,LIGHTBLUE);
                break ;
            }
            if(j>18)break ;
            step_y=j ;
            judgewho(step_x,step_y);
            break ;
        }
        case UP :

        if((step_y-1)<0)
        break ;
        else
        {
            for(i=step_x,j=step_y-1;j>=1;j--)
            if(box[i][j]==0)
            {
		draw_circle(step_x,step_y,LIGHTBLUE);
                break ;
            }
            if(j<1)break ;
            step_y=j ;
            judgewho(step_x,step_y);
            break ;
        }
        case ESC :
        break ;

        case SPACE :
        if(step_x>=1&&step_x<=18&&step_y>=1&&step_y<=18)
        {
            if(box[step_x][step_y]==0)
            {
                box[step_x][step_y]=flag ;
                if(judgeresult(step_x,step_y)==1)
                {
                    sound(1000);
                    delay(1000);
                    nosound();
                    gotoxy(30,4);
                    if(flag==1)
                    {
                        setbkcolor(BLUE);
                        cleardevice();
                        setviewport(100,100,540,380,1);
                        /*定义一个图形窗口*/
                        setfillstyle(1,2);
                        /*绿色以实填充*/
                        setcolor(YELLOW);
                        rectangle(0,0,439,279);
                        floodfill(50,50,14);
                        setcolor(12);
   			settextstyle(1,0,5);
  			 /*三重笔划字体, 水平放?5倍*/
                        outtextxy(20,20,"The White Win !");
                        setcolor(15);
                        settextstyle(3,0,5);
                        /*无衬笔划字体, 水平放大5倍*/
                        outtextxy(120,120,"The White Win !");
                        setcolor(14);
                        settextstyle(2,0,8);
                        getch();
                        closegraph();
                        exit(0);
                    }
                    if(flag==2)
                    {
                        setbkcolor(BLUE);
                        cleardevice();
                        setviewport(100,100,540,380,1);
                        /*定义一个图形窗口*/
                        setfillstyle(1,2);
                        /*绿色以实填充*/
                        setcolor(YELLOW);
                        rectangle(0,0,439,279);
                        floodfill(50,50,14);
                        setcolor(12);
                        settextstyle(1,0,8);
                        /*三重笔划字体, 水平放大8倍*/
                        outtextxy(20,20,"The Red Win !");
                        setcolor(15);
                        settextstyle(3,0,5);
                        /*无衬笔划字体, 水平放大5倍*/
                        outtextxy(120,120,"The Red Win !");
                        setcolor(14);
                        settextstyle(2,0,8);
                        getch();
                        closegraph();
                        exit(0);
                    }
                }
                change();
                break ;
            }
        }
        else 
        break ;
    }
}

void change()
{
    if(flag==1)
    flag=2 ;
    else 
    flag=1 ;
}

void judgewho(int x,int y)
{
    if(flag==1)
    draw_circle(x,y,15);
    if(flag==2)
    draw_circle(x,y,4);
}

int judgeresult(int x,int y)
{
    int j,k,n1,n2 ;
    while(1)
    {
        n1=0 ;
        n2=0 ;
        /*水平向左数*/
        for(j=x,k=y;j>=1;j--)
        {
            if(box[j][k]==flag)
            n1++;
            else 
            break ;
        }
        /*水平向右数*/
        for(j=x,k=y;j<=18;j++)
        {
            if(box[j][k]==flag)
            n2++;
            else 
            break ;
        }
        if(n1+n2-1>=5)
        {
            return(1);
            break ;
        }
        
        /*垂直向上数*/
        n1=0 ;
        n2=0 ;
        for(j=x,k=y;k>=1;k--)
        {
            if(box[j][k]==flag)
            n1++;
            else 
            break ;
        }
        /*垂直向下数*/
        for(j=x,k=y;k<=18;k++)
        {
            if(box[j][k]==flag)
            n2++;
            else 
            break ;
        }
        if(n1+n2-1>=5)
        {
            return(1);
            break ;
        }
        
        /*向左上方数*/
        n1=0 ;
        n2=0 ;
        for(j=x,k=y;j>=1,k>=1;j--,k--)
        {
            if(box[j][k]==flag)
            n1++;
            else 
            break ;
        }
        /*向右下方数*/
        for(j=x,k=y;j<=18,k<=18;j++,k++)
        {
            if(box[j][k]==flag)
            n2++;
            else 
            break ;
        }
        if(n1+n2-1>=5)
        {
            return(1);
            break ;
        }
        
        /*向右上方数*/
        n1=0 ;
        n2=0 ;
        for(j=x,k=y;j<=18,k>=1;j++,k--)
        {
            if(box[j][k]==flag)
            n1++;
            else 
            break ;
        }
        /*向左下方数*/
        for(j=x,k=y;j>=1,k<=18;j--,k++)
        {
            if(box[j][k]==flag)
            n2++;
            else 
            break ;
        }
        if(n1+n2-1>=5)
        {
            return(1);
            break ;
        }
        return(0);
        break ;
    }
}

void main()
{
    int gdriver=VGA,gmode=VGAHI;
    clrscr();
    attention();
    initgraph(&gdriver,&gmode,"c:\\tc");
    /* setwritemode(XOR_PUT);*/
    flag=1 ;
    draw_box();
    do 
    {
        step_x=0 ;
        step_y=0 ;
        /*draw_circle(step_x,step_y,8); */
        judgewho(step_x-1,step_y-1);
        do 
        {
            while(bioskey(1)==0);
            key=bioskey(0);
            judgekey();
        }
        while(key!=SPACE&&key!=ESC);
    }
    while(key!=ESC);
    closegraph();
}

