

#include <bios.h>       /* for REGS */
#include <dos.h>        /* for intdos */

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * Name:    get_date
 * Purpose: get system date from DOS
 * Date:    June 5, 1992
 * Passed:  year:  pointer to integer to store year
 *          month: pointer to integer to store month
 *          day:   pointer to integer to store day
 *          day_of_week:  pointer to integer to store dow
 * Notes:   call standard DOS interrupt 0x21, function 0x2a to get the date.
 */
void get_date( int *year, int *month, int *day, int *day_of_week  )
{
union REGS inregs, outregs;

   inregs.h.ah = 0x2a;
   intdos( &inregs, &outregs );
   *year        = (int)outregs.x.cx;
   *month       = (int)outregs.h.dh;
   *day         = (int)outregs.h.dl;
   *day_of_week = (int)outregs.h.al;
}


/*
 * Name:    get_time
 * Purpose: get system time from DOS
 * Date:    June 5, 1992
 * Passed:  hour:  pointer to integer to store hour - system returns 24 hour
 *          minutes:  pointer to integer to store minutes
 *          seconds:  pointer to integer to store seconds
 *          hundredths:  pointer to integer to store hundredths of second
 * Notes:   call standard DOS interrupt 0x21, function 0x2c to get the time.
 */
void get_time( int *hour, int *minutes, int *seconds, int *hundredths  )
{
union REGS inregs, outregs;

   inregs.h.ah = 0x2c;
   intdos( &inregs, &outregs );
   *hour       = (int)outregs.h.ch;
   *minutes    = (int)outregs.h.cl;
   *seconds    = (int)outregs.h.dh;
   *hundredths = (int)outregs.h.dl;
}


/*
 * Name:    show_modes
 * Purpose: show current editor modes in lite bar at bottom of screen
 * Date:    June 5, 1991
 */
void show_modes( void )
{
char status_line[MAX_COLS+2];

   memset( status_line, ' ', MAX_COLS );
   status_line[MAX_COLS] = '\0';
   s_output( status_line, g_display.mode_line, 0, g_display.mode_color );
   s_output( "F=   W=", g_display.mode_line, 1, g_display.mode_color );
   s_output( "m=", g_display.mode_line, 12, g_display.mode_color );
   show_window_count( g_status.window_count );
   show_file_count( g_status.file_count );
   show_avail_mem( );
   show_tab_modes( );
   show_indent_mode( );
   show_sync_mode( );
   show_control_z( );
   show_insert_mode( );
   show_search_case( );
   show_wordwrap_mode( );
   show_trailing( );
}


/*
 * Name:    show_file_count
 * Purpose: show number of open files in lite bar at bottom of screen
 * Passed:  fc:  file count - number of open files
 * Date:    June 5, 1991
 */
void show_file_count( int fc )
{
char status_line[MAX_COLS+2];

   s_output( "  ", g_display.mode_line, 3, g_display.mode_color );
   s_output( itoa( fc, status_line, 10 ), g_display.mode_line, 3,
             g_display.mode_color );
}


/*
 * Name:    show_window_count
 * Purpose: show total number of windows in lite bar at bottom of screen
 * Passed:  wc:  window count - visible and hidden.
 * Date:    September 13, 1991
 */
void show_window_count( int wc )
{
char status_line[MAX_COLS+2];

   s_output( "  ", g_display.mode_line, 8, g_display.mode_color );
   s_output( itoa( wc, status_line, 10 ), g_display.mode_line, 8,
             g_display.mode_color );
}


/*
 * Name:    show_avail_mem
 * Purpose: show available free memory in lite bar at bottom of screen
 * Date:    June 5, 1991
 */
void show_avail_mem( void )
{
char line[MAX_COLS+2];
unsigned long avail_mem;

#if defined( __MSC__ )
unsigned paragraphs;

   _dos_allocmem( 0xffff, &paragraphs );
   /*
    * A paragraph is 16 bytes.  Convert paragraphs to bytes by shifting left
    * 4 bits.
    */
   avail_mem = (long)paragraphs << 4;
#else
   avail_mem = farcoreleft( );
#endif

   s_output( "        ", g_display.mode_line, 14, g_display.mode_color );
   ultoa( avail_mem, line, 10 );
   s_output( line, g_display.mode_line, 14,
             g_display.mode_color );
}


