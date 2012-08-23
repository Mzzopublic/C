

#include "tdestr.h"     /* typedefs for global variables */
#include "define.h"     /* editor function defs */
#include "tdefunc.h"    /* prototypes for all functions in tde */
#include "global.h"     /* global variables */
#include "prompts.h"    /* prompt assignments */
#include "default.h"    /* default function key assignments */


/*
 * Name:    insert_newline
 * Purpose: insert a newline
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   There a several ways to insert a line into a file:  1) pressing
 *          a key, 2) word wrap, 3) any others?
 *          When doing word wrap or format paragraph, don't show any changes.
 *            Wait until the function finishes then show all changes at once.
 */
int  insert_newline( WINDOW *window )
{
char *source;           /* source for block move to make room for c */
char *dest;             /* destination for block move */
int  len;               /* length of current line */
int  split_len;
int  add;               /* characters to be added (usually 1 in insert mode) */
int  rcol;
int  rc;
long length;
int  carriage_return;
int  split_line;
int  wordwrap;
int  dirty;
int  old_bcol;
register WINDOW *win;   /* put window pointer in a register */
file_infos *file;       /* pointer to file structure in current window */
line_list_ptr new_node;
text_ptr new_line;      /* new line */

   rc = OK;
   win = window;
   file = win->file_info;
   length = file->length;
   wordwrap = mode.word_wrap;
   switch (g_status.command) {
      case WordWrap :
         carriage_return = TRUE;
         split_line = FALSE;
         break;
      case AddLine  :
         split_line = carriage_return = FALSE;
         break;
      case SplitLine :
         split_line = carriage_return = TRUE;
         break;
      case Rturn :
      default    :

         /*
          * if file is opened in BINARY mode, lets keep the user from
          *   unintentially inserting a line feed into the text.
          */
         if (file->crlf == BINARY)
            return( next_line( win ) );

         show_ruler_char( win );
         carriage_return = TRUE;
         split_line = FALSE;
         break;
   }

   /*
    * make window temporarily invisible to the un_copy_line function
    */
   new_node = (line_list_ptr)my_malloc( sizeof(line_list_struc), &rc );
   new_line = NULL;
   win->visible = FALSE;
   old_bcol = win->bcol;
   if (rc == OK) {
      new_node->line  = new_line;
      new_node->len   = 0;
      new_node->dirty = FALSE;

      if (win->ll->len != EOF) {
         win->file_info->modified = TRUE;
         if (mode.do_backups == TRUE)
            rc = backup_file( win );
         copy_line( win->ll );
         detab_linebuff( );
         len = g_status.line_buff_len;
         split_len = 0;
         if (win->rcol < len)
            win->ll->dirty = TRUE;

         source = g_status.line_buff + len;
         if (carriage_return || split_line) {
            if (win->rcol < len) {
               source = g_status.line_buff + win->rcol;
               split_len = len - win->rcol;
               len = win->rcol;
            }
         }
         g_status.line_buff_len = len;
         entab_linebuff( );
         if (un_copy_line( win->ll, win, TRUE ) == OK) {

            assert( split_len >= 0 );
            assert( split_len < MAX_LINE_LENGTH );

            memmove( g_status.line_buff, source, split_len );
            g_status.line_buff_len = len = split_len;
            g_status.copied = TRUE;
            entab_linebuff( );
         } else
            rc = ERROR;
      } else {
         g_status.line_buff_len = len = 0;
         g_status.copied = TRUE;
      }

      if (rc == OK) {
         new_node->line  = new_line;
         new_node->len   = 0;
         new_node->dirty = TRUE;

         /*
          * we are somewhere in the list and we need to insert the new node.
          *  if we are anywhere except the EOF node, insert the new node
          *  after the current node.  if the current node is the EOF node,
          *  insert the new node before the EOF node.  this keeps the
          *  EOF node at the end of the list.
          */
         if (win->ll->next != NULL) {
            win->ll->next->prev = new_node;
            new_node->next = win->ll->next;
            win->ll->next = new_node;
            new_node->prev = win->ll;
         } else {
            new_node->next = win->ll;
            if (win->ll->prev != NULL)
               win->ll->prev->next = new_node;
            new_node->prev = win->ll->prev;
            win->ll->prev = new_node;
            if (new_node->prev == NULL)
               win->file_info->line_list = new_node;
            win->ll = new_node;
         }

         ++file->length;
         detab_linebuff( );
         entab_linebuff( );
         rc = un_copy_line( new_node, win, FALSE );
         adjust_windows_cursor( win, 1 );

         file->dirty = NOT_LOCAL;
         if (length == 0l || wordwrap || win->cline == win->bottom_line)
            file->dirty = GLOBAL;
         else if (!split_line)
            update_line( win );

         /*
          * If the cursor is to move down to the next line, then update
          *  the line and column appropriately.
          */
         if (rc == OK  &&  (carriage_return || split_line)) {
            dirty = file->dirty;
            if (win->cline < win->bottom_line)
               win->cline++;
            win->rline++;
            if (win->ll->next != NULL) {
               win->bin_offset += win->ll->len;
               win->ll = win->ll->next;
            }
            rcol = win->rcol;
            old_bcol = win->bcol;

            if (win->ll->next != NULL) {
               if (mode.indent || wordwrap) {
                  /*
                   * autoindentation is required. Match the indentation of
                   *  the first line above that is not blank.
                   */
                  add = find_left_margin( wordwrap == FIXED_WRAP ?
                                          win->ll : win->ll->prev, wordwrap );

                  assert( add >= 0 );
                  assert( add < MAX_LINE_LENGTH );

                  copy_line( win->ll );
                  detab_linebuff( );
                  len = g_status.line_buff_len;
                  source = g_status.line_buff;
                  if (len + add > MAX_LINE_LENGTH)
                     add = MAX_LINE_LENGTH - len;
                  dest = source + add;

                  assert( len >= 0);
                  assert( len < MAX_LINE_LENGTH );

                  memmove( dest, source, len );

                  /*
                   * now put in the autoindent characters
                   */

                  assert( add >= 0 );
                  assert( add < MAX_LINE_LENGTH );

                  memset( source, ' ', add );
                  win->rcol = add;
                  g_status.line_buff_len += add;
                  entab_linebuff( );
                  rc = un_copy_line( win->ll, win, TRUE );
               } else
                  win->rcol = 0;
            }
            if (rc == OK  &&  split_line) {
               win->rline--;
               win->ll = win->ll->prev;
               if (win->cline > win->top_line + window->ruler)
                  win->cline--;
               win->rcol = rcol;
            }
            check_virtual_col( win, win->rcol, win->ccol );
            if (dirty == GLOBAL || file->dirty == LOCAL || wordwrap)
               file->dirty = GLOBAL;
            else
               file->dirty = dirty;
         }
      } else {
         if (new_node != NULL)
            my_free( new_node );
      }
   } else {
      if (new_node != NULL)
         my_free( new_node );
      error( WARNING, window->bottom_line, main4 );
   }

   /*
    * record that file has been modified
    */
   win->visible = TRUE;
   if (rc == OK) {
      if (file->dirty != GLOBAL)
         my_scroll_down( win );
      restore_marked_block( win, 1 );
      show_size( win );
      show_avail_mem( );
      if (old_bcol != win->bcol) {
         make_ruler( win );
         show_ruler( win );
      }
   }
   return( rc );
}


