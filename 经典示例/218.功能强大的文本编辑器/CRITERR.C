/*

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "criterr.h"

/*
 * Save the area of the screen that will display the Critical
 * Error info.  CEH_WIDTH and CEH_HEIGHT are the dimensions of critical
 * error screen in criterr.h.   CEH_OFFSET is the offset into the screen
 * refresh buffer.  Let the compiler calculate the offset, 'cause the offset
 * don't change anyway.
 */
#define CEH_ROW         5
#define CEH_COL         6
#define CEH_WIDTH       69
#define CEH_HEIGHT      15

#define CEH_OFFSET      ((CEH_ROW * 160) + (CEH_COL * 2))

#define NEXT_LINE       160


/*
 * buffer for ceh info screen.  make this an int array because we
 * need to save character and attribute.
 */
int ceh_buffer[CEH_HEIGHT][CEH_WIDTH];


/*
 * Name:    crit_err_handler
 * Purpose: Show user something is wrong and get a response
 * Date:    April 1, 1992
 */
int  far crit_err_handler( void )
{
int  rc;
int  c;

   save_area( (char far *)ceh_buffer );
   show_error_screen( CEH_ROW, CEH_COL );
   xygoto( 60, 17 );
   do
      c = getkey( );
   while (c != 'Q' && c != 'q' && c != 'R' && c != 'r' && c != 'A' && c != 'a');
   switch ( c ) {
      case 'A':
      case 'a':
         rc = ABORT;
         break;
      case 'Q':
      case 'q':
         rc = FAIL;
         break;
      case 'R':
      case 'r':
      default :
         rc = RETRY;
         break;
   }
   restore_area( (char far *)ceh_buffer );
   return( rc );
}


/*
 * Name:    show_error_screen
 * Purpose: Display error screen in window
 * Date:    April 1, 1992
 * Passed:  row: line to display ceh screen
 *          col: column to begin display ceh screen
 */
void show_error_screen( int row, int col )
{
char **p;

   for (p=criterr_screen; *p != NULL; p++, row++)
      s_output( *p, row, col, g_display.help_color );
   s_output( error_code[ceh.code],    8, 23, g_display.help_color );
   s_output( operation[ceh.rw],       9, 23, g_display.help_color );
   if (ceh.dattr == 0)
      c_output( ceh.drive + 'a',     23, 10, g_display.help_color );
   else
      s_output( critt1,              10, 23, g_display.help_color );
   s_output( ext_err[ceh.extended],  11, 23, g_display.help_color );
   s_output( error_class[ceh.class], 12, 23, g_display.help_color );
   s_output( locus[ceh.locus],       13, 23, g_display.help_color );
   s_output( device_type[ceh.dattr], 14, 23, g_display.help_color );
   s_output( ceh.dattr == 0 ? critt1 : ceh.dname,
                                     15, 23, g_display.help_color );
}


/*
 * Name:    save_area
 * Purpose: save a region of the screen
 * Date:    April 1, 1992
 * Passed:  dest: pointer to buffer for contents of screen under ceh
 * Notes:   this function does not check for snow.  the source is the screen
 *             and the destination is the buffer.
 */
void save_area( char far *dest )
{
char far *source;
register int hgt;

   source = (char far *)g_display.display_address + CEH_OFFSET;
   for (hgt=CEH_HEIGHT; hgt; hgt--) {
      _fmemcpy( dest, source, CEH_WIDTH*2 );
      source += NEXT_LINE;
      dest += (CEH_WIDTH*2);
   }
}


/*
 * Name:    restore_area
 * Purpose: restore a region of the screen
 * Date:    April 1, 1992
 * Passed:  source: pointer to buffer for contents of screen under ceh
 * Notes:   this function does not check for snow.  the source is the buffer
 *             and the destination is the screen.
 */
void restore_area( char far *source )
{
char far *dest;
register int hgt;

   dest = (char far *)g_display.display_address + CEH_OFFSET;
   for (hgt=CEH_HEIGHT; hgt; hgt--) {
      _fmemcpy( dest, source, CEH_WIDTH*2 );
      dest += NEXT_LINE;
      source += (CEH_WIDTH*2);
   }
}
