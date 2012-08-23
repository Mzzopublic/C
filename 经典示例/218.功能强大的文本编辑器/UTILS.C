

#include "tdestr.h"
#include "common.h"
#include "define.h"
#include "tdefunc.h"


/*
 * Name:    myiswhitespc
 * Purpose: To determine whether or not a character is *NOT* part of a "word".
 * Date:    July 4, 1992
 * Passed:  c: the character to be tested
 * Returns: TRUE if c is in the character set *NOT* part of a word
 * Notes:   The characters in the set not part of a word will not change as
 *           as much as the characters that are part of a word.  In most
 *           languages, human and computer, the delimiters are much more
 *           common than the tokens that make up a word.  For example,
 *           the set of punction characters don't change as much across
 *           languages, human and computer, as the characters that make
 *           up the alphabet, usually.  In other words, the delimiters
 *           are fairly constant across languages.
 */
int  myiswhitespc( int c )
{
   return( c == ' ' || (ispunct( c ) && c != '_') || iscntrl( c ) );
}


/*
 * Name:    check_virtual_col
 * Purpose: ensure integrity of rcol, ccol, and bcol
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          rcol: real column of cursor
 *          ccol: current or logical column of cursor
 */
void check_virtual_col( WINDOW *window, int rcol, int ccol )
{
register int bcol;
int  start_col;
int  end_col;
file_infos *file;

   file      = window->file_info;
   bcol      = window->bcol;
   start_col = window->start_col;
   end_col   = window->end_col;

   /*
    * is logical column past end of screen?
    */
   if (ccol > end_col) {
/*      ccol = start_col + (end_col + 1 - start_col) / 2;  */
      ccol = end_col;
      bcol = rcol - (ccol - start_col);
      file->dirty = LOCAL;
   }

   /*
    * is logical column behind start of screen?
    */
   if (ccol < start_col) {
      if (bcol >= (start_col - ccol))
         bcol -= (start_col - ccol);
      ccol = start_col;
      file->dirty = LOCAL;
   }

   /*
    * is real column < base column?
    */
   if (rcol < bcol) {
      ccol = rcol + start_col;
      bcol = 0;
      if (ccol > end_col) {
         bcol = rcol;
         ccol = start_col;
      }
      file->dirty = LOCAL;
   }

   /*
    * current column + base column MUST equal real column
    */
   if ((ccol - start_col) + bcol != rcol) {
      if (bcol < 0 || bcol > rcol) {
         bcol = rcol;
         file->dirty = LOCAL;
      }
      ccol = rcol - bcol + start_col;
      if (ccol > end_col) {
         bcol = rcol;
         ccol = start_col;
         file->dirty = LOCAL;
      }
   }

   /*
    * rcol CANNOT be negative
    */
   if (rcol < 0) {
      rcol = bcol = 0;
      ccol = start_col;
      file->dirty = LOCAL;
   }

   if (rcol >= MAX_LINE_LENGTH) {
      rcol = MAX_LINE_LENGTH - 1;
      bcol = rcol - (ccol - start_col);
   }

   assert( rcol >= 0 );
   assert( rcol < MAX_LINE_LENGTH );
   assert( bcol >= 0 );
   assert( bcol < MAX_LINE_LENGTH );
   assert( ccol >= start_col );
   assert( ccol <= end_col );

   window->bcol = bcol;
   window->ccol = ccol;
   window->rcol = rcol;
}


/*
 * Name:    copy_line
 * Purpose: To copy the cursor line, if necessary, into the current line
 *           buffer, so that changes can be made efficiently.
 * Date:    June 5, 1991
 * Passed:  text_line: line to be copied to line buffer
 *          line: line to display error message
 * Notes:   See un_copy_line, the reverse operation.
 *          DO NOT use the C library string functions on text in
 *           g_status.line_buff, because Null characters are allowed as
 *           normal text in the file.
 */
void copy_line( line_list_ptr ll )
{
register unsigned int len;
text_ptr text_line;

   if (g_status.copied == FALSE  &&  ll->len != EOF) {

      assert( ll != NULL );

      len = ll->len;
      text_line = ll->line;
      g_status.buff_node = ll;

      assert( len < MAX_LINE_LENGTH );

      if (text_line != NULL)
         _fmemcpy( g_status.line_buff, text_line, len );

      g_status.line_buff_len = len;
      g_status.copied = TRUE;
   }
}


/*
 * Name:    un_copy_line
 * Purpose: To copy the cursor line, if necessary, from the current line
 *           buffer, shifting the main text to make the right amount of
 *           room.
 * Date:    June 5, 1991
 * Passed:  test_line:  location in file to copy line buffer
 *          window:  pointer to current window
 *          del_trailing:  delete the trailing blanks at eol? TRUE or FALSE
 * Notes:   For some functions, trailing spaces should not be removed when
 *           returning the line buffer to the main text.  The JoinLine function
 *           is a good example.  We need to leave trailing space so when we
 *           join lines - the current line will extend at least up to
 *           the column of the cursor.  We need to leave trailing space
 *           during BOX block operations.
 *          See copy_line, the reverse operation.
 */
int  un_copy_line( line_list_ptr ll, WINDOW *window, int del_trailing )
{
text_ptr p;
size_t len;     /* length of line buffer text */
size_t ll_len;  /* length of ll->line */
int  net_change;
int  rc;
char c;
file_infos *file;
WINDOW *wp;

   rc = OK;
   if (mode.do_backups == TRUE)
      rc = backup_file( window );

   if (g_status.copied == TRUE  &&  ll->len != EOF) {

      file = window->file_info;

      /*
       * if we are deleting the entire line, don't worry about the
       *  deleting the trailing space, since we're deleting entire line.
       */
      if (g_status.command == DeleteLine)
         del_trailing = FALSE;

      if (del_trailing  &&  mode.trailing  &&  file->crlf != BINARY) {
         len = g_status.line_buff_len;
         for (p=(text_ptr)(g_status.line_buff+len); len > 0; len--, p--) {
            c = *(p - 1);
            if (c != ' '  &&  c != '\t')
               break;
            if (!mode.inflate_tabs && c == '\t')
               break;
         }
         g_status.line_buff_len = len;
         file->dirty = GLOBAL;
         if (window->visible == TRUE)
            show_changed_line( window );
      }
      len = g_status.line_buff_len;
      ll_len =  (ll->line == NULL) ? 0 : ll->len;


      assert( len < MAX_LINE_LENGTH );
      assert( ll_len < MAX_LINE_LENGTH );

      net_change = len - ll_len;

      if (ll_len != len  ||  ll->line == NULL) {
         /*
          * let malloc space for the new line before we free the old line.
          */
         p = my_malloc( len, &rc );
         if (rc == ERROR)
            error( WARNING, window->bottom_line, main4 );

         /*
          * free the space taken up by current line in far heap.
          */
         if (rc != ERROR  &&  ll->line != NULL)
            my_free( ll->line );
      } else
         p = ll->line;

      if (rc != ERROR) {
         if (len > 0)
            _fmemcpy( p, g_status.line_buff, len );
         ll->line = p;
         ll->len = len;

         if (net_change != 0) {
            for (wp=g_status.window_list; wp != NULL; wp=wp->next) {
               if (wp->file_info == file && wp != window)
                  if (wp->rline > window->rline)
                     wp->bin_offset += net_change;
            }
         }

         file->modified = TRUE;
         show_avail_mem( );
      }
   }
   g_status.copied = FALSE;
   return( rc );
}


/*
 * Name:    un_copy_tab_buffer
 * Purpose: To copy the tab buffer line the main text buffer
 * Date:    October 31, 1992
 * Passed:  line_number:  line number to copy line tab out buffer
 *          window:       pointer to current window
 */
int  un_copy_tab_buffer( line_list_ptr ll, WINDOW *window )
{
text_ptr p;
int  len;               /* length of current line buffer text */
int  net_change;
int  rc;
file_infos *file;
WINDOW *wp;

   rc = OK;
   file = window->file_info;
   /*
    * file has changed.  lets create the back_up if needed
    */
   if (mode.do_backups == TRUE) {
      window->file_info->modified = TRUE;
      rc = backup_file( window );
   }

   len = g_status.tabout_buff_len;

   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );
   assert( ll->len >= 0 );
   assert( ll->len < MAX_LINE_LENGTH );

   /*
    * if the far heap has run out of space, then only part of the
    *  current line can be moved back into the far heap. Warn the user
    *  that some of the current line has been lost.
    */
   p = my_malloc( len, &rc );
   if (rc == ERROR)
      error( WARNING, window->bottom_line, main4 );

   if (rc == OK) {
      net_change = len - ll->len;

      if (ll->line != NULL)
         my_free( ll->line );
      if (len > 0)
         _fmemcpy( p, g_status.line_buff, len );
      ll->line = p;
      ll->len  = len;

      if (net_change != 0) {
         for (wp=g_status.window_list; wp != NULL; wp=wp->next) {
            if (wp->file_info == file && wp != window)
               if (wp->rline > window->rline)
                  wp->bin_offset += net_change;
         }
      }

      file->modified = TRUE;
   }
   return( rc );
}