/*
 * Name:    insert_overwrite
 * Purpose: To make the necessary changes after the user has typed a normal
 *           printable character
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  insert_overwrite( WINDOW *window )
{
char *source;           /* source for block move to make room for c */
char *dest;             /* destination for block move */
int  len;               /* length of current line */
int  pad;               /* padding to add if cursor beyond end of line */
int  add;               /* characters to be added (usually 1 in insert mode) */
register int rcol;
register WINDOW *win;  /* put window pointer in a register */
int  rc;

   win = window;
   if (win->ll->len == EOF || g_status.key_pressed >= 256)
      rc = OK;
   else {
      rcol = win->rcol;
      /*
       * first check we have room - the editor can not
       *  cope with lines wider than g_display.line_length
       */
      if (rcol >= g_display.line_length) {
         /*
          * cannot insert more characters
          */
         error( WARNING, win->bottom_line, ed2 );
         rc = ERROR;
      } else {
         copy_line( win->ll );
         detab_linebuff( );

         /*
          * work out how many characters need to be inserted
          */
         len = g_status.line_buff_len;
         pad = rcol > len ? rcol - len : 0;

         if (mode.insert || rcol >= len)
            /*
             * inserted characters, or overwritten characters at the end of
             *  the line, are inserted.
             */
            add = 1;
         else
            /*
             *  and no extra space is required to overwrite existing characters
             */
            add = 0;

         /*
          * check that current line would not get too long.
          */
         if (len + pad + add >= g_display.line_length) {
            /*
             * no more room to add
             */
            error( WARNING, win->bottom_line, ed3 );
            rc = ERROR;
         } else {

            /*
             * make room for whatever needs to be inserted
             */
            if (pad > 0  || add > 0) {
               source = g_status.line_buff + rcol - pad;
               dest = source + pad + add;

               assert( len + pad - rcol >= 0 );
               assert( len + pad - rcol < MAX_LINE_LENGTH );

               memmove( dest, source, len + pad - rcol );

               /*
                * put in the required padding
                */

               assert( pad >= 0 );
               assert( pad < MAX_LINE_LENGTH );

               memset( source, ' ', pad );
            }
            g_status.line_buff[rcol] = (char)g_status.key_pressed;
            g_status.line_buff_len += pad + add;
            entab_linebuff( );

            /*
             * always increment the real column (rcol) then adjust the
             * logical and base column as needed.   show the changed line
             * in all but the LOCAL window.  In the LOCAL window, there are
             * two cases:  1) update the line, or 2) redraw the window if
             * cursor goes too far right.
             */
            win->file_info->dirty = NOT_LOCAL;
            win->ll->dirty = TRUE;
            show_changed_line( win );
            if (win->ccol < win->end_col) {
               show_curl_line( win );
               show_ruler_char( win );
               win->ccol++;
            } else {
               win->bcol++;
               win->file_info->dirty = LOCAL;
               make_ruler( win );
               show_ruler( win );
            }
            rcol++;
         }

         /*
          * record that file has been modified and adjust cursors,
          * file start and end pointers as needed.
          */
         check_virtual_col( win, rcol, win->ccol );
         win->file_info->modified = TRUE;
         if (mode.word_wrap) {
            add = mode.right_justify;
            mode.right_justify = FALSE;
            g_status.command = FormatText;
            word_wrap( win );
            mode.right_justify = add;
         }
         rc = OK;
      }
   }
   return( rc );
}


/*
 * Name:    join_line
 * Purpose: To join current line and line below at cursor
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   trunc the line.  then, join with line below, if it exists.
 */
int  join_line( WINDOW *window )
{
int  len;               /* length of current line */
int  new_len;           /* length of the joined lines */
int  next_len;          /* length of the line below current line */
text_ptr q;             /* next line in file */
text_ptr tab_free;      /* next line in file -- with the tabs removed */
int  pad;               /* padding spaces required */
register WINDOW *win;   /* put window pointer in a register */
WINDOW *wp;
line_list_ptr next_node;
int  rc;

   win = window;
   if (win->ll->len == EOF  ||  win->ll->next->len == EOF)
      return( ERROR );

   rc = OK;

   assert( win->ll->next != NULL );

   next_node = win->ll->next;
   load_undo_buffer( win->file_info, win->ll->line, win->ll->len );
   copy_line( win->ll );
   detab_linebuff( );

   /*
    * if cursor is in line before eol, reset len to rcol
    */
   if (win->rcol < (len = g_status.line_buff_len))
      len = win->rcol;

   /*
    * calculate needed padding
    */
   pad = win->rcol > len ? win->rcol - len : 0;

   assert( pad >= 0 );
   assert( pad < MAX_LINE_LENGTH );

   /*
    * if there any tabs in the next line, expand them because we
    *   probably have to redo them anyway.
    */
   next_len = next_node->len;
   tab_free = detab_a_line( next_node->line, &next_len );

   assert( next_len >= 0 );
   assert( next_len < MAX_LINE_LENGTH );
   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );

   /*
    * check room to combine lines
    */
   new_len = len + pad + next_len;
   if (new_len >= g_display.line_length) {
      /*
       * cannot combine lines.
       */
      error( WARNING, win->bottom_line, ed4 );
      rc = ERROR;
   } else {
      if (mode.do_backups == TRUE) {
         win->file_info->modified = TRUE;
         rc = backup_file( win );
      }
      q = (text_ptr)(g_status.line_buff + len);
      /*
       * insert padding
       */
      if (pad > 0) {
         while (pad--)
            *q++ = ' ';
      }
      _fmemcpy( q, tab_free, next_len );
      g_status.line_buff_len = new_len;
      entab_linebuff( );

      if ((rc = un_copy_line( win->ll, win, FALSE )) == OK) {

         if (next_node->next != NULL)
            next_node->next->prev = win->ll;
         win->ll->next = next_node->next;
         win->ll->dirty = TRUE;

         --win->file_info->length;
         ++win->rline;
         adjust_windows_cursor( win, -1 );
         restore_marked_block( win, -1 );
         --win->rline;

         wp = g_status.window_list;
         while (wp != NULL) {
            if (wp->file_info == win->file_info) {
               /*
                * make sure none of the window pointers point to the
                *   node we are about to delete.
                */
               if (wp != win) {
                  if (wp->ll == next_node)
                     wp->ll = win->ll->next;
               }
            }
            wp = wp->next;
         }

         /*
          * now, it's safe to delete the next_node line as well as
          *   the next node.
          */
         if (next_node->line != NULL)
            my_free( next_node->line );
         my_free( next_node );

         show_size( win );
         show_avail_mem( );
         win->file_info->dirty = GLOBAL;
      }
   }
   return( rc );
}


/*
 * Name:    word_delete
 * Purpose: To delete from the cursor to the start of the next word.
 * Date:    September 1, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is at the right of the line, then combine the
 *           current line with the next one, leaving the cursor where it
 *           is.
 *          If the cursor is on an alphanumeric character, then all
 *           subsequent alphanumeric characters are deleted.
 *          If the cursor is on a space, then all subsequent spaces
 *           are deleted.
 *          If the cursor is on a punctuation character, then all
 *           subsequent punctuation characters are deleted.
 */
