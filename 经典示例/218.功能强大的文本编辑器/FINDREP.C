

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * Name:    get_replacement_flags
 * Purpose: To input find and replace flags.
 * Date:    June 5, 1991
 * Passed:  line:  prompt line
 * Returns: OK if flags were entered, ERROR if user wanted to abort
 */
int  get_replacement_flags( int line )
{
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int c;
int  func;
int  rc;

   save_screen_line( 0, line, line_buff );
   eol_clear( 0, line, g_display.text_color );
   /*
    * options: prompt or no prompt (p/n)?
    */
   s_output( find1, line, 0, g_display.message_color );
   xygoto( strlen( find1 ) + 2, line );
   do {
      c = getkey( );
      func = getfunc( c );
   } while (c != 'P'  &&  c != 'p'  &&  c != 'N'  &&  c != 'n'  &&
            c != RTURN  &&  c != ESC  &&  func != AbortCommand);
   restore_screen_line( 0, line, line_buff );
   switch (c) {
      case 'P' :
      case 'p' :
      case RTURN :
         g_status.replace_flag = PROMPT;
         rc = OK;
         break;
      case 'N' :
      case 'n' :
         g_status.replace_flag = NOPROMPT;
         rc = OK;
         break;
      default :
         rc = ERROR;
   }
   bm.search_defined = rc;
   return( rc );
}


/*
 * Name:    ask_replace
 * Purpose: Ask user if he wants to (r)place, (s)kip, or (e)xit.
 * Date:    June 5, 1991
 * Returns: TRUE if user wants to replace, ERROR otherwise.
 * Passed:  window:   pointer to current window
 *          finished: TRUE if user pressed ESC or (e)xit.
 */
int  ask_replace( WINDOW *window, int *finished )
{
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int c;
int  func;
int  prompt_line;
int  rc;

   prompt_line = window->cline - 1;
   c = 39 - (strlen( find2 ) >> 1);
   save_screen_line( 0, prompt_line, line_buff );
   /*
    * replace skip exit (r/s/e)?
    */
   s_output( find2, prompt_line, c, g_display.message_color );
   do {
      c = getkey( );
      func = getfunc( c );
   } while (c != 'R' && c != 'r' && c != 'S' && c != 's' && c != 'E' && c != 'e'
          && c != ESC  &&  func != AbortCommand);
   restore_screen_line( 0, prompt_line, line_buff );
   switch (c) {
      case 'R' :
      case 'r' :
         rc = OK;
         break;
      case 'E' :
      case 'e' :
         *finished = TRUE;
      case 'S' :
      case 's' :
         rc = ERROR;
         break;
      default :
         *finished = TRUE;
         rc = ERROR;
         break;
   }
   return( rc );
}


/*
 * Name:    ask_wrap_replace
 * Purpose: After a wrap, ask user if he wants to (q)uit or (c)ontine.
 * Date:    June 5, 1991
 * Returns: TRUE if user wants to continue, ERROR otherwise.
 * Passed:  window:   pointer to current window
 *          finished: TRUE if user pressed ESC or (q)uit.
 */
int  ask_wrap_replace( WINDOW *window, int *finished )
{
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int c;
int  func;
int  prompt_line;
int  rc;

   prompt_line = window->bottom_line;
   save_screen_line( 0, prompt_line, line_buff );
   /*
    * search has wrapped. continue or quit (c/q)?
    */
   set_prompt( find3, prompt_line );
   do {
      c = getkey( );
      func = getfunc( c );
   } while (c != 'Q'  &&  c != 'q'  &&  c != 'C'  &&  c != 'c' &&
          c != ESC  &&  func != AbortCommand);
   restore_screen_line( 0, prompt_line, line_buff );
   switch (c) {
      case 'C' :
      case 'c' :
         rc = OK;
         break;
      case 'Q' :
      case 'q' :
      default :
         *finished = TRUE;
         rc = ERROR;
         break;
   }
   return( rc );
}


/*
 * Name:    do_replace
 * Purpose: To replace text once match has been found.
 * Date:    June 5, 1991
 * Passed:  window:     pointer to current window
 *          direction:  direction of search
 */
void do_replace( WINDOW *window, int direction )
{
int  old_len;           /* length of original text */
int  new_len;           /* length of replacement text */
int  rcol;
register int net_change;
char *source;           /* source of block move */
char *dest;             /* destination of block move */

   old_len = strlen( g_status.pattern );
   new_len = strlen( g_status.subst );
   net_change = new_len - old_len;
   rcol = window->rcol;

   /*
    * move the text to either make room for the extra replacement text
    *  or to close up the gap left
    */

   g_status.copied = FALSE;
   copy_line( window->ll );
   detab_linebuff( );

   if (net_change != 0) {
      assert( rcol + old_len >= 0);
      assert( net_change <= rcol + old_len );

      source = g_status.line_buff + rcol + old_len;
      dest  = source + net_change;

      assert( g_status.line_buff_len - rcol - old_len >= 0 );
      assert( g_status.line_buff_len - rcol - old_len < MAX_LINE_LENGTH );

      memmove( dest, source, g_status.line_buff_len - rcol - old_len );
      g_status.line_buff_len += net_change;
   }

   /*
    * insert the replacement text
    */

   assert( rcol >= 0 );
   assert( rcol < MAX_LINE_LENGTH );
   assert( g_status.line_buff_len >= 0 );
   assert( g_status.line_buff_len >= rcol );

   for (dest=g_status.line_buff+rcol, source=g_status.subst; *source;)
      *dest++ = *source++;

   entab_linebuff( );
   window->file_info->modified = TRUE;
   un_copy_line( window->ll, window, TRUE );
   window->ll->dirty = TRUE;

   if (direction == FORWARD)
      window->rcol += (new_len - 1);

   assert( window->rcol >= 0 );
   show_avail_mem( );
}


