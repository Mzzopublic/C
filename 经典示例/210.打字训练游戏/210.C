/*
练习训练游戏
*/
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <time.h>
#define MAXCHAR 100
#define ESC     0x011b  /* 退出程序键，调用quitgame()函数*/
#define F1	0x3b00	/* 查看帮助信息，调用Help()函数 */
#define F2	0x3c00	/* 查看关于...信息，调用About()函数 */
#define	F3	0x3d00	/* 以下功能键暂时保留，如果增加功能可使用 */
#define	F4	0x3e00	
#define	F5	0x3f00	
#define	F6	0x4000
#define	F7	0x4100
#define	F8	0x4200
#define	F9	0x4300
#define	F10	0x4400
#define KEY1    0x0231
#define KEY2    0x0332
#define KEY3    0x0433
#define KEY4    0x0534
#define KEY01   0x4f31
#define KEY02   0x5032
#define KEY03   0x5133
#define KEY04   0x4b34

char string[MAXCHAR+1];
int LittleWin(int WinType/*=1, quitgame; =2, Help;=3, About; =4, others;*/);/*when WinType=1, return 0, quit, return 1, not quit*/
void quitgame();
void Welcome();
void drawframe();
void Frame();
void GetCharacter();
void Typing();

int LittleWin(int WinType/*=1, quitgame; =2, Help;=3, About; =4, others;*/)
/*when WinType=1, return 0, quit, return 1, not quit*/
{
	int i;
	char ch;
	window(18,6,62,20);
	textbackground(LIGHTGRAY);
	textcolor(BLACK);
	clrscr();
	gotoxy(1,1);
	cprintf("%c",201);
	for(i=0;i<43;i++)
		cprintf("%c",205);
	cprintf("%c",187);
	for(i=0;i<13;i++)
	{
		gotoxy(1,i+2);
		cprintf("%c",186);
		gotoxy(45,i+2);
		cprintf("%c",186);
	}
	gotoxy(1,14);
	cprintf("%c",200);
	for(i=0;i<43;i++)
		cprintf("%c",205);
	cprintf("%c",188);
	gotoxy(20,1);
	switch(WinType)
	{
		case 1:
			cprintf(" Exit ");
			textcolor(LIGHTRED);
			gotoxy(18,3);
			cprintf("Warning!");
			textcolor(LIGHTBLUE);
			gotoxy(5,5);
			cprintf("This operation will exit the program!");
			gotoxy(10,7);
			cprintf("Do you really want to quit?");
			textcolor(LIGHTGREEN);
			gotoxy(18,9);
			cprintf("OK? [Y/N]");
			/*window(18,20,62,20);
			textbackground(LIGHTBLUE);
			textcolor(YELLOW);
			clrscr();*/
			window(19,18,61,18);
			textbackground(LIGHTBLUE);
			textcolor(WHITE);
			clrscr();
			gotoxy(5,1);
			cprintf("Press Y to quit, press N to return.");
			while(1)
			{
				ch=getch();
				if(ch=='Y'||ch=='y')
					return 0;
				else if(ch=='N'||ch=='n')
					return 1;
			}
		      	break;
		case 2:
			cprintf(" Help ");
		      	break;
		case 3:
		      	cprintf(" About ");
		      	break;
		case 4:
		      	cprintf(" Info ");
		      	break;
		default:
			break;

	}



}

