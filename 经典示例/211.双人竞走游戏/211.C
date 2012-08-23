/*
路线竞走游戏
*/
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <graphics.h>
#include <bios.h>

/* 功能键设置 */

#define	ESC	0x001b	/* 退出游戏 */
#define F1	0x3b00	/* 查看帮助信息，调用HelpMassage()函数 */
#define F2	0x3c00	/* 设定游戏速度等级，调用Set_Level()函数 */
#define	F3	0x3d00	/* 设定游戏者1的颜色，调用Set_Color()函数 */
#define	F4	0x3e00	/* 设定游戏者2的颜色，调用Set_Color()函数 */
#define	F5	0x3f00	/* 以下功能键暂时保留，如果增加功能可使用 */
#define	F6	0x4000
#define	F7	0x4100
#define	F8	0x4200
#define	F9	0x4300
#define	F10	0x4400

/* Player1 hot key 游戏者1热键上下左右分别用RFDG控制 */
#define RIGHT1	71
#define LEFT1	68
#define UP1	82
#define DOWN1	70

/* Player2 hot key 游戏者2热键，四个光标键控制方向 */
#define	RIGHT	0x4d00
#define	LEFT	0x4b00
#define	UP	0x4800
#define	DOWN	0x5000


/* 定义画方框的坐标X，Y，以及颜色Color，Draw为是(1)否(0)已走过 */
struct information
		{
			int color,draw;
			int x,y;
		};
typedef struct information INFOR;

/* 记录游戏者在把在位置，X与Y其实就是全局数组coordinate[][]的两个下标 */
struct playerxy
		{
			int x,y;
		};
typedef struct playerxy CurrentCoor;

/* 此全局数组是记录画每个框的坐标及颜色以及是否已走过 */
INFOR coordinate[80][60];

time_t Timeout=1;  /* 限制游戏的快慢，可用Set_Level()设定 */

int size=8,maxX=79,maxY=56; /* size定义画框的大小，单位为像素,maxX,maxY为数组coordinate下标的最大值 */

/* BackColor为游戏背景色,Player1Color与Player2Color为游戏者默认颜色,可调用Set_Color()函数设定 */
int BackColor=LIGHTBLUE,Player1Color=WHITE,Player2Color=LIGHTRED; 



/* 初始化图形模式 */
void InitialGraphics(void)
{
	int graphdriver=VGA,graphmode=VGAHI;
	int errorcode;
	initgraph(&graphdriver,&graphmode,"");
	errorcode=graphresult();
	if(errorcode!=grOk)
	{
		printf("Graphics error:%s\n",grapherrormsg(errorcode));
		GoodBye();
	}
}

/* 退出游戏显示提示信息，只有不能初始化图形界面才会调用此函数 */
int GoodBye(void)
{
	printf("Thank you for your playing!\n");
	printf("Press any key to quit...\n");
	GetKey();
	exit(0);
}

/* 初始化游戏界面 */
void InitFace(void)
{
	setbkcolor(BackColor);
	cleardevice();
	setcolor(WHITE);
	rectangle(0,0,639,479);
	setcolor(LIGHTGREEN);
	rectangle(3,3,636,452);
	setcolor(LIGHTRED);
	rectangle(3,455,250,476);
	rectangle(253,455,636,476);

}

/* 初始化全局数组coordinate[][] */
void InitCoordinate(void)
{
	int x,y;
	for(x=0;x<maxX;x++)
		for(y=0;y<maxY;y++)
		{
			coordinate[x][y].color=BackColor;
			coordinate[x][y].draw=0;
			coordinate[x][y].x=size*x+4;
			coordinate[x][y].y=size*y+4;
		}
}

/* 初始化游戏者开始位置 */
void InitPlayerPlace(CurrentCoor *player1,CurrentCoor *player2)
{
	player1->x=maxX/3;
	player2->x=maxX/3*2;
	player1->y=maxY/2;
	player2->y=maxY/2;
}

/* 画框函数，player为画框位置，who为哪一个游戏者 */
void Drawbar(CurrentCoor player,int who)
{
	int x,y;	/* 根据游戏者所在coordinate的下标位置取出在屏幕上的位置 */
	x=coordinate[player.x][player.y].x;
	y=coordinate[player.x][player.y].y;
	if(who==1)
		{	setfillstyle(1,Player1Color);
			coordinate[player.x][player.y].color=Player1Color;
		}
	else
		{	setfillstyle(1,Player2Color);
			coordinate[player.x][player.y].color=Player2Color;
		}
	bar(x,y,x+7,y+7);
	coordinate[player.x][player.y].draw=1;
}

