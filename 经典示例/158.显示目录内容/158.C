/* 在BC31下编译 */
/* compile under Borland C++ 3.1 */

#include <stdio.h>
#include <dirent.h>
#include <alloc.h>
#include <string.h>
#include <stdlib.h>

void main(int argc, char *argv[])
 { 
   DIR *directory_pointer;  
   struct dirent *entry;
   struct FileList 
   {
     char filename[64];
     struct FileList *next;
   } start, *node, *previous, *new;
   
   
   if ((directory_pointer = opendir(argv[1])) == NULL) /*取argv[1]文件夹的指针赋于*/
     printf("Error opening %s\n", argv[1]);
   else
     {
        start.next = NULL;
		/*将directory_pointer指向的文件名列表做成一个以FileList类型为结点的链*/
        while (entry = readdir(directory_pointer))/*读取directory_pointer指向的文件名*/
          { 
            // Find the correct location
            previous = &start;
            node = start.next;
            while ((node) && (strcmp(entry, node->filename) > 0))/* 以字典序搜索在链表中此文件名应该插入的位置*/
             { 
               node = node->next;
               previous = previous->next;
             }

            new = (struct FileList *) 
			malloc(sizeof(struct FileList));           
            if (new == NULL) /*内存分配失败*/
             {
               printf("Insufficient memory to store list\n");
               exit(1);
             }
            /*完成插入*/
			new->next = node;
            previous->next = new;
            strcpy(new->filename, entry);
          }

        closedir(directory_pointer);
        node = start.next;
        /*输出整个链表结点的文件名*/
		while (node)
          {
            printf("%s\n", node->filename);
            node = node->next;
          }
     }
     printf(" Press any key to quit...");
     getch();
     return;
 }


