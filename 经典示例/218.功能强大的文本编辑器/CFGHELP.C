/*
 * This module contains all the routines needed to create a new help
 * screen.  The new help screen is read from a file which must exist before
 * executing this routine.
 *
 * Program Name:  tdecfg
 * Author:        Frank Davis
 * Date:          October 5, 1991
 */

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tdecfg.h"
#include "cfghelp.h"

extern struct vcfg cfg;
extern FILE *tde_exe;                  /* FILE pointer to tde.exe */
extern long help_offset;

static WINDOW *w_ptr;


/********    EXTREMELY IMPORTANT   ************/
/*
 * If you modify tde, it is your responsibility to find the offset of
 * the help screen in your new executable, tde.exe.
 *
 */



/*
 * Name:    tdehelp
 * Date:    October 1, 1991
 * Notes:   Set up most of the window global variables.
 */
void tdehelp( void )
{
int c;
int i;
char line[200];
char out_line[82];
char fname[82];
char *rc;
FILE *help_file;                  /* FILE pointer to help screen */
long offset;


   cls( );
   show_box( 0, 0, help_screen, NORMAL );
   xygoto( 42, 14 );
   c = getkey( );
   while (c != '1' && c != '2')
      c = getkey( );
   if (c == '1') {
      puts( "" );
      puts( "" );
      puts( "" );
      puts( "Enter file name that contains new help screen :" );
      gets( fname );
      if ((c = access( fname, EXIST )) != 0) {
         puts( "\nFile not found.  Press any key to continue." );
         c = getkey( );
         cls( );
         return;
      } else if ((help_file = fopen( fname, "r" )) == NULL ) {
         puts( "\nCannot open help file.  Press any key to contine." );
         c = getkey( );
         cls( );
         return;
      }
      offset = help_offset + 8;
      rc = fgets( line, 100, help_file );
      for (c=0; c<25 && rc != NULL; c++) {
         memset( out_line, '\0', 82 );
         for (i=0; i<80 && line[i] != '\n'; i++)
             out_line[i] = line[i];
         fseek( tde_exe, offset, SEEK_SET );
         fwrite( out_line, sizeof( char ), 81, tde_exe );
         offset += 81;
         rc = fgets( line, 100, help_file );
      }
      fclose( help_file );
      puts( "" );
      puts( "" );
      puts( "" );
      puts( "New help screen successfully installed.  Press any key to continue." );
      c = getkey( );
   }
   cls( );
}