/* 帮助信息，按F1调用此函数 */
void HelpMassage(void)
{
	setfillstyle(1,WHITE);
	bar(150,160,500,340);
	setcolor(LIGHTRED);
	rectangle(152,162,498,338);
	setcolor(BLACK);
	rectangle(154,164,496,180);
	rectangle(154,182,496,336);
	setcolor(GREEN);
	outtextxy(250,170,"Race Walking V1.0");
	setcolor(BLACK);
	outtextxy(160,190,"Player1 control key:");
	outtextxy(160,200,"Player2 control key:");
	outtextxy(160,220,"Help:   Level:   Exit:");
	outtextxy(160,230,"Player color:");
	setcolor(LIGHTMAGENTA);
	outtextxy(220,260,"Press any key to start...");
	setcolor(RED);
	outtextxy(330,190,"r,f,d,g");
	outtextxy(330,200,"up,down,left,right");
	outtextxy(160,220,"     F1       F2      Esc");
	outtextxy(160,230,"             F3/F4");

	setcolor(BLUE);
	outtextxy(175,290,"If you have any question or modify these");
	outtextxy(160,300,"code,Please send email to me. Thank you");
	outtextxy(160,310,"play this game!");
	setcolor(BLACK);
	outtextxy(410,320,"30/04/2004");
}

/* 获取按键，如果低八位非0则为ASCII码，如为0则为控制键 */
int GetKey(void)
{
	int key;
	key=bioskey(0);
	if(key<<8)
	{
		key=key&0x00ff;
		if(isalpha(key)) 	/* 如果为字母则转换为大写 */
			key=toupper(key);
	}
	return key;
}

/* 初始化开始方向，只有两个游戏者都按下了方向键才开始游戏 */
void Initfx(int *player1fx,int *player2fx)
{
	int key;
	while(!(*player1fx) || !(*player2fx))
	{
		key=GetKey();
		if(key==RIGHT1 || key==LEFT1 || key==UP1 || key==DOWN1)
			(*player1fx)=key;
		else if(key==RIGHT || key==LEFT || key==UP ||key==DOWN)
			(*player2fx)=key;
	}
}

/* 最主要函数，控制画框方向以及判断是否已死 */
int ManageMove(CurrentCoor player1,CurrentCoor player2)
{
	int player1fx=0,player2fx=0; 	/* 记录游戏者的方向 */
	int die=0,die1=0,die2=0;	/* 记录游戏者是否已死 */
	int key;
	int currentfx1,currentfx2;  /* 记录游戏者所按的方向键，它需与player1fx与player2fx比较,确定是否转向 */
	clock_t current_time,front_time; /* 用于控制游戏速度的变量 */
	Initfx(&player1fx,&player2fx);
	currentfx1=player1fx;
	currentfx2=player2fx;
	front_time=clock();
	while(die==0)
	{
		if(bioskey(1))
		{
			key=GetKey();
			if(key==DOWN1 || key==UP1 || key==LEFT1 || key==RIGHT1)
				currentfx1=key;
			else if(key==DOWN || key==UP || key==LEFT || key==RIGHT)
				currentfx2=key;
		}
		current_time=clock();
		if( (current_time - front_time) > Timeout)	/* 如果超过游戏间隔则开始画 */
		{
			front_time=current_time;
			if(player1fx==RIGHT1 || player1fx==LEFT1)	/* 如果现在方向为左或右则只有按下上下键才改变方向，反之相同 */
			{
				if(currentfx1==UP1 || currentfx1==DOWN1)
				{
					if(currentfx1==UP1)
						player1.y-=1;
					else
						player1.y+=1;
					player1fx=currentfx1;
				}
				else
				{
					if(player1fx==RIGHT1)
						player1.x+=1;
					else
						player1.x-=1;
				}
			}
			else if(player1fx==UP1 || player1fx==DOWN1)
			{
				if(currentfx1==RIGHT1 || currentfx1==LEFT1)
				{
					if(currentfx1==RIGHT1)
						player1.x+=1;
					else
						player1.x-=1;
					player1fx=currentfx1;
				}
				else
				{
					if(player1fx==UP1)
						player1.y-=1;
					else
						player1.y+=1;
				}
			}
			if(player2fx==RIGHT || player2fx==LEFT)
			{
				if(currentfx2==UP || currentfx2==DOWN)
				{
					if(currentfx2==UP)
						player2.y-=1;
					else
						player2.y+=1;
					player2fx=currentfx2;
				}
				else
				{
					if(player2fx==RIGHT)
						player2.x+=1;
					else

						player2.x-=1;
				}
			}
			else if(player2fx==UP || player2fx==DOWN)
			{
				if(currentfx2==RIGHT || currentfx2==LEFT)
				{
					if(currentfx2==RIGHT)
						player2.x+=1;
					else
						player2.x-=1;
					player2fx=currentfx2;
				}
				else
				{
					if(player2fx==UP)
						player2.y-=1;
					else
						player2.y+=1;
				}
			}

			if(player1.x<0 || player1.x>=maxX ||player1.y<0 || player1.y>=maxY || coordinate[player1.x][player1.y].draw==1) /* 判断是否到边界或是已走路，如果是则退出，否则画框 */
			{	die1=1;	die=1;	}
			else
				Drawbar(player1,1);

			if(player2.x<0 || player2.x>=maxX || player2.y<0 || player2.y>=maxY || coordinate[player2.x][player2.y].draw==1)
			{	die2=1,	die=2;	}
			else
				Drawbar(player2,2);

		}
	}
	if(die1 && die2)
		die=3;
	return die;
}