/*
 * 作用: 建立并且执行查找操作
 * 参数: window:  当前窗口的指针
 */
int  find_string( WINDOW *window )
{
int  direction;
int  new_string;
char pattern[MAX_COLS];  /* 想要查找的文本 */
long found_line;
long bin_offset;
line_list_ptr ll;
register WINDOW *win;  /* 把文件指针放到一个寄存器中 */
int  rc;
int  old_rcol;
int  rcol;

   switch (g_status.command) {
      case FindForward :
         direction = FORWARD;
         new_string = TRUE;
         break;
      case FindBackward :
         direction = BACKWARD;
         new_string = TRUE;
         break;
      case RepeatFindForward1 :
      case RepeatFindForward2 :
         direction = FORWARD;
         new_string =  bm.search_defined != OK ? TRUE : FALSE;
         break;
      case RepeatFindBackward1 :
      case RepeatFindBackward2 :
         direction = BACKWARD;
         new_string =  bm.search_defined != OK ? TRUE : FALSE;
         break;
      default :
         direction = 0;
         new_string = 0;
         assert( FALSE );
         break;
   }
   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );

   /*
    * 得到搜索文本，上次的搜索文本做为缺省值
    */
   if (new_string == TRUE) {
      *pattern = '\0';
      if (bm.search_defined == OK) {

         assert( strlen( (char *)bm.pattern ) < MAX_COLS );

         strcpy( pattern, (char *)bm.pattern );
      }

      /*
       * 查找
       */
      if (get_name( find4, win->bottom_line, pattern,
                    g_display.message_color ) != OK  ||  *pattern == '\0')
         return( ERROR );
      bm.search_defined = OK;

      assert( strlen( pattern ) < MAX_COLS );

      strcpy( (char *)bm.pattern, pattern );
      build_boyer_array( );
   }

   rc = OK;
   if (bm.search_defined == OK) {
      old_rcol = win->rcol;
      if (mode.inflate_tabs)
         win->rcol = entab_adjust_rcol( win->ll->line, win->ll->len, win->rcol);
      update_line( win );
      show_search_message( SEARCHING, g_display.diag_color );
      bin_offset = win->bin_offset;
      if (direction == FORWARD) {
         if ((ll = forward_boyer_moore_search( win, &found_line, &rcol )) != NULL) {
            if (g_status.wrapped && g_status.macro_executing)
               rc = ask_wrap_replace( win, &new_string );
            if (rc == OK)
               find_adjust( win, ll, found_line, rcol );
            else
               win->bin_offset = bin_offset;
         }
      } else {
         if ((ll = backward_boyer_moore_search( win, &found_line, &rcol )) != NULL) {
            if (g_status.wrapped && g_status.macro_executing)
               rc = ask_wrap_replace( win, &new_string );
            if (rc == OK)
               find_adjust( win, ll, found_line, rcol );
            else
               win->bin_offset = bin_offset;
         }
      }
      if (g_status.wrapped)
         show_search_message( WRAPPED, g_display.diag_color );
      else
         show_search_message( CLR_SEARCH, g_display.mode_color );
      if (ll == NULL) {
         /*
          * 没有找到
          */
         if (mode.inflate_tabs)
            win->rcol = old_rcol;
         combine_strings( pattern, find5a, (char *)bm.pattern, find5b );
         error( WARNING, win->bottom_line, pattern );
         rc = ERROR;
      }
      show_curl_line( win );
      make_ruler( win );
      show_ruler( win );
   } else {
      /*
       * 没有定义查找模式
       */
      error( WARNING, win->bottom_line, find6 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    build_boyer_array
 * Purpose: To set up the boyer array for forward and backward searches.
 * Date:    June 5, 1991
 * Notes:   Set up one array for forward searches and another for backward
 *          searches.  If user decides to ignore case then fill in array
 *          with reverse case characters so both upper and lower case
 *          characters are defined.
 */
void build_boyer_array( void )
{

   /*
    * set up for forward search
    */
   if (g_status.command == DefineGrep  ||  g_status.command == RepeatGrep) {

      assert( strlen( (char *)sas_bm.pattern ) + 1 < MAX_COLS );

      memcpy( bm.pattern, sas_bm.pattern, strlen( (char *)sas_bm.pattern ) +1 );
      bm.search_defined = OK;
   }

   if (bm.search_defined == OK) {
      build_forward_skip( &bm );
      bm.forward_md2 = calculate_forward_md2( (char *)bm.pattern,
                                                bm.pattern_length );

      /*
       * set up for backward search
       */
      build_backward_skip( &bm );
      bm.backward_md2 = calculate_backward_md2( (char *)bm.pattern,
                                                  bm.pattern_length );
   }


   /*
    * build an array for search and seize.
    */
   if (sas_bm.search_defined == OK) {
      build_forward_skip( &sas_bm );
      sas_bm.forward_md2 = calculate_forward_md2( (char *)sas_bm.pattern,
                                                    sas_bm.pattern_length );

      /*
       * set up for backward search
       */
      build_backward_skip( &sas_bm );
      sas_bm.backward_md2 = calculate_backward_md2( (char *)sas_bm.pattern,
                                                      sas_bm.pattern_length );
   }
}


/*
 * Name:    build_forward_skip
 * Purpose: build Boyer-Moore forward skip array
 * Date:    October 31, 1992
 * Passed:  bmp:  pointer to Boyer-Moore structure
 * Notes:   build standard Boyer-Moore forward skip array.
 *          Thanks to Tom Waters, twaters@relay.nswc.navy.mil, for finding a
 *           bug in TDE 1.3 when building the ignore skip index array.
 */
void build_forward_skip( boyer_moore_type *bmp )
{
register unsigned char *p;
register int i;

   assert( strlen( (char *)bmp->pattern ) < MAX_COLS );

   i = bmp->pattern_length = (int)strlen( (char *)bmp->pattern );

   /*
    * set skip index of all characters to length of string
    */
   memset( bmp->skip_forward, i, 256 );
   i--;

   /*
    * for each character in string, set the skip index
    */
   for (p=bmp->pattern; i>=0; i--, p++) {

      assert( (char)i < bmp->skip_forward[*p] );

      bmp->skip_forward[*p] = (char)i;
      if (mode.search_case == IGNORE) {
         if (*p >= 'A' && *p <= 'Z') {

            assert( (char)i < bmp->skip_forward[*p+32] );

            bmp->skip_forward[*p+32] = (char)i;
         } else if (*p >= 'a' && *p <= 'z') {

            assert( (char)i < bmp->skip_forward[*p-32] );

            bmp->skip_forward[*p-32] = (char)i;
         }
      }
   }
}


/*
 * Name:    build_backward_skip
 * Purpose: build Boyer-Moore backward skip array
 * Date:    October 31, 1992
 * Passed:  bmp:  pointer to Boyer-Moore structure
 * Notes:   build standard Boyer-Moore backward skip array.
 *          Thanks to Tom Waters, twaters@relay.nswc.navy.mil, for finding a
 *           bug in TDE 1.3 when building the ignore skip index array.
 */
void build_backward_skip( boyer_moore_type *bmp )
{
register unsigned char *p;
register int i;

   i = -bmp->pattern_length;
   memset( bmp->skip_backward, i, 256 );
   i++;
   for (p=bmp->pattern + bmp->pattern_length - 1; i<=0; i++, p--) {

      assert( (char)i > bmp->skip_backward[*p] );

      bmp->skip_backward[*p] = (char)i;
      if (mode.search_case == IGNORE) {
         if (*p >= 'A' && *p <= 'Z') {

            assert( (char)i > bmp->skip_backward[*p+32] );

            bmp->skip_backward[*p+32] = (char)i;
         } else if (*p >= 'a' && *p <= 'z') {

            assert( (char)i > bmp->skip_backward[*p-32] );

            bmp->skip_backward[*p-32] = (char)i;
         }
      }
   }
}


/*
 * Name:    calculate_forward_md2
 * Purpose: Calculate mini delta2 for forward searches
 * Date:    October 31, 1992
 * Passed:  p:  pointer to pattern
 *          len: length of pattern
 * Notes:   Hume and Sunday (see above) demonstrate in their paper that
 *            using a simple shift function on mismatches with BM
 *            gives one of the fastest run times for general text searching.
 *          mini delta2 is, from the end of the string, the first leftmost
 *            occurrence of the rightmost character.  mini delta2 is 1 in
 *            the worst case.  in previous versions of TDE, the shift function
 *            was hard-coded as 1 -- the worst case.  typically, mini delta2
 *            will be the length of the search pattern.
 */
int  calculate_forward_md2( char *p, int len )
{
int  last_c;
register int i;
register int md2;

   md2 = 1;
   i = len - 1;
   last_c = p[i];
   if (mode.search_case == IGNORE) {
      last_c = tolower( last_c );
      for (i--; i >= 0  &&  last_c != tolower( p[i] ); i--)
         ++md2;
   } else
      for (i--; i >= 0  &&  last_c != p[i]; i--)
         ++md2;

   assert( md2 >= 1  &&  md2 <= len );

   return( md2 );
}


/*
 * Name:    calculate_backward_md2
 * Purpose: Calculate mini delta2 for backward searches
 * Date:    October 31, 1992
 * Passed:  p:  pointer to pattern
 *          len: length of pattern
 * Notes:   the backward mini delta2 is, from the start of the string, the
 *            first rightmost occurrence of the leftmost character.  in the
 *            worst case, mini delta2 is -1.  typically, mini delta2 is the
 *            negative length of the search pattern.
 */
int  calculate_backward_md2( char *p, int len )
{
int  first_c;
register int i;
register int md2;

   md2 = -1;
   i = 1;
   first_c = *p;
   if (mode.search_case == IGNORE) {
      first_c = tolower( first_c );
      for (; i < len  &&  first_c != tolower( p[i] ); i++)
         --md2;
   } else
      for (; i < len  &&  first_c != p[i]; i++)
         --md2;

   assert( md2 <= -1  &&  md2 >= -len );

   return( md2 );
}


/*
 * Name:    forward_boyer_moore_search
 * Purpose: search forward for pattern using boyer array
 * Passed:  window:  pointer to current window
 *          rline:   pointer to real line counter
 *          rcol:    pointer to real column variable
 * Returns: position in file if found or NULL if not found
 * Date:    June 5, 1991
 * Notes:   Start searching from cursor position to end of file.  If we hit
 *          end of file without a match, start searching from the beginning
 *          of file to cursor position.  (do wrapped searches)
 */
line_list_ptr forward_boyer_moore_search( WINDOW *window, long *rline,
     int *rcol )
{
register int len;
int  i;
int  end;
register WINDOW *win;  /* put window pointer in a register */
line_list_ptr ll;

   /*
    * if cursor is beyond end of line then start at end of line
    */
   win  = window;
   i = win->rcol + 1;
   len = win->ll->len;
   if (i > len)
      i = len;
   if (i < 0)
      i = 0;


   *rcol = i;

   assert( *rcol >= 0 );

   *rline = win->rline;
   ll = search_forward( win->ll, rline, (size_t *)rcol );

   if (ll == NULL) {

      end = 0;
      if (win->ll->next != NULL) {
         end = win->ll->next->len;
         win->ll->next->len = EOF;
      }

      /*
       * haven't found pattern yet - now search from beginning of file
       */
      g_status.wrapped = TRUE;

      *rcol = 0;
      *rline = 1L;
      ll = search_forward( win->file_info->line_list, rline, (size_t *)rcol );

      if (ll == win->ll  &&  *rcol >= win->rcol)
         ll = NULL;

      if (win->ll->next != NULL)
         win->ll->next->len = end;
   }

   if (ll != NULL)
      bin_offset_adjust( win, *rline );
   return( ll );
}


/*
 * Name:    search_forward
 * Purpose: search forward for pattern using boyer array
 * Passed:  ll:     pointer to current node in linked list
 *          rline:  pointer to real line counter
 *          offset: offset into ll->line to begin search
 * Returns: position in file if found or NULL if not found
 * Date:    January 8, 1992
 * Notes:   mini delta2 is the first leftmost occurrence of the rightmost
 *            character.
 */
line_list_ptr search_forward( line_list_ptr ll, long *rline, size_t *offset )
{
register int i;
text_ptr p;
text_ptr q;
int  mini_delta2;
unsigned int  mini_guard;
int  guard;
int  pat_len;
int  len_s;
text_ptr s;
char *skip;
boyer_moore_type *bmp;

   if (ll->len == EOF)
      return( NULL );
   else {
      if (g_status.command == DefineGrep  ||  g_status.command == RepeatGrep)
         bmp = &sas_bm;
      else
         bmp = &bm;

      pat_len  = bmp->pattern_length;
      mini_delta2 = bmp->forward_md2;
      skip = bmp->skip_forward;
      p    = bmp->pattern;

      i = pat_len - 1;
      guard = -i;
      mini_guard = *p;
      if (mode.search_case != MATCH)
         mini_guard = tolower( mini_guard );

      s = ll->line;
      s += *offset;
      len_s = ll->len - *offset;
      for (;;) {
         /*
          * Boyer-Moore fast skip loop.  check character count as we move
          *   down the line.
          */
         for (s+=i, len_s-=i; len_s > 0 && (i = skip[(unsigned char)*s]);
                              s+=i, len_s-=i);
         if (len_s > 0) {
            /*
             * i == 0, possible match.  Boyer-Moore slow match loop.
             */

            if (mode.search_case == MATCH) {
               if (s[guard] != mini_guard)
                  goto shift_func;

               q = s + 1 - pat_len;
               for (i=0; i < pat_len; i++)
                  if (q[i] != p[i])
                     goto shift_func;
            } else {
               if ((unsigned int)tolower( s[guard] ) != mini_guard)
                  goto shift_func;

               q = s + 1 - pat_len;
               for (i=0; i < pat_len; i++)
                  if (tolower( q[i] ) != tolower( p[i] ))
                     goto shift_func;
            }
            *offset = (size_t)(q - ll->line);

            assert( *offset <= (unsigned)(ll->len - pat_len) );

            return( ll );
         }
shift_func:
         if (len_s <= 0) {
            ++*rline;
            ll = ll->next;
            s = ll->line;
            if (ll->len == EOF)
               return( NULL );
            len_s = ll->len;
            i = pat_len - 1;
         } else
            i = mini_delta2;
      }
   }
}


/*
 * Name:    backward_boyer_moore_search
 * Purpose: search backward for pattern using boyer array
 * Passed:  window:  pointer to current window
 *          rline:   pointer current real line counter
 *          rcol:    pointer to real column
 * Returns: position in file if found or NULL if not found
 * Date:    June 5, 1991
 * Notes:   Start searching from cursor position to beginning of file. If we
 *          hit beginning end of file without a match, start searching from the
 *          end of file to cursor position.  (do wrapped searches)
 */
line_list_ptr backward_boyer_moore_search( WINDOW *window, long *rline, int *rcol )
{
int  i;
int  len;
int  end;
register WINDOW *win;  /* put window pointer in a register */
line_list_ptr ll;

   win  = window;
   *rline = win->rline;

   /*
    * see if cursor is on EOF line.  if so, move search start to previous node.
    */
   if (win->ll->len != EOF) {
      ll = win->ll;
      i  = win->rcol - 1;
      i += bm.pattern_length - 1;
      len = ll->len;
      if (i >= len)
         i = len - 1;
   } else {
      ll = win->ll->prev;
      --*rline;
      i = 0;
      if (ll != NULL)
         i = ll->len - 1;
   }
   *rcol = i;
   ll = search_backward( ll, rline, (size_t *)rcol );

   if (ll == NULL  &&  win->rline <= win->file_info->length) {

      end = 0;
      if (win->ll->prev != NULL) {
         end = win->ll->prev->len;
         win->ll->prev->len = EOF;
      }

      /*
       * haven't found pattern yet - now search from end of file
       */
      g_status.wrapped = TRUE;
      ll = win->file_info->line_list_end;
      if (ll->prev != NULL)
         *rcol = ll->prev->len;
      else
        *rcol = 0;
      *rline = win->file_info->length;
      ll = search_backward( ll->prev, rline, (size_t *)rcol );

      if (ll == win->ll  &&  *rcol <= win->rcol)
         ll = NULL;

      if (win->ll->prev != NULL)
         win->ll->prev->len = end;
   }
   if (ll != NULL)
      bin_offset_adjust( win, *rline );
   return( ll );
}


/*
 * Name:    search_backward
 * Purpose: search backward for pattern using boyer array
 * Passed:  ll:  pointer to node in linked list to start search
 *          rline:  pointer to real line counter
 *          offset:  offset into ll->line to start search
 * Returns: position in file if found else return NULL
 * Date:    January 8, 1992
 * Notes:   Start searching from cursor position to beginning of file.
 *          mini delta2 is the first rightmost occurrence of the leftmost character.
 */
line_list_ptr search_backward( line_list_ptr ll, long *rline, size_t *offset )
{
register int i;
text_ptr p;
int  mini_delta2;
int  pat_len;
int  len_s;
text_ptr s;

   if (ll == NULL)
      return( NULL );
   if (ll->len == EOF)
      return( NULL );
   else {
      mini_delta2 = bm.backward_md2;
      pat_len  = bm.pattern_length;
      p = bm.pattern;

      i = -bm.pattern_length + 1;

      s = ll->line;
      s += *offset;
      len_s = *offset + 1;
      for (;;) {

         /*
          * Boyer-Moore fast skip loop.  check character count as we move
          *   up the line.
          */
         for (s+=i, len_s+=i; len_s > 0 &&
                 (i = bm.skip_backward[(unsigned char)*s]); s+=i, len_s+=i);
         if (len_s > 0) {
            /*
             * i == 0, possible match.  Boyer-Moore slow match loop.
             */
            if (mode.search_case == MATCH) {
               for (i=0; i < pat_len; i++)
                  if (s[i] != p[i])
                    goto shift_func;
            } else {
               for (i=0; i < pat_len; i++)
                  if (tolower( s[i] ) != tolower( p[i] ))
                     goto shift_func;
            }
            *offset =(size_t)(s - ll->line);

            assert( *offset <= (unsigned)(ll->len - pat_len) );

            return( ll );
         }
shift_func:
         if (len_s <= 0) {
            --*rline;
            ll = ll->prev;
            if (ll == NULL)
               return( NULL );
            if (ll->len == EOF)
               return( NULL );
            len_s = ll->len;
            s = ll->line + len_s - 1;
            i = 1 - pat_len;
         } else
            i = mini_delta2;
      }
   }
}


/*
 * Name:    show_search_message
 * Purpose: display search status
 * Date:    January 8, 1992
 * Passed:  i:     index into message array
 *          color: color to display message
 */
void show_search_message( int i, int color )
{
   /*
    *  0 = blank
    *  1 = wrapped...
    *  2 = searching
    *  3 = replacing
    *  4 = nfa choked
    */
   assert( i >= 0  &&  i <= 4);
   s_output( find7[i], g_display.mode_line, 67, color );
}


/*
 * Name:    bin_offset_adjust
 * Purpose: place cursor on screen given a position in file - default cline
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          rline:   real line position some where in the file
 * Notes:   in binary mode, we keep an accurate count of the offset of the
 *          cursor from the beginning of the file.
 */
void bin_offset_adjust( WINDOW *window, long rline )
{
line_list_ptr   node;
long            found_distance;

   assert( rline >= 1L  &&  rline <= window->file_info->length );

   found_distance = window->rline - rline;
   node = window->ll;
   if (found_distance < 0) {
      while (found_distance++ < 0) {
         window->bin_offset += node->len;
         node = node->next;
      }
   } else if (found_distance > 0) {
      while (found_distance-- > 0) {
         node = node->prev;
         window->bin_offset -= node->len;
      }
   }
   assert( window->bin_offset >= 0 );
}


/*
 * Name:    find_adjust
 * Purpose: place cursor on screen given a position in file - default cline
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          ll:   position anywhere in file
 *          rline:  real line number of ll in the file
 *          rcol:   real column of ll in the file
 * Notes:   ll could be anywhere in file.  Find the start of line that
 *          ll is on.  Determine if start of line is behind or ahead of
 *          current line.  Once that is done, it is easy to determine if ll
 *          is on screen.  Lastly, current cursor position becomes start of
 *          ll line - reposition and display.
 */
void find_adjust( WINDOW *window, line_list_ptr ll, long rline, int rcol )
{
int  cmd;
long test_line;
file_infos *file;
register WINDOW *win;  /* put window pointer in a register */

   win = window;
   file = win->file_info;

   /*
    * given a real column, adjust for inflated tabs.
    */
   if (mode.inflate_tabs)
      rcol = detab_adjust_rcol( ll->line, rcol );

   /*
    * if p, start of found line, is greater than current line, see if
    * p is between current line and bottom line on screen.
    */
   if (win->rline < rline) {
      /*
       * test_line is the number of lines between rline and found line.
       */
      test_line = rline - win->rline;
      if ((long)win->cline + test_line <= (long)win->bottom_line)
         win->cline += (int)test_line;
      else
         file->dirty = LOCAL;

   /*
    * if p, start of found line, is less than current line, see if
    * p is between current line and top line on screen.
    */
   } else if (win->rline > rline) {
      test_line = win->rline - rline;
      if ((long)win->cline - test_line > (long)(win->top_line+win->ruler-1))
         win->cline -= (int)test_line;
      else
         file->dirty = LOCAL;
      if (rline < (long)(win->cline - (win->top_line+win->ruler-1)))
         win->cline = (int)rline + win->top_line+win->ruler - 1;
   }

   /*
    * cursor line becomes found line.  check if column is on screen.
    */
   win->rline = rline;
   win->ll    = ll;
   if (file->dirty == LOCAL && (win->cline == win->bottom_line ||
                                win->cline == win->top_line + win->ruler)) {
      cmd = g_status.command;
      if (cmd == RepeatFindForward1    ||  cmd == RepeatFindBackward1 ||
          cmd == DefineDiff            ||  cmd == RepeatDiff          ||
          cmd == FindRegX              ||  cmd == RepeatFindRegX      ||
          cmd == DefineGrep            ||  cmd == RepeatGrep) {
         center_window( win );
      } else if (cmd == ReplaceString) {
         if (win->visible)
            center_window( win );
      }
   }
   check_virtual_col( win, rcol, rcol );
}


/*
 * Name:    replace_string
 * Purpose: To set up and perform a replace operation.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  replace_string( WINDOW *window )
{
int  direction;
char pattern[MAX_COLS];  /* the old and replacement text */
int  net_change;
int  sub_len;
int  file_changed;
int  finished;
int  use_prev_find;
int  rc;
int  rcol;
int  rcol_limit;
int  wrapped;
int  wrapped_state;
long found_line;
long line_limit;
line_list_ptr   ll;
WINDOW          wp;
WINDOW          old_wp;
register        WINDOW *win;    /* put window pointer in a register */
int             visible;

   win = window;
   direction = get_replace_direction( win );
   if (direction == ERROR)
      return( ERROR );

   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );

   /*
    * get the search pattern, using the previous as the default
    */
   *pattern = '\0';
   if (g_status.replace_defined == OK) {

      assert( strlen( g_status.pattern ) < MAX_COLS );

      strcpy( pattern, g_status.pattern );
   }

   /*
    * string to find
    */
   if (get_name( find9, win->bottom_line, pattern,
                 g_display.message_color ) != OK  ||  *pattern == '\0')
      return( ERROR );

   assert( strlen( pattern ) < MAX_COLS );

   strcpy( g_status.pattern, pattern );

   /*
    * get the replacement text, using the previous as the default
    */
   *pattern = '\0';
   if (g_status.replace_defined == OK) {

      assert( strlen( g_status.subst ) < MAX_COLS );

      strcpy( pattern, g_status.subst );
   }
   if (get_name( find10, win->bottom_line, pattern,
                 g_display.message_color ) != OK)
      return( ERROR );

   assert( strlen( pattern ) < MAX_COLS );

   strcpy( g_status.subst, pattern );
   sub_len = strlen( pattern );
   strcpy( (char *)bm.pattern, g_status.pattern );
   net_change = sub_len - strlen( g_status.pattern );

   /*
    * get the replace flags, Prompt or NoPrompt
    */
   if (get_replacement_flags( win->bottom_line ) != OK)
      return( ERROR );

   build_boyer_array( );
   dup_window_info( &wp, win );
   update_line( win );
   g_status.replace_defined = OK;

   if (mode.inflate_tabs)
      win->rcol = entab_adjust_rcol( win->ll->line, win->ll->len, win->rcol );

   rc = OK;
   visible = win->visible;
   if (g_status.replace_flag == NOPROMPT) {
      wp.visible = FALSE;
      xygoto( win->ccol, win->cline );
      show_search_message( REPLACING, g_display.diag_color );
   }
   wrapped_state = wrapped = FALSE;
   finished = FALSE;
   file_changed = FALSE;
   use_prev_find = FALSE;
   line_limit = 0;
   rcol_limit = 0;
   dup_window_info( &old_wp, &wp );
   if (direction == FORWARD) {
      if ((ll = forward_boyer_moore_search( &wp, &found_line, &rcol )) != NULL &&
                !g_status.control_break) {
         line_limit = found_line;
         rcol_limit = rcol;
         if (g_status.wrapped)
            wrapped_state = wrapped = TRUE;
         rc = replace_and_display( &wp, ll, found_line, rcol, &finished,
                                   &file_changed, direction );
         if (rc == ERROR &&  g_status.replace_flag == PROMPT  && wrapped_state)
            use_prev_find = TRUE;
      } else {
         /*
          * string not found
          */
         error( WARNING, win->bottom_line, find8 );
         finished = TRUE;
         rc = ERROR;
      }
      while (finished == FALSE) {
         use_prev_find = wrapped_state = FALSE;
         dup_window_info( &old_wp, &wp );
         if (wp.visible == TRUE)
            update_line( &wp );
         if ((ll = forward_boyer_moore_search( &wp, &found_line, &rcol )) != NULL &&
             !g_status.control_break) {
            if (g_status.wrapped)
               wrapped_state = wrapped = TRUE;
            if (wrapped) {
               if (found_line > line_limit) {
                  finished = TRUE;
                  use_prev_find = TRUE;
               } else if (found_line == line_limit  &&  rcol == rcol_limit) {
                  finished = TRUE;
                  use_prev_find = TRUE;
               }
            }
            if (finished == FALSE) {
               rc = replace_and_display( &wp, ll, found_line, rcol, &finished,
                                         &file_changed, direction );
               if (rc == OK && ll == win->ll && rcol < rcol_limit)
                  rcol_limit += net_change;
               if (rc == ERROR &&  g_status.replace_flag == PROMPT  &&
                                        wrapped_state)
                  use_prev_find = TRUE;
            }
         } else {
            if (g_status.control_break)
               rc = getkey( );
            /*
             * string not found     or   control break
             */
            if (g_status.control_break)
               error( WARNING, win->bottom_line, cb );
            else if (wp.visible)
               error( WARNING, win->bottom_line, find8 );
            finished = TRUE;
            rc = ERROR;
         }
      }
   } else {
      if ((ll = backward_boyer_moore_search( &wp, &found_line, &rcol )) != NULL &&
          !g_status.control_break) {
         line_limit = found_line;
         rcol_limit = rcol;
         if (g_status.wrapped)
            wrapped_state = wrapped = TRUE;
         replace_and_display( &wp, ll, found_line, rcol, &finished, &file_changed,
                              direction );
         if (rc == ERROR &&  g_status.replace_flag == PROMPT  && wrapped_state)
            use_prev_find = TRUE;
      } else {
         /*
          * string not found
          */
         error( WARNING, win->bottom_line, find8 );
         finished = TRUE;
         rc = ERROR;
      }
      while (finished == FALSE) {
         use_prev_find = wrapped_state = FALSE;
         dup_window_info( &old_wp, &wp );
         if (wp.visible == TRUE)
            update_line( &wp );
         if ((ll = backward_boyer_moore_search( &wp, &found_line, &rcol )) != NULL &&
             !g_status.control_break) {
            if (g_status.wrapped)
               wrapped_state = wrapped = TRUE;
            if (wrapped) {
               if (found_line < line_limit) {
                  finished = TRUE;
                  use_prev_find = TRUE;
               } else if (found_line == line_limit  &&  rcol == rcol_limit) {
                  finished = TRUE;
                  use_prev_find = TRUE;
               }
            }
            if (finished == FALSE) {
               rc = replace_and_display( &wp, ll, found_line, rcol, &finished,
                                         &file_changed, direction );
               if (rc == OK && found_line == line_limit && rcol < rcol_limit)
                  rcol_limit += net_change;
               if (rc == ERROR  && g_status.replace_flag == PROMPT  &&
                                        wrapped_state)
                  use_prev_find = TRUE;
            }
         } else {
            if (g_status.control_break)
               rc = getkey( );
            /*
             * string not found    or   control break
             */
            if (g_status.control_break)
               error( WARNING, win->bottom_line, cb );
            else if (wp.visible)
               error( WARNING, win->bottom_line, find8 );
            finished = TRUE;
            rc = ERROR;
         }
      }
   }

   if (g_status.replace_flag == PROMPT) {
      if (use_prev_find)
         dup_window_info( &wp, &old_wp );
      dup_window_info( win, &wp );
   } else {
      show_search_message( CLR_SEARCH, g_display.mode_color );
      g_status.wrapped = FALSE;
   }

   if (mode.inflate_tabs)
      win->rcol = detab_adjust_rcol( win->ll->line, win->rcol );

   win->visible = visible;
   check_virtual_col( win, win->rcol, win->ccol );
   if (win->file_info->dirty != LOCAL && win->file_info->dirty != GLOBAL)
      show_curl_line( win );
   if (file_changed)
      win->file_info->modified = TRUE;
   make_ruler( win );
   show_ruler( win );
   show_ruler_pointer( win );
   return( rc );
}