/*
 * Name:    load_undo_buffer
 * Purpose: To copy the cursor line to the undo buffer.
 * Date:    September 26, 1991
 * Passed:  file:          pointer to file
 *          line_to_undo:  pointer to line in file to save
 * Notes:   save the last mode.undo_max lines in a stack.  when we overflow
 *           the stack, dump the oldest line.
 */
void load_undo_buffer( file_infos *file, text_ptr line_to_undo, int len )
{
int  rc;
text_ptr l;
line_list_ptr temp_ll;

   rc = OK;
   if (file->undo_count >= mode.undo_max) {
      --file->undo_count;
      temp_ll = file->undo_bot->prev;
      temp_ll->prev->next = file->undo_bot;
      file->undo_bot->prev = temp_ll->prev;
      if (temp_ll->line != NULL)
         my_free( temp_ll->line );
   } else
      temp_ll = (line_list_ptr)my_malloc( sizeof(line_list_struc), &rc );

   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );

   l = my_malloc( len, &rc );

   if (rc == ERROR) {
      if (l != NULL)
         my_free( l );
      if (temp_ll != NULL)
         my_free( temp_ll );
   } else {
      if (len > 0)
         _fmemcpy( l, line_to_undo, len );
      temp_ll->line  = l;
      temp_ll->len   = len;
      temp_ll->dirty = TRUE;

      temp_ll->prev = NULL;
      temp_ll->next = file->undo_top;
      file->undo_top->prev = temp_ll;
      file->undo_top = temp_ll;

      ++file->undo_count;
   }
}


/*
 * Name:    set_prompt
 * Purpose: To display a prompt, highlighted, at the bottom of the screen.
 * Date:    October 1, 1989
 * Passed:  prompt: prompt to be displayed
 *          line:   line to display prompt
 */
void set_prompt( char *prompt, int line )
{
register int prompt_col;

   /*
    * work out where the answer should go
    */
   prompt_col = strlen( prompt );

   assert( prompt_col <= MAX_COLS );

   /*
    * output the prompt
    */
   s_output( prompt, line, 0, g_display.message_color );
   eol_clear( prompt_col, line, g_display.message_color );

   /*
    * put cursor at end of prompt
    */
   xygoto( prompt_col, line );
}


/*
 * Name:    get_name
 * Purpose: To prompt the user and read the string entered in response.
 * Date:    June 5, 1992
 * Passed:  prompt: prompt to offer the user
 *          line:   line to display prompt
 *          name:   default answer
 *          color:  color to display prompt
 * Returns: name:   user's answer
 *          OK if user entered something
 *          ERROR if user aborted the command
 * Notes:   with the addition of macros in tde, this function became a little
 *           more complicated.  we have to deal with both executing macros
 *           and macros that are the user uses when entering normal text
 *           at the prompt.  i call these local and global macros.  a global
 *           macro is when this function is called from a running macro.
 *           the running macro can enter text and return from this function
 *           w/o any action from the user.  a local macro is when the user
 *           presses a key inside this function, which happens quite often
 *           when keys are assigned to ASCII and Extended ASCII characters.
 */
int  get_name( char *prompt, int line, char *name, int color )
{
int  col;               /* cursor column for answer */
int  c;                 /* character user just typed */
char *cp;               /* cursor position in answer */
char *answer;           /* user's answer */
int first = TRUE;       /* first character typed */
register int len;       /* length of answer */
int  plen;              /* length of prompt */
int  func;              /* function of key pressed */
int  stop;              /* flag to stop getting characters */
char *p;                /* for copying text in answer */
char buffer[MAX_COLS+2];/* line on which name is being entered */
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
int  normal;
int  local_macro = FALSE;
int  next;
int  regx_help_on;
char **pp;
int  i;

   /*
    * set up prompt and default
    */
   assert( strlen( prompt ) < MAX_COLS );
   assert( strlen( name )   < MAX_COLS );

   strcpy( buffer, prompt );
   plen = strlen( prompt );
   answer = buffer + plen;
   strcpy( answer, name );

   /*
    * let user edit default string
    */
   regx_help_on = FALSE;
   len = strlen( answer );
   col = strlen( buffer );
   g_status.prompt_line = line;
   g_status.prompt_col = col;
   cp = answer + len;
   normal = g_display.text_color;
   save_screen_line( 0, line, line_buff );
   s_output( buffer, line, 0, color );
   eol_clear( col, line, normal );
   for (stop = FALSE; stop == FALSE;) {
      if (regx_help_on == TRUE)
         xygoto( -1, -1 );
      else
         xygoto( col, line );
      if (g_status.macro_executing) {
         next = g_status.macro_next;
         g_status.macro_next = macro.strokes[g_status.macro_next].next;
         if (g_status.macro_next != -1) {
            c = macro.strokes[g_status.macro_next].key;
            func = getfunc( c );
            if (func == PlayBack) {
               stop = TRUE;
               g_status.macro_next = next;
            }
         } else {
            c = 0x100;
            func = AbortCommand;
            stop = TRUE;
         }
      } else {
         if (local_macro == FALSE) {
            c = getkey( );
            func = getfunc( c );

            /*
             * User may have redefined the Enter and ESC keys.  Make the Enter
             *  key perform a Rturn in this function. Make the ESC key do an
             *  AbortCommand.
             */
            if (c == RTURN)
               func = Rturn;
            else if (c == ESC)
               func = AbortCommand;

            if (func == PlayBack) {
               local_macro = TRUE;
               next = macro.first_stroke[ c-256 ];
               c = macro.strokes[next].key;
               func = getfunc( c );
               next = macro.strokes[next].next;
            } else {
               g_status.key_pressed = c;
               record_keys( line );
            }
         } else {
            if (next != -1) {
               c = macro.strokes[next].key;
               next = macro.strokes[next].next;
            } else {
               local_macro = FALSE;
               c = 0x100;
            }
            func = getfunc( c );
         }
      }
      if (c == _F1)
         func = Help;
      if (regx_help_on == TRUE  &&  g_status.current_window != NULL) {
         redraw_screen( g_status.current_window );
         s_output( buffer, line, 0, color );
         eol_clear( col, line, normal );
         s_output( cp, line, col, color );
         regx_help_on = FALSE;
      } else {
         switch (func) {
            case Help :
               if ((g_status.command == FindRegX  ||
                    g_status.command == RepeatFindRegX) &&
                    regx_help_on == FALSE) {
                  regx_help_on = TRUE;
                  for (i=3,pp=regx_help; *pp != NULL; pp++, i++)
                     s_output( *pp, i, 12, g_display.help_color );
               }
               break;
            case ToggleSearchCase :
               mode.search_case = mode.search_case == IGNORE ? MATCH : IGNORE;
               build_boyer_array( );
               show_search_case( );
               break;
            case Rturn       :
            case NextLine    :
            case BegNextLine :
               answer[len] = '\0';
               assert( strlen( answer ) < MAX_COLS );
               strcpy( name, answer );
               /*
                * finished
                */
               stop = TRUE;
               break;
            case BackSpace :
               /*
                * delete to left of cursor
                */
               if (cp > answer) {
                  for (p=cp-1; p < answer+len; p++) {
                     *p = *(p+1);
                  }
                  --len;
                  --col;
                  --cp;
                  c_output( ' ', plen+len, line, normal );
                  s_output( cp, line, col, color );
                  *(answer + len) = '\0';
               }
               break;
            case DeleteChar :
               /*
                * delete char under cursor
                */
               if (*cp) {
                  for (p=cp; p < answer+len; p++) {
                     *p = *(p+1);
                  }
                  --len;
                  c_output( ' ', plen+len, line, normal );
                  s_output( cp, line, col, color );
                  *(answer + len) = '\0';
               }
               break;
            case DeleteLine :
               /*
                * delete current line
                */
               col = plen;
               cp = answer;
               *cp = '\0';
               len = 0;
               eol_clear( col, line, normal );
               break;
            case AbortCommand :
               stop = TRUE;
               break;
            case CharLeft :
               /*
                * move cursor left
                */
               if (cp > answer) {
                  col--;
                  cp--;
               }
               break;
            case CharRight :
               /*
                * move cursor right
                */
               if (*cp) {
                  col++;
                  cp++;
                }
                break;
            case BegOfLine :
               /*
                * move cursor to start of line
                */
               col = plen;
               cp = answer;
               break;
            case EndOfLine :
               /*
                * move cursor to end of line
                */
               col = plen + len;
               cp = answer + len;
               break;
            default :
               if (c < 0x100) {
                  /*
                   * insert character at cursor
                   */
                  if (first) {
                     /*
                      * delete previous answer
                      */
                     col = plen;
                     cp = answer;
                     *cp = '\0';
                     len = 0;
                     eol_clear( col, line, normal );
                  }

                  /*
                   * insert new character
                   */
                  if (col < g_display.ncols-1) {
                     if (*cp == '\0') {
                        ++len;
                        *(answer + len) = '\0';
                     }
                     *cp = (char)c;
                     c_output( c, col, line, color );
                     ++cp;
                     ++col;
                  }
               }
               break;
         }
      }
      first = FALSE;
   }
   restore_screen_line( 0, line, line_buff );
   return( func == AbortCommand ? ERROR : OK );
}


