/*
 * This module contains the word wrap and format paragraph functions.  The
 *  right_justify( ) function is based on the spread function in _Software
 *  Tools_ by Brian Kernighan and P J Plauger.  My version of the spread
 *  function handles lines with extra blanks in the text, e.g. two blanks
 *  after periods.  All of the other word processing routines are original
 *  and written by me, Frank, and are not guaranteed to work as designed.
 *
 * See:
 *
 *   Brian W. Kernighan and P. J. Plauger, _Software Tools_, Addison-
 *    Wesly, Reading, Mass., 1976, Section 7.7, "Right Margin Justification",
 *    pp 239-242.  ISBN 0-201-03669-X.
 *
 * Note: right margin justification was added in TDE 2.2.
 *
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991, version 1.0
 * Date:             July 29, 1991, version 1.1
 * Date:             October 5, 1991, version 1.2
 * Date:             January 20, 1992, version 1.3
 * Date:             February 17, 1992, version 1.4
 * Date:             April 1, 1992, version 1.5
 * Date:             June 5, 1992, version 2.0
 * Date:             October 31, 1992, version 2.1
 * Date:             April 1, 1993, version 2.2
 * Date:             June 5, 1993, version 3.0
 * Date:             August 29, 1993, version 3.1
 *
 * This code is released into the public domain, Frank Davis.
 * You may distribute it freely.
 */

#include "tdestr.h"     /* global variables definitions */
#include "common.h"     /* external global variable declarations */
#include "define.h"
#include "tdefunc.h"


/*
 * Name:    find_left_margin
 * Purpose: find left margin depending on word wrap mode
 * Date:    June 5, 1992
 * Passed:  ll:         node pointer to current line
 *          wrap_mode:  current word wrap mode
 * Notes:   the algorithm used to figure the indent column was yanked out
 *           of the insert_newline( ) function and was made into a more
 *           general algorithm for figuring the left margin irregardless
 *           of word wrap or indent mode.  when in the DYNAMIC_WRAP mode,
 *           the user don't have to keep changing the left margin when
 *           special indentation is needed.
 */
int  find_left_margin( line_list_ptr ll, int wrap_mode )
{
register int lm;
int  len;
text_ptr source;

   if (wrap_mode == FIXED_WRAP) {
      /*
       * for FIXED_WRAP mode, the left and paragraph margins are determined
       *   from the master mode structure.
       */
      if (g_status.copied) {
         source = (text_ptr)g_status.line_buff;
         len    = g_status.line_buff_len;
      } else {
         if (ll->prev != NULL) {
            source = ll->prev->line;
            len    = ll->prev->len;
         } else {
            source = NULL;
            len    = 0;
         }
      }
      if (source == NULL)
         lm = mode.parg_margin;
      else if (find_end( source, len ) == 0)
         lm = mode.parg_margin;
      else
         lm = mode.left_margin;
   } else {
      /*
       * for Indent and DYNAMIC_WRAP modes, the left margin is determined
       *  from the first non blank line above the cursor.
       */
      if (g_status.copied == TRUE) {
         source = (text_ptr)g_status.line_buff;
         len    = g_status.line_buff_len;
      } else {
         source = ll->line;
         len    = ll->len;
      }
      lm = first_non_blank( source, len );
      if (is_line_blank( source, len ) && ll->prev != NULL) {
         for (ll=ll->prev; ll != NULL; ll=ll->prev) {
            lm = first_non_blank( ll->line, ll->len );
            if (!is_line_blank( ll->line, ll->len ))
               break;
         }
      }
   }
   return( lm );
}


/*
 * Name:    word_wrap
 * Purpose: make sure lines don't get longer than right margin
 * Date:    November 27, 1991
 * Passed:  window:  pointer to current window
 * Notes:   rcol, lm, rm, pm all start counting at zero.
 *          len (line length) starts counting at 1.
 *
 *          when we compare margins and line lengths, we either have to
 *          add one to the margins or subtract one from the len.  I add
 *          one to the margins.
 */