/*
 * Name:    replace_and_display
 * Purpose: To make room for replacement string
 * Date:    June 5, 1991
 * Passed:  window:            pointer to current window
 *          ll:                pointer to position of pattern in file
 *          rline:             pointer to real line counter
 *          rcol:              pointer to real column
 *          finished:          stop replacing on this occurrence?
 *          modified:          skip this replacement?
 *          direction:         direction of search
 * Notes:   Show user where pattern_location is on screen if needed.
 *          Replace and display if needed.   Always ask the user if he
 *          wants to do wrapped replacing.
 */
int  replace_and_display( WINDOW *window, line_list_ptr ll, long rline,
                     int rcol, int *finished, int *modified, int direction )
{
register int rc;
file_infos *file;
register WINDOW *win;  /* put window pointer in a register */

   win = window;
   file = win->file_info;
   rc = OK;
   if (g_status.wrapped) {
      rc = ask_wrap_replace( win, finished );
      g_status.wrapped = FALSE;
      show_search_message( CLR_SEARCH, g_display.mode_color );
   }
   if (rc == OK) {
      find_adjust( win, ll, rline, rcol );
      if (win->visible == TRUE) {
         make_ruler( win );
         show_ruler( win );
         show_ruler_pointer( win );
         xygoto( win->ccol, win->cline );
         if (file->dirty) {
            display_current_window( win );
            file->dirty = FALSE;
         } else
            show_curl_line( win );
      }

      if (g_status.replace_flag == PROMPT && rc == OK) {
         show_line_col( win );
         rc = ask_replace( win, finished );
      }
      if (rc == OK) {
         do_replace( win, direction );
         *modified = TRUE;
         file->dirty = GLOBAL;
         if (win->visible == TRUE) {
            show_changed_line( win );
            file->dirty = FALSE;
         }
      }
      if (mode.inflate_tabs) {
         if (win->rcol >= 0)
            win->rcol = entab_adjust_rcol( ll->line, ll->len, win->rcol );
      }
   }
   return( rc );
}


