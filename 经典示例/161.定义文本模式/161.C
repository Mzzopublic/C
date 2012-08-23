#include <conio.h>
#include <dos.h>
#include <stdlib.h>

#define VIDEO_BIOS     0x10 /*int 10h是bios中对视频函数的调用*/

int	setfont8x8(int); /*设置不同的显示模式*/
void	setstdfont(int); /*恢复成系统默认的显示模式*/

void 	main(void)
{
	int	lines,i;

	lines = setfont8x8(C80); /*设置8X8点阵，每行80字符的显示模式，并获取可显示的最大行数*/
	textattr(WHITE); /*textattr()函数设置字符模式下窗口的前景色和背景色*/
	clrscr(); /*清除屏幕*/
	if (lines < 43) {
		textattr(LIGHTRED);
		cprintf("\n\r Drivers of EGA or VGA not found...\n\r"); /*cprintf()的功能是向窗口输出文本*/
		exit(1);
	}
	window(20,15,70,35); /*画字符模式窗口，4个参数依次为左，上，右，下的位置*/
	textattr((RED<<4)+WHITE); /*把窗口设置成前景色为白色，背景色为红色*/
	clrscr();
	for (i=1;i<=lines;i++) { /*循环输出最多能输出的行数*/
		cprintf("\n\r No. %d ",i);
		delay(200); /*每输出一行，等待200ms*/
	}
	getch(); /*等待用户输入一个字符*/
	window(1,1,80,lines); /*重新设置窗口*/
	textattr(LIGHTGRAY<<4); /*将窗口背景色设置为灰色*/
	clrscr();
	cprintf("\n\r Full screen 80x%d display mode.\n\r",lines);
	getch(); 

	lines = setfont8x8(C40); /*将窗口设置为每行40个字符的显示模式*/
	textattr((BLUE<<4)+LIGHTGREEN); /*设置窗口，前景亮绿色，背景蓝色*/
	clrscr();
	cprintf("\n\r Can be also set as 40x%d mode.\n\r",lines);
	getch();

	setstdfont(C80); /*重新设置成标准的每行80字符的显示模式*/
	clrscr();
	cprintf("\n\r Back to normal mode...\n\r");
	printf(" Press any key to quit...");
	getch();
	exit(0);
}

int setfont8x8(mode)
int mode;
{
int maxlines,maxcol;
char vtype,displaytype;

	textmode(mode);  /*设置文本格式，mode含义为每行可以显示的字符数*/

	_AH = 0x0F;   /*int 10h的 0fh功能为获取当前的显示模式，执行后，每行可显示字符数保存在ah中*/
	geninterrupt(VIDEO_BIOS); /*geninterrupt()函数执行一个软中断，调用int 10h*/
	maxcol = _AH; /*获取每行可以显示的字符数*/

	_AX = 0x1A00;  /*int 10h的 1ah功能为获取当前的显示代码*/
	geninterrupt(VIDEO_BIOS);
	displaytype = _AL; /*int 10h返回后，al中为显示类型*/
	vtype       = _BL; /*bl中为显示器的类型*/

	if (displaytype == 0x1A) { /*可以直接获取最大行数*/
		switch (vtype) {
			case 4:
			case 5:  maxlines = 43;
						break;
			case 7:
			case 8:
			case 11:
			case 12: maxlines = 50;
						break;
			default: maxlines = 25;
						break;
		}
	}
	else { /*无法读取显示器的类型 */
		_AH = 0x12;  /*int 10h的 12h功能为选择显示器程序*/
		_BL = 0x10;
		geninterrupt(VIDEO_BIOS);

		if (_BL == 0x10)
			maxlines = 25;
		else
			maxlines = 43;
	}

	if (maxlines > 25) { /*如果可以设置更多的行*/
		_AX = 0x1112; /*以下的部分都是int 10h的11h号功能调用，作用是生成相应的显示字符*/
		_BL = 0;
		geninterrupt(VIDEO_BIOS);

		_AX = 0x1103;
		_BL = 0;
		geninterrupt(VIDEO_BIOS);
	}

	*((char *) &directvideo - 8) = maxlines; /*设置显示行数*/
	window(1,1,maxcol,maxlines); /*画出相应大小的窗口*/
	return(maxlines); /*返回可以设置的最大行数*/
}

void setstdfont(mode)
int mode;
{
	if (mode != LASTMODE)
		_AL = mode;
	else {
		_AH = 0x0F; /*获取当前显示模式*/
		geninterrupt(VIDEO_BIOS);
		mode = _AL;
	}

	_AH = 0; /*恢复成系统标准模式*/
	geninterrupt(VIDEO_BIOS);

	*((char *) &directvideo - 8) = 25; /*行数设置成25行*/
	textmode(mode); 
}