
#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * Name:    tab_key
 * Purpose: To make the necessary changes after the user types the tab key.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If in insert mode, then this function adds the required
 *           number of spaces in the file.
 *          If not in insert mode, then tab simply moves the cursor right
 *           the required distance.
 */
int  tab_key( WINDOW *window )
{
int  spaces;    /* the spaces to move to the next tab stop */
char *source;   /* source for block move to make room for c */
char *dest;     /* destination for block move */
int  pad;
int  len;
register int rcol;
int  old_bcol;
register WINDOW *win;   /* put window pointer in a register */
int  rc;

   win  = window;
   if (win->ll->len  ==  EOF)
      return( OK );
   rcol = win->rcol;
   old_bcol = win->bcol;
   show_ruler_char( win );
   /*
    * work out the number of spaces to the next tab stop
    */
   if (mode.smart_tab)
      spaces = next_smart_tab( win );
   else
      spaces = mode.ltab_size - (rcol % mode.ltab_size);

   assert( spaces >= 0 );
   assert( spaces < MAX_LINE_LENGTH );

   rc = OK;
   if (mode.insert && rcol + spaces < g_display.line_length) {
      copy_line( win->ll );
      detab_linebuff( );

      /*
       * work out how many characters need to be inserted
       */
      len = g_status.line_buff_len;
      pad = rcol > len ? rcol - len : 0;
      if (len + pad + spaces >= g_display.line_length) {
         /*
          *  line too long to add
          */
         error( WARNING, win->bottom_line, ed1 );
         rc = ERROR;
         g_status.copied = FALSE;
      } else {
         if (pad > 0  || spaces > 0) {
            source = g_status.line_buff + rcol - pad;
            dest = source + pad + spaces;

            assert( len + pad - rcol >= 0 );
            assert( len + pad - rcol < MAX_LINE_LENGTH );

            memmove( dest, source, len + pad - rcol );

            /*
             * if padding was required, then put in the required spaces
             */

            assert( pad + spaces >= 0 );
            assert( pad + spaces < MAX_LINE_LENGTH );

            memset( source, ' ', pad + spaces );
            g_status.line_buff_len += pad + spaces;
            entab_linebuff( );
         }

         win->ll->dirty = TRUE;
         win->file_info->dirty = GLOBAL;
         show_changed_line( win );
         rcol += spaces;
         win->ccol += spaces;
      }
   } else if (rcol + spaces <= g_display.line_length) {
      /*
       * advance the cursor without changing the text underneath
       */
      rcol += spaces;
      win->ccol += spaces;
   }
   check_virtual_col( win, rcol, win->ccol );
   if (old_bcol != win->bcol) {
      make_ruler( win );
      show_ruler( win );
   }
   return( rc );
}


/*
 * Name:    backtab
 * Purpose: To make the necessary changes after the user presses the backtab.
 * Date:    November 1, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If in insert mode, then this function subs the required
 *           number of spaces in the file.
 *          If not in insert mode, then tab simply moves the cursor left
 *           the required distance.
 */