/*
 * Name:    scan_forward
 * Purpose: To find the corresponding occurrence of target, ignoring
 *           embedded pairs of opp and target, searching forwards.
 * Date:    June 5, 1991
 * Passed:  rline:  pointer to real line position
 *          rcol:   pointer to real column position
 *          ll:     pointer to current node in linked list
 *          opp:    the opposite to target
 *          target: the string to be found
 *          rc:     OK if found, ERROR otherwise
 * Returns: the location of the corresponding target in the text buffer
 * Notes:   Every 8,000 characters, check pointer forward.
 */
line_list_ptr scan_forward( long *rline, int *rcol, line_list_ptr ll,
                            char opp, char target, int *rc )
{
text_ptr s;
int  count = 0;          /* number of unmatched opposites found */
int  len;
register char c;

   *rc = OK;
   if (ll != NULL  &&  ll->len != EOF) {
      len = ll->len - *rcol - 1;
      s = ll->line + *rcol + 1;
      while (ll->len != EOF) {
         while (len > 0) {

            assert( s != NULL);

            c = *s++;
            --len;
            if (opp == c)
               count++;
            else if (target == c) {
               if (count == 0)
                 goto match;
               --count;
            }
         }
         ++*rline;
         ll  = ll->next;
         s   = ll->line;
         len = ll->len;
      }
match:
      if (ll->len != EOF) {

         assert( s != NULL );

         *rcol = (int)(s - ll->line - 1);
      } else
         *rc = ERROR;
   } else
      *rc = ERROR;

   if (*rc == OK) {
      assert( *rcol >= 0 );
      assert( *rcol < ll->len );
   }

   return( ll );
}


