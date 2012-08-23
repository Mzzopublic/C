

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tdecfg.h"
#include "cfgmacro.h"

extern struct vcfg cfg;
extern FILE *tde_exe;                  /* FILE pointer to tde.exe */
extern long macro_offset;

static WINDOW *w_ptr;

MACRO macros;

/********    EXTREMELY IMPORTANT   ************/
/*
 * If you modify tde, it is your responsibility to find the offset of
 * the macro buffer in your new executable, tde.exe.
 *
 */



/*
 * Name:    tdehelp
 * Date:    October 1, 1991
 * Notes:   Set up most of the window global variables.
 */
void tdemacro( void )
{
int c;
char fname[82];
FILE *macro_file;                  /* FILE pointer to macro */


   cls( );
   show_box( 0, 0, macro_screen, NORMAL );
   xygoto( 42, 14 );
   c = getkey( );
   while (c != '1' && c != '2')
      c = getkey( );
   if (c == '1') {
      puts( "" );
      puts( "" );
      puts( "" );
      puts( "Enter file name that contains the macro definitions :" );
      gets( fname );
      if ((c = access( fname, EXIST )) != 0) {
         puts( "\nFile not found.  Press any key to continue." );
         c = getkey( );
         cls( );
         return;
      } else if ((macro_file = fopen( fname, "rb" )) == NULL ) {
         puts( "\nCannot open macro file.  Press any key to contine." );
         c = getkey( );
         cls( );
         return;
      }

      fread( (void *)&macros.first_stroke[0], sizeof(int), MAX_KEYS, macro_file );
      fread( (void *)&macros.strokes[0], sizeof(STROKES), STROKE_LIMIT, macro_file );
      fseek( tde_exe, macro_offset + 8, SEEK_SET );
      fwrite( (void *)&macros.first_stroke[0], sizeof(int), MAX_KEYS, tde_exe );
      fwrite( (void *)&macros.strokes[0], sizeof(STROKES), STROKE_LIMIT, tde_exe );
      fclose( macro_file );
      puts( "" );
      puts( "" );
      puts( "" );
      puts( "New macros successfully installed.  Press any key to continue." );
      c = getkey( );
   }
   cls( );
}
