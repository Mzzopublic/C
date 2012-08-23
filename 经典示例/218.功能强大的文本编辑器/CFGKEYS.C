/*
 * This module contains all the routines needed to redefine the keys.
 *
 * Program Name:  tdecfg
 * Author:        Frank Davis
 * Date:          October 5, 1991
 */


#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tdecfg.h"
#include "cfgkeys.h"

extern struct vcfg cfg;
extern FILE *tde_exe;                  /* FILE pointer to tde.exe */
extern long keys_offset;

static WINDOW *w_ptr;


/********    EXTREMELY IMPORTANT   ************/
/*
 * If you modify tde, it is your responsibility to find the offset of
 * the key definition array in your new executable, tde.exe.
 *
 */


/*
 * Name:    tdekeys
 * Date:    October 1, 1991
 * Notes:   Set up most of the window global variables.
 */
void tdekeys( void )
{
HELP_WINDOW hw;
char t[80];
int  ch;
int  i;

   w_ptr = NULL;
   hw.dply_col = 2;
   hw.dply_row = 3;
   hw.line_length = 69;
   hw.avail_lines = 12;
   hw.v_row = 0;
   hw.select = 0;
   hw.num_entries = AVAIL_KEYS;
   hw.ulft_col = 3;
   hw.ulft_row = 2;
   hw.total_col = 73;
   hw.total_row = 21;

   initialize_keys( );
   master_help( &hw, key_defs, key_head, t, &ch );
   if (ch == F10) {
      for (i=0; i<hw.num_entries; i++)
         key_func.key[key_defs[i].key_index]  = key_defs[i].func_index;
      fseek( tde_exe, keys_offset, SEEK_SET );
      fwrite( (void *)&key_func, sizeof(KEY_FUNC), 1, tde_exe );
   }
   cls( );
}


/*
 * Name:    initialize_keys
 * Date:    October 1, 1991
 * Notes:   Get the current key definitions from the tde executable file.
 */
void initialize_keys( void )
{
int i;

   fseek( tde_exe, keys_offset, SEEK_SET );
   fread( (void *)&key_func, sizeof(KEY_FUNC), 1, tde_exe );
   for (i=0; i<AVAIL_KEYS; i++)
      key_defs[i].func_index = key_func.key[key_defs[i].key_index];
}


/*
 * Name:    show_key_def_list
 * Date:    October 1, 1991
 * Notes:   Show the key name and current function in a window.
 */
void show_key_def_list( HELP_WINDOW *hw, KEY_DEFS *keys )
{
int row, col, i, j;

   col = hw->ulft_col + hw->dply_col;
   row = hw->ulft_row + hw->dply_row;
   scroll_window( 0, row, col, row+hw->avail_lines-1, col+hw->line_length,
                  NORMAL );
   i = 0;
   j = hw->select - hw->v_row;
   for (; i < hw->avail_lines && j<hw->num_entries; ++i, j++, row++) {
      s_output( keys[j].key, row, col, NORMAL );
      s_output( avail_func[keys[j].func_index], row, col+31, NORMAL );
   }
}


/*
 * Name:    show_func_list
 * Date:    October 1, 1991
 * Notes:   Show the available functions in the small pop-up window.
 */
void show_func_list( HELP_WINDOW *hw, char *func[] )
{
int row, col, i, j;

   col = hw->ulft_col + hw->dply_col;
   row = hw->ulft_row + hw->dply_row;
   scroll_window( 0, row, col, row+hw->avail_lines-1, col+hw->line_length,
                  NORMAL );
   i = 0;
   j = hw->select - hw->v_row;
   for (; i < hw->avail_lines && j<hw->num_entries; ++i, j++, row++)
      s_output( func[j], row, col, NORMAL );
}


/*
 * Name:    position_cursor
 * Date:    October 1, 1991
 * Notes:   Given a key from the user, find out what action to do.
 */
void position_cursor( HELP_WINDOW *hw, int len, int *un, int *cc, int *ch )
{
int col, row, y, yy;

   col = hw->ulft_col + hw->dply_col;
   row = hw->ulft_row + hw->dply_row + hw->v_row;
   y  = hw->ulft_row + hw->dply_row;
   yy = hw->ulft_row + hw->dply_row + hw->avail_lines - 1;
   *un = FALSE;
   switch (*ch) {
      case UP    :
         if (hw->v_row > 0 && hw->select > 0) {
            hlight_line( col, row, len, NORMAL );
            --hw->v_row;
            --hw->select;
            *cc = TRUE;
         } else if (hw->v_row == 0 && hw->select > 0) {
            hlight_line( col, row, len, NORMAL );
            --hw->select;
            scroll_window( -1, y, col, yy+1, col+hw->line_length, NORMAL );
            *un = TRUE;
            *cc = TRUE;
         }
         break;
      case DOWN :
         if (hw->v_row < hw->avail_lines-1 &&
                                      hw->select < hw->num_entries-1) {
            hlight_line( col, row, len, NORMAL );
            ++hw->v_row;
            ++hw->select;
            *cc = TRUE;
         } else if (hw->v_row == hw->avail_lines-1 &&
                                      hw->select < hw->num_entries-1) {
            hlight_line( col, row, len, NORMAL );
            ++hw->select;
            scroll_window( 1, y, col, yy, col+hw->line_length, NORMAL );
            *un = TRUE;
            *cc = TRUE;
         } else if (hw->select == hw->num_entries - 1 && hw->v_row > 0) {
            --hw->v_row;
            scroll_window(1, y, col, yy, col+hw->line_length, NORMAL );
         }
         break;
   }
}


