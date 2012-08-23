


#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tdecfg.h"
#include "cfgmodes.h"


extern struct vcfg cfg;         /* video stuff */
extern FILE *tde_exe;           /* FILE pointer to tde.exe */
extern long mode_offset;

MODE_INFO in_modes;           /* play around with modes in this struct */

int color;

/*
 * Name:    tdemodes
 * Date:    July 21, 1991
 * Notes:   Strategy is fairly straight forward -  1) initialize all the
 *          variables  2) show the user a color sample  3) make the changes
 *          permanent if desired.
 */
void tdemodes( void )
{
   color = 7;
   initialize_modes( );
   show_init_mode( );
   change_modes( );
}


/*
 * Name:    initialize_modes
 * Date:    July 21, 1991
 * Notes:   Set up the global mode variables.
 */
void initialize_modes( void )
{
int i, j;

   fseek( tde_exe, mode_offset, SEEK_SET );
   fread( (void *)&in_modes, sizeof( MODE_INFO ), 1, tde_exe );

   mode[Ins].mode      = in_modes.insert;
   mode[Ind].mode      = in_modes.indent;
   mode[PTAB].mode     = in_modes.ptab_size;
   mode[LTAB].mode     = in_modes.ltab_size;
   mode[Smart].mode    = in_modes.smart_tab;
   mode[Write_Z].mode  = in_modes.control_z;
   mode[Crlf].mode     = in_modes.crlf;
   mode[Trim].mode     = in_modes.trailing;
   mode[Eol].mode      = in_modes.show_eol;
   mode[WW].mode       = in_modes.word_wrap;
   mode[Left].mode     = in_modes.left_margin;
   mode[Para].mode     = in_modes.parg_margin;
   mode[Right].mode    = in_modes.right_margin;
   mode[Size].mode     = in_modes.cursor_size;
   mode[Backup].mode   = in_modes.do_backups;
   mode[Ruler].mode    = in_modes.ruler;
   mode[Date].mode     = in_modes.date_style;
   mode[Time].mode     = in_modes.time_style;
   mode[Initcase].mode = in_modes.search_case;
}


/*
 * Name:    show_init_mode
 * Date:    July 21, 1991
 * Notes:   Draw the sample screen.
 */
void show_init_mode( void )
{
char *sample;
int line, i;

   cls( );
   xygoto( -1, -1 );
   sample = mode_screen[0];
   for (i=0,line=1; sample != NULL; line++, i++) {
      sample = mode_screen[i];
      s_output( (char far *)sample, line, 0, 7 );
   }
   for (i=0; i<19; i++)
      (*mode[i].show_me)();
}


/*
 * Name:    show_insert_mode
 * Date:    January 20, 1992
 */
void show_insert_mode( void )
{
char *p[] = {
   "Overwrite",
   "Insert   "
};

   s_output( p[mode[Ins].mode], mode[Ins].line, mode[Ins].col, color );
}


/*
 * Name:    show_indent_mode
 * Date:    January 20, 1992
 */
void show_indent_mode( void )
{
char *p[] = {
   "Indent off",
   "Indent on "
};

   s_output( p[mode[Ind].mode], mode[Ind].line, mode[Ind].col, color );
}


/*
 * Name:    show_ptabsize
 * Date:    January 20, 1992
 */
void show_ptabsize( void )
{
char temp[10];

   s_output( "   ", mode[PTAB].line, mode[PTAB].col, color );
   s_output( itoa(mode[PTAB].mode,temp,10), mode[PTAB].line, mode[PTAB].col,color);
}


/*
 * Name:    show_ltabsize
 * Date:    January 20, 1992
 */
void show_ltabsize( void )
{
char temp[10];

   s_output( "   ", mode[LTAB].line, mode[LTAB].col, color );
   s_output( itoa(mode[LTAB].mode,temp,10), mode[LTAB].line, mode[LTAB].col,color);
}


/*
 * Name:    show_controlz
 * Date:    January 20, 1992
 */
void show_controlz( void )
{
char *p[] = {
   "No ^Z   ",
   "Write ^Z",
};

   s_output( p[mode[Write_Z].mode], mode[Write_Z].line,mode[Write_Z].col,color);
}


/*
 * Name:    show_eol_out
 * Date:    January 20, 1992
 */
void show_eol_out( void )
{
char *p[] = {
   "",
   "CRLF",
   "LF  ",
};

   s_output( p[mode[Crlf].mode], mode[Crlf].line, mode[Crlf].col, color );
}


/*
 * Name:    show_trail
 * Date:    January 20, 1992
 */
void show_trail( void )
{
char *p[] = {
   "No Trim",
   "Trim   ",
};

   s_output( p[mode[Trim].mode], mode[Trim].line, mode[Trim].col, color );
}