int  backtab( WINDOW *window )
{
int  spaces;    /* the spaces to move to the next tab stop */
char *source;   /* source for block move to make room for c */
char *dest;     /* destination for block move */
int  pad;
int  len;
register int rcol;
int  old_bcol;
register WINDOW *win;   /* put window pointer in a register */

   win  = window;
   rcol = win->rcol;
   if (win->ll->len == EOF || win->rcol == 0)
      return( OK );
   old_bcol = win->bcol;
   show_ruler_char( win );

   /*
    * work out the number of spaces to the previous tab stop
    */
   if (mode.smart_tab)
      spaces = prev_smart_tab( win );
   else
      spaces = win->rcol % mode.ltab_size;

   if (spaces == 0)
      spaces = mode.ltab_size;
   copy_line( win->ll );
   detab_linebuff( );
   len = g_status.line_buff_len;
   if (mode.insert && rcol - spaces < len) {
      pad = rcol > len ? rcol - len : 0;
      if (pad > 0  || spaces > 0) {
         /*
          * if padding was required, then put in the required spaces
          */
         if (pad > 0) {

            assert( rcol - pad >= 0 );
            assert( pad < MAX_LINE_LENGTH );

            source = g_status.line_buff + rcol - pad;
            dest = source + pad;

            assert( pad >= 0 );
            assert( pad < MAX_LINE_LENGTH );

            memmove( dest, source, pad );
            memset( source, ' ', pad );
            g_status.line_buff_len += pad;
         }
         source = g_status.line_buff + rcol;
         dest = source - spaces;

         assert( len + pad - rcol >= 0 );
         assert( len + pad - rcol < MAX_LINE_LENGTH );

         memmove( dest, source, len + pad - rcol );
         g_status.line_buff_len -= spaces;
         entab_linebuff( );
      }

      win->ll->dirty = TRUE;
      win->file_info->dirty = GLOBAL;
      show_changed_line( win );
      rcol -= spaces;
      win->ccol -= spaces;
   } else {
      /*
       * move the cursor without changing the text underneath
       */
      rcol -= spaces;
      if (rcol < 0)
         rcol = 0;
      win->ccol -= spaces;
   }
   check_virtual_col( win, rcol, win->ccol );
   if (old_bcol != win->bcol) {
      make_ruler( win );
      show_ruler( win );
   }
   return( OK );
}


/*
 * Name:    next_smart_tab
 * Purpose: To find next smart tab
 * Date:    June 5, 1992
 * Passed:  window: pointer to the current window
 * Notes:   To find a smart tab 1) find the first non-blank line above the
 *            current line, 2) find the first non-blank character after
 *            column of the cursor.
 */
int  next_smart_tab( WINDOW *window )
{
register int spaces;    /* the spaces to move to the next tab stop */
text_ptr s;             /* pointer to text */
line_list_ptr ll;
register WINDOW *win;   /* put window pointer in a register */
int  len;

   /*
    * find first previous non-blank line above the cursor.
    */
   win = window;
   ll = win->ll->prev;
   while (ll != NULL  && is_line_blank( ll->line, ll->len ))
      ll = ll->prev;

   if (ll != NULL) {
      s = ll->line;
      /*
       * if cursor is past the eol of the smart line, lets find the
       *   next fixed tab.
       */
      if (window->rcol >= find_end( s, ll->len ))
         spaces = mode.ltab_size - (window->rcol % mode.ltab_size);
      else {

         len = ll->len;
         s = detab_a_line( s, &len );

         spaces = 0;
         s = s + window->rcol;
         len -= window->rcol;

         /*
          * if we are on a word, find the end of it.
          */
         while (*s != ' '  &&  len > 0) {
            ++s;
            ++spaces;
            --len;
         }

         /*
          * now find the start of the next word.
          */
         if (len > 0)
            while (*s == ' ' && len > 0) {
               ++s;
               ++spaces;
               --len;
            }
      }
   } else
      spaces = mode.ltab_size - (window->rcol % mode.ltab_size);
   return( spaces );
}


/*
 * Name:    prev_smart_tab
 * Purpose: To find previous smart tab
 * Date:    June 5, 1992
 * Passed:  window: pointer to the current window
 * Notes:   To find a smart tab 1) find the first non-blank line above the
 *            current line, 2) find the first non-blank character before
 *            column of the cursor.
 *          there are several cases to consider:  1) the cursor is past the
 *            the end of the smart line, 2) the smart pointer is in the
 *            middle of a word, 3) there are no more words between the
 *            smart pointer and the beginning of the line.
 */