void word_wrap( WINDOW *window )
{
int  c;                 /* character the user just entered. */
register int len;       /* length of current line */
int  i;                 /* padding spaces required */
line_list_ptr p;        /* line above wrapped line */
int  rcol;
int  lm;
int  rm;
int  side;
register WINDOW *win;          /* put window pointer in a register */

   win = window;

   /*
    * set up a few local variables.
    */
   c = g_status.key_pressed;
   rcol = win->rcol;
   copy_line( win->ll );
   detab_linebuff( );

   /*
    * always start the right margin justification on the right side
    *  at the beginning of paragraphs.  then, alternate with left margin.
    */
   side = 1;
   p = win->ll->prev;
   while (p != NULL  &&  !is_line_blank( p->line, p->len )) {
      ++side;
      p = p->prev;
   }
   side = (side & 1) ? RIGHT : LEFT;


   /*
    * when we wrap, we need know where the left margin is.
    * let's look at the line above to see if this is the first line
    * in a paragraph.
    */
   p = win->ll->prev;

   lm = find_left_margin( win->ll, mode.word_wrap );
   rm = mode.right_margin;

   /*
    * there two ways that words are pushed onto next line.
    *  1. if the word being typed goes over the right margin
    *  2. typing a word in the middle of the line pushes words at end of
    *     line to next line
    *
    * if the user enters spaces past the right margin then we don't
    *  word wrap spaces.
    */
   len = g_status.line_buff_len;
   if (rcol > rm+1 && c != ' ') {

      /*
       * if this is the first line in a paragraph then set left margin
       *  to paragraph margin.
       */
      if ((p == NULL || is_line_blank( p->line, p->len )) &&
           first_non_blank( (text_ptr)g_status.line_buff,
                 g_status.line_buff_len ) > rm && mode.word_wrap == FIXED_WRAP)
         lm = mode.parg_margin;

      /*
       * simple word wrap.  the cursor goes past the right margin.
       *  find the beginning of the word and put it on a new line.
       *
       * Special case - if the word begins at the left margin then
       *  don't wrap it.
       */
      for (i=rcol-1; i > lm  &&  g_status.line_buff[i] != ' '; )
         i--;
      if (i > lm) {
         i++;
         win->rcol = i;
         g_status.command = WordWrap;
         insert_newline( win );
         if (mode.right_justify == TRUE)
            justify_right_margin( win, win->ll->prev,
                 mode.word_wrap == FIXED_WRAP ? find_left_margin( win->ll->prev,
                 mode.word_wrap ) : lm, rm, side );

         /*
          * find out where to place the cursor on the new line.
          */
         win->rcol = lm + rcol - i;
         check_virtual_col( win, win->rcol, win->rcol );

         /*
          * we just wrapped the word at the eol.  now, let's see if
          *  we can combine it with the line below.  since just added
          *  a line, set new_line to false - don't add another line.
          */

         len = find_end( win->ll->line, win->ll->len );
         if (len < rm+1)
            combine_wrap_spill( win, len, lm, rm, side, FALSE );
      }
   } else if (len > rm+1) {

      /*
       * this is the second word wrap case.  we are pushing words onto
       * next line.  we need to now what character is in the right margin.
       *
       * 1) if the character is not a space, then we need to search backwards
       *    to find the start of the word that is on the right margin.
       * 2) if the character is a space, then we need to search forward to
       *    find the word that is over the right margin.
       */

      /*
       * don't wrap spaces past right margin
       */
      if (c == ' ' && rcol > rm) {
         for (i=rcol; i<len && g_status.line_buff[i] == ' ';)
            i++;

         /*
          * if i == len then all that's left on line is blanks - don't wrap.
          */
         if (i < len)
            combine_wrap_spill( win, i, lm, rm, side, TRUE );

      } else if (g_status.line_buff[rm+1] != ' ') {

         /*
          * search backwards for the word to put on next line.
          */
         for (i=rm+1; i > lm  &&  g_status.line_buff[i] != ' '; )
            i--;

         /*
          * if we search all the way back to left margin then test for
          * a special case - see the matching else for more info.
          */
         if (i > lm) {
            i++;

            /*
             * if i > rcol then cursor stays on same line.
             */
            if (i > rcol) {
               combine_wrap_spill( win, i, lm, rm, side, TRUE );

            /*
             * split the line at or behind the cursor.  almost the
             *  same as when the cursor goes over the right margin.
             */
            } else if (i <= rcol) {
               win->rcol = i;
               g_status.command = WordWrap;
               insert_newline( win );
               if (mode.right_justify == TRUE)
                  justify_right_margin( win, win->ll->prev,
                   mode.word_wrap == FIXED_WRAP ?
                   find_left_margin( win->ll->prev, mode.word_wrap ) : lm,
                   rm, side );
               win->rcol = lm + rcol - i;
               check_virtual_col( win, win->rcol, win->rcol );
               len = find_end( win->ll->line, win->ll->len );
               if (len < rm+1)
                  combine_wrap_spill( win, len, lm, rm, side, FALSE );
            }
         }

         /*
          * if the user changed margins or for some reason there's a long
          *  text line, let's see if there are any words past the right
          *  margin.  if we get to this else, we know the current word
          *  begins at least at the left margin.
          *
          * now search forwards for a break
          */
      } else {

         /*
          * go to the right margin and see if there are any words past
          *  right margin.
          */
         for (i=rm+1; i<len && g_status.line_buff[i] == ' '; )
            i++;

         /*
          * we either found a space or the eol.  test for eol.
          * if i == len then this is one big word - don't wrap it.
          */
         if (i != len)
            combine_wrap_spill( win, i, lm, rm, side, TRUE );
      }
   }
}