/*
 * Name:    master_help
 * Date:    October 1, 1991
 * Notes:   master routine for setting up a help window.
 */
void master_help( HELP_WINDOW *hw, KEY_DEFS *help, struct screen *help_heading,
                  char *t, int *ch )
{
FILE *fp;               /* file pointer for PRN device */
int col, row, i, j;
int update_name, change_color, draw_page;
char str[80];
char *blank = "                                                 ";
char temp[20];
HELP_WINDOW hw_k;

   xygoto( -1, -1 );
   save_and_draw( hw, help_heading, &w_ptr );
   show_key_def_list( hw, help );

   col = hw->ulft_col + hw->dply_col;
   row = hw->ulft_row + hw->dply_row + hw->v_row;
   change_color = TRUE;
   *ch = 0;
   while (*ch!=F3 && *ch != F10 && *ch!=ESC) {
      draw_page = FALSE;
      position_cursor( hw, hw->line_length, &update_name, &change_color, ch );
      switch (*ch) {
         case PGUP :
            if (hw->select > hw->avail_lines-1) {
               hw->select = hw->select - hw->avail_lines;
               if (hw->v_row > hw->select)
                  hw->select = hw->v_row;
               draw_page = TRUE;
            } else if (hw->select - hw->v_row > 0) {
               hw->select = hw->v_row;
               draw_page = TRUE;
            }
            break;
         case PGDN :
            if (hw->select + hw->avail_lines < hw->num_entries) {
               hw->select = hw->select + hw->avail_lines;
               draw_page = TRUE;
            } else if (hw->select + hw->avail_lines - hw->v_row <
                                                         hw->num_entries) {
               hw->select = hw->num_entries - 1;
               draw_page = TRUE;
            }
            if ((hw->num_entries - 1) - hw->select <
                                              hw->avail_lines && draw_page) {
               i = row - hw->v_row;
               scroll_window( 0, i, col, i+hw->avail_lines-1,
                              col+hw->line_length, NORMAL );
            }
            break;
         case F5 :
            hw_k.dply_col = 2;
            hw_k.dply_row = 3;
            hw_k.line_length = 33;
            hw_k.avail_lines = 6;
            hw_k.v_row = 0;
            hw_k.select = 0;
            hw_k.num_entries = NUM_FUNC;
            hw_k.ulft_col = 20;
            hw_k.ulft_row = row + 1;
            if (hw_k.ulft_row > 12)
               hw_k.ulft_row = row - 12;
            hw_k.total_col = 37;
            hw_k.total_row = 12;
            new_assignment_help( &hw_k, avail_func, func_head, ch );
            if (*ch == RTURN) {
               help[hw->select].key[0] = '*';
               help[hw->select].func_index = (unsigned char)hw_k.select;
               s_output( help[hw->select].key, row, col, cfg.attr );
               s_output( "                     ", row, col+31, cfg.attr );
               s_output( avail_func[help[hw->select].func_index],
                         row, col+31, cfg.attr );
               *ch = 0;
            }
            break;
         case F7 :
            hw_k.ulft_col = 14;
            hw_k.ulft_row = 10;
            hw_k.total_col = 52;
            hw_k.total_row = 5;
            save_and_draw( &hw_k, file_head, &w_ptr );
            xygoto( hw_k.ulft_col+28, hw_k.ulft_row+2 );
            gets( str );
            j = TRUE;
            if ((i = access( str, EXIST )) == 0) {
               s_output( blank, hw_k.ulft_row+2, hw_k.ulft_col+1, NORMAL );
               s_output( "OK to overwrite exiting file (y/n)?",
                          hw_k.ulft_row+2, hw_k.ulft_col+1, NORMAL );
               xygoto( hw_k.ulft_col+38, hw_k.ulft_row+2 );
               i = getkey( );
               while (i != 'Y' && i != 'y' && i != 'N' && i != 'n')
                  i = getkey( );
               if (i == 'n' || i == 'N')
                  j = FALSE;
            }
            xygoto( -1, -1 );
            if (j == TRUE && (fp = fopen( str, "w" )) != NULL) {
               for (i=0; i<hw->num_entries && j; i++) {
                  fprintf( fp, " %22s  = ", help[i].key+3 );
                  fprintf( fp, "  %s\n",  avail_func[help[i].func_index] );
               }
               fclose( fp );
            } else {
               s_output( blank, hw_k.ulft_row+2, hw_k.ulft_col+1, NORMAL );
               s_output( "Cannot open file.  Press any key to contine.",
                          hw_k.ulft_row+2, hw_k.ulft_col+1, NORMAL );
               j = getkey( );
            }
            window_control( &w_ptr, RESTORE, hw_k.ulft_col, hw_k.ulft_row,
                            hw_k.total_col, hw_k.total_row );
            break;
         case F8 :
            if ((fp = fopen( "PRN", "a" )) != NULL) {
               hw_k.ulft_col = 20;
               hw_k.ulft_row = 10;
               hw_k.total_col = 42;
               hw_k.total_row = 5;
               save_and_draw( &hw_k, print_head, &w_ptr );
               j = TRUE;
               for (i=0; i<hw->num_entries && j; i++) {
                  itoa( i+1, temp, 10 );
                  s_output( temp, hw_k.ulft_row+2, hw_k.ulft_col+16, NORMAL );
                  fprintf( fp, " %22s  = ", help[i].key+3 );
                  fprintf( fp, "  %s\n",  avail_func[help[i].func_index] );
                  if (kbhit()) {
                     j = getkey( );
                     if (j == ESC)
                        j = FALSE;
                     else
                        j = TRUE;
                  }
               }
               fprintf( fp, "\f" );
               fclose( fp );
               window_control( &w_ptr, RESTORE, hw_k.ulft_col, hw_k.ulft_row,
                               hw_k.total_col, hw_k.total_row );
            }
            break;
      }
      if (draw_page == TRUE) {
         show_key_def_list( hw, help );
         update_name = TRUE;
         change_color = TRUE;
      }
      row = hw->ulft_row + hw->dply_row + hw->v_row;
      if (update_name) {
         s_output( help[hw->select].key, row, col, NORMAL );
         s_output( avail_func[help[hw->select].func_index], row, col+31,
                      NORMAL );
      }
      if (change_color)
         hlight_line( col, row, hw->line_length, cfg.attr );
      *ch = getkey( );
      change_color = FALSE;
   }
   window_control( &w_ptr, RESTORE, hw->ulft_col, hw->ulft_row,
                   hw->total_col, hw->total_row );
}