/*
 * Name:    get_sort_order
 * Purpose: To prompt the user and get sort direction
 * Date:    June 5, 1992
 * Passed:  window
 * Returns: OK if user entered something
 *          ERROR if user aborted the command
 */
int  get_sort_order( WINDOW *window )
{
register int c;
int  col;
char line_buff[(MAX_COLS+1)*2];         /* buffer for char and attribute  */

   save_screen_line( 0, window->bottom_line, line_buff );
   /*
    * sort ascending or descending
    */
   s_output( utils4, window->bottom_line, 0, g_display.message_color );
   c = strlen( utils4 );
   eol_clear( c, window->bottom_line, g_display.text_color );
   xygoto( c, window->bottom_line );
   do {
      c = getkey( );
      col = getfunc( c );
      if (c == ESC)
         col = AbortCommand;
   } while (col != AbortCommand  &&  c != 'A'  &&  c != 'a'  &&
            c != 'D'  &&  c != 'd');
   switch ( c ) {
      case 'A' :
      case 'a' :
         sort.direction = ASCENDING;
         break;
      case 'D' :
      case 'd' :
         sort.direction = DESCENDING;
         break;
      default  :
         col = AbortCommand;
         break;
   }
   restore_screen_line( 0, window->bottom_line, line_buff );
   return( col == AbortCommand ? ERROR : OK );
}


/*
 * Name:    get_replace_direction
 * Purpose: To prompt the user and get replace string direction
 * Date:    October 31, 1992
 * Passed:  window
 * Returns: OK if user entered something
 *          ERROR if user aborted the command
 */
int  get_replace_direction( WINDOW *window )
{
register int c;
int  col;
char line_buff[(MAX_COLS+1)*2];         /* buffer for char and attribute  */

   save_screen_line( 0, window->bottom_line, line_buff );
   /*
    * replace forward or backward
    */
   s_output( utils5, window->bottom_line, 0, g_display.message_color );
   c = strlen( utils5 );
   eol_clear( c, window->bottom_line, g_display.text_color );
   xygoto( c, window->bottom_line );
   do {
      c = getkey( );
      col = getfunc( c );
      if (c == ESC)
         col = AbortCommand;
   } while (col != AbortCommand  &&  c != 'F'  &&  c != 'f'  &&
            c != 'B'  &&  c != 'b');
   switch ( c ) {
      case 'F' :
      case 'f' :
         c = FORWARD;
         break;
      case 'B' :
      case 'b' :
         c = BACKWARD;
         break;
      default  :
         c = ERROR;
   }
   restore_screen_line( 0, window->bottom_line, line_buff );
   return( col == AbortCommand ? ERROR : c );
}


/*
 * Name:    get_yn
 * Purpose: To input a response of yes or no.
 * Date:    October 1, 1989
 * Returns: the user's answer.  A_??? - see tdestr.h
 */
int  get_yn( void )
{
int  c;                 /* the user's response */
register int rc;        /* return code */

   do {
      c = getkey( );
      rc = getfunc( c );
      if (c== ESC)
         rc = AbortCommand;
   } while (rc != AbortCommand  &&  c != 'Y'  &&  c != 'y'  &&
            c != 'N'  &&  c != 'n');
   if (rc == AbortCommand || c == ESC)
      rc = ERROR;
   else {
      switch ( c ) {
         case 'Y' :
         case 'y' :
            rc = A_YES;
            break;
         case 'N' :
         case 'n' :
            rc = A_NO;
            break;
      }
   }
   return( rc );
}


/*
 * Name:    get_lr
 * Purpose: To input a response of yes or no.
 * Date:    June 1, 1991
 * Returns: the user's answer, LEFT or RIGHT.
 */
int  get_lr( void )
{
int  c;                 /* the user's response */
register int rc;        /* return code */

   for (rc=OK; rc == OK;) {
      c = getkey( );
      if (getfunc( c ) == AbortCommand || c == ESC)
         rc = ERROR;
      else {
         switch ( c ) {
            case 'L' :
            case 'l' :
               rc = LEFT;
               break;
            case 'R' :
            case 'r' :
               rc = RIGHT;
               break;
         }
      }
   }
   return( rc );
}


/*
 * Name:    get_bc
 * Purpose: To input a response of beginning or current cursor postion
 * Date:    October 31, 1992
 * Returns: the user's answer, Beginning or Current.
 */
int  get_bc( void )
{
int  c;                 /* the user's response */
register int rc;        /* return code */

   for (rc=OK; rc == OK;) {
      c = getkey( );
      if (getfunc( c ) == AbortCommand || c == ESC)
         rc = ERROR;
      else {
         switch ( c ) {
            case 'B' :
            case 'b' :
               rc = BEGINNING;
               break;
            case 'C' :
            case 'c' :
               rc = CURRENT;
               break;
         }
      }
   }
   return( rc );
}


/*
 * Name:    get_oa
 * Purpose: To input a response of overwrite or append.
 * Date:    October 1, 1989
 * Returns: the user's answer.  A_??? - see tdestr.h
 */
int  get_oa( void )
{
int  c;                 /* the user's response */
register int rc;        /* return code */
int  func;

   rc = 0;
   while (rc != AbortCommand && rc != A_OVERWRITE && rc != A_APPEND) {
      c = getkey( );
      func = getfunc( c );
      if (func == AbortCommand || c == ESC)
         rc = AbortCommand;
      switch ( c ) {
         case 'O' :
         case 'o' :
            rc = A_OVERWRITE;
            break;
         case 'A' :
         case 'a' :
            rc = A_APPEND;
            break;
      }
   }
   return( rc );
}


/*
 * Name:    show_eof
 * Purpose: display eof message
 * Date:    September 16, 1991
 * Notes:   line:  ususally, line to is display "<=== eof ===>"
 */
void show_eof( WINDOW *window )
{
register int color;
char temp[MAX_COLS+2];

   assert( strlen( mode.eof ) < MAX_COLS );

   strcpy( temp, mode.eof );
   color = window->end_col + 1 - window->start_col;
   if (strlen( temp ) > (unsigned)color)
      temp[color] = '\0';
   color = g_display.eof_color;
   window_eol_clear( window, color );
   s_output( temp, window->cline, window->start_col, color );
}


/*
 * Name:    display_current_window
 * Purpose: display text in current window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   use a temporary window structure, "w", to do the dirty work.
 */
void display_current_window( WINDOW *window )
{
int  count;     /* number of lines updated so far */
int  number;    /* number of lines visible in window */
register int i; /* register variable */
WINDOW w;       /* scratch window structure */
int  curl;      /* current line on screen, window->cline */
int  eof;

   /*
    * initialize the scratch variables
    */
   number = window->bottom_line - ((window->top_line + window->ruler) - 1);
   count  = window->cline - (window->top_line + window->ruler);
   dup_window_info( &w, window );

   w.cline -= count;
   w.rline -= count;
   for (eof=count; eof > 0; eof--)
      w.ll = w.ll->prev;


   /*
    * start at the top of the window and display a window full of text
    */
   eof = FALSE;
   curl = window->cline;
   for (i=number; i>0; i--) {
      if (w.ll->len != EOF) {
         /*
          * if this is window->cline, do not show the line because we
          *  show the curl at the end of this function.  don't show it twice
          */
         if (w.cline != curl)
            update_line( &w );
         w.ll = w.ll->next;
      } else if (eof == FALSE) {
         show_eof( &w );
         eof = TRUE;
      } else
         window_eol_clear( &w, COLOR_TEXT );
      ++w.cline;
      ++w.rline;
   }
   show_asterisk( window );
   show_curl_line( window );
}