int  prev_smart_tab( WINDOW *window )
{
register int spaces;    /* the spaces to move to the next tab stop */
text_ptr s;             /* pointer to text */
int  len;
line_list_ptr ll;
WINDOW *win;            /* put window pointer in a register */

   /*
    * find first previous non-blank line above the cursor, if it exists.
    */
   win = window;
   ll = win->ll->prev;
   while (ll != NULL  && is_line_blank( ll->line, ll->len ))
      ll = ll->prev;

   if (ll != NULL) {
      s = ll->line;

      /*
       * if there are no words between the cursor and column 1 of the
       *   smart tab line, find previous fixed tab.
       */
      if (window->rcol < first_non_blank( s, ll->len ))
         spaces = window->rcol % mode.ltab_size;
      else {

         len = ll->len;
         if (mode.inflate_tabs)
            s = detab_a_line( s, &len );

         /*
          * now, we need to figure the initial pointer and space.
          *   if the cursor is past the eol of the smart line, then
          *   set the smart pointer "s" to the end of line and "spaces" to
          *   the number of characters between the cursor and the eol
          *   of the smart line.  otherwise, set the smart pointer "s" to
          *   the column of the cursor and "spaces" to 0.
          */
         if (len < window->rcol) {
            s += len;
            spaces = window->rcol - len;
         } else {
            len = window->rcol;
            s += window->rcol;
            spaces = 0;
         }

         while (*(s-1) == ' ' && len > 0) {
            --s;
            ++spaces;
            --len;
         }

         /*
          * now find the beginning of the first word at eol.
          */
         while (*(s-1) != ' '  &&  len > 0) {
            --s;
            ++spaces;
            --len;
         }
         if (len == 0 && *s == ' ')
            spaces = window->rcol % mode.ltab_size;
         if (spaces > window->rcol)
            spaces = window->rcol;
      }
   } else
      spaces = window->rcol % mode.ltab_size;

   /*
    * spaces cannot be negative.
    */
   if (spaces < 0)
      spaces = 0;
   return( spaces );
}


/*
 * Name:    entab
 * Purpose: To compress spaces to tabs
 * Date:    October 31, 1992
 * Passed:  s: pointer to current line
 * Returns: pointer to tabout_buf
 * Notes:   the text_ptr can point to either the g_status.line_buff or
 *            a line in the main text buffer.
 *          this function assumes that a '\n' terminates the line.
 */
text_ptr entab( text_ptr s, int len )
{
int  tab_size;
int  last_col;
int  space;
register int col;
text_ptr to;

   assert( s != NULL );
   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );

   tab_size = mode.ptab_size;
   to = (text_ptr)g_status.tabout_buff;
   if (s == NULL)
      g_status.tabout_buff_len = 0;
   else {
      g_status.tabout_buff_len = len;
      for (last_col=col=0; ; s++, len--) {
         if (len == 0) {

            /*
             * when we reach the eol, compress trailing spaces to tabs.
             *   the un_copy_line function is responsible for trimming
             *   trailing space.
             */
            if (col != last_col) {
               while (last_col < col) {
                  space = tab_size - last_col % tab_size;
                  if (space <= 1) {
                     *to++ = ' ';
                     last_col++;
                  } else if (last_col + space <= col) {
                     *to++ = '\t';
                     last_col += space;
                     g_status.tabout_buff_len -= (space - 1);
                  } else {
                     *to++ = ' ';
                     last_col++;
                  }
               }
            }

            /*
             * stop entabbing when we get to EOL
             */
            break;
         } else if (*s == ' ')
            col++;
         else {
            if (col != last_col) {
               while (last_col < col) {
                  space = tab_size - last_col % tab_size;

                  /*
                   * when space == 1, forget about emmitting a tab
                   *   for 1 space.
                   */
                  if (space <= 1) {
                     *to++ = ' ';
                     last_col++;
                  } else if (last_col + space <= col) {
                     *to++ = '\t';
                     last_col += space;
                     g_status.tabout_buff_len -= (space - 1);
                  } else {
                     *to++ = ' ';
                     last_col++;
                  }
               }
            }

            /*
             * if *s == tab, then adjust the col pointer
             */
            if (*s == '\t')
               col = col + tab_size - (col % tab_size);
            else
               ++col;
            last_col = col;
            *to++ = *s;

            /*
             * when we see a quote character, stop entabbing.
             */
            if (*s == '\"' || *s == '\'') {
               while (len > 0) {
                 *to++ = *++s;
                 --len;
               }
               break;
            }
         }
      }
   }
   return( (text_ptr)g_status.tabout_buff );
}