/*
 * Name:    format_paragraph
 * Purpose: format paragraph using left, right, and paragraph margins.
 * Date:    November 27, 1991
 * Passed:  window:  pointer to current window
 */
int  format_paragraph( WINDOW *window )
{
register int len;       /* length of current line */
int  first_line;        /* boolean, first line formatted? */
int  spaces;            /* no. of spaces to add */
line_list_ptr p;        /* scratch pointers */
line_list_ptr pp;
char *source;           /* scratch line buffer pointers */
char *dest;
int  rcol;              /* scratch cols and margins */
int  lm;
int  rm;
int  pm;
int  margin;
int  eop;               /* boolean, (e)nd (o)f (p)aragraph? */
int  old_ww;            /* save state of word wrap flag */
long rline;
WINDOW w;               /* scratch window */

   if (window->ll->len == EOF)
      return( ERROR );
   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   if (!is_line_blank( window->ll->line, window->ll->len )) {
      old_ww = mode.word_wrap;
      if (old_ww == NO_WRAP)
         mode.word_wrap = FIXED_WRAP;
      dup_window_info( &w, window );
      g_status.screen_display = FALSE;

      /*
       * find the beginning of the paragraph.
       */
      p = w.ll->prev;
      if (g_status.command == FormatParagraph) {
         while (p != NULL &&  !is_line_blank( p->line, p->len )) {
            --w.rline;
            w.ll = w.ll->prev;
            p = p->prev;
         }
         pm = mode.parg_margin;

      /*
       * if format text, don't find the beginning of the paragraph.
       *  but we need to know if this is the first line in a paragraph.
       */
      } else if (g_status.command == FormatText) {
         if (p == NULL || is_line_blank( p->line, p->len ))
            pm = mode.parg_margin;
         else
            pm = mode.left_margin;
      } else
         pm = mode.left_margin;

      g_status.command = WordWrap;
      p = w.ll;
      if (mode.word_wrap == FIXED_WRAP)
         lm = mode.left_margin;
      else
         lm = pm = find_left_margin( p, mode.word_wrap );
      rm = mode.right_margin;
      eop = FALSE;

      /*
       * do the paragraph
       */
      for (first_line=TRUE; p != NULL  &&  !is_line_blank( p->line, p->len ) &&
                            eop == FALSE  &&  !g_status.control_break;) {

         /*
          * find out what margin to use
          */
         if (first_line) {
            margin = pm;
            first_line = FALSE;
         } else
            margin = lm;

         /*
          * line up the margin
          */
         w.ll->dirty = TRUE;
         copy_line( w.ll );
         detab_linebuff( );
         remove_spaces( 0 );
         rcol = find_word( (text_ptr)g_status.line_buff,
                                     g_status.line_buff_len, 0 );
         if (rcol != ERROR && rcol != margin) {

            /*
             * must add spaces to get the indentation right
             */
            if (rcol < margin) {
               source = g_status.line_buff;
               spaces = margin - rcol;
               dest = source + spaces;

               assert( g_status.line_buff_len >= 0 );
               assert( g_status.line_buff_len < MAX_LINE_LENGTH );

               memmove( dest, source, g_status.line_buff_len );
               g_status.line_buff_len += spaces;
               while (spaces--)
                  *source++ = ' ';
            } else {
               w.rcol = margin;
               word_delete( &w );
               entab_linebuff( );
               un_copy_line( p, &w, TRUE );
               copy_line( w.ll );
               detab_linebuff( );
               remove_spaces( margin );
            }
         }

         /*
          * now make sure rest of line is formatted
          */

         source = g_status.line_buff;
         len = g_status.line_buff_len;
         for (; len < rm+1 && eop == FALSE;) {
            pp = p->next;
            if (is_line_blank( pp->line, pp->len ))
               eop = TRUE;
            else {
               w.ll = p;
               w.rcol = len + 1;
               if (*(p->line+len-1) == '.')
                  ++w.rcol;
               word_delete( &w );
               entab_linebuff( );
               un_copy_line( p, &w, TRUE );
               copy_line( p );
               detab_linebuff( );
               remove_spaces( margin );
               len = g_status.line_buff_len;
            }
         }
         if (len <= rm+1) {
            entab_linebuff( );
            un_copy_line( p, &w, TRUE );
            p = p->next;
            if (is_line_blank( p->line, p->len ))
               eop = TRUE;
            else {
               w.ll = w.ll->next;
               w.rline++;
            }
         } else {
            w.rcol = rm;
            g_status.key_pressed = *(w.ll->line + rm);
            rline = w.rline;
            word_wrap( &w );
            if (rline == w.rline) {
               w.ll = w.ll->next;
               ++w.rline;
            }
         }
         g_status.copied = FALSE;
         p = w.ll;
      }
      mode.word_wrap = old_ww;
      g_status.copied = FALSE;
      w.file_info->dirty = GLOBAL;
      g_status.screen_display = TRUE;
   }
   return( OK );
}