/*
 * Name:    show_tab_modes
 * Purpose: show smart tab mode in lite bar at bottom of screen
 * Date:    October 31, 1992
 */
void show_tab_modes( void )
{
char *blank_tab = "   ";
char ascii_tab[10];

   s_output( tabs, g_display.mode_line, 22, g_display.mode_color );
   s_output( mode.smart_tab ? smart : fixed, g_display.mode_line, 27,
             g_display.mode_color );
   s_output( mode.inflate_tabs ? intab : outtab, g_display.mode_line, 28,
             g_display.mode_color );
   s_output( blank_tab, g_display.mode_line, 29, g_display.mode_color );
   s_output( itoa( mode.ptab_size, ascii_tab, 10), g_display.mode_line, 29,
             g_display.mode_color );
}


/*
 * Name:    show_indent_mode
 * Purpose: show indent mode in lite bar at bottom of screen
 * Date:    June 5, 1991
 */
void show_indent_mode( void )
{
   s_output( mode.indent ? indent : blank, g_display.mode_line, 32,
             g_display.mode_color );
}


/*
 * Name:    show_search_case
 * Purpose: show search mode in lite bar
 * Date:    June 5, 1991
 */
void show_search_case( void )
{
   s_output( mode.search_case == IGNORE ? ignore : match, g_display.mode_line,
             40, g_display.mode_color );
}


/*
 * Name:    show_sync_mode
 * Purpose: show sync mode in lite bar
 * Date:    January 15, 1992
 */
void show_sync_mode( void )
{
   s_output( mode.sync ? sync_on : sync_off, g_display.mode_line, 48,
             g_display.mode_color );
}


/*
 * Name:    show_wordwrap_mode
 * Purpose: display state of word wrap mode
 * Date:    June 5, 1991
 */
void show_wordwrap_mode( void )
{
   s_output( ww_mode[mode.word_wrap], g_display.mode_line, 54,
             g_display.mode_color );
}


/*
 * Name:    show_trailing
 * Purpose: show state of trailing flag
 * Date:    June 5, 1991
 */
void show_trailing( void )
{
   c_output( mode.trailing ? 'T' : ' ', 66, g_display.mode_line,
             g_display.mode_color );
}


/*
 * Name:    show_control_z
 * Purpose: show state of control z flag
 * Date:    June 5, 1991
 */
void show_control_z( void )
{
   c_output( mode.control_z ? 'Z' : ' ', 77, g_display.mode_line,
             g_display.mode_color );
}


/*
 * Name:    show_insert_mode
 * Purpose: show insert mode in lite bar
 * Date:    June 5, 1991
 */
void show_insert_mode( void )
{
   c_output( mode.insert ? 'i' : 'o', 79, g_display.mode_line,
             g_display.mode_color );
}


/*
 * Name:    my_scroll_down
 * Purpose: display a portion of a window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Using the bios scroll functions causes a slightly noticable
 *            "flicker", even on fast VGA monitors.  This is caused by changing
 *            the high-lited cursor line to text color then calling the bios
 *            function to scroll.  Then, the current line must then be
 *            hilited.
 *          This function assumes that win->cline is the current line.
 */
void my_scroll_down( WINDOW *window )
{
int  i;
int  curl;
int  eof;
WINDOW w;              /* scratch window struct for dirty work */

   if (!window->visible  ||  !g_status.screen_display)
      return;
   dup_window_info( &w, window );
   curl = i = window->bottom_line + 1 - window->cline;
   eof = FALSE;
   for (; i>0; i--) {
      if (w.ll->len != EOF) {
         /*
          * if this is window->cline, do not show the line because we
          * show the curl at the end of this function.  don't show it twice
          */
         if (i != curl)
            update_line( &w );
      } else if (eof == FALSE) {
         show_eof( &w );
         eof = TRUE;
      } else
         window_eol_clear( &w, COLOR_TEXT );
      if (w.ll->next != NULL)
         w.ll = w.ll->next;
      ++w.cline;
      ++w.rline;
   }
   show_curl_line( window );
}


