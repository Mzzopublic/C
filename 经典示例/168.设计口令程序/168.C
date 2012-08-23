#include <stdio.h>
#include <conio.h>
#include <dos.h>

void main(void)
{
	struct date today;
	struct time now;
	getdate(&today);  /*把系统当前日期存入today所指向的date结构中*/
	gettime(&now);    /*把系统当前时间存入now所指向的time结构中*/
	/*设定字符颜色和背景色*/
	textcolor(LIGHTGREEN);
	textbackground(MAGENTA);
	/*当输入口令不对时，反复进行以下循环*/
	do{
		clrscr();     /*调用清屏函数*/
		gotoxy(25,10);
		printf("Today's date is %d-%d-%d\n",today.da_year,today.da_mon,today.da_day);
		gotoxy(25,12);
		printf("Current time is %02d:%02d:%02d\n",now.ti_hour,now.ti_min,now.ti_sec);
		gotoxy(1,1);
		printf(" ====== Welcome to this password program! ======\n Please input ");
	}
	while (atoi((char *) getpass("password:")) != today.da_mon+now.ti_hour);
	/*如果输入正确，则显示正确信息并退出*/
	textcolor(WHITE);
	textbackground(BLACK);
	clrscr();
	gotoxy(1,1);
	printf(" Password Correct!!\n");
	printf(" Press any key to quit...\n");
	getchar();
}