/*
 * Name:    redraw_screen
 * Purpose: display all visible windows, modes, and headers
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  redraw_screen( WINDOW *window )
{
register WINDOW *above;        /* window above current */
register WINDOW *below;        /* window below current */

   cls( );
   /*
    * display the current window
    */
   redraw_current_window( window );

   /*
    * now update all the other windows
    */
   above = below = window;
   while (above->prev || below->next) {
      if (above->prev) {
         above = above->prev;
         redraw_current_window( above );
      }
      if (below->next) {
         below = below->next;
         redraw_current_window( below );
      }
   }
   window->file_info->dirty = FALSE;
   show_modes( );
   return( OK );
}


/*
 * Name:    redraw_current_window
 * Purpose: redraw all info in window
 * Date:    July 13, 1991
 * Passed:  window:  pointer to current window
 */
void redraw_current_window( WINDOW *window )
{

   /*
    * display the current window
    */
   if (window->visible) {
      display_current_window( window );
      show_window_header( window );
      show_ruler( window );
      show_ruler_pointer( window );
      if (window->vertical)
         show_vertical_separator( window );
   }
}


/*
 * Name:    show_changed_line
 * Purpose: Only one line was changed in file, just show it
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_changed_line( WINDOW *window )
{
WINDOW *above;                  /* window above current */
WINDOW *below;                  /* window below current */
WINDOW w;                       /* scratch window structure */
long changed_line;              /* line number in file that was changed */
long top_line, bottom_line;     /* top and bottom line in file on screen */
int  line_on_screen;            /* is changed line on screen? */
file_infos *file;               /* file pointer */

   file = window->file_info;
   if ((file->dirty == LOCAL || file->dirty == GLOBAL) && window->visible)
      show_curl_line( window );
   changed_line = window->rline;

   /*
    * now update the line in all other windows
    */
   if (file->dirty != LOCAL) {
      above = below = window;
      while (above->prev || below->next) {
         if (above->prev) {
            above = above->prev;
            dup_window_info( &w, above );
         } else if (below->next) {
            below = below->next;
            dup_window_info( &w, below );
         }

         /*
          * is this window the changed file and is it visible?
          */
         if (w.file_info == file && w.visible) {

            /*
             * calculate file lines at top and bottom of screen.
             * the changed line may not be the curl in other windows.
             */
            line_on_screen = FALSE;
            top_line = w.rline - (w.cline - (w.top_line + w.ruler));
            bottom_line = w.rline + (w.bottom_line - w.cline);
            if (changed_line == w.rline)
               line_on_screen = CURLINE;
            else if (changed_line < w.rline && changed_line >= top_line) {
               line_on_screen = NOTCURLINE;
               while (w.rline > changed_line) {
                  w.ll = w.ll->prev;
                  --w.rline;
                  --w.cline;
               }
            } else if (changed_line > w.rline && changed_line <= bottom_line) {
               line_on_screen = NOTCURLINE;
               while (w.rline < changed_line) {
                  w.ll = w.ll->next;
                  ++w.rline;
                  ++w.cline;
               }
            }

            /*
             * display the changed line if on screen
             */
            if (line_on_screen == NOTCURLINE)
               update_line( &w );
            else if (line_on_screen == CURLINE)
               show_curl_line( &w );
         }
      }
   }
   file->dirty = FALSE;
}


/*
 * Name:    show_curl_line
 * Purpose: show current line in curl color
 * Date:    January 16, 1992
 * Passed:  window:  pointer to current window
 */
void show_curl_line( WINDOW *window )
{
int  text_color;
int  dirty_color;

   if (window->visible  &&  g_status.screen_display) {
      text_color = g_display.text_color;
      dirty_color = g_display.dirty_color;
      g_display.dirty_color = g_display.text_color = g_display.curl_color;
      update_line( window );
      g_display.text_color = text_color;
      g_display.dirty_color = dirty_color;
   }
}


/*
 * Name:    dup_window_info
 * Purpose: Copy window info from one window pointer to another
 * Date:    June 5, 1991
 * Passed:  dw: destination window
 *          sw: source window
 */
void dup_window_info( WINDOW *dw, WINDOW *sw )
{
   memcpy( dw, sw, sizeof( WINDOW ) );
}


/*
 * Name:    adjust_windows_cursor
 * Purpose: A change has been made, make sure pointers are not ahead of
 *           or behind file.
 * Date:    June 5, 1991
 * Passed:  window:       pointer to current window
 *          line_change:  number of lines add to or subtracted from file
 * Notes:   If a file has been truncated in one window and there is another
 *           window open to the same file and its current line is near the
 *           end, the current line is reset to the last line of the file.
 */
void adjust_windows_cursor( WINDOW *window, long line_change )
{
register WINDOW *next;
long i;
file_infos *file;
MARKER *marker;
long length;

   file = window->file_info;
   length = file->length;
   next = g_status.window_list;
   while (next != NULL) {
      if (next != window) {
         if (next->file_info == file) {
            if (next->rline > length + 1) {
               next->rline = length;
               next->ll    = file->line_list_end;
               file->dirty = NOT_LOCAL;
            } else if (next->rline < 1) {
               next->rline = 1;
               next->cline = next->top_line + next->ruler;
               next->ll    = file->line_list;
               next->bin_offset = 0;
               file->dirty = NOT_LOCAL;
            }
            if (next->rline > window->rline  &&  line_change) {
               file->dirty = NOT_LOCAL;
               if (line_change < 0) {
                  for (i=line_change; i < 0 && next->ll->next != NULL; i++) {
                     next->bin_offset += next->ll->len;
                     next->ll = next->ll->next;
                  }
               } else if (line_change > 0) {
                  for (i=line_change; i > 0 && next->ll->prev != NULL; i--) {
                     next->ll = next->ll->prev;
                     next->bin_offset -= next->ll->len;
                  }
               }
            }
            if (next->rline < (next->cline -(next->top_line+next->ruler-1))) {
               next->cline = (int)next->rline+(next->top_line+next->ruler)-1;
               file->dirty = NOT_LOCAL;
            }
         }
      }
      next = next->next;
   }

   /*
    * now adjust any markers.
    */
   for (i=0; i<3; i++) {
      marker = &file->marker[ (int) i ];
      if (marker->rline > window->rline) {
         marker->rline += line_change;
         if (marker->rline < 1L)
            marker->rline = 1L;
         else if (marker->rline > length)
            marker->rline = length;
      }
   }
}


/*
 * Name:    first_non_blank
 * Purpose: To find the column of the first non-blank character
 * Date:    June 5, 1991
 * Passed:  s:    the string to search
 *          len:  length of string
 * Returns: the first non-blank column
 */
int  first_non_blank( text_ptr s, int len )
{
register int count = 0;

   if (s != NULL) {
      if (mode.inflate_tabs) {
         for (; len > 0 && (*s == ' ' || *s == '\t'); s++, len--) {
            if (*s != '\t')
               ++count;
            else
               count += mode.ptab_size - (count % mode.ptab_size);
         }
      } else {
         while (len-- > 0  &&  *s++ == ' ')
           ++count;
      }
   }
   return( count );
}


/*
 * Name:    find_end
 * Purpose: To find the last character in a line
 * Date:    October 31, 1992
 * Passed:  s:    the string to search
 *          len:  length of string
 * Returns: the first non-blank column
 */
int  find_end( text_ptr s, int len )
{
register int count = 0;

   if (s != NULL) {
      if (mode.inflate_tabs) {
         for (;len > 0; s++, len--) {
            if (*s == '\t')
               count += mode.ptab_size - (count % mode.ptab_size);
            else
               ++count;
         }
      } else
         count = len;
   }
   return( count );
}


/*
 * Name:    is_line_blank
 * Purpose: is line empty or does it only contain spaces?
 * Date:    November 28, 1991
 * Passed:  s:    the string to search
 *          len:  length of string
 * Returns: TRUE if line is blank or FALSE if something is in line
 */