int  word_delete( WINDOW *window )
{
int  len;               /* length of current line */
int  count;             /* number of characters deleted from line */
register int start;     /* column that next word starts in */
char *source;           /* source for block move to delete word */
char *dest;             /* destination for block move */
text_ptr p;
register WINDOW *win;   /* put window pointer in a register */
int  rc;

   win = window;
   if (win->rline > win->file_info->length  || win->ll->len == EOF)
      return( ERROR );

   rc = OK;
   copy_line( win->ll );
   detab_linebuff( );
   if (win->rcol >= (len = g_status.line_buff_len)) {
      rc = join_line( win );
      if (rc == OK) {
         p = win->ll->line;
         if (p != NULL) {
            p += win->rcol;
            if (win->rcol < win->ll->len) {
               len = win->ll->len - win->rcol;
               load_undo_buffer( win->file_info, p, len );
            }
         }
      }
   } else {

      assert( len >= 0);
      assert( len < MAX_LINE_LENGTH );

      /*
       * normal word delete
       *
       * find the start of the next word
       */
      start = win->rcol;
      if (isspace( g_status.line_buff[start] )) {
         /*
          * the cursor was on a space, so eat all consecutive spaces
          *  from the cursor onwards.
          */
         while (start < len  &&  isspace( g_status.line_buff[start] ))
            ++start;
      } else {
         /*
          * eat all consecutive characters in the same class (spaces
          *  are considered to be in the same class as the cursor
          *  character)
          */
         while (start < len  &&  !isspace( g_status.line_buff[start] ))
            ++start;
         while (start < len  &&  isspace( g_status.line_buff[start] ))
            ++start;
      }

      /*
       * move text to delete word
       */
      count = start - win->rcol;
      source = g_status.line_buff + start;
      dest = g_status.line_buff + win->rcol;

      assert( len - start >= 0 );

      memmove( dest, source, len - start );
      g_status.line_buff_len = len - count;
      entab_linebuff( );
      win->file_info->modified = TRUE;
      win->file_info->dirty = GLOBAL;
      win->ll->dirty = TRUE;

      /*
       * word_delete is also called by the word processing functions to get
       *   rid of spaces.
       */
      if (g_status.command == WordDelete)
         show_changed_line( win );
   }
   return( rc );
}


/*
 * Name:    dup_line
 * Purpose: Duplicate current line
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   cursor stays on current line
 */
int  dup_line( WINDOW *window )
{
register int len;       /* length of current line */
text_ptr p;
register WINDOW *win;   /* put window pointer in a register */
line_list_ptr next_node;
int  rc;

   win = window;

   /*
    * don't dup a NULL line
    */
   if (win->rline > win->file_info->length  ||  win->ll->len == EOF)
      return( ERROR );

   entab_linebuff( );
   rc = un_copy_line( win->ll, win, TRUE );
   len = win->ll->len;

   assert( len >= 0);
   assert( len < MAX_LINE_LENGTH );

   p = NULL;
   next_node = NULL;
   if (rc == OK) {
      p = (text_ptr)my_malloc( len, &rc );
      next_node = (line_list_ptr)my_malloc( sizeof(line_list_struc), &rc );
   }

   if (rc == OK) {
      win->file_info->modified = TRUE;
      if (mode.do_backups == TRUE)
         rc = backup_file( win );
      ++win->file_info->length;

      if (len > 0)
         _fmemcpy( p, win->ll->line, len );

      next_node->line  = p;
      next_node->dirty = TRUE;
      next_node->len   = len;

      if (win->ll->next != NULL)
         win->ll->next->prev = next_node;

      next_node->next = win->ll->next;
      next_node->prev = win->ll;
      win->ll->next = next_node;

      adjust_windows_cursor( win, 1 );

      /*
       * if current line is the bottom line, we can't see the dup line because
       * cursor doesn't move and dup line is added after current line.
       */
      if  (win->cline != win->bottom_line)
         my_scroll_down( win );
      win->file_info->dirty = NOT_LOCAL;

      /*
       * record that file has been modified
       */
      restore_marked_block( win, 1 );
      show_size( win );
      show_avail_mem( );
   } else {
      /*
       * cannot duplicate line
       */
      if (p != NULL)
         my_free( p );
      if (next_node != NULL)
         my_free( next_node );
      error( WARNING, win->bottom_line, ed5 );
   }
   return( rc );
}


/*
 * Name:    back_space
 * Purpose: To delete the character to the left of the cursor.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is at the left of the line, then combine the
 *           current line with the previous one.
 *          If in indent mode, and the cursor is on the first non-blank
 *           character of the line, then match the indentation of an
 *           earlier line.
 */
int  back_space( WINDOW *window )
{
int  rc;                /* return code */
int  len;               /* length of the current line */
char *source;           /* source of block move to delete character */
char *dest;             /* destination of block move */
text_ptr p;             /* previous line in file */
int  plen;              /* length of previous line */
int  del_count;         /* number of characters to delete */
int  pos;               /* the position of the first non-blank char */
register int rcol;
int  ccol;
int  old_bcol;
register WINDOW *win;  /* put window pointer in a register */
WINDOW *wp;
line_list_ptr temp_ll;

   win = window;
   if (win->rline > win->file_info->length || win->ll->len == EOF)
      return( ERROR );
   rc = OK;
   copy_line( win->ll );
   detab_linebuff( );
   len = g_status.line_buff_len;
   rcol = win->rcol;
   ccol = win->ccol;
   old_bcol = win->bcol;
   if (rcol == 0) {
      if (win->rline > 1) {
         /*
          * combine this line with the previous, if any
          */

         assert( win->ll->prev != NULL );

         p = win->ll->prev->line;
         plen = win->ll->prev->len;
         if (len + 2 + plen >= g_display.line_length) {
            /*
             * cannot combine lines
             */
            error( WARNING, win->bottom_line, ed4 );
            return( ERROR );
         }

         win->file_info->modified = TRUE;
         if ((rc = un_copy_line( win->ll, win, TRUE )) == OK) {
            --win->rline;
            win->ll = win->ll->prev;
            win->bin_offset -= win->ll->len;
            win->ll->dirty = TRUE;
            copy_line( win->ll );
            detab_linebuff( );
            len = g_status.line_buff_len;
            rcol = len;

            p = win->ll->next->line;
            plen = win->ll->next->len;

            /*
             * copy previous line into new previous line.
             */
            assert( plen >= 0 );
            assert( len  >= 0 );

            _fmemcpy( g_status.line_buff+len, p, plen );
            g_status.line_buff_len = len + plen;

            load_undo_buffer( win->file_info, p, plen );
            if (p != NULL)
               my_free( p );

            temp_ll = win->ll->next;

            if (temp_ll->prev != NULL)
               temp_ll->prev->next = temp_ll->next;
            temp_ll->next->prev = temp_ll->prev;

            --win->file_info->length;
            ++win->rline;
            restore_marked_block( win, -1 );
            adjust_windows_cursor( win, -1 );
            --win->rline;

            wp = g_status.window_list;
            while (wp != NULL) {
               if (wp->file_info == win->file_info) {
                  if (wp != win) {
                     if (wp->ll == temp_ll)
                        wp->ll = win->ll->next;
                  }
               }
               wp = wp->next;
            }

            my_free( temp_ll );

            if (win->cline > win->top_line + win->ruler)
               --win->cline;

            /*
             * make sure cursor stays on the screen, at the end of the
             *  previous line
             */
            ccol = rcol - win->bcol;
            show_size( win );
            show_avail_mem( );
            check_virtual_col( win, rcol, ccol );
            win->file_info->dirty = GLOBAL;
            make_ruler( win );
            show_ruler( win );
         }
      } else
         return( ERROR );
   } else {
      /*
       * normal delete
       *
       * find out how much to delete (depends on indent mode)
       */
      del_count = 1;   /* the default */
      if (mode.indent) {
         /*
          * indent only happens if the cursor is on the first
          *  non-blank character of the line
          */
         pos = first_non_blank( (text_ptr)g_status.line_buff, len );
         if (pos == rcol  ||
                         is_line_blank( (text_ptr)g_status.line_buff, len )) {
            /*
             * now work out how much to indent
             */
            temp_ll = win->ll->prev;
            for (; temp_ll != NULL; temp_ll=temp_ll->prev) {
               p = temp_ll->line;
               plen = first_non_blank( p, temp_ll->len );
               if (plen < rcol  &&  plen != temp_ll->len) {
                  /*
                   * found the line to match
                   */
                  del_count = rcol - plen;
                  break;
               }
            }
         }
      }

      /*
       * move text to delete char(s), unless no chars actually there
       */
      if (rcol - del_count < len) {
         dest = g_status.line_buff + rcol - del_count;
         if (rcol > len) {
            source = g_status.line_buff + len;
            pos = 0;
            len = (rcol + 1) - del_count;
         } else {
            source = g_status.line_buff + rcol;
            pos = len - rcol;
            len = len - del_count;
         }

         assert( pos >= 0 );
         assert( len >= 0 );
         assert( len <= MAX_LINE_LENGTH );

         memmove( dest, source, pos );
         g_status.line_buff_len = len;
         entab_linebuff( );
      }
      rcol -= del_count;
      ccol -= del_count;
      win->file_info->dirty = NOT_LOCAL;
      win->ll->dirty = TRUE;
      show_ruler_char( win );
      show_changed_line( win );
      check_virtual_col( win, rcol, ccol );
      if (!win->file_info->dirty)
         show_curl_line( win );
      if (old_bcol != win->bcol) {
         make_ruler( win );
         show_ruler( win );
      }
   }
   win->file_info->modified = TRUE;
   return( rc );
}


