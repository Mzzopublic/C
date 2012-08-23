#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <bios.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define BORDER  1
#define ESC 27
#define REV_VID 0x1e
#define NORM_VID 0x40

char *main_menu[] = 
{
	"F --- 文件管理",
	"D --- 磁盘管理",
	"H --- 打印服务",
	"X --- 退出菜单",
};

char* sub_menu1[] = 
{
	"S --- 文件显示",
	"P --- 文件打印",
	"C --- 文件拷贝",
	"D --- 文件删除",
	"X --- 返回上级",
};

char* sub_menu2[] = 
{
	"B --- BOOT区备份",
	"R --- BOOT区恢复",
	"X --- 返回上级",
};

char* sub_menu3[] =
{
	"O --- 打印图形",
	"F --- 打印文本",
	"X --- 返回上级",
};

void goto_xy(int x, int y) /*将光标移动到指定位置*/
{
	union REGS regs;
	regs.h.ah = 2;
	regs.h.dl = x;
	regs.h.dh = y;
	regs.h.bh = 0;
	int86( 0x10, &regs, &regs );
}

void save_video( int startx, int endx, int starty, int endy, unsigned int* buf_ptr )
/*将指定区域的视频模式存储到指定的内存中去*/
{
	union REGS r;
	register int i, j;
	for( i = starty; i < endy; i++)
		for( j = startx; j < endx; j++ )
		{
			goto_xy(j, i);
			r.h.ah = 8;
			r.h.bh = 0;
			*buf_ptr++ = int86( 0x10, &r, &r );
			putchar(' ');
		}
}

void chineseputs( int x, int y, char *p, int attrib ) /*在指定的位置显示中文字符*/
{
	union REGS r;
	while(*p)
	{
		r.h.ah = 2;
		r.h.dl = x++;
		r.h.dh = y;
		int86(0x10, &r, &r);
		r.h.ah = 9;
		r.h.bh = 0;
		r.x.cx = 1;
		r.h.al = *p++;
		r.h.bl = attrib;
		int86( 0x10, &r, &r );
		
	}
}

void disp_box( int x1, int y1, int x2, int y2, int attrib) /*显示边框*/
{
	unsigned char* boxchar[6] = {"┏","━","┓","┃","┗","┛"};
	register int i;
	for( i = x1; i <= x2; i++ )
	{
		chineseputs( i, y1, boxchar[1], attrib );
		chineseputs( i, y2, boxchar[1], attrib );
	}
	for( i = y1; i <= y2; i++ )
	{
		chineseputs( x1, i, boxchar[3], attrib );
		chineseputs( x2, i, boxchar[3], attrib );
	}
	chineseputs( x1, y1, boxchar[0], attrib );
	chineseputs( x2, y1, boxchar[2], attrib );
	chineseputs( x2, y2, boxchar[5], attrib );
	chineseputs( x1, y2, boxchar[4], attrib );
}

void display_menu( char* menu[], int x, int y, int count ) /*显示菜单项*/
{
	register int i;
	for( i = 0; i < count; i++, x++)
	{
		goto_xy( x, y );
		cprintf( menu[i]);
	}
}

int is_in( char* s, char c) /*返回字符串中热键的位置*/
{
	register int i;
	for( i = 0; *s; i++ )
		if( *s++ == c ) return i + 1;
	return 0;
}

int get_resp( int x, int y, int count, char* menu[], char* keys ) /*获得用户的响应*/
{
	union inkey
	{
		char ch[2];
		int i ;
	} c ;
	int arrow_choice = 0, key_choice;
	y++;
	goto_xy(x, y);
	chineseputs( y, x, menu[0], REV_VID);
	for(;;)
	{
		while(!bioskey(1));
		c.i = bioskey(0);
		goto_xy( x + arrow_choice, y);
		chineseputs( y, x+arrow_choice, menu[arrow_choice], NORM_VID );
		if( c.ch[0] )
		{
			key_choice = is_in( keys, tolower( c.ch[0] ));
			if( key_choice ) return key_choice - 1;
			switch( c.ch[0] )
			{
			case '\r': return arrow_choice;
			case ' ':  arrow_choice++;
				   break;
			case ESC : return -1;
			}
		}
		else
		{
			switch( c.ch[1] )
			{
			case 72: arrow_choice--;
				 break;
			case 80: arrow_choice++;
				 break;
			}
		}
		if( arrow_choice == count ) arrow_choice = 0;
		if( arrow_choice < 0 ) arrow_choice = count - 1;
		goto_xy( x + arrow_choice, y );
		chineseputs( y, x+arrow_choice, menu[ arrow_choice ], REV_VID );
	}
}