int is_line_blank( text_ptr s, int len )
{
   if (s != NULL) {
      if (mode.inflate_tabs) {
        while (len > 0  &&  (*s == ' ' || *s == '\t')) {
           ++s;
           --len;
        }
      } else {
         while (len > 0  &&  *s == ' ') {
            ++s;
            --len;
         }
      }
   } else
      len = 0;
   return( len == 0 );
}


/*
 * Name:    page_up
 * Purpose: To move the cursor one page up the window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   The cursor line is moved back the required number of lines
 *           towards the start of the file.
 *          If the start of the file is reached, then movement stops.
 */
int  page_up( WINDOW *window )
{
int  i;                 /* count of lines scanned */
int  rc = OK;           /* default return code */
register WINDOW *win;   /* put window pointer in a register */
long number;
long len;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->rline != (win->cline - (win->top_line + win->ruler - 1))) {
      i = win->cline - (win->top_line + win->ruler - 1);
      number = win->rline;
      if (( win->rline - i) < win->page)
         win->rline = (win->cline-(win->top_line + win->ruler -1)) + win->page;
      win->rline -= win->page;
      for (len =0, i=(int)(number - win->rline); i>0; i--)
         if (win->ll->prev != NULL) {
            win->ll = win->ll->prev;
            len -= win->ll->len;
         }
      win->file_info->dirty = LOCAL;
      win->bin_offset += len;
   } else
      rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    page_down
 * Purpose: To move the cursor one page down the window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   The cursor line is moved forwards the required number of lines
 *           towards the end of the file.
 *          If the end of the file is reached, then movement stops.
 */
int  page_down( WINDOW *window )
{
int  i;                 /* count of lines scanned so far */
int  k;
int  rc = OK;
long len;
register WINDOW *win;  /* put window pointer in a register */
line_list_ptr p;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   p = win->ll;
   k = win->cline - (win->top_line + win->ruler);
   for (len=i=0; i < win->page && p->next != NULL; i++, k++, p=p->next)
      if (p->len != EOF)
         len += p->len;
   if (k >= win->page) {
      win->rline += i;
      win->cline = win->cline + i - win->page;
      win->bin_offset += len;
      win->ll = p;
      win->file_info->dirty = LOCAL;
   } else
      rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    scroll_down
 * Purpose: scroll window down one line
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If there is a line to scroll_down, make the window LOCAL dirty.
 *          We have to redraw the screen anyway, so don't update here.
 */
int  scroll_down( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->cline == win->top_line + win->ruler) {
      if (win->ll->next != NULL) {
         ++win->rline;
         win->bin_offset += win->ll->len;
         win->ll = win->ll->next;
         win->file_info->dirty = LOCAL;
      } else
         rc = ERROR;
   } else {
      --win->cline;
      win->file_info->dirty = LOCAL;
   }
   sync( win );
   return( rc );
}


/*
 * Name:    scroll_up
 * Purpose: To scroll the window up one line
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If this is the first page, then update screen here.  Else, make
 *           the window LOCAL dirty because we have to redraw screen.
 */
int  scroll_up( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->rline > 1) {
      if (win->rline == (win->cline - (win->top_line + win->ruler - 1))) {
         if (!mode.sync)
            update_line( win );
         win->ll = win->ll->prev;
         win->bin_offset -= win->ll->len;
         --win->rline;
         --win->cline;
         if (!mode.sync)
            show_curl_line( win );
      } else {
         if (win->cline == win->bottom_line) {
            --win->rline;
            win->ll = win->ll->prev;
            win->bin_offset -= win->ll->len;
            win->file_info->dirty = LOCAL;
         } else {
            ++win->cline;
            win->file_info->dirty = LOCAL;
         }
      }
   } else
     rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    pan_up
 * Purpose: To leave cursor on same logical line and scroll text up
 * Date:    September 1, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If cursor is on first page then do not scroll.
 */
int  pan_up( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );

   /*
    * see if cursor is on the first page. if it's not then pan_up.
    */
   if (win->rline != (win->cline+1 - (win->top_line + win->ruler))) {
      if (win->rline > 1) {
         --win->rline;
         win->ll = win->ll->prev;
         win->bin_offset -= win->ll->len;
         win->file_info->dirty = LOCAL;
      }
   } else
      rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    pan_down
 * Purpose: To leave cursor on same logical line and scroll text down
 * Date:    September 1, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If cursor is on last line in file then do not scroll.
 */
int  pan_down( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->ll->len != EOF) {
      ++win->rline;
      win->bin_offset += win->ll->len;
      win->ll = win->ll->next;
      win->file_info->dirty = LOCAL;
   } else
      rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    show_window_header
 * Purpose: show file stuff in window header
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Clear line and display header in a lite bar
 */
void show_window_header( WINDOW *window )
{
char status_line[MAX_COLS+2];   /* status line at top of window */
register WINDOW *win;           /* put window pointer in a register */
int  len;

   win = window;
   len = win->vertical ? win->end_col + 1 - win->start_col : win->end_col;

   assert( len >= 0 );
   assert( len <= MAX_COLS );

   memset( status_line, ' ', len );
   status_line[len] = '\0';
   s_output( status_line, win->top_line-1, win->start_col,g_display.head_color);
   show_window_number_letter( win );
   show_window_fname( win );
   show_crlf_mode( win );
   show_size( win );
   show_line_col( win );
}


/*
 * Name:    show_window_number_letter
 * Purpose: show file number and letter of window in lite bar
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_window_number_letter( WINDOW *window )
{
int  col;
char temp[10];
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   col = win->start_col;
   s_output( "   ", win->top_line-1, col, g_display.head_color );
   itoa( win->file_info->file_no, temp, 10 );
   s_output( temp, win->top_line-1, strlen( temp ) > 1 ? col : col+1,
             g_display.head_color );
   c_output( win->letter, col+2, win->top_line-1, g_display.head_color );
}


/*
 * Name:    show_window_fname
 * Purpose: show file name in window header.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Clear name field and display name in a lite bar
 */
void show_window_fname( WINDOW *window )
{
char status_line[MAX_COLS+2];   /* status line at top of window */
register int  fattr;
char *p;
register WINDOW *win;          /* put window pointer in a register */
int  col;
int  len;

   win = window;
   col = win->start_col;
   len = win->vertical ? 11 : FNAME_LENGTH;

   assert( len >= 0 );
   assert( len <= MAX_COLS );

   memset( status_line, ' ', len );
   status_line[len] = '\0';
   s_output( status_line, win->top_line-1, col+5, g_display.head_color );

   assert( strlen( win->file_info->file_name ) < MAX_COLS );

   strcpy( status_line, win->file_info->file_name );
   p = status_line;
   if (win->vertical) {
      len = strlen( status_line );
      for (p=status_line+len;*(p-1) != ':' && *(p-1) != '\\' && p>status_line;)
         --p;
   } else {
      status_line[FNAME_LENGTH] = '\0';
      p = status_line;
   }
   s_output( p, win->top_line-1, col+5, g_display.head_color );
   if (!win->vertical) {
      fattr = win->file_info->file_attrib;
      p = status_line;
      *p++ = (char)(fattr & ARCHIVE   ? 'A' : '-');
      *p++ = (char)(fattr & SYSTEM    ? 'S' : '-');
      *p++ = (char)(fattr & HIDDEN    ? 'H' : '-');
      *p++ = (char)(fattr & READ_ONLY ? 'R' : '-');
      *p   = '\0';
      s_output( status_line, win->top_line-1, col+51, g_display.head_color );
   }
}


/*
 * Name:    show_crlf_mode
 * Purpose: display state of crlf flag
 * Date:    June 5, 1991
 */
void show_crlf_mode( WINDOW *window )
{
char status_line[MAX_COLS+2];   /* status line at top of window */

   if (!window->vertical) {
      switch (window->file_info->crlf) {
         case LF :
            strcpy( status_line, "lf  " );
            break;
         case CRLF :
            strcpy( status_line, "crlf" );
            break;
         case BINARY :
            strcpy( status_line, "BIN " );
            break;
         default :
            assert( FALSE );
      }
      s_output( status_line, window->top_line-1, window->start_col+56,
                g_display.head_color );
   }
}


