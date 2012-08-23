#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <alloc.h>
#include <string.h>

void main(int argc, char **argv)
 {
   void delete_tree(void);

   char buffer[128];

   char drive[MAXDRIVE], directory[MAXDIR], filename[MAXFILE], ext[MAXEXT];

   if (argc < 2)
     {
       printf (" >> Syntax error\n");
       exit(0);
     }


   fnsplit (argv[1], drive, directory, filename, ext);
   getcwd (buffer, sizeof(buffer));

   if (drive[0] == NULL)
     {
       fnsplit (buffer, drive, directory, filename, ext);
       strcpy (buffer, directory);
       strcat (buffer, filename);
       strcat (buffer, ext);
     }
   else
     { 
       printf (" >> Do not specify drive letter\n");
       exit (1);
     }

   if (strcmpi(buffer, argv[1]) == 0)
     {
       printf (" >> Cannot delete current directory\n");
       exit (1);
     }
		  
   getcwd (directory, 64);

   if (chdir (argv[1]))
     printf (" >> Invalid directory %s\n", argv[1]);
   else
     delete_tree(); 

   chdir (directory);
   rmdir (argv[1]);
   printf(" >> Delete Successful!\n");
   printf(" Press any key to quit...");
   getch();
   return;
}

union REGS inregs, outregs;
struct SREGS segs;

void delete_tree(void)
 {
   struct ffblk fileinfo;
   int result;
   char far *farbuff;
   unsigned dta_seg, dta_ofs;

   result = findfirst("*.*", &fileinfo, 16);
   inregs.h.ah = 0x2f;
   intdosx (&inregs, &outregs, &segs);
   dta_seg = segs.es;
   dta_ofs = outregs.x.bx;

   while (! result)
    {
      if ((fileinfo.ff_attrib & 16) && (fileinfo.ff_name[0] != '.'))
	{
	   inregs.h.ah = 0x1A;
	   inregs.x.dx = FP_SEG(farbuff);
	   segread(&segs);
	   intdosx (&inregs, &outregs, &segs);
		
	   chdir (fileinfo.ff_name);
	   delete_tree();
	   chdir ("..");

	   inregs.h.ah = 0x1A;
	   inregs.x.dx = dta_ofs;
	   segs.ds = dta_seg;
	   rmdir (fileinfo.ff_name);
	}
      else if (fileinfo.ff_name[0] != '.')
	{
	   remove (fileinfo.ff_name);
	}
	  
      result = findnext (&fileinfo);
    }
 }