/* 设定游戏等级函数 */
void Set_Level(void)
{
	int key,pass=0,i=0;
	clock_t front_time,current_time;
	front_time=clock();
	setfillstyle(1,WHITE);
	bar(180,150,460,300);
	setcolor(LIGHTRED);
	rectangle(182,152,458,298);
	setcolor(BLACK);
	rectangle(184,154,456,174);
	rectangle(184,176,456,296);
	setcolor(RED);
	outtextxy(260,160,"Level Setting");
	setcolor(BLACK);
	outtextxy(190,200,"Here have three level:");
	outtextxy(190,260,"Please input a number[0,1,2]:");
	setcolor(BLUE);
	outtextxy(200,220,"0: fast");
	outtextxy(200,230,"1: normal");
	outtextxy(200,240,"2: slow");

	while(pass==0)
	{
		if(bioskey(1))
		{
			key=GetKey();
			if(key=='0' || key=='1' || key=='2') /* 通过设置全局变量Timeout来确定游戏速度 */
			{
				pass=1;
				if(key=='0')	Timeout=0;
				else if(key=='1')	Timeout=1;
				else	Timeout=2;
			}
			else
			{
				setcolor(LIGHTRED);
				outtextxy(190,280,"Please input 0,1,2!");
			}
		}
		else
		{
			current_time=clock();
			if(current_time-front_time>6) /* 光标闪动效果，不知谁有更有效的方法？ */
			{
				front_time=current_time;
				i=(i+1)%2;
				if(i==0) setcolor(BLACK);
				else     setcolor(WHITE);
				outtextxy(420,260,"_");
			}
		}
	}
}

/* 设置游戏者颜色，其实就是设置全局变量Player2Color与Player1Color，根据who来判断哪一个游戏者 */
void Set_Color(int who)
{
	int pass=0,color,key,i=0;
	int x,y,count;
	clock_t front_time,current_time;
	setfillstyle(1,WHITE);
	bar(150,160,500,340);
	setcolor(LIGHTRED);
	rectangle(152,162,498,338);
	setcolor(BLACK);
	rectangle(154,164,496,180);
	rectangle(154,182,496,336);
	setcolor(LIGHTRED);
	if(who==1)
		outtextxy(245,170,"Player1");
	else if(who==2)
		outtextxy(245,170,"Player2");

	setcolor(GREEN);
	outtextxy(245,170,"        Color Setting");
	setcolor(BLACK);
	outtextxy(160,190,"You can select underside color:");
	outtextxy(160,300,"Please input color number[1-f]:");

	for(count=1;count<16;count++)
	{
		x=189+count*16;
		y=220;
		setfillstyle(1,count);
		bar(x,y,x+14,y+40);
		rectangle(x,y,x+14,y+40);
	}
	outtextxy(210,265,"1 2 3 4 5 6 7 8 9 A B C D E F");

	front_time=clock();
	while(pass==0)
	{
		if(bioskey(1))
		{
			key=GetKey();
			if( key>='1'&& key<='9')
			{
				pass=1;
				color=key-'0';
			}
			else if( key>='A' && key<='F')
			{
				pass=1;
				color=key-55; /* key-'A'+10*/
			}
			else
			{
				setcolor(LIGHTRED);
				outtextxy(160,320,"Please input 1 to 9,or a to f!");
			}
		}
		else
		{
			current_time=clock();
			if(current_time-front_time>6)
			{
				front_time=current_time;
				i=(i+1)%2;
				if(i==0) setcolor(BLACK);
				else     setcolor(WHITE);
				outtextxy(410,300,"_");
			}
		}
	}
	if(who==1)
		Player1Color=color;
	else if(who==2)
		Player2Color=color;
}