/*
 * Name:    detab_linebuff
 * Purpose: To expand tabs in line buffer so we can handle editing functions
 * Date:    October 31, 1992
 * Notes:   before we do any editing function that modifies text, let's
 *            expand any tabs to space.
 *          if inflate_tabs is FALSE, then nothing is done.
 */
void detab_linebuff( void )
{
int  show_eol;
int  len;

   if (mode.inflate_tabs  &&  g_status.copied) {
      len = g_status.line_buff_len;
      show_eol = mode.show_eol;
      mode.show_eol = FALSE;
      tabout( (text_ptr)g_status.line_buff, &len );

      assert( len >= 0 );
      assert( len < MAX_LINE_LENGTH );

      memmove( g_status.line_buff, g_status.tabout_buff, len );
      g_status.line_buff_len = len;
      mode.show_eol = show_eol;
   }
}


/*
 * Name:    entab_linebuff
 * Purpose: To compress space in line buffer
 * Date:    October 31, 1992
 * Notes:   compress spaces back to tabs in the line buffer, if
 *             inflate_tabs == TRUE
 */
void entab_linebuff( void )
{
   if (mode.inflate_tabs  &&  g_status.copied) {
      entab( (text_ptr)g_status.line_buff, g_status.line_buff_len );

      assert( g_status.tabout_buff_len >= 0 );
      assert( g_status.tabout_buff_len < MAX_LINE_LENGTH );

      memmove( g_status.line_buff, g_status.tabout_buff,
                       g_status.tabout_buff_len );
      g_status.line_buff_len = g_status.tabout_buff_len;
   }
}


/*
 * Name:    detab_a_line
 * Purpose: To inflate tabs in any line in the file
 * Date:    October 31, 1992
 * Returns: pointer to text or tabout_buff
 * Notes:   expand an arbitrary line in the file.
 */
text_ptr detab_a_line( text_ptr s, int *len )
{
int  show_eol;

   if (mode.inflate_tabs) {

      assert( *len >= 0 );
      assert( *len < MAX_LINE_LENGTH );
      assert( s != NULL );

      show_eol = mode.show_eol;
      mode.show_eol = FALSE;
      s = tabout( s, len );
      mode.show_eol = show_eol;
   }
   return( s );
}


/*
 * Name:    detab_adjust_rcol
 * Purpose: given rcol in a line, find virtual column
 * Date:    October 31, 1992
 * Passed:  s:  string pointer
 * Notes:   without expanding tabs, calculate the display column according
 *            to current tab stop.
 */
int  detab_adjust_rcol( text_ptr s, int rcol )
{
register int col;

   assert( rcol >= 0 );
   assert( rcol < MAX_LINE_LENGTH );
   assert( s != NULL );
   assert( mode.ptab_size != 0 );

   for (col=0; rcol > 0; rcol--,s++) {
      if (*s == '\t')
         col += (mode.ptab_size - (col % mode.ptab_size));
      else if (rcol > 0)
         col++;
   }

   assert( col >= 0 );
   assert( col < MAX_LINE_LENGTH );

   return( col );
}


/*
 * Name:    entab_adjust_rcol
 * Purpose: given virtual rcol in a line, find real column
 * Date:    October 31, 1992
 * Passed:  s:     string pointer
 *          len:   length of string
 *          rcol:  virtual real column
 * Notes:   without expanding tabs, calculate which col the real cursor
 *            referencing.
 */