void quitgame()
{
	if(LittleWin(1))
	{
		Frame();
		return;
	}
	window(1,1,80,25);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	clrscr();
	exit(0);
}
void Welcome()
{
	int driver=VGA,mode=VGAHI;         /*定义变量*/
	int x1=20,y1=20,r=10,num,i;
	int x2=20,y2=450;
	int color1=10,color2=10;          /*在此设置颜色，一改全改*/
	static char ch1[28][2]={"W","e","l","c","o","m","e"," ","t","o"," ","t","h","e"," ","T","y","p","i","n","g"," ","W","o","r","l","d","!"};
	initgraph(&driver,&mode,"C:\\tc ");    /*初始化图形模式*/
	setbkcolor(LIGHTBLUE);
	setcolor(color1);                   /*步骤一、设置当前颜色用于绘图*/
	for(num=0;num<30;num++)
	{
		circle(x1,y1,r);              /*步骤二、使用当前颜色绘制一个闭合图形*/
		setfillstyle(num%12,color1);     /*步骤三、设置填充方式*/
		floodfill(x1,y1,color1);       /*步骤四、对闭合图形进行填充*/
		x1+=20;
		sound(300); /*输出声音*/
		delay(3000);
		nosound();
	}
	setcolor(color2);
	for(num=0;num<30;num++)
	{
	       	circle(x2,y2,r);
	       	setfillstyle(num%12,color2);
	       	floodfill(x2,y2,color2);
	       	x2+=20;
	       	sound(300); /*输出声音*/
		delay(3000);
		nosound();
	}

	settextstyle(0,0,2);
	setcolor(LIGHTRED);
	sound(300); /*输出声音*/
	delay(3000);
	nosound();
	outtextxy(180,90,"^o^ Hello! ^o^");
	sound(300); /*输出声音*/
	delay(3000);
	nosound();
	setcolor(YELLOW);
	gotoxy(50,150);
	for(i=0;i<28;i++)
	{
		outtextxy(50+i*16,150,ch1[i]);
		sound(300); /*输出声音*/
		delay(3000);
		nosound();
	}
	/*outtextxy(50,150,"Welcome to the Typing World!");*/
	setcolor(WHITE);
	outtextxy(50,200,"This is a little");
	sound(300); /*输出声音*/
	delay(6000);
	nosound();
	outtextxy(100,250,"Typing Training");
	sound(300); /*输出声音*/
	delay(6000);
	nosound();
	outtextxy(150,300,"Software ... ^_^");
	sound(300); /*输出声音*/
	delay(6000);
	nosound();
	setcolor(LIGHTMAGENTA);
	outtextxy(100,350,"Ver. 2004-04-30");
	sound(300); /*输出声音*/
	delay(6000);
	nosound();
	setcolor(LIGHTGREEN);
	outtextxy(100,400,"Press any key to start...");
	sound(300); /*输出声音*/
	delay(6000);
	nosound();
	getch();
	closegraph();                        /*关闭图形*/

}
void drawframe()
{
	int i;
	window(1,1,80,1);
	textbackground(LIGHTGRAY);
	textcolor(BLACK);
	clrscr();
	gotoxy(7,1);
	cprintf("File   Edit   Run   Compile   Project   Options   Debug   Help");
	textcolor(RED);
	gotoxy(7,1);
	cprintf("F");
	gotoxy(14,1);
	cprintf("E");
	gotoxy(21,1);
	cprintf("R");
	gotoxy(27,1);
	cprintf("C");
	gotoxy(37,1);
	cprintf("P");
	gotoxy(47,1);
	cprintf("O");
	gotoxy(57,1);
	cprintf("D");
	gotoxy(65,1);
	cprintf("H");
	window(1,25,80,25);
	textbackground(LIGHTGRAY);
	textcolor(BLACK);
	clrscr();
	gotoxy(7,1);
	printf("F1-Help   F2-About   F3-Open  F4-Restart   Ctrl+F9-Run   ESC-Quit");
	textcolor(RED);
	gotoxy(7,1);
	cprintf("F1");
	gotoxy(17,1);
	cprintf("F2");
	gotoxy(28,1);
	cprintf("F3");
	gotoxy(37,1);
	cprintf("F4");
	gotoxy(50,1);
	cprintf("Ctrl+F9");
	gotoxy(64,1);
	cprintf("ESC");
	window(1,2,80,24);
	textbackground(LIGHTBLUE);
	textcolor(LIGHTGRAY);
	clrscr();
	gotoxy(1,2);
	cprintf("%c",213);
	for(i=0;i<78;i++)
		cprintf("%c",205);
	cprintf("%c",184);
	for(i=0;i<21;i++)
	{
		gotoxy(1,i+3);
		cprintf("%c",179);
		gotoxy(80,i+3);
		cprintf("%c",179);
	}
	gotoxy(1,22);
	cprintf("%c",192);
	for(i=0;i<78;i++)
		cprintf("%c",196);
	cprintf("%c",217);
	textcolor(WHITE);
	gotoxy(37,1);
	cprintf(" Type ");
}
void Frame()                              /*设置菜单函数*/
{
	drawframe();
	window(2,3,79,22);
	textbackground(LIGHTBLUE);
	textcolor(YELLOW);
	clrscr();
	gotoxy(25,2);
        cprintf("The Typing World Menu\n");
        gotoxy(12,4);
	cprintf("1:Practice Only ENGLISH Characters.");
        gotoxy(12,6);
	cprintf("2:Practice Other Charcters.");
        gotoxy(12,8);
	cprintf("3:Practice All Charcters.");
        gotoxy(12,10);
	cprintf("4:Quit at once!");
	GetCharacter();

}
void GetCharacter(void)         /*设置得到字符函数*/
{
	void Typing();          /*声明typing()函数*/
	FILE *in;
	int i,t,choice;
	char ch;
	t=abs(time(0))%700;   /*获取随机数来指定下面指针的位*/
	gotoxy(12,12);
	cprintf("Please input your choice: ");
	while(1)
	{
		gotoxy(38,12);
		choice=bioskey(0);
		if(choice==ESC||choice==KEY4||choice==KEY04)
		{
			quitgame();
			break;
		}
		else if(choice==KEY1||choice==KEY01)
		{
			in=fopen("english.dat","r");
		       	break;
		}
		else if(choice==KEY2||choice==KEY02)
		{
			in=fopen("others.dat","r");
		       	break;
		}
		else if(choice==KEY3||choice==KEY03)
		{
			in=fopen("typeall.dat","r");
		       	break;
		}
	}
	clrscr();         /*清屏*/
	fseek(in,t*1l,0);
	fgets(string,MAXCHAR+1,in);
	textcolor(WHITE);
	gotoxy(1,2);
	cprintf("******************************************************************************");
	textcolor(YELLOW);
	for(i=0;i<MAXCHAR;i++)
	cprintf("%c",string[i]);
	gotoxy(1,5);
	textcolor(WHITE);
	cprintf("******************************************************************************");
	fclose(in);
	gotoxy(1,6);
	cprintf("Let's begin typing,OK?[Y/N]");
	while(1)
	{
		gotoxy(28,6);
		ch=getch();
		if(ch=='n'||ch=='N')             /*判断是否练习打字*/
		{
		      quitgame();
		      break;
		}
		else if(ch=='y'||ch=='Y')
		{
		      Typing();
		      break;
		}
	}
}
void Typing(void)                               /*设置打字（包括计算其他结果）函数*/
{
	int i,j,Right_char=0,Wrong_char=0,Sum_char=0;
	float Speed,Timeused,Right_rate;
	char absorb_char,ch_1,ch_2,ch_3;
	time_t star,stop;                              /*定义time变量，获取系统时间并显示在屏幕上*/
	time(&star);
	textcolor(WHITE);
	gotoxy(1,6);
	cprintf("time begin:%s",ctime(&star));
	gotoxy(1,7);
	i=0;
	textcolor(YELLOW);
	absorb_char=getch();             /*接受键盘输入的字符并在下面的while语句判断正错和计算结果*/
	while(i<MAXCHAR)
	{
		if(absorb_char=='\n')
			continue;
		else
			cprintf("%c",absorb_char);
	       if(absorb_char==string[i])
	            {
	               Right_char++;
	               Sum_char++;
	            }
	       else
	            {
	              Wrong_char++;
	              Sum_char++;
	            }
	       i++;
	       absorb_char=getch();
	}
	time(&stop);
	Timeused=difftime(stop,star);        /*利用difftime()函数输出所用时间*/
	Right_rate=(float)Right_char/(float)Sum_char*100;
	Speed=(Sum_char/Timeused)*60;
	textcolor(WHITE);
	gotoxy(1,11);
	cprintf("time end:%s",ctime(&stop));    /*下面显示分数*/
	textcolor(LIGHTGREEN);
	gotoxy(1,12);
	cprintf("********************Your Score!*************************");
	gotoxy(7,13);
	textcolor(WHITE);
	cprintf("    1: TOTAL TIME USED:%.3f",Timeused);
	gotoxy(7,14);
	cprintf("    2: YOU HAVE TYPED:%d",Sum_char);
	gotoxy(7,15);
	cprintf("    3: Typing Right_Characters are:%d",Right_char);
	gotoxy(7,16);
	cprintf("    4: Typing Wrong_Characters are:%d\n",Wrong_char);
	gotoxy(7,17);
	cprintf("    5: YOUR TYPING SPEED IS :%.2f\\min\n",Speed);
	gotoxy(7,18);
	cprintf("    6: YOUR TYPING RIGHT_RATE IS :%.2f%%\n",Right_rate);
	textcolor(LIGHTGREEN);
	gotoxy(1,19);
	cprintf("********************Your Score!*************************");
	gotoxy(1,20);
	textcolor(WHITE);
  	if(Speed<=50||Right_rate<=80)           /*询问用户是否重来一遍*/
	{
		cprintf("Not Very Good!   Try it Again,OK?[Y/N]");
 		while(1)
	        {
			gotoxy(39,20);
		        ch_2=getch();
		        if(ch_2=='n'||ch_2=='N')
		        {
		        	quitgame();
		        	break;
		        }
		        else if(ch_2=='y'||ch_2=='Y')
		        {
		        	Frame();
		        	break;
		        }
		}
        }
  	else
  	{
		cprintf("Well Done!!    One More Time?[Y/N]");    /*询问用户是否重来一遍*/
  		while(1)
        	{
			gotoxy(35,20);
	        	ch_3=getch();
	        	if(ch_3=='n'||ch_3=='N')
	        	{
	        		quitgame();
	        		break;
	        	}
	        	else if(ch_3=='y'||ch_3=='Y')
	        	{
		        	Frame();
		        	break;
	         	}
        	}
        }
}
main(void)                                       /*主函数包含两个要调用的函数*/
{
	/*Welcome();*/
	Frame();
}