/*
 * Name:    line_kill
 * Purpose: To delete the line the cursor is on.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   win->ll->s == NULL then do not do a
 *          line kill (can't kill a NULL line).
 */
int  line_kill( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */
register WINDOW *wp;
line_list_ptr killed_node;
int  rc;

   win = window;
   killed_node = win->ll;
   rc = OK;
   if (killed_node->len != EOF) {
      win->file_info->modified = TRUE;
      if (mode.do_backups == TRUE)
         rc = backup_file( win );

      if (rc == OK) {
         load_undo_buffer( win->file_info,
            g_status.copied ? (text_ptr)g_status.line_buff : killed_node->line,
            g_status.copied ? g_status.line_buff_len       : killed_node->len );

         --win->file_info->length;

         win->ll = win->ll->next;

         if (killed_node->prev != NULL)
            killed_node->prev->next = killed_node->next;
         else
            win->file_info->line_list = win->ll;

         killed_node->next->prev = killed_node->prev;

         wp = g_status.window_list;
         while (wp != NULL) {
            if (wp->file_info == win->file_info) {
               if (wp != win) {
                  if (wp->ll == killed_node)
                     wp->ll = win->ll;
               }
            }
            wp = wp->next;
         }

         /*
          * free the line and the node
          */
         if (killed_node->line != NULL)
            my_free( killed_node->line );
         my_free( killed_node );

         win->file_info->dirty = NOT_LOCAL;

         g_status.copied = FALSE;
         /*
          * move all cursors one according to i, restore begin and end block
          */
         adjust_windows_cursor( win, -1 );
         restore_marked_block( win, -1 );

         /*
          * we are not doing a GLOBAL update, so update current window here
          */
         if (win->file_info->dirty == NOT_LOCAL)
            my_scroll_down( win );
         show_size( win );
         show_avail_mem( );
      }
   } else
      rc = ERROR;
   return( rc );
}


/*
 * Name:    char_del_under
 * Purpose: To delete the character under the cursor.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is beyond the end of the line, then this
 *           command is ignored.
 *          DeleteChar and StreamDeleteChar use this function.
 */
int  char_del_under( WINDOW *window )
{
char *source;    /* source of block move to delete character */
int  len;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   if (win->rline > win->file_info->length || win->ll->len == EOF)
      return( OK );
   copy_line( win->ll );
   detab_linebuff( );
   if (win->rcol < (len = g_status.line_buff_len)) {
      /*
       * move text to delete using buffer
       */
      source = g_status.line_buff + win->rcol + 1;

      assert( len - win->rcol >= 0 );

      memmove( source-1, source, len - win->rcol );
      --g_status.line_buff_len;
      entab_linebuff( );
      win->file_info->dirty    = GLOBAL;
      win->file_info->modified = TRUE;
      win->ll->dirty = TRUE;
      show_changed_line( win );
   } else if (g_status.command == StreamDeleteChar)
      join_line( win );
   return( OK );
}


/*
 * Name:    eol_kill
 * Purpose: To delete everything from the cursor to the end of the line.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the cursor is beyond the end of the line, then this
 *           command is ignored.
 */
int  eol_kill( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   if (win->rline > win->file_info->length  ||  win->ll->len == EOF)
      return( OK );
   copy_line( win->ll );
   detab_linebuff( );
   load_undo_buffer( win->file_info, (text_ptr)g_status.line_buff,
                     g_status.line_buff_len );
   if (win->rcol < g_status.line_buff_len) {
      /*
       * truncate to delete rest of line
       */
      g_status.line_buff_len = win->rcol;
      entab_linebuff( );
      win->file_info->dirty = GLOBAL;
      win->ll->dirty = TRUE;
      show_changed_line( win );
   }
   return( OK );
}


/*
 * Name:    undo_line
 * Purpose: To retrieve unaltered line if possible.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Changes are made to the line buffer so the underlying text has
 *          not changed.  Put the unchanged line from the file into the
 *          line buffer and display it.
 */
int  undo_line( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   if (win->rline <= win->file_info->length  &&  win->ll->len != EOF &&
                            g_status.copied) {
      g_status.copied = FALSE;
      copy_line( win->ll );
      detab_linebuff( );
      win->file_info->dirty = GLOBAL;
      show_changed_line( win );
   }
   return( OK );
}


/*
 * Name:    undo
 * Purpose: To retrieve (pop) a line from the undo stack
 * Date:    September 26, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Insert an empty line into the file then pop the line in the undo
 *          stack.  When we pop line 0, there are no more lines on the stack.
 *          Set the stack pointer to -1 to indicate an empty stack.
 */