int  entab_adjust_rcol( text_ptr s, int len, int rcol )
{
register int col;
register int last_col;

   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );
   assert( rcol >= 0 );
   assert( rcol < MAX_LINE_LENGTH );
   assert( mode.ptab_size != 0 );

   if (s != NULL) {
      for (last_col=col=0; col < rcol  &&  s != NULL  &&  len > 0; s++, len--) {
         if (*s != '\t')
            ++col;
         else
            col += (mode.ptab_size - (col % mode.ptab_size));
         if (col > rcol)
            break;
         ++last_col;
      }
   } else
      last_col = rcol;

   assert( last_col >= 0 );
   assert( last_col < MAX_LINE_LENGTH );

   return( last_col );
}


/*
 * Name:    block_expand_tabs
 * Purpose: Expand tabs in a marked block.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Tabs are expanded using the current tab interval.
 *          Lines are checked to make sure they are not too long.
 */
int  block_expand_tabs( WINDOW *window )
{
int  prompt_line;
int  len;
int  tab;
int  tab_size;
int  dirty;
register int spaces;
line_list_ptr p;                /* pointer to block line */
file_infos *file;
WINDOW *sw, s_w;
long er;
int  i;
int  rc;
char *b;

   /*
    * make sure block is marked OK and that this is a LINE block
    */
   prompt_line = window->bottom_line;

   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   check_block( );
   rc = OK;
   if (g_status.marked == TRUE) {

      file  = g_status.marked_file;
      if (file->block_type != LINE) {
         /*
          * can only expand tabs in line blocks
          */
         error( WARNING, prompt_line, block20 );
         return( ERROR );
      }

      /*
       * initialize everything
       */
      dirty = FALSE;
      tab_size = mode.ptab_size;
      sw = g_status.window_list;
      for (; ptoul( sw->file_info ) != ptoul( file );)
         sw = sw->next;
      dup_window_info( &s_w, sw );
      p  = file->block_start;
      er = file->block_er;
      s_w.rline = file->block_br;
      s_w.visible = FALSE;
      for (; s_w.rline <= er  &&  !g_status.control_break; s_w.rline++) {

         /*
          * use the line buffer to expand LINE blocks.
          */
         tab = FALSE;
         g_status.copied = FALSE;

         copy_line( p );
         len = g_status.line_buff_len;
         for (b=g_status.line_buff, i=1; len > 0  &&  rc == OK; b++, len--) {

            /*
             * each line in the LINE block is copied to the g_status.line_buff.
             *  look at the text in the buffer and expand tabs.
             */
            if (*b == '\t') {
               tab = TRUE;
               spaces = i % tab_size;
               if (spaces)
                  spaces = tab_size - spaces;
               if (spaces) {

                  assert( len >= 0 );
                  assert( len < MAX_LINE_LENGTH );

                  memmove( b + spaces, b, len );
               }

               assert( spaces + 1 >= 0 );
               assert( spaces + 1 < MAX_LINE_LENGTH );

               memset( b, ' ', spaces+1 );
               i += spaces + 1;
               b += spaces;
               g_status.line_buff_len += spaces;
            } else
               i++;
         }

         /*
          * if any tabs were found, write g_status.line_buff to file.
          */
         if (tab) {
            rc = un_copy_line( p, &s_w, TRUE );
            dirty = TRUE;
         }
         p = p->next;
      }

      /*
       * IMPORTANT:  we need to reset the copied flag because the cursor may
       * not necessarily be on the last line of the block.
       */
      g_status.copied = FALSE;
      if (dirty)
         file->dirty = GLOBAL;
   }
   return( rc );
}


/*
 * Name:    block_compress_tabs
 * Purpose: Expand tabs in a marked block.
 * Date:    October 31, 1992
 * Passed:  window:  pointer to current window
 * Notes:   Tabs are compress using the current tab setting.
 */