/*
 * Name:    combine_wrap_spill
 * Purpose: combine word wrap lines so we don't push each word onto a
 *          separate line.
 * Date:    November 27, 1991
 * Passed:  window:   pointer to current window
 *          wrap_col: col to combine next line
 *          lm:       left margin
 *          rm:       right margin
 *          side:     left or right margin to insert spaces
 *          new_line: boolean, should we insert a new line?
 */
void combine_wrap_spill( WINDOW *window, int wrap_col, int lm, int rm,
                         int side, int new_line )
{
line_list_ptr p;        /* line we wrapped */
line_list_ptr pp;       /* pointer to next line after wrapped line */
int  p_len;             /* length of line we just word wrapped */
int  non_blank;         /* first non-blank column on next line */
int  control_t;         /* number of times to call word_delete */
int  next_line_len;     /* length of next line counting from 1st word */
WINDOW w;               /* scratch window */

   dup_window_info( &w, window );
   g_status.command = WordWrap;
   w.rcol = wrap_col;
   if (new_line) {
      insert_newline( &w );
      if (mode.right_justify == TRUE)
         justify_right_margin( &w, w.ll->prev, mode.word_wrap == FIXED_WRAP ?
                find_left_margin( w.ll->prev, mode.word_wrap ) : lm, rm, side );
      p = window->ll->next;
   } else
      p = window->ll;
   if (p != NULL) {
      p_len = find_end( p->line, p->len );
      pp = p->next;
      if (pp != NULL) {
         non_blank = first_non_blank( pp->line, pp->len );
         next_line_len = find_end( pp->line, pp->len ) - non_blank;
         if (!is_line_blank( pp->line, pp->len ) && p_len + next_line_len <= rm) {
            control_t = 1;
            if (mode.inflate_tabs) {
               if (*pp->line == ' '  ||  *pp->line == '\t')
                  ++control_t;
            } else if (*pp->line == ' ')
               ++control_t;
            w.ll = p;
            w.rcol = p_len + 1;
            if (*(p->line+p_len-1) == '.')
               ++w.rcol;
            while (control_t--)
               word_delete( &w );
            remove_spaces( lm );
            un_copy_line( w.ll, &w, TRUE );
         }
         window->file_info->dirty = GLOBAL;
      }
   }
}