/*
 * Name:    show_eol_display
 * Date:    January 20, 1992
 */
void show_eol_display( void )
{
char *p[] = {
   "Off",
   "On ",
};

   s_output( p[mode[Eol].mode], mode[Eol].line, mode[Eol].col, color );
}


/*
 * Name:    show_smart_mode
 * Date:    January 20, 1992
 */
void show_smart_mode( void )
{
char *p[] = {
   "Off",
   "On ",
};

   s_output( p[mode[Smart].mode], mode[Smart].line, mode[Smart].col, color );
}


/*
 * Name:    show_ww
 * Date:    January 20, 1992
 */
void show_ww( void )
{
char *p[] = {
   "Off                ",
   "Fixed margins      ",
   "Dynamic left margin",
};

   s_output( p[mode[WW].mode], mode[WW].line, mode[WW].col, color );
}


/*
 * Name:    show_initcase
 * Date:    January 20, 1992
 */
void show_initcase( void )
{
char *p[] = {
   "      ",
   "IGNORE",
   "MATCH ",
};

   s_output( p[mode[Initcase].mode], mode[Initcase].line, mode[Initcase].col,
              color );
}


/*
 * Name:    show_left
 * Date:    January 20, 1992
 */
void show_left( void )
{
char temp[10];

   s_output( "   ", mode[Left].line, mode[Left].col, color );
   s_output( itoa( mode[Left].mode+1, temp, 10 ), mode[Left].line, mode[Left].col,
             color);
}


/*
 * Name:    show_para
 * Date:    January 20, 1992
 */
void show_para( void )
{
char temp[10];

   s_output( "   ", mode[Para].line, mode[Para].col, color );
   s_output( itoa( mode[Para].mode+1, temp, 10 ), mode[Para].line, mode[Para].col,
             color);
}


/*
 * Name:    show_right
 * Date:    January 20, 1992
 */
void show_right( void )
{
char temp[10];

   s_output( "   ", mode[Right].line, mode[Right].col, color );
   s_output( itoa(mode[Right].mode+1,temp,10), mode[Right].line, mode[Right].col,
             color);
}


/*
 * Name:    show_cursor_size
 * Date:    January 20, 1992
 */
void show_cursor_size( void )
{
char *p[] = {
   "Small Insert Cursor",
   "Big Insert Cursor  ",
};

   s_output( p[mode[Size].mode], mode[Size].line, mode[Size].col, color );
}


/*
 * Name:    show_backup_mode
 * Date:    January 20, 1992
 */
void show_backup_mode( void )
{
char *p[] = {
   "No .bak files    ",
   "Create .bak files"
};

   s_output( p[mode[Backup].mode], mode[Backup].line, mode[Backup].col, color );
}


/*
 * Name:    show_backup_mode
 * Date:    January 20, 1992
 */
void show_ruler_mode( void )
{
char *p[] = {
   "No ruler  ",
   "Show ruler"
};

   s_output( p[mode[Ruler].mode], mode[Ruler].line, mode[Ruler].col, color );
}


/*
 * Name:    show_date_style
 * Date:    June 5, 1992
 */
void show_date_style( void )
{
char *p[] = {
   "MM_DD_YY  ",
   "DD_MM_YY  ",
   "YY_MM_DD  ",
   "MM_DD_YYYY",
   "DD_MM_YYYY",
   "YYYY_MM_DD",
};

   s_output( p[mode[Date].mode], mode[Date].line, mode[Date].col, color );
}


/*
 * Name:    show_time_style
 * Date:    June 5, 1992
 */
void show_time_style( void )
{
char *p[] = {
   "12_HOUR",
   "24_HOUR"
};

   s_output( p[mode[Time].mode], mode[Time].line, mode[Time].col, color );
}


/*
 * Name:    change_modes
 * Date:    July 21, 1991
 * Notes:   Real workhorse function of the utility.  Get a key and then
 *          figure out what to do with it.
 */