int  undo( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */
line_list_ptr node;

   win = window;
   if (win->file_info->undo_count > 0) {
      entab_linebuff( );
      if (un_copy_line( win->ll, win, TRUE ) == ERROR)
         return( ERROR );

      node = win->file_info->undo_top;
      win->file_info->undo_top = node->next;
      win->file_info->undo_top->prev = NULL;
      --win->file_info->undo_count;

      node->next = node->prev = NULL;

      ++win->file_info->length;

      if (win->ll->prev != NULL)
         win->ll->prev->next = node;
      node->prev = win->ll->prev;

      win->ll->prev = node;
      node->next = win->ll;
      win->ll = node;
      win->ll->dirty = TRUE;

      if (win->ll->prev == NULL)
         win->file_info->line_list = win->ll;

      adjust_windows_cursor( win, 1 );

      /*
       * we have now undeleted a line.  increment the file length and display
        * it.
       */
      win->file_info->dirty = GLOBAL;
      show_size( win );
      show_avail_mem( );
   }
   return( OK );
}


/*
 * Name:    beg_next_line
 * Purpose: To move the cursor to the beginning of the next line.
 * Date:    October 4, 1991
 * Passed:  window:  pointer to current window
 */
int  beg_next_line( WINDOW *window )
{
int  rc;

   window->rcol = 0;
   rc = prepare_move_down( window );
   check_virtual_col( window, window->rcol, window->ccol );
   sync( window );
   make_ruler( window );
   show_ruler( window );
   return( rc );
}


/*
 * Name:    next_line
 * Purpose: To move the cursor to the first character of the next line.
 * Date:    October 4, 1991
 * Passed:  window:  pointer to current window
 */
int  next_line( WINDOW *window )
{
register int rcol;
register WINDOW *win;   /* put window pointer in a register */
int  rc;

   win = window;
   rc = prepare_move_down( win );
   rcol = first_non_blank( win->ll->line, win->ll->len );
   check_virtual_col( win, rcol, win->ccol );
   sync( win );
   make_ruler( win );
   show_ruler( win );
   return( rc );
}


/*
 * Name:    home
 * Purpose: To move the cursor to the left of the current line.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   this routine is made a little more complicated with cursor sync.
 *            if the g_status.copied flag is set we need to see from what file
 *            the line_buff was copied.
 */
int  home( WINDOW *window )
{
register int rcol;
register WINDOW *win;   /* put window pointer in a register */
text_ptr p;

   win = window;
   if (g_status.copied && win->file_info == g_status.current_window->file_info){
      rcol = first_non_blank( (text_ptr)g_status.line_buff,
                                        g_status.line_buff_len );
      if (is_line_blank( (text_ptr)g_status.line_buff, g_status.line_buff_len))
         rcol = 0;
   } else {
      p = win->ll->line;
      if (p == NULL)
         rcol = 0;
      else {
         rcol = first_non_blank( p, win->ll->len );
         if (is_line_blank( p, win->ll->len ))
            rcol = 0;
      }
   }
   if (win->rcol == rcol)
      rcol = 0;
   check_virtual_col( win, rcol, win->ccol );
   sync( win );
   make_ruler( win );
   show_ruler( win );
   return( OK );
}


/*
 * Name:    goto_eol
 * Purpose: To move the cursor to the eol character of the current line.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   this routine is made a little more complicated with cursor sync.
 *            if the g_status.copied flag is set we need to see from what file
 *            the line_buff was copied.
 */
int  goto_eol( WINDOW *window )
{
register int rcol;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   rcol = find_end( win->ll->line, win->ll->len );
   if (g_status.copied) {
      if (win->file_info == g_status.current_window->file_info)
         rcol = find_end( (text_ptr)g_status.line_buff, g_status.line_buff_len);
   }
   win->ccol = win->start_col + rcol - win->bcol;
   check_virtual_col( win, rcol, win->ccol );
   sync( win );
   make_ruler( win );
   show_ruler( win );
   return( OK );
}


/*
 * Name:    goto_top
 * Purpose: To move the cursor to the top of the current window.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the start of the file occurs before the top of the window,
 *           then the start of the file is moved to the top of the window.
 */
int  goto_top( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   update_line( win );
   for (; win->cline > win->top_line+win->ruler; win->cline--,win->rline--) {
      if (win->rline <= 1L)
         break;
      else {
         win->ll = win->ll->prev;
         win->bin_offset -= win->ll->len;
      }
   }
   show_curl_line( win );
   sync( win );
   return( OK );
}


/*
 * Name:    goto_bottom
 * Purpose: To move the cursor to the bottom of the current window.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  goto_bottom( WINDOW *window )
{
register WINDOW *win;   /* put window pointer in a register */
int  at_top;

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );
   if (win->ll->len == EOF) {
      if (win->rline > 1) {
         at_top = FALSE;
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
      }
   } else {
      update_line( win );
      for (; win->cline < win->bottom_line; win->cline++,win->rline++) {
         if (win->ll == NULL || win->ll->next == NULL || win->ll->next->len == EOF)
            break;
         else {
            win->bin_offset += win->ll->len;
            win->ll = win->ll->next;
         }
      }
      show_curl_line( win );
   }
   sync( win );
   return( OK );
}


/*
 * Name:    set_tabstop
 * Purpose: To set the current interval between tab stops
 * Date:    October 1, 1989
 * Notes:   Tab interval must be reasonable, and this function will
 *           not allow tabs more than MAX_COLS / 2.
 */
int  set_tabstop( WINDOW *window )
{
char num_str[MAX_COLS]; /* tab interval as a character string */
int  tab;               /* new tab interval */
register int rc;
register file_infos *file;

   itoa( mode.ltab_size, num_str, 10 );
   /*
    * tab interval:
    */
   rc = get_name( ed7a, window->bottom_line, num_str, g_display.message_color );
   if (rc == OK   &&  *num_str != '\0') {
      tab = atoi( num_str );
      if (tab < MAX_COLS/2) {
         mode.ltab_size = tab;
         if (mode.inflate_tabs) {
            for (file=g_status.file_list; file != NULL; file=file->next)
               file->dirty = GLOBAL;
         }
      } else {
         /*
          * tab size too long
          */
         error( WARNING, window->bottom_line, ed8 );
         rc = ERROR;
      }
   }

   itoa( mode.ptab_size, num_str, 10 );
   /*
    * tab interval:
    */
   rc = get_name( ed7b, window->bottom_line, num_str, g_display.message_color );
   if (rc == OK  &&  *num_str != '\0') {
      tab = atoi( num_str );
      if (tab < MAX_COLS/2) {
         mode.ptab_size = tab;
         show_tab_modes( );
         if (mode.inflate_tabs) {
            for (file=g_status.file_list; file != NULL; file=file->next)
               file->dirty = GLOBAL;
         }
      } else {
         /*
          * tab size too long
          */
         error( WARNING, window->bottom_line, ed8 );
         rc = ERROR;
      }
   }
   return( rc );
}


/*
 * Name:    show_line_col
 * Purpose: show current real line and column of current cursor position
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Blank old position and display new position.  current line and
 *          column may take up to 12 columns, which allows the display of
 *          9,999 columns and 9,999,999 lines.
 */
