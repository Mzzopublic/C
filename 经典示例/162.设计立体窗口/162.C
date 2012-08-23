#include <conio.h>

void window_3d( int, int, int, int, int, int );

int main(void)
{
	directvideo = 0;
	textmode(3);
	textbackground( WHITE );
	textcolor( BLACK );
	clrscr();
	window_3d( 10,4,50,12, BLUE, WHITE );
	gotoxy( 17,6);
	cputs("The first window");
	window_3d(20,10,60,18,RED, WHITE );
	gotoxy(17,6);
	cputs("The second window");
	window_3d(30,16,70,24,GREEN, WHITE );
	gotoxy(17,6);
	cputs("The third window");
	getch();
	return 0;
}

void window_3d( int x1, int y1, int x2, int y2, int bk_color, int fo_color)
/*立体投影窗口的显示
  x1,y1, x2, y2是窗口的大小
  bk_color是背景色
  fo_color是前景色，即文本的颜色
*/
{
	textbackground(BLACK);
	window(x1, y1,x2, y2); /*画背景窗口*/
	clrscr();
	textbackground(bk_color); /*设置背景色*/
	textcolor(fo_color); /*设置前景色*/
	window(x1-2, y1-1, x2-2, y2-1); /*画实际的窗口*/
	clrscr();
}