/*
 * Name:    scan_backward
 * Purpose: To find the corresponding occurrence of target, ignoring
 *           embedded pairs of opp and target, searching backwards.
 * Date:    June 5, 1991
 * Passed:  rline:  pointer to real line position
 *          rcol:   pointer to real column position
 *          ll:     pointer to current node in linked list
 *          opp:    the opposite to target
 *          target: the string to be found
 *          rc:     OK if found, ERROR otherwise
 * Returns: the location of the corresponding target in the text buffer
 */
line_list_ptr scan_backward( long *rline, int *rcol, line_list_ptr ll,
                             char opp, char target, int *rc )
{
text_ptr s;
int  count = 0;         /* number of unmatched opposites found */
register int len;
register char c;

   *rc = OK;
   if (ll != NULL  &&  ll->len != EOF) {
      s = ll->line + *rcol - 1;
      len = *rcol;
      while (ll != NULL) {
         while (len > 0) {

            assert( s != NULL );

            c = *s--;
            if (opp == c)
               count++;
            else if (target == c) {
               if (count == 0)
                 goto match;
               --count;
            }
            --len;
         }
         --*rline;
         ll = ll->prev;
         if (ll != NULL) {
            len = ll->len;
            s = ll->line + len - 1;
         }
      }
match:
      if (ll != NULL) {

         assert( s != NULL );

         *rcol = (int)(s - ll->line + 1);
      } else
         *rc = ERROR;
   } else
      *rc = ERROR;

   if (*rc == OK) {
      assert( *rcol >= 0 );
      assert( *rcol < ll->len );
   }

   return( ll );
}