/*
 * Name:    show_size
 * Purpose: show number of lines in file
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_size( WINDOW *window )
{
char csize[20];

   if (!window->vertical  &&  window->file_info->crlf != BINARY) {
      s_output( "       ", window->top_line-1, 61, g_display.head_color );
      ltoa( window->file_info->length, csize, 10 );
      s_output( csize, window->top_line-1, 61, g_display.head_color );
   }
}


/*
 * Name:    quit
 * Purpose: To close the current window without saving the current file.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the file has been modified but not saved, then the user is
 *           given a second chance before the changes are discarded.
 *          Note that this is only necessary if this is the last window
 *           that refers to the file.  If another window still refers to
 *           the file, then the check can be left until later.
 */
int  quit( WINDOW *window )
{
int  prompt_line;
char line_buff[(MAX_COLS+2)*2]; /* buffer for char and attribute  */
register file_infos *file;
WINDOW *wp;
int  count = 0;
int  rc = OK;

   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   prompt_line = window->bottom_line;
   file = window->file_info;
   for (wp=g_status.window_list; wp != NULL; wp=wp->next) {
      if (wp->file_info == file && wp->visible)
         ++count;
   }
   if (file->modified && count == 1) {
      save_screen_line( 0, prompt_line, line_buff );
      /*
       * abandon changes (y/n)
       */
      set_prompt( utils12, prompt_line );
      if (get_yn( ) != A_YES)
         rc = ERROR;
      restore_screen_line( 0, prompt_line, line_buff );
   }

   /*
    * remove window, allocate screen lines to other windows etc
    */
   if (rc == OK)
      finish( window );
   return( OK );
}


/*
 * Name:    move_up
 * Purpose: To move the cursor up one line
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is at the top of the window, then the file must
 *           be scrolled down.
 */
int  move_up( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */
int  at_top = FALSE;    /* is cline at top of screen? */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );

   /*
    * if no previous line, give up
    */
   if (win->rline > 1) {
      if (win->cline == win->top_line + win->ruler) {
         win->file_info->dirty = LOCAL;
         at_top = TRUE;
      }
      if (!at_top)
         update_line( win );
      --win->rline;             /* ALWAYS decrement line counter */
      win->ll = win->ll->prev;
      win->bin_offset -= win->ll->len;
      if (!at_top) {
         --win->cline;          /* we aren't at top of screen - so move up */
         show_curl_line( win );
      }
   } else
      rc = ERROR;
   sync( win );
   return( rc );
}


/*
 * Name:    move_down
 * Purpose: To move the cursor down one line
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is at the bottom of the window, then the file must
 *           be scrolled up.   If the cursor is at the bottom of the file,
 *           then scroll line up until it is at top of screen.
 */
int  move_down( WINDOW *window )
{
int  rc;

   rc = prepare_move_down( window );
   sync( window );
   return( rc );
}


/*
 * Name:    prepare_move_down
 * Purpose: Do the stuff needed to move the cursor down one line.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Put all the stuff needed to move the cursor down one line in
 *           one function, so several functions can use the guts of the
 *           algorithm.
 */
int  prepare_move_down( WINDOW *window )
{
int  rc = OK;
register WINDOW *win;   /* put window pointer in a register */
int  at_bottom = FALSE; /* is cline at bottom of screen */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->cline == win->bottom_line) {
      win->file_info->dirty = LOCAL;
      at_bottom = TRUE;
   }
   if (!at_bottom)
      update_line( win );
   if (win->ll->len != EOF) {
      win->bin_offset += win->ll->len;
      ++win->rline;             /* ALWAYS increment line counter */
      win->ll = win->ll->next;
      if (!at_bottom) {
         ++win->cline;          /* if not at bottom of screen move down */
         show_curl_line( win );
      }
   } else if (win->cline > win->top_line + win->ruler) {
      --win->cline;
      win->file_info->dirty = LOCAL;
      rc = ERROR;
   } else
      rc = ERROR;
   return( rc );
}


/*
 * Name:    move_left
 * Purpose: To move the cursor left one character
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is already at the left of the screen, then
 *           scroll horizontally if we're not at beginning of line.
 */
int  move_left( WINDOW *window )
{
int  new_ruler = FALSE;

   if (window->ccol > window->start_col) {
      show_ruler_char( window );
      --window->ccol;
      --window->rcol;
   } else if (window->ccol == window->start_col && window->rcol > 0) {
      --window->rcol;
      --window->bcol;
      window->file_info->dirty = LOCAL;
      new_ruler = TRUE;
   }
   sync( window );
   if (new_ruler) {
      make_ruler( window );
      show_ruler( window );
   }
   return( OK );
}


/*
 * Name:    move_right
 * Purpose: To move the cursor right one character
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is already at the right of the screen (logical
 *          column 80) then scroll horizontally right.
 */
int  move_right( WINDOW *window )
{
int  new_ruler = FALSE;

   if (window->rcol < g_display.line_length - 1) {
      if (window->ccol < window->end_col) {
         show_ruler_char( window );
         ++window->ccol;
         ++window->rcol;
      } else if (window->ccol == window->end_col) {
         ++window->rcol;
         ++window->bcol;
         window->file_info->dirty = LOCAL;
         new_ruler = TRUE;
      }
   }
   sync( window );
   if (new_ruler) {
      make_ruler( window );
      show_ruler( window );
   }
   return( OK );
}


/*
 * Name:    pan_left
 * Purpose: To pan the screen left one character
 * Date:    January 5, 1992
 * Passed:  window:  pointer to current window
 */
int  pan_left( WINDOW *window )
{
/*
 * if (window->bcol == 0) {
 *    if (window->ccol > window->start_col) {
 *       show_ruler_char( window );
 *       --window->ccol;
 *       --window->rcol;
 *    }
 * } else if (window->bcol > 0 ) {
 * * *  Scroll window left function:
 * * *      --window->bcol;
 * * *      if (window->ccol < g_display.ncols - 1)
 * * *         ++window->ccol;
 * * *      else
 * * *         --window->rcol;
 */
   if (window->bcol > 0 ) {
      --window->bcol;
      --window->rcol;
      window->file_info->dirty = LOCAL;
      make_ruler( window );
      show_ruler( window );
   }
   sync( window );
   return( OK );
}


/*
 * Name:    pan_right
 * Purpose: To pan the screen right one character
 * Date:    January 5, 1992
 * Passed:  window:  pointer to current window
 */
int  pan_right( WINDOW *window )
{
   if (window->rcol < g_display.line_length - 1) {
/*
 *      scroll screen right function:
 *      if (window->ccol > 0)
 *         --window->ccol;
 *      else
 *         ++window->rcol;
 */
      ++window->rcol;
      ++window->bcol;
      window->file_info->dirty = LOCAL;
      make_ruler( window );
      show_ruler( window );
   }
   sync( window );
   return( OK );
}


/*
 * Name:    word_left
 * Purpose: To move the cursor left one word
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Words are considered strings of letters, numbers and underscores,
 *          which must be separated by other characters.
 */
int  word_left( WINDOW *window )
{
text_ptr p;             /* text pointer */
int  len;               /* length of current line */
int  rc;
register int rcol;
long rline;
line_list_ptr ll;
WINDOW w;

   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   dup_window_info( &w, window );
   rline = window->rline;
   rcol  = window->rcol;
   ll = window->ll;
   if (ll->len != EOF) {
      p = ll->line;
      len = ll->len;

      if (p != NULL  &&  rcol > 0  &&  rcol >= len  &&
                                      !myiswhitespc( *(p + len - 1) )) {
         rcol = len - 1;
         p += rcol;
         for (; rcol >= 0 && !myiswhitespc( *p ); rcol--, p--);
         ++rcol;
         check_virtual_col( window, rcol, rcol );
         make_ruler( window );
         show_ruler( window );
      } else {
         rcol = rcol >= len ? len-1 : rcol;
         if (rcol >= 0)
            p += rcol;
         if (p != NULL  &&  rcol > 0  &&  !myiswhitespc( *p )  &&
                                          !myiswhitespc( *(p-1) )) {
            for (; rcol >= 0 && !myiswhitespc( *p ); rcol--, p--);
            ++rcol;
            check_virtual_col( window, rcol, rcol );
            make_ruler( window );
            show_ruler( window );
         } else {

            /*
             * if we are on the first letter of a word, get off.
             */
            if (p != NULL)
               for (; rcol >= 0 && !myiswhitespc( *p ); rcol--, p--);

            /*
             * go to the next line if word begins at 1st col in line.
             */
            if (rcol < 0) {
               if (ll->prev != NULL) {
                  --rline;
                  ll = ll->prev;
                  p = ll->line;
                  rcol = ll->len - 1;
                  if (rcol >= 0)
                     p += rcol;
               } else
                  rc = ERROR;
            }

            /*
             * skip all blanks until we get to a previous word
             */
            while (rc == OK  &&  (p == NULL  ||  (p != NULL  &&
                                                  myiswhitespc( *p )))) {
               for (; rcol >= 0 && myiswhitespc( *p ); rcol--, p--);
               if (rcol < 0) {
                  if (ll->prev != NULL) {
                     --rline;
                     ll = ll->prev;
                     p = ll->line;
                     rcol = ll->len - 1;
                     if (rcol >= 0)
                        p += rcol;
                  } else
                     rc = ERROR;
               } else
                  break;
            }

            /*
             * now, find the beginning of the word.
             */
            if (rc == OK  &&  p != NULL) {
               for (; rcol >= 0 && !myiswhitespc( *p ); rcol--, p--);
               bin_offset_adjust( window, rline );
               find_adjust( window, ll, rline, rcol+1 );
               if (rline != w.rline && !window->file_info->dirty) {
                  update_line( &w );
                  show_curl_line( window );
               }
               make_ruler( window );
               show_ruler( window );
            } else
               rc = ERROR;
         }
      }
   } else
      rc = ERROR;

   sync( window );
   return( rc );
}


