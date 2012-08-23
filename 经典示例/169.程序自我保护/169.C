/* 在BC31下编译 */
/* compile under Borland C++ 3.1 */

#include <dos.h>
#include <dir.h>
#include <stdio.h>
#include <io.h>
#include <sys\stat.h>

void main( int argc, char* argv[] )
{
	struct time now;
	FILE* fp;
	int errno;
	gettime( &now );
	if( now.ti_min > 30 ) /*如非法使用系统则删除程序*/
	{
		errno = chmod( argv[0], S_IWRITE );
		if((errno)&& ( fp != NULL ))
		{
			fclose( fp );
			/*将文件长度截止为0*/
			unlink( argv[0] );
			exit(0);
			/*然后删除本文件退出*/
		}
		else
		{
			/*如不能删除打印错误退出*/
			printf( "\n You are a illegal user to run this program! \007\n" );
			exit(1);
		}
	}
	printf(" You are a legitimate user to run this program!\n");
	getch();
	return;
}