/*
 * Name:    match_pair
 * Purpose: To find the corresponding pair to the character under the
 *           cursor.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Searching is very simple-minded, and does not cope with things
 *          like brackets embedded within quoted strings.
 */
int  match_pair( WINDOW *window )
{
long rline;
char c;
register WINDOW *win;  /* put window pointer in a register */
int  rc;
int  rcol;
line_list_ptr ll;

   win = window;
   entab_linebuff( );
   rline = win->rline;
   ll = win->ll;
   if (un_copy_line( ll, win, TRUE ) == ERROR)
      return( ERROR );
   /*
    * make sure the character under the cursor is one that has a
    *  matched pair
    */
   if (ll->len == EOF || win->rcol >= ll->len)
      return( ERROR );
   rcol = win->rcol;
   c = *(ll->line + rcol);
   rc = OK;

   /*
    * find the matching pair
    */
   switch (c) {
      case '[':
         ll = scan_forward( &rline, &rcol, ll, '[', ']', &rc );
         break;
      case '(':
         ll = scan_forward( &rline, &rcol, ll, '(', ')', &rc );
         break;
      case '{':
         ll = scan_forward( &rline, &rcol, ll, '{', '}', &rc );
         break;
      case ']':
         ll = scan_backward( &rline, &rcol, ll, ']', '[', &rc );
         break;
      case ')':
         ll = scan_backward( &rline, &rcol, ll, ')', '(', &rc );
         break;
      case '}':
         ll = scan_backward( &rline, &rcol, ll, '}', '{', &rc );
         break;
      default :
         rc = ERROR;
   }

   /*
    * now show the user what we have found
    */
   if (rc == OK) {
      update_line( win );
      bin_offset_adjust( win, rline );
      find_adjust( win, ll, rline, rcol );
      if (!win->file_info->dirty)
         show_curl_line( win );
      make_ruler( win );
      show_ruler( win );
   }
   return( rc );
}