/*
 * Name:    combine_strings
 * Purpose: stick 3 strings together
 * Date:    June 5, 1991
 * Passed:  buff:    buffer to hold concatenation of 3 strings
 *          s1:  pointer to string 1
 *          s2:  pointer to string 2
 *          s3:  pointer to string 3
 */
void combine_strings( char *buff, char *s1, char *s2, char *s3 )
{
   assert( strlen( s1 ) + strlen( s2 ) + strlen( s3 ) < MAX_COLS );
   strcpy( buff, s1 );
   strcat( buff, s2 );
   strcat( buff, s3 );
}


/*
 * Name:    make_ruler
 * Purpose: make ruler with tabs, tens, margins, etc...
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void make_ruler( WINDOW *window )
{
register WINDOW *win;
char num[20];
register unsigned char *p;
int  len;
int  col;
int  i;
int  mod;

   win = window;

   /*
    * need to have a least two lines in a window when we display a ruler.
    */
   if (win->bottom_line - win->top_line < 1)
      win->ruler = FALSE;
   if (win->ruler) {

      /*
       * find the width of the window and fill the ruler with dots.
       */
      len = win->end_col + 1 - win->start_col;

      assert( len >= 0 );
      assert( len <= MAX_COLS );

      memset( win->ruler_line, RULER_FILL, len );
      win->ruler_line[len] = '\0';
      col = win->bcol+1;

      assert( col >= 1 );
      assert( col <= MAX_LINE_LENGTH );

      for (p=(unsigned char *)win->ruler_line; *p; col++, p++) {

         /*
          * put a tens digit in the tens column
          */
         mod = col % 10;
         if (mod == 0) {
            itoa( col/10, num, 10 );

            /*
             * let the margin chars have precidence over tens digit
             */
            for (i=0; num[i] && *p; col++, i++) {
               if (col == mode.left_margin+1)
                  *p = LM_CHAR;
               else if (col == mode.right_margin+1) {
                  if (mode.right_justify == TRUE)
                     *p = RM_CHAR_JUS;
                  else
                     *p = RM_CHAR_RAG;
               } else if (col == mode.parg_margin+1)
                  *p = PGR_CHAR;
               else
                  *p = num[i];
               p++;
            }

            /*
             * we may have come to the end of the ruler in the for loop.
             */
            if (*p == '\0')
               break;
         } else if (mod == 5)
            *p = RULER_TICK;
         if (col == mode.parg_margin+1)
            *p = PGR_CHAR;
         if (col == mode.left_margin+1)
            *p = LM_CHAR;
         else if (col == mode.right_margin+1) {
            if (mode.right_justify == TRUE)
               *p = RM_CHAR_JUS;
            else
               *p = RM_CHAR_RAG;
         }
      }
   }
}


/*
 * Name:    show_ruler
 * Purpose: show ruler with tens, margins, etc...
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_ruler( WINDOW *window )
{
   if (window->ruler && window->visible)
      s_output( window->ruler_line, window->top_line, window->start_col,
                g_display.ruler_color );
}


/*
 * Name:    show_ruler_char
 * Purpose: show ruler character under ruler pointer
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_ruler_char( WINDOW *window )
{
register WINDOW *win;
char c;

   win = window;
   if (win->ruler && win->visible) {
      c = win->ruler_line[win->ccol - win->start_col];
      c_output( c, win->ccol, win->top_line, g_display.ruler_color );
   }
}


/*
 * Name:    show_ruler_pointer
 * Purpose: show ruler pointer
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_ruler_pointer( WINDOW *window )
{
   if (window->ruler && window->visible)
      c_output( RULER_PTR, window->ccol, window->top_line,
                g_display.ruler_pointer );
}


/*
 * Name:    show_all_rulers
 * Purpose: make and show all rulers in all visible windows
 * Date:    June 5, 1991
 */
void show_all_rulers( void )
{
register WINDOW *wp;

   wp = g_status.window_list;
   while (wp != NULL) {
      make_ruler( wp );
      if (wp->visible) {
         show_ruler( wp );
         show_ruler_pointer( wp );
      }
      wp = wp->next;
   }
}