/* 确认退出函数,返回1（退出）与0（继续游戏） */
int Exit_Game(void)
{
	int Quit=-1;
	int key,i=0;
	clock_t front_time,current_time;
	front_time=clock();
	setfillstyle(1,WHITE);
	bar(180,150,460,300);
	setcolor(LIGHTRED);
	rectangle(182,152,458,298);
	setcolor(BLACK);
	rectangle(184,154,456,174);
	rectangle(184,176,456,296);
	setcolor(LIGHTRED);
	outtextxy(280,160,"Exit Game");
	setcolor(BLACK);
	outtextxy(200,220,"^_^ Thanks play this game! ^_^");
	setcolor(LIGHTRED);
	outtextxy(200,280,"Really quit game(Y/N)?:");

	while(Quit==-1)
	{
		if(bioskey(1))
		{
			key=GetKey();
			if(key=='Y')
				Quit=1;
			else if(key=='N')
				Quit=0;
		}
		else
		{
			current_time=clock();
			if(current_time-front_time>6)
			{
				front_time=current_time;
				i=(i+1)%2;
				if(i==0) setcolor(BLACK);
				else     setcolor(WHITE);
				outtextxy(385,280,"_");
			}
		}
	}
	return Quit;
}

/* 游戏控制函数，完成一系列初始化操作，最后调用ManageMove函数，完后便显示谁胜及一些快捷键 */
void GameManage(void)
{
	int Quit=0,key,die,i=1;
	int start_game;
	CurrentCoor player1,player2;	/* 记录游戏者下标位置 */
	clock_t front_time,current_time;
	InitFace();
	HelpMassage();
	GetKey();
	while(Quit==0)
	{
		die=0;
		InitFace();
		InitCoordinate();
		InitPlayerPlace(&player1,&player2);
		Drawbar(player1,1);
		Drawbar(player2,2);
		while(die==0)
		{
			 die=ManageMove(player1,player2);
		}
		setcolor(WHITE);
		if(die==1)
			outtextxy(7,457,"Player2 Win!");
		else if(die==2)
			outtextxy(7,457,"Player1 Win!");
		else
			outtextxy(7,457,"Player1 as same as Player2!");

		front_time=current_time=clock();
		while(current_time-front_time<20)
		{
			current_time=clock();
		}

		setcolor(WHITE);
		outtextxy(257,457,"F1:      F2:       F3:");
		outtextxy(257,467,"Esc:     F4:");
		setcolor(YELLOW);
		outtextxy(257,457,"    Help    Level     Player1 Color");
		outtextxy(257,467,"    Exit    Player2 Color");

		front_time=clock();

		start_game=0;
		while(start_game==0)
		{
			while(!bioskey(1))
			{
				current_time=clock();
				if(current_time-front_time>8)
				{
					front_time=current_time;
					i=(i+1)%2;
					if(i==0)	setcolor(LIGHTRED);
					else		setcolor(BackColor);
					outtextxy(7,467,"Press F5 to continue...");
				}
			}
			key=GetKey();
			if(key==F1)
			{	HelpMassage();
				GetKey();
			}
			else if(key==F2)	Set_Level();
			else if(key==F3)	Set_Color(1);	/* set player1 color */
			else if(key==F4)	Set_Color(2);	/* set player2 color */
			else if(key==F5)	start_game=1;
			else if(key==ESC)
			{		Quit=Exit_Game();
					if(Quit==1)
						start_game=1;
			}
		}
	}
}

/* 主函数 */
void main(void)
{
	InitialGraphics();
	GameManage();
	closegraph();
}