void restore_video( int startx, int endx, int starty, int endy, unsigned char* buf_ptr )
/*恢复原有的视频方式*/
{
	union REGS r;
	register int i, j;
	for( j = startx; j < endx; j++)
		for( j = starty; j < endy; j++ )
		{
			goto_xy(i, j);
			r.h.ah = 9;
			r.h.bh = 0;
			r.x.cx = 1;
			r.h.al = *buf_ptr++;
			r.h.bl = *buf_ptr++;
			int86( 0x10, &r, &r );
		}
}

int popup( char* menu[], char* keys, int count, int x, int y, int border )
/*弹出式菜单的显示*/
{
	register int i, len;
	int endx, endy, choice;
	unsigned int *p;
	if((x>24)||(x<0)||(y>79)||(y<0))
	{
		printf("范围错");
		return -2;
	}
	len = 0;
	for( i = 0; i < count; i++ )
		if( strlen( menu[i]) > len ) len = strlen( menu[i] );
	endy = len + 2 + y;
	endx = count + 1 + x;
	if((endx+1>24)||(endy+1>79))
	{
		printf("菜单不匹配");
		return -2;
	}
	p = (unsigned int *) malloc((endx-x+1)*(endy-y+1));
	if( !p ) exit(1);
	save_video( x, endx + 1, y, endy+1, p );
	if( border ) disp_box( y, x, endy, endx, YELLOW );
	display_menu( menu, x + 1, y + 1, count );
	choice = get_resp( x + 1, y, count, menu, keys );
	restore_video( x, endx + 1, y, endy + 2, (unsigned char* ) p);
	free( p );
	return choice;
}

void main()
{
	int i, j;
	directvideo = 0;
	for(;;)
	{
		clrscr();
		goto_xy(0, 0);
		textcolor( BLACK  );
		textbackground( RED );
		switch( popup( main_menu, "fdhx", 4,5, 16, BORDER ))
		{
		case 0:
			clrscr();
			switch( popup( sub_menu1, "spcdx", 5, 5, 16, BORDER ))
			{
			case 0:
				clrscr();
				goto_xy( 24, 20 );
				cprintf("你选择了文件显示功能");
				getch();
				break;
			case 1:
				clrscr();
				goto_xy( 24, 20 );
				cprintf("你选择了文件打印功能");
				getch();
				break;
			case 2:
				clrscr();
				goto_xy( 24, 20 );
				cprintf("你选择了文件拷贝功能");
				getch();
				break;
			case 3:
				clrscr();
				goto_xy(24,20);
				cprintf("你选择了文件删除功能");
				getch();
				break;
			case 4:
				break;
			default:
				break;
			}
			break;
		case 1:
			clrscr();
			switch( popup( sub_menu2, "brx", 3, 5, 16, BORDER ))
			{
			case 0:
				clrscr();
				goto_xy(24,20);
				cprintf("你选择了BOOT区备份功能");
				getch();
				break;
			case 1:
				clrscr();
				goto_xy(24,20);
				cprintf("你选择了BOOT区恢复功能");
				getch();
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 2:
			clrscr();
			switch( popup( sub_menu3, "of", 3, 5, 16, BORDER ))
			{
			case 0:
				clrscr();
				goto_xy( 24, 20 );
				cprintf("你选择了打印图形功能");
				getch();
				break;
			case 1:
				clrscr();
				goto_xy(24,20);
				cprintf("你选择了打印文本功能");
				getch();
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case 3:
		case -1:
			return 0;
		default:
			break;
		}
	}
}