void change_modes( void )
{
int c;
int m;
int new_field;

   m = 0;
   xygoto( mode[m].col, mode[m].line );
   color = 112;
   (*mode[m].show_me)();
   for (c=0; c != F3  &&  c != F10  &&  c != ESC;) {
      c = getkey( );
      new_field = FALSE;
      color = 112;
      switch (c) {
         case RTURN :
         case DOWN  :
            color = 7;
            (*mode[m].show_me)();
            ++m;
            if (m > 18)
               m = 0;
            new_field = TRUE;
            break;
         case UP    :
            color = 7;
            (*mode[m].show_me)();
            --m;
            if (m < 0)
               m = 18;
            new_field = TRUE;
            break;
         case LEFT  :
            switch (m) {
               case Ins     :
               case Ind     :
               case Write_Z :
               case Smart   :
               case Trim    :
               case Eol     :
               case Size    :
               case Backup  :
               case Ruler   :
               case Time    :
                  mode[m].mode = !mode[m].mode;
                  break;
               case PTAB    :
               case LTAB    :
                  if (mode[m].mode > 1)
                     --mode[m].mode;
                  else
                     mode[m].mode = 1040 / 2;
                  break;
               case WW      :
                  --mode[m].mode;
                  if (mode[m].mode < 0)
                     mode[m].mode = 2;
                  break;
               case Date    :
                  --mode[m].mode;
                  if (mode[m].mode < 0)
                     mode[m].mode = 5;
                  break;
               case Crlf    :
                  if (mode[m].mode == CRLF)
                     mode[m].mode = LF;
                  else
                     mode[m].mode = CRLF;
                  break;
               case Left    :
               case Para    :
                  if (mode[m].mode > 0)
                     --mode[m].mode;
                  else
                     mode[m].mode = mode[Right].mode - 1;
                  break;
               case Right   :
                  if (mode[m].mode > mode[Left].mode + 1 &&
                      mode[m].mode > mode[Para].mode + 1)
                     --mode[m].mode;
                  else
                     mode[m].mode = 1040;
                  break;
               case Initcase   :
                  if (mode[m].mode == 1)
                     mode[m].mode = 2;
                  else
                     mode[m].mode = 1;
                  break;
            }
            (*mode[m].show_me)();
            break;
         case RIGHT :
            switch (m) {
               case Ins     :
               case Ind     :
               case Write_Z :
               case Smart   :
               case Trim    :
               case Eol     :
               case Size    :
               case Backup  :
               case Ruler   :
               case Time    :
                  mode[m].mode = !mode[m].mode;
                  break;
               case PTAB     :
               case LTAB     :
                  if (mode[m].mode < 1040 / 2)
                    ++mode[m].mode;
                  else
                     mode[m].mode = 1;
                  break;
               case WW      :
                  ++mode[m].mode;
                  if (mode[m].mode > 2)
                     mode[m].mode = 0;
                  break;
               case Date    :
                  ++mode[m].mode;
                  if (mode[m].mode > 5)
                     mode[m].mode = 0;
                  break;
               case Crlf    :
                  if (mode[m].mode == CRLF)
                     mode[m].mode = LF;
                  else
                     mode[m].mode = CRLF;
                  break;
               case Left    :
               case Para    :
                  if (mode[m].mode < mode[Right].mode)
                     ++mode[m].mode;
                  else
                     mode[m].mode = 0;
                  break;
               case Right   :
                  if (mode[m].mode < 1038)
                     ++mode[m].mode;
                  else {
                     if (mode[Left].mode < mode[Para].mode)
                        mode[m].mode = mode[Para].mode + 1;
                     else
                        mode[m].mode = mode[Left].mode + 1;
                  }
                  break;
               case Initcase   :
                  if (mode[m].mode == 1)
                     mode[m].mode = 2;
                  else
                     mode[m].mode = 1;
                  break;
            }
            (*mode[m].show_me)();
            break;
      }
      if (new_field) {
         color = 112;
         (*mode[m].show_me)();
         xygoto( mode[m].col, mode[m].line );
      }
   }

   /*
    * write changes to "tde.exe" if user presses F10.
    */
   if (c == F10) {
      in_modes.insert        = mode[Ins].mode;
      in_modes.indent        = mode[Ind].mode;
      in_modes.ptab_size     = mode[PTAB].mode;
      in_modes.ltab_size     = mode[LTAB].mode;
      in_modes.smart_tab     = mode[Smart].mode;
      in_modes.control_z     = mode[Write_Z].mode;
      in_modes.crlf          = mode[Crlf].mode;
      in_modes.trailing      = mode[Trim].mode;
      in_modes.show_eol      = mode[Eol].mode;
      in_modes.word_wrap     = mode[WW].mode;
      in_modes.left_margin   = mode[Left].mode;
      in_modes.parg_margin   = mode[Para].mode;
      in_modes.right_margin  = mode[Right].mode;
      in_modes.cursor_size   = mode[Size].mode;
      in_modes.do_backups    = mode[Backup].mode;
      in_modes.ruler         = mode[Ruler].mode;
      in_modes.date_style    = mode[Date].mode;
      in_modes.time_style    = mode[Time].mode;
      in_modes.search_case   = mode[Initcase].mode;
      fseek( tde_exe, mode_offset, SEEK_SET );
      fwrite( (void *)&in_modes, sizeof( MODE_INFO ), 1, tde_exe );
   }
   cls( );
}