/*
 * Name:    new_assignment_help
 * Date:    October 1, 1991
 * Notes:   customized the master_help routine for the available functions.
 */
void new_assignment_help( HELP_WINDOW *hw, char *help[],
                          struct screen *help_heading, int *ch )
{
int col, row, i;
int update_name, change_color, draw_page;

   save_and_draw( hw, help_heading, &w_ptr );
   show_func_list( hw, help );

   col = hw->ulft_col + hw->dply_col;
   row = hw->ulft_row + hw->dply_row + hw->v_row;
   change_color = TRUE;
   *ch = 0;
   while (*ch!=RTURN && *ch!=ESC) {
      draw_page = FALSE;
      position_cursor( hw, hw->line_length, &update_name, &change_color, ch );
      switch (*ch) {
         case PGUP :
            if (hw->select > hw->avail_lines-1) {
               hw->select = hw->select - hw->avail_lines;
               if (hw->v_row > hw->select)
                  hw->select = hw->v_row;
               draw_page = TRUE;
            } else if (hw->select - hw->v_row > 0) {
               hw->select = hw->v_row;
               draw_page = TRUE;
            }
            break;
         case PGDN :
            if (hw->select + hw->avail_lines < hw->num_entries) {
               hw->select = hw->select + hw->avail_lines;
               draw_page = TRUE;
            } else if (hw->select + hw->avail_lines - hw->v_row <
                                                         hw->num_entries) {
               hw->select = hw->num_entries - 1;
               draw_page = TRUE;
            }
            if ((hw->num_entries - 1) - hw->select <
                                              hw->avail_lines && draw_page) {
               i = row - hw->v_row;
               scroll_window( 0, i, col, i+hw->avail_lines-1,
                              col+hw->line_length, NORMAL );
            }
            break;
      }
      if (draw_page == TRUE) {
         show_func_list( hw, help );
         update_name = TRUE;
         change_color = TRUE;
      }
      row = hw->ulft_row + hw->dply_row + hw->v_row;
      if (update_name)
         s_output( help[hw->select], row, col, NORMAL );
      if (change_color)
         hlight_line( col, row, hw->line_length, cfg.attr );
      *ch = getkey( );
      change_color = FALSE;
   }
   window_control( &w_ptr, RESTORE, hw->ulft_col, hw->ulft_row,
                   hw->total_col, hw->total_row );
   if (*ch != RTURN)
      *ch = 0;
}


/*
 * Name:    save_and_draw
 * Date:    October 1, 1991
 * Notes:   save the contents of the screen where the pop-up or pull-down
 *          window is to be displayed.  then draw the outline of the window
 *          and show the help.
 */
void save_and_draw( HELP_WINDOW *hw, struct screen *help_heading,
                    WINDOW **w_ptr )
{
   window_control( w_ptr, SAVE, hw->ulft_col, hw->ulft_row,
                   hw->total_col, hw->total_row );
   make_window( hw->ulft_col, hw->ulft_row, hw->total_col, hw->total_row,
                NORMAL );
   show_box( hw->ulft_col, hw->ulft_row, help_heading, NORMAL );
}