/*
 * Name:    word_right
 * Purpose: To move the cursor right one word
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Words are considered strings of letters, numbers and underscores,
 *           which must be separated by other characters.
 */
int  word_right( WINDOW *window )
{
int  len;               /* length of current line */
text_ptr p;             /* text pointer */
int  rc;
WINDOW w;
register int rcol;
line_list_ptr ll;
long rline;

   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   dup_window_info( &w, window );
   rline = window->rline;
   rcol  = window->rcol;
   ll = window->ll;
   if (ll->len != EOF) {
      p = ll->line;
      len = ll->len;

      /*
       * if rcol is past EOL, move it to EOL
       */
      rcol = rcol >= len ? len-1 : rcol;
      if (rcol >= 0)
         p += rcol;

      /*
       * if cursor is on a word, find end of word.
       */
      if (p != NULL)
         for (; rcol < len && !myiswhitespc( *p ); rcol++, p++);
      else
         rcol = len;

      /*
       * go to the next line if word ends at eol.
       */
      if (rcol == len) {
         ++rline;
         ll = ll->next;
         if (ll->len != EOF) {
            p = ll->line;
            len = ll->len;
            rcol = 0;
         } else
            rc = ERROR;
      }

      /*
       * now, go forward thru the file looking for the first letter of word.
       */
      while (rc == OK && (p == NULL  ||  (p != NULL && myiswhitespc( *p )))) {
         for (; rcol < len && myiswhitespc( *p ); rcol++, p++);
         if (rcol == len) {
            ++rline;
            ll = ll->next;
            if (ll->len != EOF) {
               p = ll->line;
               len = ll->len;
               rcol = 0;
            } else
               rc = ERROR;
         } else
            break;
      }
   } else
      rc = ERROR;

   if (rc == OK) {
      bin_offset_adjust( window, rline );
      find_adjust( window, ll, rline, rcol );
      make_ruler( window );
      show_ruler( window );
   }

   if (rline != w.rline && !window->file_info->dirty) {
      update_line( &w );
      show_curl_line( window );
   }
   sync( window );
   return( rc );
}


/*
 * Name:    next_dirty_line
 * Purpose: To move the cursor to the next dirty line, if it exists
 * Date:    April 1, 1993
 * Passed:  window:  pointer to current window
 */
int  next_dirty_line( WINDOW *window )
{
int  rc;
line_list_ptr ll;
long rline;
long bin_offset;       /* binary offset */
WINDOW w;

   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   dup_window_info( &w, window );
   rline = window->rline;
   ll = window->ll;
   bin_offset = window->bin_offset;
   if (ll->len != EOF) {
      while (rc == OK) {
         if (ll->len != EOF) {
            ++rline;
            bin_offset += ll->len;
            ll = ll->next;
            if (ll->dirty == TRUE)
               break;
         } else
            rc = ERROR;
      }
   } else
      rc = ERROR;

   if (rc == OK) {
      window->bin_offset = bin_offset;
      find_adjust( window, ll, rline, window->rcol );
      make_ruler( window );
      show_ruler( window );
   } else
      error( WARNING, window->bottom_line, utils16 );

   if (rline != w.rline && !window->file_info->dirty) {
      update_line( &w );
      show_curl_line( window );
   }
   sync( window );
   return( rc );
}


/*
 * Name:    prev_dirty_line
 * Purpose: To move the cursor to the prev dirty line, if it exists
 * Date:    April 1, 1993
 * Passed:  window:  pointer to current window
 */
int  prev_dirty_line( WINDOW *window )
{
int  rc;
line_list_ptr ll;
long rline;
long bin_offset;        /* binary offset */
WINDOW w;

   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   dup_window_info( &w, window );
   rline = window->rline;
   ll = window->ll;
   bin_offset = window->bin_offset;
   if (ll->prev != NULL) {
      while (rc == OK) {
         if (ll->prev != NULL) {
            --rline;
            ll = ll->prev;
            bin_offset -= ll->len;
            if (ll->dirty == TRUE)
               break;
         } else
            rc = ERROR;
      }
   } else
      rc = ERROR;

   if (rc == OK) {
      window->bin_offset = bin_offset;
      find_adjust( window, ll, rline, window->rcol );
      make_ruler( window );
      show_ruler( window );
   } else
      error( WARNING, window->bottom_line, utils16 );

   if (rline != w.rline && !window->file_info->dirty) {
      update_line( &w );
      show_curl_line( window );
   }
   sync( window );
   return( rc );
}


/*
 * Name:    center_window
 * Purpose: To place the current line or cursor in the center of a window.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  center_window( WINDOW *window )
{
int  center;
int  center_line;
int  diff;
register file_infos *file;
register WINDOW *win;           /* put window pointer in a register */

   win = window;
   file = win->file_info;
   center = (win->bottom_line + 1 - win->top_line) / 2 - win->ruler;
   center_line = win->top_line + win->ruler + center;
   diff = center_line - win->cline;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (g_status.command == CenterWindow) {
      if (diff > 0) {
         if (win->rline + diff <= file->length) {
            update_line( win );
            win->cline += diff;
            win->rline += diff;
            for (; diff > 0; diff--) {
               win->bin_offset += win->ll->len;
               win->ll = win->ll->next;
            }
            show_curl_line( win );
         }
      } else if (diff < 0) {
         update_line( win );
         win->cline += diff;
         win->rline += diff;
         for (; diff < 0; diff++) {
            win->ll = win->ll->prev;
            win->bin_offset -= win->ll->len;
         }
         show_curl_line( win );
      }
   } else {
      if (diff > 0) {
         win->cline += diff;
         if ((long)(win->cline+1 - (win->top_line + win->ruler)) > win->rline)
            win->cline = (win->top_line + win->ruler) - 1 + (int)win->rline;
         file->dirty = LOCAL;
      } else if (diff < 0) {
         win->cline = win->cline + diff;
         file->dirty = LOCAL;
      }
   }
   if (g_status.command == CenterWindow  ||  g_status.command == CenterLine)
      sync( win );
   return( OK );
}


/*
 * Name:    horizontal_screen_right
 * Purpose: To move the cursor one screen to the right
 * Date:    September 13, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Add 80 columns to the real cursor.  If the cursor is past the
 *          maximum line length then move it back.
 */
int  horizontal_screen_right( WINDOW *window )
{
int  col;

   col = window->rcol;
   col += (window->end_col + 1 - window->start_col);
   if (col < MAX_LINE_LENGTH) {
      window->rcol = col;
      window->bcol += (window->end_col + 1 - window->start_col);
      window->file_info->dirty = LOCAL;
      check_virtual_col( window, window->rcol, window->ccol );
      make_ruler( window );
      show_ruler( window );
   }
   sync( window );
   return( OK );
}


/*
 * Name:    horizontal_screen_left
 * Purpose: To move the cursor one screen to the left
 * Date:    September 13, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Subtract screen width from the real cursor.  If the cursor is less
 *           than zero then see if bcol is zero.  If bcol is not zero then make
 *           bcol zero.
 */