void show_line_col( WINDOW *window )
{
int  i;
register int k;
char line_col[20], num[10];
char *hex_digit = "0123456789abcdef";

   /*
    * blank out current line:column position.
    */
   memset( line_col, ' ', 13 );
   line_col[13] = '\0';

   /*
    * convert column to ascii and store in display buffer.
    */
   itoa( window->rcol+1, num, 10 );
   i = strlen( num ) - 1;
   for (k=12; i>=0; i--, k--)
      line_col[k] = num[i];

   /*
    * put in colon to separate line and column
    */
   line_col[k--] = ':';

   /*
    * convert line to ascii and store in display buffer.
    */
   ltoa( window->rline, num, 10 );
   i = strlen( num ) - 1;
   for (; i>=0; i--, k--)
      line_col[k] = num[i];

   /*
    * find line to start line:column display then output
    */
   s_output( line_col, window->top_line-1, window->end_col-12,
             g_display.head_color );

   strcpy( line_col, " =   " );
   i = window->rcol;
   if (g_status.copied) {
      if (mode.inflate_tabs)
         i = entab_adjust_rcol( (text_ptr)g_status.line_buff,
                                 g_status.line_buff_len, i );
      if (i < g_status.line_buff_len) {
         k = (int)g_status.line_buff[i];
         line_col[2] = *(hex_digit + (k >> 4));
         line_col[3] = *(hex_digit + (k & 0x000f));
         line_col[4] = 'x';
         i = TRUE;
      } else
         i = FALSE;
   } else {
      if (mode.inflate_tabs  &&  window->ll->len != EOF)
         i = entab_adjust_rcol( window->ll->line, window->ll->len, i );
      if (i < window->ll->len) {
         k = (int)window->ll->line[i];
         line_col[2] = *(hex_digit + (k >> 4));
         line_col[3] = *(hex_digit + (k & 0x000f));
         line_col[4] = 'x';
         i = TRUE;
      } else
         i = FALSE;
   }
   s_output( line_col, g_display.mode_line, 58, g_display.mode_color );
   if (i == TRUE)
      c_output( k, 58, g_display.mode_line, g_display.mode_color );


   /*
    * if file was opened in binary mode, show offset from beginning of file.
    */
   if (window->file_info->crlf == BINARY && !window->vertical) {
      k =  window->ll->line == NULL  ?  0  :  window->rcol;
      memset( line_col, ' ', 7 );
      line_col[7] = '\0';
      s_output( line_col, window->top_line-1, 61, g_display.head_color );
      ltoa( window->bin_offset + k, line_col, 10 );
      s_output( line_col, window->top_line-1, 61, g_display.head_color );
   }
   show_asterisk( window );
}


/*
 * Name:    show_asterisk
 * Purpose: give user an indication if file is dirty
 * Date:    September 16, 1991
 * Passed:  window:  pointer to current window
 */
void show_asterisk( WINDOW *window )
{
   c_output( window->file_info->modified ? '*' : ' ', window->start_col+4,
             window->top_line-1, g_display.head_color );
}


/*
 * Name:    toggle_overwrite
 * Purpose: toggle overwrite-insert mode
 * Date:    September 16, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_overwrite( WINDOW *arg_filler )
{
   mode.insert = !mode.insert;
   show_insert_mode( );
   set_cursor_size( mode.insert ? g_display.insert_cursor :
                    g_display.overw_cursor );
   return( OK );
}


/*
 * Name:    toggle_smart_tabs
 * Purpose: toggle smart tab mode
 * Date:    June 5, 1992
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_smart_tabs( WINDOW *arg_filler )
{
   mode.smart_tab = !mode.smart_tab;
   show_tab_modes( );
   return( OK );
}


/*
 * Name:    toggle_indent
 * Purpose: toggle indent mode
 * Date:    September 16, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_indent( WINDOW *arg_filler )
{
   mode.indent = !mode.indent;
   show_indent_mode( );
   return( OK );
}


/*
 * Name:    set_left_margin
 * Purpose: set left margin for word wrap
 * Date:    November 27, 1991
 * Passed:  window
 */
int  set_left_margin( WINDOW *window )
{
register int rc;
char temp[MAX_COLS];

   itoa( mode.left_margin + 1, temp, 10 );
   /*
    * enter left margin
    */
   rc = get_name( ed9, window->bottom_line, temp, g_display.message_color );
   if (rc == OK  &&  *temp != '\0') {
      rc = atoi( temp ) - 1;
      if (rc < 0 || rc >= mode.right_margin) {
         /*
          * left margin out of range
          */
         error( WARNING, window->bottom_line, ed10 );
         rc = ERROR;
      } else {
         mode.left_margin = rc;
         show_all_rulers( );
      }
   }
   return( rc );
}


/*
 * Name:    set_right_margin
 * Purpose: set right margin for word wrap
 * Date:    November 27, 1991
 * Passed:  window
 */
int  set_right_margin( WINDOW *window )
{
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int rc;
int  prompt_line;
char temp[MAX_COLS];

   prompt_line = window->bottom_line;
   save_screen_line( 0, prompt_line, line_buff );
   set_prompt( ed11a, prompt_line );
   rc = get_yn( );
   restore_screen_line( 0, prompt_line, line_buff );
   if (rc != ERROR) {
      mode.right_justify =  rc == A_YES ? TRUE : FALSE;

      itoa( mode.right_margin + 1, temp, 10 );
      /*
       * enter right margin
       */
      rc = get_name( ed11, prompt_line, temp, g_display.message_color );
      if (rc == OK  &&  *temp != '\0') {
         rc = atoi( temp ) - 1;
         if (rc <= mode.left_margin || rc > MAX_LINE_LENGTH) {
            /*
             * right margin out of range
             */
            error( WARNING, prompt_line, ed12 );
            rc = ERROR;
         } else {
            mode.right_margin = rc;
            show_all_rulers( );
         }
      }
   }
   return( rc );
}


/*
 * Name:    set_paragraph_margin
 * Purpose: set column to begin paragraph
 * Date:    November 27, 1991
 * Passed:  window
 * Notes:   paragraph may be indented, flush, or offset.
 */
int  set_paragraph_margin( WINDOW *window )
{
register int rc;
char temp[80];

   itoa( mode.parg_margin + 1, temp, 10 );
   /*
    * enter paragraph margin
    */
   rc = get_name( ed13, window->bottom_line, temp, g_display.message_color );
   if (rc == OK  &&  *temp != '\0') {
      rc = atoi( temp ) - 1;
      if (rc < 0 || rc >= mode.right_margin) {
         /*
          * paragraph margin out of range
          */
         error( WARNING, window->bottom_line, ed14 );
         rc = ERROR;
      } else {
         mode.parg_margin = rc;
         show_all_rulers( );
      }
   }
   return( rc );
}