/*
 * Name:    justify_right_margin
 * Purpose: distribute blanks in line to justify right margin
 * Date:    December 30, 1992
 * Passed:  window:   pointer to current window
 *          ll:       current node pointing to current line
 *          lm:       left margin
 *          rm:       right margin
 *          side:     which side to start inserting space? LEFT or RIGHT
 * Notes:   this routine is based on the spread function by
 *           Kernighan and Plauger in _Software Tools_, Addison-Wesly,
 *           Reading, Mass., 1976, ISBN 0-201-03669-X, pp 240-241.
 */
void justify_right_margin( WINDOW *window, line_list_ptr ll, int lm, int rm,
                           int side )
{
int  len;
int  i;
int  word_count;
int  holes;
int  nb;
int  spaces;
text_ptr s;

   /*
    * make sure line is longer than the left margin and less than the right.
    */
   len = find_end( ll->line, ll->len );
   if (len <= lm || len >= rm+1)
      return;

   /*
    * now, count the number of words in line.
    */
   i = entab_adjust_rcol( ll->line, ll->len, lm );
   s = ll->line + i;
   len -= i;
   word_count = 0;
   while (len > 0) {
      while (len-- > 0  &&  *s++ == ' ');
      if (len == 0)
         break;
      ++word_count;
      while (len-- > 0  &&  *s++ != ' ');
   }

   /*
    * can't justify right margin with one word or less.
    */
   if (word_count <= 1)
      return;

   holes = word_count - 1;
   copy_line( ll );
   detab_linebuff( );
   remove_spaces( lm );

   /*
    * find out how many spaces we need.  if spaces <= 0, then get out.
    */
   i = g_status.line_buff_len - 1;
   spaces = rm - i;
   if (spaces <= 0)
      return;
   g_status.line_buff_len += spaces;

   /*
    * this while loop is based on the while loop on page 241 in
    *   _Software Tools_, Kernighan and Plauger.  I added a while loop
    *   to skip extra blanks after a period or other space intentially
    *   inserted into the text.
    */
   while (i < rm) {
      g_status.line_buff[rm] = g_status.line_buff[i];
      if (g_status.line_buff[rm] == ' ') {

         /*
          * this while loop copies extra blanks after a period or
          *   blanks otherwise intentially left in the text.
          */
         while (g_status.line_buff[i-1] == ' ')
            g_status.line_buff[--rm] = g_status.line_buff[--i];

         nb = side == LEFT ? spaces / holes : (spaces - 1) / holes + 1;
         spaces -= nb;
         --holes;
         while (nb-- > 0)
            g_status.line_buff[--rm] = ' ';
      }
      --i;
      --rm;
   }
   entab_linebuff( );
   un_copy_line( ll, window, window->bottom_line );
}


/*
 * Name:    remove_spaces
 * Purpose: remove any extra spaces in the text
 * Date:    December 30, 1992
 * Passed:  lm:   left margin
 * Notes:   this routine squeezes out extra space from a previous format.
 *          the line must be placed in the line buffer before calling this
 *            function.
 */
