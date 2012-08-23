#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <dos.h>

int main(void)
{
   char *path, *ptr;
   int i = 0;
   clrscr();
   puts(" This program is to get the Path and change it.");
   /* 获得当前环境变量中的path信息 */
   ptr = getenv("PATH");

   /* 更新path */
   path = (char *) malloc(strlen(ptr)+15); /*分配内存空间，大小比path的长度多15个字节*/
   strcpy(path,"PATH="); /*复制path*/
   strcat(path,ptr);
   strcat(path,";c:\\temp"); /*在当前的path后追加一个路径*/

   /* 更新路径信息并显示所有的环境变量 */
   putenv(path); /*设置环境变量*/
   while (environ[i]) /*循环输出所有的环境变量*/
       printf(" >> %s\n",environ[i++]);
   printf(" Press any key to quit...");
   getch();
   return 0;
}