/*
 * Name:    toggle_crlf
 * Purpose: toggle crlf mode
 * Date:    November 27, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_crlf( WINDOW *window )
{
register WINDOW *w;

   ++window->file_info->crlf;
   if (window->file_info->crlf > BINARY)
      window->file_info->crlf = CRLF;
   w = g_status.window_list;
   while (w != NULL) {
      if (w->file_info == window->file_info  &&  w->visible)
         show_crlf_mode( w );
      w = w->next;
   }
   return( OK );
}


/*
 * Name:    toggle_ww
 * Purpose: toggle word wrap mode
 * Date:    November 27, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_ww( WINDOW *arg_filler )
{
   ++mode.word_wrap;
   if (mode.word_wrap > DYNAMIC_WRAP)
      mode.word_wrap = NO_WRAP;
   show_wordwrap_mode( );
   return( OK );
}


/*
 * Name:    toggle_trailing
 * Purpose: toggle eleminating trainling space at eol
 * Date:    November 25, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_trailing( WINDOW *arg_filler )
{
   mode.trailing = !mode.trailing;
   show_trailing( );
   return( OK );
}


/*
 * Name:    toggle_z
 * Purpose: toggle writing control z at eof
 * Date:    November 25, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_z( WINDOW *arg_filler )
{
   mode.control_z = !mode.control_z;
   show_control_z( );
   return( OK );
}


/*
 * Name:    toggle_eol
 * Purpose: toggle writing eol character at eol
 * Date:    November 25, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_eol( WINDOW *arg_filler )
{
register file_infos *file;

   mode.show_eol = !mode.show_eol;
   for (file=g_status.file_list; file != NULL; file=file->next)
      file->dirty = GLOBAL;
   return( OK );
}


/*
 * Name:    toggle_search_case
 * Purpose: toggle search case
 * Date:    September 16, 1991
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_search_case( WINDOW *arg_filler )
{
   mode.search_case = (mode.search_case == IGNORE) ? MATCH : IGNORE;
   show_search_case( );
   build_boyer_array( );
   return( OK );
}


/*
 * Name:    toggle_sync
 * Purpose: toggle sync mode
 * Date:    January 15, 1992
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_sync( WINDOW *arg_filler )
{
   mode.sync = !mode.sync;
   show_sync_mode( );
   return( OK );
}


/*
 * Name:    toggle_ruler
 * Purpose: toggle ruler
 * Date:    March 5, 1992
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_ruler( WINDOW *arg_filler )
{
register WINDOW *wp;

   mode.ruler = !mode.ruler;
   wp = g_status.window_list;
   while (wp != NULL) {
      if (mode.ruler) {
         /*
          * there has to be more than one line in a window to display a ruler.
          *   even if the ruler mode is on, we need to check the num of lines.
          */
         if (wp->bottom_line - wp->top_line >0) {
            if (wp->cline == wp->top_line)
               ++wp->cline;
            if (wp->cline > wp->bottom_line)
               wp->cline = wp->bottom_line;
            wp->ruler = TRUE;
         } else
            wp->ruler = FALSE;
      } else {

         /*
          * if this is the first page in a file, then we may need to "pull"
          *   the file up before displaying the first page.
          */
         if (wp->rline == ((wp->cline - wp->ruler) - (wp->top_line - 1)))
            --wp->cline;
         if (wp->cline < wp->top_line)
            wp->cline = wp->top_line;
         wp->ruler = FALSE;
      }
      make_ruler( wp );
      setup_window( wp );
      if (wp->visible)
         redraw_current_window( wp );
      wp = wp->next;
   }
   return( OK );
}


/*
 * Name:    toggle_tabinflate
 * Purpose: toggle inflating tabs
 * Date:    October 31, 1992
 * Passed:  arg_filler:  argument to satify function prototype
 */
int  toggle_tabinflate( WINDOW *arg_filler )
{
register file_infos *file;

   mode.inflate_tabs = !mode.inflate_tabs;
   for (file=g_status.file_list; file != NULL; file=file->next)
      file->dirty = GLOBAL;
   show_tab_modes( );
   return( OK );
}


/*
 * Name:    sync
 * Purpose: carry out cursor movements in all visible windows
 * Date:    January 15, 1992
 * Passed:  window
 * Notes:   switch sync semaphore when we do this so we don't get into a
 *          recursive loop.  all cursor movement commands un_copy_line before
 *          moving the cursor off the current line.   you MUST make certain
 *          that the current line is uncopied in the task routines that
 *          move the cursor off the current line before calling sync.
 */
void sync( WINDOW *window )
{
register WINDOW *wp;
register file_infos *fp;

   if (mode.sync && mode.sync_sem) {

   /*
    * these functions must un_copy a line before sync'ing
    */
#if defined( __MSC__ )
      switch (g_status.command) {
         case  NextLine        :
         case  BegNextLine     :
         case  LineDown        :
         case  LineUp          :
         case  WordRight       :
         case  WordLeft        :
         case  ScreenDown      :
         case  ScreenUp        :
         case  EndOfFile       :
         case  TopOfFile       :
         case  BotOfScreen     :
         case  TopOfScreen     :
         case  JumpToLine      :
         case  CenterWindow    :
         case  CenterLine      :
         case  ScrollDnLine    :
         case  ScrollUpLine    :
         case  PanUp           :
         case  PanDn           :
         case  NextDirtyLine   :
         case  PrevDirtyLine   :
         case  ParenBalance    :
            assert( g_status.copied == FALSE );
            break;
         default  :
            break;
      }
#endif

      mode.sync_sem = FALSE;
      for (wp = g_status.window_list;  wp != NULL;  wp = wp->next) {
         if (wp->visible  &&  wp != window) {

            /*
             * when we sync a command, we need to use the same assertions
             *  as those in editor( ).
             *
             * if everything is everything, these core asserts are TRUE.
             */
#if defined( __MSC__ )
            assert( wp != NULL );
            assert( wp->file_info != NULL );
            assert( wp->file_info->line_list != NULL );
            assert( wp->file_info->line_list_end != NULL );
            assert( wp->file_info->line_list_end->len == EOF );
            assert( wp->visible == TRUE );
            assert( wp->rline >= 0 );
            assert( wp->rline <= wp->file_info->length + 1 );
            assert( wp->rcol >= 0 );
            assert( wp->rcol < MAX_LINE_LENGTH );
            assert( wp->ccol >= wp->start_col );
            assert( wp->ccol <= wp->end_col );
            assert( wp->bcol >= 0 );
            assert( wp->bcol < MAX_LINE_LENGTH );
            assert( wp->bcol == wp->rcol-(wp->ccol - wp->start_col) );
            assert( wp->start_col >= 0 );
            assert( wp->start_col < wp->end_col );
            assert( wp->end_col < g_display.ncols );
            assert( wp->cline >= wp->top_line );
            assert( wp->cline <= wp->bottom_line );
            assert( wp->top_line > 0 );
            assert( wp->top_line <= wp->bottom_line );
            assert( wp->bottom_line < MAX_LINES );
            assert( wp->bin_offset >= 0 );
            if (wp->ll->next == NULL)
               assert( wp->ll->len == EOF );
            else
               assert( wp->ll->len >= 0 );
            assert( wp->ll->len <  MAX_LINE_LENGTH );
#endif

            (*do_it[g_status.command])( wp );
            show_line_col( wp );
            show_ruler_pointer( wp );
         }
      }
      mode.sync_sem = TRUE;
      for (fp = g_status.file_list; fp != NULL; fp = fp->next)
         if (fp->dirty != FALSE)
            fp->dirty = GLOBAL;
   }
}


/*
 * 作用: 建立起编辑器的结构，并且在需要的时候显示变化
 */