int  block_compress_tabs( WINDOW *window )
{
register int col;
register int spaces;
int  len;
int  rc;
int  prompt_line;
int  last_col;
int  tab;
int  tab_size;
int  dirty;
line_list_ptr p;                /* pointer to block line */
text_ptr from;                  /* line in main text buff being compressed */
file_infos *file;
WINDOW *sw, s_w;
long er;
char *to;
int  indent_only;

   /*
    * make sure block is marked OK and that this is a LINE block
    */
   prompt_line = window->bottom_line;
   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   check_block( );
   rc = OK;
   if (g_status.marked == TRUE) {

      file  = g_status.marked_file;
      if (file->block_type != LINE) {
         /*
          * can only compress tabs in line blocks
          */
         error( WARNING, prompt_line, block26 );
         return( ERROR );
      }

      indent_only = g_status.command == BlockIndentTabs ? TRUE : FALSE;

      /*
       * set the command to word wrap so the un_copy_line function will
       * not display the lines while expanding.
       */
      g_status.command = WordWrap;

      /*
       * initialize everything
       */
      dirty = FALSE;
      tab_size = mode.ptab_size;
      sw = g_status.window_list;
      for (; ptoul( sw->file_info ) != ptoul( file );)
         sw = sw->next;
      dup_window_info( &s_w, sw );
      s_w.visible = FALSE;
      s_w.ll  =  p  = file->block_start;
      er = file->block_er;
      s_w.rline = file->block_br;
      for (; rc == OK  &&  s_w.rline <= er  &&  !g_status.control_break; s_w.rline++) {

         /*
          * use the line buffer to compress LINE blocks.
          */
         tab = FALSE;

         from = p->line;
         to   = g_status.line_buff;
         g_status.line_buff_len = len  = p->len;

         for (last_col=col=0; ; from++, len--) {
            if (len == 0) {

               /*
                * when we reach the eol, compress trailing spaces to tabs.
                *   the un_copy_line function is responsible for trimming
                *   trailing space.
                */
               if (col != last_col) {
                  while (last_col < col) {
                     spaces = tab_size - last_col % tab_size;
                     if (spaces <= 1) {
                        *to++ = ' ';
                        last_col++;
                     } else if (last_col + spaces <= col) {
                        *to++ = '\t';
                        last_col += spaces;
                        g_status.line_buff_len -= (spaces - 1);
                        tab = TRUE;
                     } else {
                        *to++ = ' ';
                        last_col++;
                     }
                  }
               }

               /*
                * stop entabbing when we get to EOL
                */
               break;
            } else if (*from == ' ')
               col++;
            else {
               if (col != last_col) {
                  while (last_col < col) {
                     spaces = tab_size - last_col % tab_size;

                  /*
                   * when space == 1, forget about emmitting a tab
                   *   for 1 space.
                   */
                     if (spaces <= 1) {
                        *to++ = ' ';
                        last_col++;
                     } else if (last_col + spaces <= col) {
                        *to++ = '\t';
                        last_col += spaces;
                        g_status.line_buff_len -= (spaces - 1);
                        tab = TRUE;
                     } else {
                        *to++ = ' ';
                        last_col++;
                     }
                  }
               }

               /*
                * if *from == tab, then adjust the col pointer
                */
               if (*from == '\t')
                  col = col + tab_size - (col % tab_size);
               else
                  ++col;
               last_col = col;
               *to++ = *from;

               /*
                * when we see a quote character, stop entabbing.
                */
               if (*from == '\"' || *from == '\''  || indent_only) {
                  while (len > 0) {
                     *to++ = *++from;
                     --len;
                  }
                  break;
               }
            }
         }

         /*
          * if any tabs were found, write g_status.line_buff to file.
          */
         if (tab) {
            g_status.copied = TRUE;
            rc = un_copy_line( p, &s_w, TRUE );
            dirty = TRUE;
         }
         p = p->next;
      }

      /*
       * IMPORTANT:  we need to reset the copied flag because the cursor may
       * not necessarily be on the last line of the block.
       */
      g_status.copied = FALSE;
      if (dirty)
         file->dirty = GLOBAL;
   }
   return( rc );
}