int  horizontal_screen_left( WINDOW *window )
{
int  screen_width;

   screen_width = window->end_col + 1 - window->start_col;
   if (window->rcol - screen_width < 0) {
      if (window->bcol != 0) {
         window->bcol = 0;
         window->file_info->dirty = LOCAL;
      }
   } else {
      window->rcol -= screen_width;
      window->bcol -= screen_width;
      if (window->bcol < 0)
         window->bcol = 0;
      window->file_info->dirty = LOCAL;
   }
   check_virtual_col( window, window->rcol, window->ccol );
   sync( window );
   make_ruler( window );
   show_ruler( window );
   return( OK );
}


/*
 * Name:    goto_top_file
 * Purpose: To move the cursor to the top of the file.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  goto_top_file( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */
long num;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->rline != win->cline - (win->top_line+win->ruler-1)) {
      win->bin_offset = 0;
      win->rline = win->cline - (win->top_line+win->ruler-1);
      win->ll = win->file_info->line_list;
      for (num=1; num < win->rline; num++) {
         win->bin_offset += win->ll->len;
         win->ll = win->ll->next;
      }
      display_current_window( win );
   }
   sync( win );
   return( OK );
}


/*
 * Name:    goto_end_file
 * Purpose: To move the cursor to the end of the file.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  goto_end_file( WINDOW *window )
{
register WINDOW *win;  /* put window pointer in a register */
line_list_ptr ll;
long length;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   length = win->file_info->length;
   if (length > win->rline + win->bottom_line - win->cline) {
      win->rline = length - (win->bottom_line - win->cline) + 1;
      win->ll = win->file_info->line_list_end;
      for (;length >= win->rline; length--)
         win->ll = win->ll->prev;

      win->bin_offset = 0;
      ll = win->file_info->line_list;
      for (length = 1; length < win->rline; length++) {
         win->bin_offset += ll->len;
         ll = ll->next;
      }

      display_current_window( win );
   }
   sync( win );
   return( OK );
}


/*
 * Name:    goto_line
 * Purpose: To move the cursor to a particular line in the file
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  goto_line( WINDOW *window )
{
long number;            /* line number selected */
long n;
char num_str[MAX_COLS]; /* line number as string */
register WINDOW *win;   /* put window pointer in a register */
line_list_ptr ll;
int  rc;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   /*
    * find out where we are going
    */
   num_str[0] = '\0';
   /*
    * line number:
    */
   if (get_name( find11, win->bottom_line, num_str,
                 g_display.message_color ) != OK  ||  *num_str == '\0')
      return( ERROR );
   number = atol( num_str );

   if (number > 0  && number <= (long)win->file_info->length) {
      update_line( win );
      ll = win->ll;
      n = win->rline;
      if (number < win->rline) {
         if (n - number < number - 1) {
            for (; n > number; n--) {
               ll = ll->prev;
               win->bin_offset -= ll->len;
            }
         } else {
            ll = win->file_info->line_list;
            n = 1;
            for (; n < number; n++) {
               win->bin_offset += ll->len;
               ll = ll->next;
            }
         }
      } else if (number > win->rline) {
         for (; n < number; n++) {
            win->bin_offset += ll->len;
            ll = ll->next;
         }
      }
      find_adjust( win, ll, number, win->rcol );
      if (!win->file_info->dirty)
         show_curl_line( win );
      rc = OK;
   } else {
      /*
       * out of range.  must be in the range 1 -
       */
      strcat( num_str, find12 );
      ltoa( win->file_info->length, num_str+25, 10 );
      error( WARNING, win->bottom_line, num_str );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    set_marker
 * Purpose: To set file marker
 * Date:    December 28, 1991
 * Passed:  window:  pointer to current window
 */
int  set_marker( WINDOW *window )
{
register MARKER  *marker;       /* put the marker in a register */

   marker = &window->file_info->marker[g_status.command - SetMark1];
   marker->rline  = window->rline;
   marker->rcol   = window->rcol;
   marker->ccol   = window->ccol;
   marker->bcol   = window->bcol;
   marker->marked = TRUE;
   return( OK );
}


/*
 * Name:    goto_marker
 * Purpose: To goto a file marker
 * Date:    December 28, 1991
 * Passed:  window:  pointer to current window
 */
int  goto_marker( WINDOW *window )
{
int  m;
file_infos *file;
long new_rline;
long n;
MARKER *marker;
register WINDOW *win;   /* put window pointer in a register */
line_list_ptr ll;
int  rc;

   win = window;
   m = g_status.command - GotoMark1;
   file = win->file_info;
   marker = &file->marker[m];
   if (marker->marked) {
      entab_linebuff( );
      if (un_copy_line( win->ll, win, TRUE ) == ERROR)
         return( ERROR );
      file->dirty = LOCAL;
      if (marker->rline > file->length)
         marker->rline = file->length;
      if (marker->rline < 1l)
         marker->rline = 1l;
      new_rline = marker->rline;
      ll = win->ll;
      if (new_rline < win->rline) {
         if (win->rline - new_rline < new_rline - 1) {
            for (n=win->rline; n > new_rline; n--) {
               ll = ll->prev;
               win->bin_offset -= ll->len;
            }
         } else {
            ll = win->file_info->line_list;
            win->bin_offset = 0;
            n = 1;
            for (; n < new_rline; n++) {
               win->bin_offset += ll->len;
               ll = ll->next;
            }
         }
      } else if (new_rline > win->rline) {
         n = win->rline;
         for (; n < new_rline; n++) {
            win->bin_offset += ll->len;
            ll = ll->next;
         }
      }
      win->rline  = new_rline;
      win->ll     = ll;
      win->rcol   = marker->rcol;
      win->ccol   = marker->ccol;
      win->bcol   = marker->bcol;
      if (win->rline < (win->cline - ((win->top_line + win->ruler) - 1)))
         win->cline = (int)win->rline + (win->top_line + win->ruler) - 1;
      check_virtual_col( win, win->rcol, win->ccol );
      make_ruler( window );
      show_ruler( window );
      rc = OK;
   } else {
      if (m == 9)
         m = -1;
      *(utils13 + 7) = (char)('0' + m + 1);
      /*
       * marker not set
       */
      error( WARNING, win->bottom_line, utils13 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    date_time_stamp
 * Purpose: put system date and time into file at cursor position
 * Date:    June 5, 1992
 * Passed:  window:  pointer to current window
 */
int  date_time_stamp( WINDOW *window )
{
char date_time[MAX_COLS];
char stuff[20];
register char *dt;
int  year, month, day;
int  hours, minutes;
int  one, two, three;
int  i;
int  pm;


   get_date( &year, &month, &day, &i );
   get_time( &hours, &minutes, &i, &i );
   dt = date_time;

   /*
    * mod year with 100 if needed.
    */
   switch (mode.date_style) {
      case MM_DD_YY  :
      case DD_MM_YY  :
      case YY_MM_DD  :
         year = year % 100;
   }

   switch (mode.date_style) {
      case DD_MM_YY   :
      case DD_MM_YYYY :
         one = day;
         two = month;
         three = year;
         break;
      case YY_MM_DD   :
      case YYYY_MM_DD :
         one = year;
         two = month;
         three = day;
         break;
      case MM_DD_YY   :
      case MM_DD_YYYY :
      default         :
         one = month;
         two = day;
         three = year;
         break;
   }
   strcpy( dt, itoa( one, stuff, 10 ) );
   strcat( dt, "-" );
   strcat( dt, itoa( two, stuff, 10 ) );
   strcat( dt, "-" );
   strcat( dt, itoa( three, stuff, 10 ) );

   strcat( dt, "  " );

   pm = FALSE;
   if (mode.time_style == _12_HOUR) {
      if (hours >= 12 && hours < 24)
         pm = TRUE;
      if (hours < 1)
         hours = 12;
      else if (hours >= 13)
         hours -= 12;
   }

   if (hours < 1)
      strcat( dt, "0" );
   strcat( dt, itoa( hours, stuff, 10 ) );
   strcat( dt, ":" );
   if (minutes < 10)
      strcat( dt, "0" );
   strcat( dt, itoa( minutes, stuff, 10 ) );
   if (mode.time_style == _12_HOUR)
      strcat( dt, pm == FALSE ? "am" : "pm" );
   strcat( dt, "  " );
   return( add_chars( dt, window ) );
}


/*
 * Name:    add_chars
 * Purpose: insert string into file
 * Date:    June 5, 1992
 * Passed:  string:  string to add to file
 *          window:  pointer to current window
 */
int  add_chars( char *string, WINDOW *window )
{
int  rc = OK;

   while (*string) {
      g_status.key_pressed = *string;
      rc = insert_overwrite( window );
      ++string;
   }
   return( rc );
}
