/* ÔÚBC31ÏÂ±àÒë */
/* compile under Borland C++ 3.1 */

#include <stdio.h>
#include <dirent.h>
#include <dos.h>
#include <io.h>
#include <direct.h>
#include <string.h>

void show_directory(char *directory_name)
 { 
   DIR *directory_pointer;
   struct dirent *entry;

   unsigned attributes;

   if ((directory_pointer = opendir(directory_name)) == NULL)
     printf("Error opening %s\n", directory_name);
   else
     {
        chdir(directory_name);
        while (entry = readdir(directory_pointer))
          {
            attributes = _chmod(entry, 0);
            
            // Check if entry is for a subdirectory and is not "." or ".."
            if ((attributes & FA_DIREC) && 
               (strncmp(entry, ".", 1) != 0))
             { 
               printf("\n\n----%s----\n", entry);
               show_directory(entry);
             }
            else
             printf("%s\n", entry);
          }
        
        closedir(directory_pointer);
        chdir("..");
     }
 }

 void main(void)
  {
    char buffer[MAXPATH];

    // Save current directory so you can restore it later
    getcwd(buffer, sizeof(buffer));
    show_directory("\\");
    chdir(buffer);
    printf(" Press any key to quit...");
     getch();
     return;
  }