void remove_spaces( int lm )
{
int  period;
int  len;
int  i;
int  c;
char *s;
char *d;

   if ((i = len = g_status.line_buff_len) <= lm)
      return;

   period = FALSE;
   s = d = g_status.line_buff + lm;
   i -= lm;
   c = (int)*s++;
   while (c == ' ' && i > 0) {
      c = *s++;
      --i;
      --len;
   }
   period = c == '.' ? TRUE : FALSE;
   while (i > 0) {
      *d++ = (char)c;
      c = (int)*s++;
      --i;
      if (c != ' ')
         period =  c == '.' ? TRUE : FALSE;
      else {
         *d++ = (char)c;
         c = (int)*s++;
         --i;
         if (period  &&  c == ' ') {
            *d++ = (char)c;
            period = FALSE;
            if (i > 0)
               ++len;
         }
         while (c == ' '  &&  i > 0) {
            c = (int)*s++;
            --len;
            --i;
         }
      }
   }
   *d = (char)c;
   g_status.line_buff_len = len;
}


/*
 * Name:    find_word
 * Purpose: find a word on a line
 * Date:    November 29, 1991
 * Passed:  p:          pointer to a line of text
 *          len:        len of line
 *          start_col:  col to start the search
 * Notes:   returns the column of the next word or -1 (ERROR) if no more words
 */
int  find_word( text_ptr p, int len, int start_col )
{
register int rc;
register char c;

   if (len <= start_col  ||  len < 0  || start_col < 0)
      return( ERROR );
   p += start_col;
   rc = start_col;

   if (mode.inflate_tabs) {
      while (len-- > 0 && ((c = *p++) == ' ' || c == '\t'))
         if (c != '\t')
            ++rc;
         else
            rc += mode.ptab_size - (rc % mode.ptab_size);
   } else
      while (len-- > 0  &&  (c = *p++) == ' ')
         ++rc;
   if (len <= 0)
     rc = ERROR;
   return( rc );
}


/*
 * Name:    flush_left
 * Purpose: flush line on left margin
 * Date:    November 27, 1991
 * Passed:  window:  pointer to current window
 */
int  flush_left( WINDOW *window )
{
int  len;       /* length of current line */
register int spaces;
char *source;
char *dest;
int  rcol;
int  lm;
register WINDOW *win;          /* put window pointer in a register */

   win = window;
   copy_line( win->ll );
   detab_linebuff( );
   lm = mode.left_margin;
   rcol = find_word( (text_ptr)g_status.line_buff, g_status.line_buff_len, 0 );
   if (rcol != ERROR && rcol != lm) {

      /*
       * must add spaces to get the indentation correct
       */
      if (rcol < lm) {
         source = g_status.line_buff;
         spaces = lm - rcol;
         dest = source + spaces;
         len = g_status.line_buff_len;
         if (len + spaces > MAX_LINE_LENGTH) {
            /*
             * line would be too long
             */
            error( WARNING, win->bottom_line, ww1 );
            return( ERROR );
         } else {
            load_undo_buffer( win->file_info, win->ll->line, win->ll->len );

            assert( len >= 0 );
            assert( len < MAX_LINE_LENGTH );

            memmove( dest, source, len );
            g_status.line_buff_len += spaces;
            while (spaces--)
               *source++ = ' ';
            win->file_info->dirty = GLOBAL;
         }

      /*
       * else delete spaces to get the indentation correct
       */
      } else {
         dest = g_status.line_buff + lm;
         source = g_status.line_buff + rcol;

         assert( g_status.line_buff_len - rcol >= 0 );
         assert( g_status.line_buff_len - rcol < MAX_LINE_LENGTH );

         memmove( dest, source, g_status.line_buff_len - rcol );
         g_status.line_buff_len -= (rcol - lm);
         win->file_info->dirty = GLOBAL;
      }
      win->ll->dirty = TRUE;
      show_changed_line( win );
   }
   return( OK );
}


/*
 * Name:    flush_right
 * Purpose: flush line on right margin
 * Date:    November 27, 1991
 * Passed:  window:  pointer to current window
 */