void editor( )
{
char *name;  /* 开始编辑时的文件的名字 */
register WINDOW *window;        /* 当前活动窗口 */
int  c;

   /*
    * 初始化搜索结构
    */
   g_status.sas_defined = FALSE;
   for (c=0; c<SAS_P; c++)
      g_status.sas_arg_pointers[c] = NULL;

   g_status.file_mode = TEXT;
   /*
    * 检查用户是否指定了具体的文件来编辑，如果没有提供帮助
    */
   if (g_status.argc > 1) {
      c = *g_status.argv[1];
      if (c == '/'  ||  c == '-') {
         c = *(g_status.argv[1] + 1);
         if (c == 'f'  ||  c == 'g') {
            /*
             * 如果是搜索文件，那么用户在命令参数中至少要提供4个参数
             * 比如   editor -f findme *.c
             */
            if (g_status.argc >= 4) {

               assert( strlen( g_status.argv[2] ) < MAX_COLS );

               if (c == 'f') {
                  g_status.command = DefineGrep;
                  strcpy( (char *)sas_bm.pattern, g_status.argv[2] );
               } else {
                  g_status.command = DefineRegXGrep;
                  strcpy( (char *)regx.pattern, g_status.argv[2] );
               }

               for (c=3; c <= g_status.argc; c++)
                  g_status.sas_arg_pointers[c-3] = g_status.argv[c];
               g_status.sas_argc = g_status.argc - 3;
               g_status.sas_arg = 0;
               g_status.sas_argv = g_status.sas_arg_pointers;
               g_status.sas_found_first = FALSE;
               if (g_status.command == DefineGrep) {
                  g_status.sas_defined = TRUE;
                  g_status.sas_search_type = BOYER_MOORE;
                  bm.search_defined = sas_bm.search_defined = OK;
                  build_boyer_array( );
                  c = OK;
               } else {
                  c = build_nfa( );
                  if (c == OK) {
                     g_status.sas_defined = TRUE;
                     g_status.sas_search_type = REG_EXPRESSION;
                     regx.search_defined = sas_regx.search_defined = OK;
                  } else
                     g_status.sas_defined = FALSE;
               }
               if (c != ERROR)
                  c = search_and_seize( g_status.current_window );
            } else
               c = ERROR;
         } else if (c == 'b' || c == 'B') {
            c = atoi( g_status.argv[1] + 2 );
            if (c <= 0 || c >= MAX_LINE_LENGTH)
               c = DEFAULT_BIN_LENGTH;
            ++g_status.arg;
            g_status.file_mode = BINARY;
            g_status.file_chunk = c;
            c = edit_next_file( g_status.current_window );
         } else
            c = ERROR;
      } else
         c = edit_next_file( g_status.current_window );
   } else {
      name = g_status.rw_name;
      *name = '\0';
      /*
       * 要编辑的文件名
       */
      c = get_name( ed15, g_display.nlines, name, g_display.text_color );

      assert( strlen( name ) < MAX_COLS );

      if (c == OK) {
         if (*name != '\0')
            c = attempt_edit_display( name, GLOBAL, TEXT, 0 );
         else
            c = dir_help( (WINDOW *)NULL );
      }
   }

   g_status.stop =   c == OK  ?  FALSE  :  TRUE;
   if (c == OK)
      set_cursor_size( mode.insert ? g_display.insert_cursor :
                       g_display.overw_cursor );

   /*
    * 主循环：在用户结束文档编辑前，处理用户的编辑命令
	* 并且负责在视图中反映变化
    */
   for (; g_status.stop != TRUE;) {
      window = g_status.current_window;


      /*
       * 在处理任何编辑命令前，检测一些参数的设置
       */
      assert( window != NULL );
      assert( window->file_info != NULL );
      assert( window->file_info->line_list != NULL );
      assert( window->file_info->line_list_end != NULL );
      assert( window->file_info->line_list_end->len == EOF );
      assert( window->visible == TRUE );
      assert( window->rline >= 0 );
      assert( window->rline <= window->file_info->length + 1 );
      assert( window->rcol >= 0 );
      assert( window->rcol < MAX_LINE_LENGTH );
      assert( window->ccol >= window->start_col );
      assert( window->ccol <= window->end_col );
      assert( window->bcol >= 0 );
      assert( window->bcol < MAX_LINE_LENGTH );
      assert( window->bcol == window->rcol-(window->ccol - window->start_col) );
      assert( window->start_col >= 0 );
      assert( window->start_col < window->end_col );
      assert( window->end_col < g_display.ncols );
      assert( window->cline >= window->top_line );
      assert( window->cline <= window->bottom_line );
      assert( window->top_line > 0 );
      assert( window->top_line <= window->bottom_line );
      assert( window->bottom_line < MAX_LINES );
      assert( window->bin_offset >= 0 );
      if (window->ll->next == NULL)
         assert( window->ll->len == EOF );
      else
         assert( window->ll->len >= 0 );
      assert( window->ll->len <  MAX_LINE_LENGTH );

      display_dirty_windows( window );

      /*
       * 在处理编辑命令前，把错误处理器的状态设置成一个已知的状态。
       */
      ceh.flag = OK;



      /*
       * 得到用户的输入。查找赋予用户输入的这个键的命令功能。
       * 所有普通文本的输入都赋予了功能0，这些文本包括ASCII码和扩展ASCII码
       */
      g_status.key_pressed = getkey( );
      g_status.command = getfunc( g_status.key_pressed );
      if (g_status.wrapped  ||  g_status.key_pending) {
         g_status.key_pending = FALSE;
         g_status.wrapped = FALSE;
         show_search_message( CLR_SEARCH, g_display.mode_color );
      }
      g_status.control_break = FALSE;
      if (g_status.command >= 0 && g_status.command < NUM_FUNCS) {
         record_keys( window->bottom_line );
         (*do_it[g_status.command])( window );
      }
   }
   cls( );
   xygoto( 0, 0 );
}


/*
 * Name:    display_dirty_windows
 * Purpose: Set up the editor structures and display changes as needed.
 * Date:    June 5, 1991
 * Notes:   Display all windows with dirty files.
 */
void display_dirty_windows( WINDOW *window )
{
register WINDOW *below;         /* window below current */
register WINDOW *above;         /* window above current */
file_infos *file;               /* temporary file structure */

   /*
    * update all windows that point to any file that has been changed
    */
   above = below = window;
   while (above->prev || below->next) {
      if (above->prev) {
         above = above->prev;
         show_dirty_window( above );
      }
      if (below->next) {
         below = below->next;
         show_dirty_window( below );
      }
   }
   file = window->file_info;
   if (file->dirty == LOCAL || file->dirty == GLOBAL)
      display_current_window( window );
   for (file=g_status.file_list; file != NULL; file=file->next)
      file->dirty = FALSE;

   /*
    * Set the cursor position at window->ccol, window->cline.  Show the
    * user where in the file the cursor is positioned.
    */
   xygoto( window->ccol, window->cline );
   show_line_col( window );
   show_ruler_pointer( window );
}



/*
 * Name:    show_dirty_window
 * Purpose: show changes in non-current window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
void show_dirty_window( WINDOW *window )
{
register WINDOW *win;   /* register window pointer */
int  dirty;

  win = window;
  if (win->visible) {
     dirty = win->file_info->dirty;
     if (dirty == GLOBAL || dirty == NOT_LOCAL) {
        display_current_window( win );
        show_size( win );
     }
     show_asterisk( win );
  }
}
