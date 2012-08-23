/*
解救人质游戏
只要可以走到下面的小人那里就赢
*/
#define WIN if(x==60&&y==20) break; /*胜利条件*/
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4b00
#define RIGHT 0x4d00
#define MAXTIME 120 /*解救人质的最长时间*/
#include <conio.h>
#include <time.h>
long t;
int x,y;
void quitgame(int);
void initscreen();
void JudgeKey();
void littlewin(int);
void main()
{
	JudgeKey();	
}

void littlewin(int WinGam/*1--Win game, 0--Lost game */)
{
	window(20,9,60,15);
	textbackground(LIGHTGRAY);
	textcolor(RED);
	clrscr();
	gotoxy(1,6);
	cprintf("-----------------------------------------");
	gotoxy(1,1);
	if(WinGam)
	{
		cprintf("=============== Win Game ================");
		textcolor(MAGENTA);
		gotoxy(12,2);
		cprintf("Congratulations!");
		gotoxy(7,3);
		cprintf("You have Save the Hostage!");
		textcolor(LIGHTGREEN);
		gotoxy(8,4);
		cprintf("Press any key to quit...");
		textcolor(LIGHTRED);
		gotoxy(20,5);
		cprintf("%c",1);
	}
	else
	{
		cprintf("============== Lost Game ================");
		textcolor(MAGENTA);
		gotoxy(14,2);
		cprintf("Very Sorry!");
		gotoxy(2,3);
		cprintf("You have no time to Save the Hostage!");
		textcolor(LIGHTGREEN);
		gotoxy(6,4);
		cprintf("Press any key to restart...");
	}
	getch();
	
}
void initscreen()
{
	int i,j;
	char ch;
	window(1,1,80,25);
	textbackground(BLUE);
	textcolor(LIGHTGREEN); /*图象初始化*/
	clrscr();
	textcolor(MAGENTA);
	for(i=20;i<=60;i++)
	{
		for(j=4;j<=18;j+=2)
		{
			gotoxy(i,j);
			cprintf("%c",219);
		}
		gotoxy(i,21);
		cprintf("%c",219);
	}
	for(i=4;i<=21;i++)
	{
		gotoxy(19,i);
		cprintf("%c",219);
		gotoxy(61,i);
		cprintf("%c",219);
		gotoxy(62,i);
		cprintf("%c",219);
		gotoxy(18,i);
		cprintf("%c",219);
	}
	textcolor(YELLOW);
	gotoxy(20,5);
	cprintf("\1");
	textcolor(12);
	gotoxy(60,20);
	cprintf("\2");
	gotoxy(20,5);
	textcolor(WHITE);
	gotoxy(20,22);
	cprintf("Welcome to this little Saving Hostage Game!");
	gotoxy(16,23);
	cprintf("All you have to do is finding a way to the Red Person.");
	gotoxy(22,24);
	cprintf("Press any key to start, Q to quit...");
	ch=getch();
	if(ch=='Q'||ch=='q')
		quitgame(0);
	else
	{
		gotoxy(22,24);
		printf("                                    ");
	}
	textcolor(LIGHTGREEN);
	gotoxy(22,1);
	cprintf("%c(UP) %c(DOWN) %c(LEFT) %c(RIGHT) ESC(QUIT)\n",24,25,27,26);
	textcolor(YELLOW);
	gotoxy(23,2);
	cprintf("You have only %d seconds!",MAXTIME);
	gotoxy(50,2);
	cprintf("TIME:"); /*图象初始化结束*/
	x=20;
	y=5;
	t=0;
	return;	
}
void quitgame(int Conf/*1--Confirm, 0--no confirm*/)
{
	char ch;
	if(Conf)
	{
		window(20,9,60,15);
		textbackground(LIGHTGRAY);
		textcolor(RED);
		clrscr();
		gotoxy(1,6);
		cprintf("-----------------------------------------");
		gotoxy(1,1);
		cprintf("-------------- Exit Game ----------------");
		textcolor(MAGENTA);
		gotoxy(16,2);
		cprintf("Warning!");
		gotoxy(2,3);
		cprintf("Do you really want to quit this game?");
		textcolor(LIGHTGREEN);
		while(1)
		{
			gotoxy(15,4);
			cprintf("OK? [Y/N] ");
			ch=getch();
			if(ch=='Y'||ch=='y')
				break;
			else if(ch=='N'||ch=='n')
			{
				JudgeKey();
				return;
			}
		}
	}
	window(1,1,80,25);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	clrscr();
	exit(0);
}
void JudgeKey()
{
	int key,a,b,n=0;
	long m;
	initscreen();
	for(;;)
	{
		for(;!kbhit();) /*计时器*/
		{
			gotoxy(56,2);
			if(t%500==0)
				m=t/500;
			cprintf("%ld",m);
			t++;
			if(m==MAXTIME)
			{
				littlewin(0);
				JudgeKey();
			}
			delay(100);
		} /*计时器结束*/
		++n;
		a=x;
		b=y;
		key=bioskey(0);
		if(key==UP) /*判断按键*/
		{
			if(y==5)
			continue;
			gotoxy(x,--y);
			WIN;
		}
		else if(key==DOWN)
		{
			if(y==20)
				continue;
			else if(y==5&&x!=60)
				continue;
			else if(y==7&&x!=50)
				continue;
			else if(y==9&&x!=40)
				continue;
			else if(y==11&&x!=30)
				continue;
			else if(y==13&&x!=20)
				continue;
			else if(y==15&&x!=25)
				continue;
			else if(y==17&&x!=55)
				continue;
			gotoxy(x,++y);
			WIN;
		}
		else if(key==LEFT)
		{
			if(x==20)
			continue;
			gotoxy(--x,y);
			WIN;
		}
		else if(key==RIGHT)
		{
			if(x==60)
			continue;
			gotoxy(++x,y);
			WIN;
		}
		else if(key==0x11b)
			quitgame(1);
		else continue; /*判断结束*/
		cprintf("\1"); /*输出图形*/
		gotoxy(a,b);
		printf(" ");
		gotoxy(x,y);
		sound(300); /*输出声音*/
		delay(1000);
		nosound();
	}
	/*输出结果*/
	littlewin(1);
	quitgame(0);
} 
 