int  flush_right( WINDOW *window )
{
int  len;               /* length of current line */
int  i;
int  spaces;
char *source;
char *dest;
register int rcol;
int  rm;
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   copy_line( win->ll );
   detab_linebuff( );
   source = g_status.line_buff;
   len = g_status.line_buff_len;
   if (!is_line_blank( (text_ptr)source, len )) {
      rm = mode.right_margin;
      for (rcol=len-1; rcol>=0 && *(source+rcol) == ' ';)
         rcol--;
      if (rcol != rm) {

         /*
          * if rcol is less than right margin then we need to add spaces.
          */
         if (rcol < rm) {
            spaces = rm - rcol;
            dest = source + spaces;
            if (len + spaces > MAX_LINE_LENGTH) {
               /*
                * line would be too long
                */
               error( WARNING, win->bottom_line, ww1 );
               return( ERROR );
            } else {
               load_undo_buffer( win->file_info, win->ll->line, win->ll->len );

               assert( len >= 0 );
               assert( len < MAX_LINE_LENGTH );

               memmove( dest, source, len );
               g_status.line_buff_len += spaces;
               while (spaces--)
                  *source++ = ' ';
               win->file_info->dirty = GLOBAL;
            }

         /*
          * if rcol is greater than right margin then we need to sub spaces.
          */
         } else {
            load_undo_buffer( win->file_info, win->ll->line, win->ll->len );
            rcol = rcol - rm;
            i = first_non_blank( (text_ptr)source, len );
            if (rcol > i)
               rcol = i;
            dest = source + rcol;

            assert( len - rcol >= 0 );
            assert( len - rcol < MAX_LINE_LENGTH );

            memmove( source, dest, len - rcol );
            g_status.line_buff_len -= (rcol - rm);
            win->file_info->dirty = GLOBAL;
         }
         win->ll->dirty = TRUE;
         show_changed_line( win );
      }
   }
   return( OK );
}


/*
 * Name:    flush_center
 * Purpose: flush line in center of margins
 * Date:    November 27, 1991
 * Passed:  window:  pointer to current window
 */
int  flush_center( WINDOW *window )
{
int  len;               /* length of current line */
char *source;           /* temp line buffer pointers */
char *dest;
int  rm;
int  lm;
register int spaces;    /* center of text on current line */
int  center;            /* center of current margins */
int  first;             /* column of first char on line */
int  last;              /* column of last char on line */
register WINDOW *win;   /* put window pointer in a register */

   win = window;
   copy_line( win->ll );
   detab_linebuff( );
   source = g_status.line_buff;
   len = g_status.line_buff_len;
   if (!is_line_blank( (text_ptr)source, len )) {
      rm = mode.right_margin;
      lm = mode.left_margin;
      center = (rm + lm) / 2;
      first = first_non_blank( (text_ptr)source, len );
      for (last=len-1; last>=0 && *(source+last) == ' ';)
         last--;
      spaces = last + first - 1;
      spaces = (spaces / 2) + (spaces & 1);
      if (spaces != center) {

         /*
          * if spaces is less than center margin then we need to add spaces.
          */
         if (spaces < center) {
            spaces = center - spaces;
            dest = source + spaces;
            if (len + spaces > MAX_LINE_LENGTH) {
               /*
                * line would be too long
                */
               error( WARNING, win->bottom_line, ww1 );
               return( ERROR );
            } else {
               load_undo_buffer( win->file_info, win->ll->line, win->ll->len );

               assert( len >= 0 );
               assert( len < MAX_LINE_LENGTH );

               memmove( dest, source, len );
               g_status.line_buff_len += spaces;
               while (spaces--)
                  *source++ = ' ';
               win->file_info->dirty = GLOBAL;
            }

         /*
          * if spaces is greater than center margin then we need to sub spaces.
          */
         } else {
            load_undo_buffer( win->file_info, win->ll->line, win->ll->len );
            spaces = spaces - center;
            if (spaces > first)
               spaces = first;
            dest = source + spaces;

            assert( len - spaces >= 0 );
            assert( len - spaces < MAX_LINE_LENGTH );

            memmove( source, dest, len - spaces );
            g_status.line_buff_len -= spaces;
            win->file_info->dirty = GLOBAL;
         }
         win->ll->dirty = TRUE;
         show_changed_line( win );
      }
   }
   return( OK );
}
