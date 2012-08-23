

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"



int  mark_block( WINDOW *window )
{
int type;
int num;
long lnum;
register file_infos *file;      /* 临时文件 */
register WINDOW *win;           /* 把当前窗口指针放到一个临时寄存器里面 */
int rc;

   win  = window;
   file = win->file_info;
   if (win->rline > file->length || win->ll->len == EOF)
      return( ERROR );
   if (g_status.marked == FALSE) {
      g_status.marked = TRUE;
      g_status.marked_file = file;
   }
   if (g_status.command == MarkBox)
      type = BOX;
   else if (g_status.command == MarkLine)
      type = LINE;
   else if (g_status.command == MarkStream)
      type = STREAM;
   else
      return( ERROR );

   rc = OK;
   /*
    * 仅对于一个文件定义块。用户可以在这个文件的任何窗口进行操作。
    */
   if (file == g_status.marked_file) {

      /*
       * 不管块的模式标识块的起始和中止位置
       */
      if (file->block_type == NOTMARKED) {
         file->block_ec  = file->block_bc = win->rcol;
         file->block_er  = file->block_br = win->rline;
      } else {
         if (file->block_br > win->rline) {
            file->block_br = win->rline;
            if (file->block_bc < win->rcol && type != STREAM)
               file->block_ec = win->rcol;
            else
               file->block_bc = win->rcol;
         } else {
            if (type != STREAM) {
               file->block_ec = win->rcol;
               file->block_er = win->rline;
            } else {
               if (win->rline == file->block_br &&
                   win->rline == file->block_er) {
                  if (win->rcol < file->block_bc)
                     file->block_bc = win->rcol;
                  else
                     file->block_ec = win->rcol;
               } else if (win->rline == file->block_br)
                  file->block_bc = win->rcol;
               else {
                  file->block_ec = win->rcol;
                  file->block_er = win->rline;
               }
            }
         }

         /*
          * 如果用户标识的块的终止位置在起始位置前，那么交换两个位置。
          */
         if (file->block_er < file->block_br) {
            lnum = file->block_er;
            file->block_er = file->block_br;
            file->block_br = lnum;
         }

         /*
          * 如果用户标识的块的终止列在起始列前，那么交换两个位置。
          */
         if ((file->block_ec < file->block_bc) && (type != STREAM ||
              (type == STREAM && file->block_br == file->block_er))) {
            num = file->block_ec;
            file->block_ec = file->block_bc;
            file->block_bc = num;
         }
      }

      /*
       * 如果块类型已经被定义，但是如果用户使用混和模式，那么块的
	   * 类型被置为当前块的类型
       
       */
      if (file->block_type != NOTMARKED) {
         /*
          * 如果块的类型是矩形块，那么要保证左上角小于右下脚
          * 如果块的类型是stream块，那么保证起始列小于中止列
          */
         if (type == BOX) {
            if (file->block_ec < file->block_bc) {
               num = file->block_ec;
               file->block_ec = file->block_bc;
               file->block_bc = num;
            }
         }
      }

      assert( file->block_er >= file->block_br );

      file->block_type = type;
      file->dirty = GLOBAL;
   } else {
      /*
       * 已经定义好块
       */
      error( WARNING, win->bottom_line, block1 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    unmark_block
 * Class:   primary editor function
 * Purpose: To set all block information to NULL or 0
 * Date:    June 5, 1991
 * Passed:  arg_filler: variable to match array of function pointers prototype
 * Notes:   Reset all block variables if marked, otherwise return.
 *           If a marked block is unmarked then redraw the screen(s).
 */
int  unmark_block( WINDOW *arg_filler )
{
register file_infos *marked_file;

   if (g_status.marked == TRUE) {
      marked_file              = g_status.marked_file;
      g_status.marked          = FALSE;
      g_status.marked_file     = NULL;
      marked_file->block_start = NULL;
      marked_file->block_end   = NULL;
      marked_file->block_bc    = marked_file->block_ec = 0;
      marked_file->block_br    = marked_file->block_er = 0l;
      if (marked_file->block_type)
         marked_file->dirty = GLOBAL;
      marked_file->block_type  = NOTMARKED;
   }
   return( OK );
}


/*
 * Name:    restore_marked_block
 * Class:   helper function
 * Purpose: To restore block beginning and ending row after an editing function
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          net_change: number of bytes added or subtracted
 * Notes:   If a change has been made before the marked block then the
 *           beginning and ending row need to be adjusted by the number of
 *           lines added or subtracted from file.
 */
void restore_marked_block( WINDOW *window, int net_change )
{
long length;
register file_infos *marked_file;

   if (g_status.marked == TRUE && net_change != 0) {
      marked_file = g_status.marked_file;
      length = marked_file->length;

      /*
       * restore is needed only if a block is defined and window->file_info is
       * same as marked file and there was a net change in file length.
       */
      if (marked_file == window->file_info) {

         /*
          * if cursor is before marked block then adjust block by net change.
          */
         if (marked_file->block_br > window->rline) {
            marked_file->block_br += net_change;
            marked_file->block_er += net_change;
            marked_file->dirty = GLOBAL;
         /*
          * if cursor is somewhere in marked block don't restore, do redisplay
          */
         } else if (marked_file->block_er >= window->rline)
            marked_file->dirty = GLOBAL;

         /*
          * check for lines of marked block beyond end of file
          */
         if (marked_file->block_br > length)
            unmark_block( window );
         else if (marked_file->block_er > length) {
            marked_file->block_er = length;
            marked_file->dirty = GLOBAL;
         }
      }
   }
}


/*
 * Name:    prepare_block
 * Class:   helper function
 * Purpose: To prepare a window/file for a block read, move or copy.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          file: pointer to file information.
 *          text_line: pointer to line in file to prepare.
 *          lend: line length.
 *          bc: beginning column of BOX.
 * Notes:   The main complication is that the cursor may be beyond the end
 *           of the current line, in which case extra padding spaces have
 *           to be added before the block operation can take place.
 *           this only occurs in BOX and STREAM operations.
 *          since we are padding a line, do not trim trailing space.
 */
int  prepare_block( WINDOW *window, line_list_ptr ll, int bc )
{
register int pad = 0;   /* amount of padding to be added */
register int len;

   assert( bc >= 0 );
   assert( bc < MAX_LINE_LENGTH );
   assert( ll->len != EOF );
   assert( g_status.copied == FALSE );

   copy_line( ll );
   detab_linebuff( );

   len = g_status.line_buff_len;
   pad = bc - len;
   if (pad > 0) {
      /*
       * insert the padding spaces
       */

      assert( pad >= 0 );
      assert( pad < MAX_LINE_LENGTH );

      memset( g_status.line_buff+len, ' ', pad );
      g_status.line_buff_len += pad;
   }
   /*
    * if mode.inflate_tabs, let's don't entab the line until we get
    *   thru processing this line, e.g. copying, numbering....
    */
   return( un_copy_line( ll, window, FALSE ) );
}


/*
 * Name:    pad_dest_line
 * Class:   helper function
 * Purpose: To prepare a window/file for a block move or copy.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          dest_file: pointer to file information.
 *          dest_line: pointer to line in file to prepare.
 *          ll:        pointer to linked list node to insert new node
 * Notes:   We are doing a BOX action (except DELETE).   We have come
 *          to the end of the file and have no more lines.  All this
 *          routine does is add a blank line to file.
 */
int  pad_dest_line( WINDOW *window, file_infos *dest_file, line_list_ptr ll )
{
int rc;
text_ptr l;
line_list_ptr new_node;

   rc = OK;

   l = NULL;
   new_node = (line_list_ptr)my_malloc( sizeof(line_list_struc), &rc );
   if (rc == OK) {
      new_node->len   = 0;
      new_node->dirty = FALSE;
      new_node->line  = l;
      if (ll->next != NULL) {
         ll->next->prev = new_node;
         new_node->next = ll->next;
         ll->next = new_node;
         new_node->prev = ll;
      } else {
         new_node->next = ll;
         if (ll->prev != NULL)
            ll->prev->next = new_node;
         new_node->prev = ll->prev;
         ll->prev = new_node;
         if (new_node->prev == NULL)
            window->file_info->line_list = new_node;
      }
      ++dest_file->length;
   } else {
      /*
       * file too big
       */
      error( WARNING, window->bottom_line, block4 );
      if (new_node != NULL)
         my_free( new_node );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    move_copy_delete_overlay_block
 * Class:   primary editor function
 * Purpose: Master BOX, STREAM, or LINE routine.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Operations on BOXs, STREAMs, or LINEs require several common
 *           operations.  All require finding the beginning and ending marks.
 *          This routine will handle block operations across files.  Since one
 *           must determine the relationship of source and destination blocks
 *           within a file, it is relatively easy to expand this relationship
 *           across files.
 *          This is probably the most complicated routine in the editor.  It
 *           is not easy to understand.
 */
int  move_copy_delete_overlay_block( WINDOW *window )
{
int  action;
WINDOW *source_window;          /* source window for block moves */
line_list_ptr source;           /* source for block moves */
line_list_ptr dest;             /* destination for block moves */
long number;                    /* number of characters for block moves */
int  lens;                      /* length of source line */
int  lend;                      /* length of destination line */
int  add;                       /* characters being added from another line */
int  block_len;                 /* length of the block */
line_list_ptr block_start;      /* start of block in file */
line_list_ptr block_end;  /* end of block in file - not same for LINE or BOX */
int  prompt_line;
int  same;                      /* are dest and source files the same file? */
int  source_first;              /* is source file lower in memory than dest */
file_infos *source_file;
file_infos *dest_file;
int  rcol, bc, ec;              /* temporary column variables */
long rline;                     /* temporary real line variable */
long br, er;                    /* temporary line variables */
long block_num;                 /* starting number for block number */
long block_inc;                 /* increment to use for block number */
int  block_just;                /* left or right justify numbers? */
int  block_type;
int  fill_char;
int  rc;

   /*
    * initialize block variables
    */
   entab_linebuff( );
   rc = un_copy_line( window->ll, window, TRUE );
   if (g_status.marked == FALSE || rc == ERROR)
      return( ERROR );
   switch (g_status.command) {
      case MoveBlock :
         action = MOVE;
         break;
      case DeleteBlock :
         action = DELETE;
         break;
      case CopyBlock :
         action = COPY;
         break;
      case KopyBlock :
         action = KOPY;
         break;
      case FillBlock :
         action = FILL;
         break;
      case OverlayBlock :
         action = OVERLAY;
         break;
      case NumberBlock :
         action = NUMBER;
         break;
      case SwapBlock :
         action = SWAP;
         break;
      default :
         return( ERROR );
   }
   source_file = g_status.marked_file;
   source_window = g_status.window_list;
   for (; ptoul( source_window->file_info ) != ptoul( source_file );)
      source_window = source_window->next;
   prompt_line = window->bottom_line;
   dest_file = window->file_info;
   check_block( );
   if (g_status.marked == FALSE)
      return( ERROR );
   block_start = source_file->block_start;
   block_end = source_file->block_end;
   if (block_start == NULL  ||  block_end == NULL)
      return( ERROR );

   block_type = source_file->block_type;
   if (block_type != LINE  &&  block_type != STREAM  &&  block_type != BOX)
      return( ERROR );

   dest = window->ll;
   rline = window->rline;
   if (dest->len == EOF)
      return( ERROR );
   rc = OK;

   /*
    * set up Beginning Column, Ending Column, Beginning Row, Ending Row
    */
   bc = source_file->block_bc;
   ec = source_file->block_ec;
   br = source_file->block_br;
   er = source_file->block_er;

   /*
    * if we are BOX FILLing or BOX NUMBERing, beginning column is bc,
    *   not the column of cursor
    */
   rcol =  (action == FILL || action == NUMBER) ? bc : window->rcol;

   /*
    * must find out if source and destination file are the same.
    * it don't matter with FILL and DELETE - those actions only modify the
    * source file.
    */
   source_first = same = FALSE;
   if (action == FILL) {
      if (block_type == BOX) {
         if (get_block_fill_char( window, &fill_char ) == ERROR)
            return( ERROR );
         dest = block_start;
         same = TRUE;
      } else {
         /*
          * can only fill box blocks.
          */
         error( WARNING, prompt_line, block2 );
         return( ERROR );
      }
   }
   block_inc = 1;
   if (action == NUMBER) {
      if (block_type == BOX) {
         if (get_block_numbers( window, &block_num, &block_inc, &block_just )
              == ERROR)
            return( ERROR );
         dest = block_start;
         same = TRUE;
      } else {
         /*
          * can only number box blocks.
          */
         error( WARNING, prompt_line, block3a );
         return( ERROR );
      }
   }
   if (action == SWAP) {
      if (block_type != BOX) {
         /*
          * can only swap box blocks.
          */
         error( WARNING, prompt_line, block3b );
         return( ERROR );
      }
   }
   if (source_file == dest_file && action != DELETE && action != FILL) {
      same = TRUE;
      if (block_type == BOX && action == MOVE) {
         if (rline == br  &&  (rcol >= bc && rcol <= ec))
            /*
             * a block moved to within the block itself has no effect
             */
            return( ERROR );
      } else if (block_type == LINE || block_type == STREAM) {
         if (rline >= br && rline <= er) {
            if (block_type == LINE) {
                /*
                 * if COPYing or KOPYing within the block itself, reposition the
                 * destination to the next line after the block (if it exists)
                 */
               if (action == COPY || action == KOPY)
                  dest = block_end;
                /*
                 * a block moved to within the block itself has no effect
                 */
               else if (action == MOVE)
                  return( ERROR );
            } else {

               /*
                * to find out if cursor is in a STREAM block we have to do
                * a few more tests.  if cursor is on the beginning row or
                * ending row, then check the beginning and ending column.
                */
               if ((rline > br && rline < er) ||
                   (br == er && rcol >= bc && rcol <= ec) ||
                   (br != er && ((rline == br && rcol >= bc) ||
                                 (rline == er && rcol <= ec)))) {

                  /*
                   * if the cursor is in middle of STREAM, make destination
                   * the last character following the STREAM block.
                   */
                  if (action == COPY || action == KOPY) {
                     dest = block_end;
                     rcol = ec + 1;
                     rline = er;
                  } else if (action == MOVE)
                     return( ERROR );
               }
            }
         }
      }
   }
   if (br < rline)
      source_first = TRUE;

   /*
    * 1. can't create lines greater than g_display.line_length
    * 2. if we are FILLing a BOX - fill block buff once right here
    * 3. only allow overlaying BOXs
    */
   block_len = (ec+1) - bc;
   if (block_type == BOX) {
      if (action != DELETE && action != FILL) {
         if (rcol + block_len > MAX_LINE_LENGTH) {
            /*
             * line too long
             */
            error( WARNING, prompt_line, ltol );
            return( ERROR );
         }
      }
   } else if (block_type == LINE) {
      block_len = 0;
      if (action == OVERLAY) {
         /*
          * can only overlay box blocks
          */
         error( WARNING, prompt_line, block5 );
         return( ERROR );
      }
   } else if (block_type == STREAM) {

      if (action == OVERLAY) {
         /*
          * can only overlay box blocks
          */
         error( WARNING, prompt_line, block5 );
         return( ERROR );
      }

      lend = block_end->len;
      if (action == DELETE || action == MOVE) {

         /*
          * Is what's left on start of STREAM block line plus what's left at
          * end of STREAM block line too long?
          */
         if (lend > ec)
            lend -= ec;
         else
            lend = 0;
         if (bc + lend > MAX_LINE_LENGTH) {
            /*
             * line too long
             */
            error( WARNING, prompt_line, ltol );
            return( ERROR );
         }
      }

      if (action != DELETE) {

         /*
          * We are doing a MOVE, COPY, or KOPY.  Find out if what's on the
          * current line plus the start of the STREAM line are too long.
          * Then find out if end of the STREAM line plus what's left of
          * the current line are too long.
          */
         lens = block_start->len;

         /*
          * if we had to move the destination of the STREAM COPY or KOPY
          * to the end of the STREAM block, then dest and window->ll->line
          * will not be the same.  In this case, set length to length of
          * first line in STREAM block.  Then we can add the residue of
          * the first line in block plus residue of the last line of block.
          */
         if (dest->line == window->ll->line)
            add = dest->len;
         else
            add = lens;

         /*
          * Is current line plus start of STREAM block line too long?
          */
         if (lens > bc)
            lens -= bc;
         else
            lens = 0;
         if (rcol + lens > MAX_LINE_LENGTH) {
            /*
             * line too long
             */
            error( WARNING, prompt_line, ltol );
            return( ERROR );
         }

         /*
          * Is residue of current line plus residue of STREAM block line
          * too long?
          */
         if (add > bc)
            add -= bc;
         else
            add = 0;
         if (lend > ec)
            lend -= ec;
         else
            lend = 0;
         if (add + lend > MAX_LINE_LENGTH) {
            /*
             * line too long
             */
            error( WARNING, prompt_line, ltol );
            return( ERROR );
         }
      }
      if (ptoul( block_start ) == ptoul( block_end )) {
         block_type = BOX;
         block_len = (ec+1) - bc;
      }
   }

   if (mode.do_backups == TRUE) {
      switch (action) {
         case MOVE :
         case DELETE :
         case COPY :
         case KOPY :
         case SWAP :
            window->file_info->modified = TRUE;
            rc = backup_file( window );
            break;
      }
      switch (action) {
         case MOVE :
         case DELETE :
         case FILL :
         case NUMBER :
         case SWAP :
            source_window->file_info->modified = TRUE;
            if (rc != ERROR)
               rc = backup_file( source_window );
            break;
      }
   }
   source = block_start;

   assert( block_start != NULL );
   assert( block_start->len != EOF );
   assert( block_end != NULL );
   assert( block_end->len != EOF );

   if (block_type == LINE)
      do_line_block( window,  source_window,  action,
                     source_file,  dest_file,  block_start,  block_end,
                     source,  dest,  br,  er,  &rc );

   else if (block_type == STREAM)
      do_stream_block( window,  source_window,  action,
                       source_file,  dest_file,  block_start,  block_end,
                       source,  dest,  rline,  br,  er,  bc,  ec,  rcol,  &rc );

   else
      do_box_block( window,  source_window,  action,
                    source_file,  dest_file,  source,  dest,  br,  er,
                    block_inc, rline, block_num, block_just, fill_char,
                    same, block_len, bc, ec,  rcol, &rc );

   dest_file->modified = TRUE;
   dest_file->dirty = GLOBAL;
   if (action == MOVE || action == DELETE || action == FILL || action==NUMBER) {
      source_file->modified = TRUE;
      source_file->dirty = GLOBAL;
   }

   /*
    * unless we are doing a KOPY, FILL, NUMBER, or OVERLAY we need to unmark
    * the block.  if we just did a KOPY, the beginning and ending may have
    * changed.  so, we must readjust beginning and ending rows.
    */
   if (action == KOPY) {
      if (same && !source_first && block_type == LINE  &&  rc != ERROR) {
         number = (er+1) - br;
         source_file->block_br += number;
         source_file->block_er += number;
      }
   } else if (action != FILL && action != OVERLAY && action != NUMBER)
      unmark_block( window );
   show_avail_mem( );
   g_status.copied = FALSE;
   return( rc );
}


/*
 * Name:    do_line_block
 * Purpose: delete, move, copy, or kopy a LINE block
 * Date:    April 1, 1993
 * Passed:  window:  pointer to current window
 * Passed:  window:         pointer to destination window (current window)
 *          source_window:  pointer to source window
 *          action:         block action  --  KOPY, MOVE, etc...
 *          source_file:    pointer to source file structure
 *          dest_file:      pointer to destination file
 *          block_start:    pointer to first node in block
 *          block_end:      pointer to last node in block
 *          source:         pointer to source node
 *          dest:           pointer to destination node
 *          br:             beginning line number in marked block
 *          er:             ending line number in marked block
 *          rc:             return code
 */
void do_line_block( WINDOW *window,  WINDOW *source_window,  int action,
                    file_infos *source_file,  file_infos *dest_file,
                    line_list_ptr block_start,  line_list_ptr block_end,
                    line_list_ptr source,  line_list_ptr dest,
                    long br,  long er, int *rc )
{
line_list_ptr temp_ll;          /* temporary list pointer */
text_ptr l;
int  lens;                      /* length of source line */
long li;                        /* temporary line variables */
long diff;

   if (action == COPY || action == KOPY) {

      assert( br >= 1 );
      assert( br <= source_file->length );
      assert( er >= br );
      assert( er <= source_file->length );

      for (li=br; li <= er  &&  *rc == OK; li++) {
         lens = source->len;

         assert( lens * sizeof(char) < MAX_LINE_LENGTH );

         l = (text_ptr)my_malloc( lens * sizeof(char), rc );
         temp_ll = (line_list_ptr)my_malloc( sizeof(line_list_struc), rc );
         if (*rc == OK) {
            if (lens > 0)
               _fmemcpy( l, source->line, lens );
            temp_ll->line  = l;
            temp_ll->len   = lens;
            temp_ll->dirty = TRUE;

            if (dest->next != NULL) {
               dest->next->prev = temp_ll;
               temp_ll->next = dest->next;
               dest->next = temp_ll;
               temp_ll->prev = dest;
            } else {
               temp_ll->next = dest;
               if (dest->prev != NULL)
                  dest->prev->next = temp_ll;
               temp_ll->prev = dest->prev;
               dest->prev = temp_ll;
               if (temp_ll->prev == NULL)
                  window->file_info->line_list = temp_ll;
            }

            dest = temp_ll;
            source = source->next;
         } else {
            /*
             * file too big
             */
            error( WARNING, window->bottom_line, dir3 );
            if (l != NULL)
               my_free( l );
            if (temp_ll != NULL)
               my_free( temp_ll );
            *rc = ERROR;
            er = li - 1;
         }
      }
   } else if (action == MOVE) {
      if (dest->len != EOF  &&  dest->next != NULL) {
         temp_ll = block_start;
         for (li=br; li <= er  &&  *rc == OK; li++) {
            temp_ll->dirty = TRUE;
            temp_ll = temp_ll->next;
         }
         if (block_start->prev == NULL)
            source_file->line_list = block_end->next;
         if (block_start->prev != NULL)
            block_start->prev->next = block_end->next;
         block_end->next->prev = block_start->prev;
         dest->next->prev = block_end;
         block_start->prev = dest;
         block_end->next = dest->next;
         dest->next = block_start;
      }
   } else if (action == DELETE) {
      block_end->next->prev = block_start->prev;
      if (block_start->prev == NULL)
         source_file->line_list = block_end->next;
      else
         block_start->prev->next = block_end->next;
      block_end->next = NULL;
      while (block_start != NULL) {
         temp_ll = block_start;
         block_start = block_start->next;
         if (temp_ll->line != NULL)
            my_free( temp_ll->line );
         my_free( temp_ll );
      }
   }

   diff =  er + 1L - br;
   if (action == COPY || action == KOPY || action == MOVE)
      dest_file->length += diff;
   if (action == DELETE || action == MOVE)
      source_file->length -= diff;
   if (action == DELETE && source_window->rline >= br) {
      source_window->rline -= diff;
      if (source_window->rline < br)
         source_window->rline = br;
   }
   /*
    * restore all cursors in all windows
    */
   restore_cursors( dest_file );
   if (dest_file != source_file)
      restore_cursors( source_file );
   show_avail_mem( );
}


/*
 * Name:    do_stream_block
 * Purpose: delete, move, copy, or kopy a STREAM block
 * Date:    June 5, 1991
 * Passed:  window:         pointer to destination window (current window)
 *          source_window:  pointer to source window
 *          action:         block action  --  KOPY, MOVE, etc...
 *          source_file:    pointer to source file structure
 *          dest_file:      pointer to destination file
 *          block_start:    pointer to first node in block
 *          block_end:      pointer to last node in block
 *          source:         pointer to source node
 *          dest:           pointer to destination node
 *          rline:          current line number in destination file
 *          br:             beginning line number in marked block
 *          er:             ending line number in marked block
 *          bc:             beginning column of block
 *          ec:             ending column of block
 *          rcol:           current column of cursor
 *          rc:             return code
 */
void do_stream_block( WINDOW *window,  WINDOW *source_window,  int action,
                    file_infos *source_file,  file_infos *dest_file,
                    line_list_ptr block_start,  line_list_ptr block_end,
                    line_list_ptr source,  line_list_ptr dest, long rline,
                    long br,  long er, int bc, int ec, int rcol, int *rc )
{
line_list_ptr temp_ll;          /* temporary list pointer */
text_ptr l;
int  lens;                      /* length of source line */
int  lend;                      /* length of destination line */
long li;                        /* temporary line variables */
long diff;
WINDOW s_w, d_w;                /* a couple of temporary WINDOWs */

   dup_window_info( &s_w, source_window );
   dup_window_info( &d_w, window );
   s_w.rline   = br;
   s_w.ll      = block_start;
   s_w.visible = FALSE;
   d_w.rline   = rline;
   d_w.ll      = dest;
   d_w.visible = FALSE;

   /*
    * pad the start of the STREAM block if needed.
    */
   lens = block_start->len;
   detab_a_line( block_start->line, &lens );
   if (lens < bc || mode.inflate_tabs)
      *rc = prepare_block( &s_w, block_start, bc );

   /*
    * pad the end of the STREAM block if needed.
    */
   lens = block_end->len;
   detab_a_line( block_end->line, &lens );
   if (*rc == OK  &&  (lens < ec+1  ||  mode.inflate_tabs))
      *rc = prepare_block( &s_w, block_end, ec+1 );

   /*
    * pad the destination line if necessary
    */
   copy_line( dest );
   detab_linebuff( );
   *rc = un_copy_line( dest, &d_w, FALSE );
   lend = dest->len;
   if (*rc == OK && (action==MOVE || action==COPY || action==KOPY)) {
      if (lend < rcol || mode.inflate_tabs)
         *rc = prepare_block( &d_w, dest, rcol );
   }

   if ((action == COPY || action == KOPY) && *rc == OK) {

      /*
       * concatenate the end of the STREAM block with the end of the
       *   destination line.
       */
      lens = dest->len - rcol;

      assert( lens >= 0 );
      assert( lens <= MAX_LINE_LENGTH );
      assert( ec + 1 >= 0 );
      assert( ec + 1 <= MAX_LINE_LENGTH );
      assert( rcol >= 0 );

      _fmemcpy( g_status.line_buff, block_end->line, ec+1 );
      _fmemcpy( g_status.line_buff+ec+1, dest->line+rcol, lens );
      lens += ec + 1;
      g_status.line_buff_len = lens;

      temp_ll = (line_list_ptr)my_malloc( sizeof(line_list_struc), rc );
      if (*rc == OK) {
         temp_ll->line  = NULL;
         temp_ll->len   = 0;
         temp_ll->dirty = FALSE;
         g_status.copied = TRUE;
         *rc = un_copy_line( temp_ll, &d_w, TRUE );

         if (*rc == OK) {
            dest->next->prev = temp_ll;
            temp_ll->next = dest->next;
            dest->next = temp_ll;
            temp_ll->prev = dest;
         } else
            if (temp_ll != NULL)
               my_free( temp_ll );
      } else {
         if (temp_ll != NULL)
            my_free( temp_ll );
      }

      /*
       * file too big
       */
      if (*rc != OK)
         error( WARNING, window->bottom_line, dir3 );

      if (*rc == OK) {
         g_status.copied = FALSE;
         copy_line( dest );
         lens = block_start->len - bc;

         assert( lens >= 0 );
         assert( lens <= MAX_LINE_LENGTH );
         assert( bc >= 0 );
         assert( bc <= MAX_LINE_LENGTH );
         assert( rcol >= 0 );

         _fmemcpy( g_status.line_buff+rcol, block_start->line+bc, lens );
         lens = rcol + lens;
         g_status.line_buff_len = lens;
         *rc = un_copy_line( dest, &d_w, TRUE );
      }

      source = block_start->next;
      for (li=br+1; li < er  &&  *rc == OK; li++) {
         lens = source->len;
         temp_ll = (line_list_ptr)my_malloc( sizeof(line_list_struc), rc );

         assert( lens >= 0 );
         assert( lens <= MAX_LINE_LENGTH );

         l = (text_ptr)my_malloc( lens * sizeof(char), rc );
         if (*rc == OK) {
            if (lens > 0)
               _fmemcpy( l, source->line, lens );
            temp_ll->line  = l;
            temp_ll->len   = lens;
            temp_ll->dirty = TRUE;

            if (dest->next != NULL) {
               dest->next->prev = temp_ll;
               temp_ll->next = dest->next;
               dest->next = temp_ll;
               temp_ll->prev = dest;
            } else {
               temp_ll->next = dest;
               if (dest->prev != NULL)
                  dest->prev->next = temp_ll;
               temp_ll->prev = dest->prev;
               dest->prev = temp_ll;
               if (temp_ll->prev == NULL)
                  window->file_info->line_list = temp_ll;
            }

            dest = temp_ll;
            source = source->next;
         } else {
            /*
             * file too big
             */
            error( WARNING, window->bottom_line, dir3 );
            if (l != NULL)
               my_free( l );
            if (temp_ll != NULL)
               my_free( temp_ll );
            *rc = WARNING;
         }
      }
   } else if (action == MOVE) {

      /*
       * is the dest on the same line as the block_start?
       */
      if (ptoul( dest ) == ptoul( block_start )) {

         /*
          * move the text between rcol and bc in block_start->line
          *   to block_end->line + ec.
          */
         lens = bc - rcol;
         lend = block_end->len - (ec + 1);
         g_status.copied = FALSE;
         copy_line( block_end );


         assert( lens >= 0 );
         assert( lens <= MAX_LINE_LENGTH );
         assert( lend >= 0 );
         assert( lend <= MAX_LINE_LENGTH );
         assert( ec + lens + 1 <= MAX_LINE_LENGTH );
         assert( rcol >= 0 );


         _fmemmove( g_status.line_buff + ec + lens + 1,
                    g_status.line_buff + ec + 1,  lend );
         _fmemcpy( g_status.line_buff+ec+1, block_start->line+rcol, lens );
         g_status.line_buff_len = block_end->len + lens;
         *rc = un_copy_line( block_end, &d_w, TRUE );

         /*
          * now, remove the text between rcol and bc on block_start->line
          */
         if (*rc == OK) {
            lend = block_start->len - bc;
            copy_line( block_start );

            assert( lend >= 0 );
            assert( lend < MAX_LINE_LENGTH );

            _fmemmove( g_status.line_buff + rcol,
                       g_status.line_buff + bc, lend );

            assert( block_start->len - (bc - rcol) >= 0 );
            assert( block_start->len - (bc - rcol) <= MAX_LINE_LENGTH );

            g_status.line_buff_len = block_start->len - (bc - rcol);
            *rc = un_copy_line( block_start, &d_w, TRUE );
         }

      /*
       * is the dest on the same line as the block_end?
       */
      } else if (ptoul( dest ) == ptoul( block_end )) {

         /*
          * move the text between rcol and ec on block_end->line to
          *   block_start->line + bc.
          */
         lens = rcol - ec;
         lend = block_start->len - bc;
         g_status.copied = FALSE;
         copy_line( block_start );

         assert( lens >= 0 );
         assert( lens <= MAX_LINE_LENGTH );
         assert( lend >= 0 );
         assert( lend <= MAX_LINE_LENGTH );
         assert( bc + lens <= MAX_LINE_LENGTH );
         assert( ec + 1 >= 0 );

         _fmemmove( g_status.line_buff + bc + lens,
                    g_status.line_buff + bc,  lend );
         _fmemcpy( g_status.line_buff+bc, block_end->line+ec+1, lens );

         assert( block_start->len + lens >= 0 );
         assert( block_start->len + lens <= MAX_LINE_LENGTH );

         g_status.line_buff_len = block_start->len + lens;
         *rc = un_copy_line( block_start, &d_w, TRUE );

         /*
          * now, remove the text on block_end->line between rcol and ec
          */
         if (*rc == OK) {
            lend = block_end->len - (rcol + 1);
            copy_line( block_end );

            assert( lend >= 0 );
            assert( lend <= MAX_LINE_LENGTH );
            assert( ec + 1 >= 0 );
            assert( rcol + 1 >= 0 );
            assert( ec + 1 <= MAX_LINE_LENGTH );
            assert( rcol + 1 <= MAX_LINE_LENGTH );
            assert( block_end->len - (rcol - ec) >= 0 );
            assert( block_end->len - (rcol - ec) <= MAX_LINE_LENGTH );


            _fmemmove( g_status.line_buff + ec + 1,
                       g_status.line_buff + rcol + 1, lend );
            g_status.line_buff_len = block_end->len - (rcol - ec);
            *rc = un_copy_line( block_end, &d_w, TRUE );
         }
      } else {

         lens = dest->len - rcol;

         assert( ec + 1 >= 0 );
         assert( ec + 1 <= MAX_LINE_LENGTH );
         assert( lens >= 0 );
         assert( lens <= MAX_LINE_LENGTH );
         assert( rcol >= 0 );
         assert( rcol <= MAX_LINE_LENGTH );

         _fmemcpy( g_status.line_buff, block_end->line, ec+1 );
         _fmemcpy( g_status.line_buff+ec+1, dest->line+rcol, lens );
         lens += ec + 1;
         g_status.line_buff_len = lens;

         temp_ll = (line_list_ptr)my_malloc( sizeof(line_list_struc), rc );
         if (*rc == OK) {
            temp_ll->line  = NULL;
            temp_ll->len   = 0;
            temp_ll->dirty = FALSE;
            g_status.copied = TRUE;
            *rc = un_copy_line( temp_ll, &d_w, TRUE );

            if (*rc != ERROR) {
               dest->next->prev = temp_ll;
               temp_ll->next = dest->next;
               dest->next = temp_ll;
               temp_ll->prev = dest;
            } else
               if (temp_ll != NULL)
                  my_free( temp_ll );
         } else {
            if (temp_ll != NULL)
               my_free( temp_ll );
         }

         /*
          * file too big
          */
         if (*rc != OK)
            error( WARNING, window->bottom_line, dir3 );

         if (*rc == OK) {
            copy_line( dest );
            lens = block_start->len - bc;

            assert( bc >= 0 );
            assert( bc <= MAX_LINE_LENGTH );
            assert( lens >= 0 );
            assert( lens <= MAX_LINE_LENGTH );
            assert( rcol >= 0 );
            assert( rcol <= MAX_LINE_LENGTH );

            _fmemcpy( g_status.line_buff+rcol, block_start->line+bc, lens );
            g_status.line_buff_len = lens + rcol;
            *rc = un_copy_line( dest, &d_w, TRUE );
            dest->dirty = TRUE;
         }

         if (*rc == OK  &&  ptoul( block_start->next ) != ptoul( block_end )) {
            block_start->next->prev = dest;
            temp_ll->prev = block_end->prev;
            block_end->prev->next = temp_ll;
            dest->next = block_start->next;
         }

         if (*rc == OK) {
            copy_line( block_start );
            detab_linebuff( );
            lend = bc;
            lens = block_end->len - (ec + 1);

            assert( bc >= 0 );
            assert( bc <= MAX_LINE_LENGTH );
            assert( lens >= 0 );
            assert( lens <= MAX_LINE_LENGTH );
            assert( lend >= 0 );
            assert( lend <= MAX_LINE_LENGTH );
            assert( ec + 1 >= 0 );
            assert( ec + 1 <= MAX_LINE_LENGTH );
            assert( lens + lend >= 0 );
            assert( lens + lend <= MAX_LINE_LENGTH );

            _fmemcpy( g_status.line_buff+bc, block_end->line+ec+1, lens );
            g_status.line_buff_len = lend + lens;
            *rc = un_copy_line( block_start, &s_w, TRUE );
            block_start->dirty = TRUE;
            block_start->next = block_end->next;
            block_end->next->prev = block_start;
            if (block_end->line != NULL)
               my_free( block_end->line );
            my_free( block_end );
         }
      }
   } else if (action == DELETE) {
      copy_line( block_start );
      lens = block_end->len - (ec + 1);

      assert( bc >= 0 );
      assert( bc <= MAX_LINE_LENGTH );
      assert( lens >= 0 );
      assert( lens <= MAX_LINE_LENGTH );
      assert( ec + 1 >= 0 );
      assert( ec + 1 <= MAX_LINE_LENGTH );
      assert( bc + lens >= 0 );
      assert( bc + lens <= MAX_LINE_LENGTH );

      _fmemcpy( g_status.line_buff+bc, block_end->line + ec+1, lens );
      g_status.line_buff_len = bc + lens;
      *rc = un_copy_line( block_start, &s_w, TRUE );
      block_start->dirty = TRUE;
      source = block_start->next;
      block_start->next = block_end->next;
      block_end->next->prev = block_start;
      block_end->next = NULL;
      while (source != NULL) {
         temp_ll = source;
         source = source->next;
         if (temp_ll->line != NULL)
            my_free( temp_ll->line );
         my_free( temp_ll );
      }
   }

   if (*rc == OK) {
      diff = er - br;
      if (action == COPY || action == KOPY || action == MOVE)
         dest_file->length += diff;
      if (action == DELETE || action == MOVE)
         source_file->length -= diff;
      if (action == DELETE && source_window->rline >= br) {
         source_window->rline -= diff;
         if (source_window->rline < br)
            source_window->rline = br;
      }
   }

   /*
    * restore all cursors in all windows
    */
   restore_cursors( dest_file );
   if (dest_file != source_file)
      restore_cursors( source_file );
   show_avail_mem( );
}


/*
 * Name:    do_box_block
 * Purpose: delete, move, copy, or kopy a BOX block
 * Date:    June 5, 1991
 * Passed:  window:         pointer to destination window (current window)
 *          source_window:  pointer to source window
 *          action:         block action  --  OVERLAY, FILL, etc...
 *          source_file:    pointer to source file structure
 *          dest_file:      pointer to destination file
 *          source:         pointer to source node
 *          dest:           pointer to destination node
 *          br:             beginning line number in marked block
 *          er:             ending line number in marked block
 *          block_inc:      increment used to number a block
 *          rline:          current line number in destination file
 *          block_num:      starting number when numbering a block
 *          block_just:     LEFT or RIGHT justified numbers in block
 *          fill_char:      character to fill a block
 *          same:           are source and destination files same? T or F
 *          block_len:      width of box block
 *          bc:             beginning column of block
 *          ec:             ending column of block
 *          rcol:           current column of cursor
 *          rc:             return code
 */
void do_box_block( WINDOW *window,  WINDOW *source_window,  int action,
                    file_infos *source_file,  file_infos *dest_file,
                    line_list_ptr source,  line_list_ptr dest, long br,
                    long er, long block_inc,
                    long rline, long block_num, int block_just, int fill_char,
                    int same, int block_len, int bc, int ec, int rcol, int *rc )
{
line_list_ptr p;                /* temporary list pointer */
int  lens;                      /* length of source line */
int  lend;                      /* length of destination line */
int  add;                       /* characters being added from another line */
char *block_buff;
char *swap_buff;
int  xbc, xec;                  /* temporary column variables */
long li;                        /* temporary line variables */
long dest_add;                  /* number of bytes added to destination file */
WINDOW s_w, d_w;       /* a couple of temporary WINDOWs for BOX stuff */
int  padded_file;
WINDOW *w;

   padded_file = FALSE;
   dup_window_info( &s_w, source_window );
   dup_window_info( &d_w, window );
   s_w.rline   = br;
   s_w.ll      = source;
   s_w.visible = FALSE;
   d_w.rline   = rline;
   d_w.ll      = dest;
   d_w.visible = FALSE;

   block_buff = (char *)calloc( BUFF_SIZE + 2, sizeof(char) );
   swap_buff  = (char *)calloc( BUFF_SIZE + 2, sizeof(char) );
   if (block_buff == NULL || swap_buff == NULL) {
      error( WARNING, window->bottom_line, block4 );
      *rc = ERROR;
   }

   /*
    * special case for block actions.  since block actions always
    *   move forward thru the file, overlapping text in an OVERLAY
    *   action don't do right.  make the operation start at the end
    *   of the block and work backwards.
    */
   if (*rc == OK  &&  (action == OVERLAY || action == SWAP) &&
           same  &&  rline > br  &&  rline <= er) {

      /*
       * see if we need to add padd lines at eof.
       */
      dest_add = rline - br;
      if (dest_add + er > window->file_info->length) {
         dest_add = dest_add - (window->file_info->length - er);
         p = dest_file->line_list_end->prev;
         for (; dest_add > 0  &&  *rc == OK; dest_add--)
            *rc = pad_dest_line( window, dest_file, p );
         padded_file = TRUE;
      }

      /*
       * move source and dest pointers to the end of the OVERLAY
       */
      for (li=er-br; li > 0; li--) {
         load_undo_buffer( dest_file, dest->line, dest->len );
         dest = dest->next;
         ++d_w.rline;
         source = source->next;
         ++s_w.rline;
      }

      /*
       * work backwards so the overlapped OVERLAY block don't use
       * overlayed text to fill the block.  same for SWAPPing blocks.
       */
      for (li=er; *rc == OK  &&  li >= br  &&  !g_status.control_break;
                                       li--, s_w.rline--, d_w.rline--) {
         lens = find_end( source->line, source->len );
         lend = find_end( dest->line, dest->len );
         if (lens != 0 || lend != 0) {
            load_box_buff( block_buff, source, bc, ec, ' ' );
            if (action == SWAP)
               load_box_buff( swap_buff, dest, rcol, rcol+block_len, ' ' );
            *rc = copy_buff_2file( &d_w, block_buff, dest, rcol,
                                    block_len, action );
            dest->dirty = TRUE;
            if (action == SWAP) {
               add = 0;
               *rc = copy_buff_2file( &s_w, swap_buff, source, bc,
                                block_len, action );
               source->dirty = TRUE;
            }
         }
         source = source->prev;
         dest = dest->prev;
      }
   } else {
      if (action == FILL)
         block_fill( block_buff, fill_char, block_len );
      for (li=br; *rc == OK  &&  li <= er  &&  !g_status.control_break;
                           li++, s_w.rline++, d_w.rline++) {
         lens = find_end( source->line, source->len );
         lend = find_end( dest->line, dest->len );

         switch (action) {
            case FILL    :
            case NUMBER  :
            case DELETE  :
            case MOVE    :
               load_undo_buffer( source_file, source->line, source->len );
               break;
            case COPY    :
            case KOPY    :
            case OVERLAY :
               load_undo_buffer( dest_file, dest->line, dest->len );
               break;
         }

         /*
          * with FILL and NUMBER operations, we're just adding chars
          *   to the file at the source location.  we don't have to
          *   worry about bookkeeping.
          */
         if (action == FILL || action == NUMBER) {
            if (action == NUMBER) {
              number_block_buff( block_buff, block_len, block_num, block_just );
              block_num += block_inc;
            }
            *rc = copy_buff_2file( &s_w, block_buff, source, rcol,
                                block_len, action );
            source->dirty = TRUE;

         /*
          * if we are doing a BOX action and both the source and
          * destination are 0 then we have nothing to do.
          */
         } else if (lens != 0 || lend != 0) {

            /*
             * do actions that may require adding to file
             */
            if (action == MOVE     ||  action == COPY || action == KOPY ||
                action == OVERLAY  ||  action == SWAP) {
               xbc = bc;
               xec = ec;
               if (action != OVERLAY  &&  action != SWAP  &&  same) {
                  if (rcol < bc && rline > br && rline <=er)
                     if (li >= rline) {
                        xbc = bc + block_len;
                        xec = ec + block_len;
                     }
               }
               load_box_buff( block_buff, source, xbc, xec, ' ' );
               if (action == SWAP)
                  load_box_buff( swap_buff, dest, rcol, rcol+block_len, ' ' );
               *rc = copy_buff_2file( &d_w, block_buff, dest, rcol,
                                block_len, action );
               dest->dirty = TRUE;
               if (action == SWAP && *rc == OK) {
                  *rc = copy_buff_2file( &s_w, swap_buff, source, xbc,
                                   block_len, action );
                  source->dirty = TRUE;
               }
            }

            /*
             * do actions that may require deleting from file
             */
            if (action == MOVE || action == DELETE) {
               lens = find_end( source->line, source->len );
               if (lens >= (bc + 1)) {
                  source->dirty = TRUE;
                  add = block_len;
                  xbc = bc;
                  if (lens <= (ec + 1))
                     add = lens - bc;
                  if (same && action == MOVE) {
                     if (rcol < bc && rline >= br && rline <=er)
                        if (li >= rline) {
                           xbc = bc + block_len;
                           if (lens <= (ec + block_len + 1))
                              add = lens - xbc;
                        }
                  }
                  if (add > 0)
                     *rc = delete_box_block( &s_w, source, xbc, add );
               }
            }
         }

         /*
          * if we are doing any BOX action we need to move the source pointer
          * to the next line.
          */
         source = source->next;

         /*
          * if we are doing any action other than DELETE, we need to move
          * the destination to the next line in marked block.
          * In BOX mode, we may need to pad the end of the file
          * with a blank line before we process the next line.
          */
         if (action != DELETE && action != FILL && action != NUMBER) {
            p = dest->next;
            if (p->len != EOF)
               dest = p;
            else if (li < er) {
               padded_file = TRUE;
               pad_dest_line( window, dest_file, p );
               dest = dest->next;
            }
         }
      }
   }
   if (block_buff != NULL)
      free( block_buff );
   if (swap_buff != NULL)
      free( swap_buff );
   if (padded_file) {
      w = g_status.window_list;
      while (w != NULL) {
         if (w->file_info == dest_file  &&  w->visible)
            show_size( w );
         w = w->next;
      }
   }
   show_avail_mem( );
}


/*
 * Name:    load_box_buff
 * Class:   helper function
 * Purpose: copy the contents of a BOX to a block buffer.
 * Date:    June 5, 1991
 * Passed:  block_buff: local buffer for block moves
 *          ll:         node to source line in file to load
 *          bc:     beginning column of BOX. used only in BOX operations.
 *          ec:     ending column of BOX. used only in BOX operations.
 *          filler: character to fill boxes that end past eol
 * Notes:   For BOX blocks, there are several things to take care of:
 *            1) The BOX begins and ends within a line - just copy the blocked
 *            characters to the block buff.  2) the BOX begins within a line
 *            but ends past the eol - copy all the characters within the line
 *            to the block buff then fill with padding.  3) the BOX begins and
 *            ends past eol - fill entire block buff with padding (filler).
 *          the fill character varies with the block operation.  for sorting
 *            a box block, the fill character is '\0'.  for adding text to
 *            the file, the fill character is a space.
 */
void load_box_buff( char *block_buff, line_list_ptr ll, int bc, int ec,
                    char filler )
{
int len;
int avlen;
register int i;
register char *bb;
text_ptr s;

   assert( bc >= 0 );
   assert( ec >= bc );
   assert( ec < MAX_LINE_LENGTH );

   bb = block_buff;
   len = ll->len;
   s = detab_a_line( ll->line, &len );
   /*
    * block start may be past eol
    */
   if (len < ec + 1) {
      /*
       * does block start past eol? - fill with pad
       */
      assert( ec + 1 - bc >= 0 );

      memset( block_buff, filler, (ec + 1) - bc );
      if (len >= bc) {
         /*
          * block ends past eol - fill with pad
          */
         avlen = len - bc;
         s += bc;
         for (i=avlen; i>0; i--)
            *bb++ = *s++;
      }
   } else {
      /*
       * block is within line - copy block to buffer
       */
      avlen = (ec + 1) - bc;
      s = s + bc;
      for (i=avlen; i>0; i--)
         *bb++ = *s++;
   }
}


/*
 * Name:    copy_buff_2file
 * Class:   helper function
 * Purpose: copy the contents of block buffer to destination file
 * Date:    June 5, 1991
 * Passed:  window:     pointer to current window
 *          block_buff: local buffer for moves
 *          dest:       pointer to destination line in destination file
 *          rcol:       if in BOX mode, destination column in destination file
 *          block_len:  if in BOX mode, width of block to copy
 *          action:     type of block action
 * Notes:   In BOX mode, the destination line has already been prepared.
 *          Just copy the BOX buffer to the destination line.
 */
int  copy_buff_2file( WINDOW *window, char *block_buff, line_list_ptr dest,
                      int rcol, int block_len, int action )
{
char *s;
char *d;
int len;
int pad;
int add;

   copy_line( dest );
   if (mode.inflate_tabs)
      detab_linebuff( );

   len = g_status.line_buff_len;

   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );
   assert( rcol >= 0 );
   assert( rcol < MAX_LINE_LENGTH );
   assert( block_len >= 0 );
   assert( block_len < BUFF_SIZE );

   if (rcol > len) {
      pad = rcol - len;

      assert( pad >= 0 );
      assert( pad < MAX_LINE_LENGTH );

      memset( g_status.line_buff + len, ' ', pad );
      len += pad;
   }

   s = g_status.line_buff + rcol;

   /*
    * s is pointing to location to perform BOX operation.  If we do a
    * FILL or OVERLAY, we do not necessarily add any extra space.  If the
    * line does not extend all the thru the BOX then we add.
    * we always add space when we COPY, KOPY, or MOVE
    */
   if (action == FILL || action == OVERLAY || action == NUMBER || action == SWAP) {
      add = len - rcol;
      if (add < block_len) {
         pad = block_len - add;

         assert( pad >= 0 );
         assert( pad < MAX_LINE_LENGTH );

         memset( g_status.line_buff + len, ' ', pad );
         len += pad;
      }
   } else {
      d = s + block_len;
      add = len - rcol;

      assert( add >= 0 );
      assert( add < MAX_LINE_LENGTH );

      memmove( d, s, add );
      len += block_len;
   }

   assert( rcol + block_len <= len );
   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );

   memmove( s, block_buff, block_len );
   g_status.line_buff_len = len;
   if (mode.inflate_tabs)
      entab_linebuff( );
   return( un_copy_line( dest, window, TRUE ) );
}


/*
 * Name:    block_fill
 * Class:   helper function
 * Purpose: fill the block buffer with character
 * Date:    June 5, 1991
 * Passed:  block_buff: local buffer for moves
 *          fill_char:  fill character
 *          block_len:  number of columns in block
 * Notes:   Fill block_buffer for block_len characters using fill_char.  This
 *          function is used only for BOX blocks.
 */
void block_fill( char *block_buff, int fill_char, int block_len )
{
   assert( block_len >= 0 );
   assert( block_len < BUFF_SIZE );
   assert( block_buff != NULL );

   memset( block_buff, fill_char, block_len );
}


/*
 * Name:    number_block_buff
 * Class:   helper function
 * Purpose: put a number into the block buffer
 * Date:    June 5, 1991
 * Passed:  block_buff: local buffer for moves
 *          block_len:  number of columns in block
 *          block_num:  long number to fill block
 *          just:       LEFT or RIGHT justified?
 * Notes:   Fill block_buffer for block_len characters with number.
 *          This function is used only for BOX blocks.
 */
void number_block_buff( char *block_buff, int block_len, long block_num,
                        int just )
{
int len;                /* length of number buffer */
int i;
char temp[MAX_COLS];    /* buffer for long number to ascii conversion  */

   assert( block_len >= 0 );
   assert( block_len < BUFF_SIZE );

   block_fill( block_buff, ' ', block_len );
   len = strlen( ltoa( block_num, temp, 10 ) );
   if (just == RIGHT) {
      block_len--;
      len--;
      for (;block_len >= 0 && len >= 0; block_len--, len--)
         block_buff[block_len] = temp[len];
   } else {
      for (i=0; block_len > 0 && i < len; block_len--, i++)
         block_buff[i] = temp[i];
   }
}


/*
 * Name:    restore_cursors
 * Class:   helper function
 * Purpose: a file has been modified - must restore all cursor pointers
 * Date:    June 5, 1991
 * Passed:  file:  pointer to file with changes
 * Notes:   Go through the window list and adjust the cursor pointers
 *          as needed.
 */
void restore_cursors( file_infos *file )
{
register WINDOW *window;
line_list_ptr ll;
long n;

   assert( file != NULL );

   window = g_status.window_list;
   while (window != NULL) {
      if (window->file_info == file) {
         window->bin_offset = 0;
         if (window->rline < 1L)
            window->rline = 1L;
         if (window->rline > file->length)
            window->rline = file->length;
         ll = file->line_list;
         n = 1L;
         for (; n < window->rline; n++) {
            window->bin_offset += ll->len;
            ll = ll->next;
         }
         window->ll = ll;
         if (window->rline < (window->cline - (window->top_line+window->ruler-1)))
            window->cline = (int)window->rline + window->top_line+window->ruler-1;
         if (window->cline < window->top_line + window->ruler)
            window->cline = window->top_line + window->ruler;
         if (window->visible)
            show_size( window );
      }
      window = window->next;
   }
}


/*
 * Name:    delete_box_block
 * Class:   helper function
 * Purpose: delete the marked text
 * Date:    June 5, 1991
 * Passed:  s_w:    source window
 *          source: pointer to line with block to delete
 *          bc:     beginning column of block - BOX mode only
 *          add:    number of characters in block to delete
 * Notes:   Used only for BOX blocks.  Delete the block.
 */
int  delete_box_block( WINDOW *s_w, line_list_ptr source, int bc, int add )
{
char *s;
int number;

   assert( s_w != NULL );
   assert( source != NULL );
   assert( bc >= 0 );
   assert( bc < MAX_LINE_LENGTH );
   assert( add >= 0 );
   assert( add < MAX_LINE_LENGTH );

   copy_line( source );
   detab_linebuff( );
   number = g_status.line_buff_len - bc;
   s = g_status.line_buff + bc + add;

   assert( number >= 0 );
   assert( number < MAX_LINE_LENGTH );
   assert( bc + add >= 0 );
   assert( bc + add < MAX_LINE_LENGTH );
   assert( add <= g_status.line_buff_len );

   memmove( s - add, s, number );
   g_status.line_buff_len -= add;
   entab_linebuff( );
   return( un_copy_line( source, s_w, TRUE ) );
}


/*
 * Name:    check_block
 * Class:   helper function
 * Purpose: To check that the block is still valid.
 * Date:    June 5, 1991
 * Notes:   After some editing, the marked block may not be valid.  For example,
 *          deleting all the lines in a block in another window.  We don't
 *          need to keep up with the block text pointers while doing normal
 *          editing; however, we need to refresh them before doing block stuff.
 */
void check_block( void )
{
register file_infos *file;
WINDOW filler;

   file = g_status.marked_file;
   if (file == NULL || file->block_br > file->length)
      unmark_block( &filler );
   else {
      if (file->block_er > file->length)
         file->block_er = file->length;
      find_begblock( file );
      find_endblock( file );
   }
}


/*
 * Name:    find_begblock
 * Class:   helper editor function
 * Purpose: find the beginning line in file with marked block
 * Date:    June 5, 1991
 * Passed:  file: file containing marked block
 * Notes:   file->block_start contains starting line of marked block.
 */
void find_begblock( file_infos *file )
{
line_list_ptr ll;
long li;           /* line counter (long i) */

   assert( file != NULL );
   assert( file->line_list != NULL );

   ll = file->line_list;
   for (li=1; li<file->block_br && ll->next != NULL; li++)
      ll = ll->next;

   file->block_start = ll;
}


/*
 * Name:    find_endblock
 * Class:   helper function
 * Purpose: find the ending line in file with marked block
 * Date:    June 5, 1991
 * Passed:  file: file containing marked block
 * Notes:   If in LINE mode, file->block_end is set to end of line of last
 *          line in block.  If in BOX mode, file->block_end is set to
 *          beginning of last line in marked block.  If the search for the
 *          ending line of the marked block goes past the eof, set the
 *          ending line of the block to the last line in the file.
 */
void find_endblock( file_infos *file )
{
line_list_ptr ll; /* start from beginning of file and go to end */
long i;           /* line counter */
register file_infos *fp;

   assert( file != NULL );
   assert( file->block_start != NULL );

   fp = file;
   ll = fp->block_start;
   if (ll != NULL) {
      for (i=fp->block_br;  i < fp->block_er && ll->next != NULL; i++)
         ll = ll->next;
      if (ll != NULL)
         fp->block_end = ll;
      else {

         /*
          * last line in marked block is NULL.  if LINE block, set end to
          * last character in the file.  if STREAM or BOX block, set end to
          * start of last line in file.  ending row, or er, is then set to
          * file length.
          */
         fp->block_end = fp->line_list_end->prev;
         fp->block_er = fp->length;
      }
   }
}


/*
 * Name:    block_write
 * Class:   primary editor function
 * Purpose: To write the currently marked block to a disk file.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   If the file already exists, the user gets to choose whether
 *           to overwrite or append.
 */
int  block_write( WINDOW *window )
{
int prompt_line;
int rc;
char buff[MAX_COLS+2]; /* buffer for char and attribute  */
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
file_infos *file;
int block_type;
int fattr;

   /*
    * make sure block is marked OK
    */
   entab_linebuff( );
   rc = un_copy_line( window->ll, window, TRUE );
   check_block( );
   if (rc == OK  &&  g_status.marked == TRUE) {
      prompt_line = window->bottom_line;
      file        = g_status.marked_file;

      assert( file != NULL );

      block_type  = file->block_type;

      /*
       * find out which file to write to
       */
      save_screen_line( 0, prompt_line, line_buff );
      *g_status.rw_name = '\0';
      if (get_name( block6, prompt_line, g_status.rw_name,
                    g_display.message_color ) == OK) {
         /*
          * if the file exists, find out whether to overwrite or append
          */
         rc = get_fattr( g_status.rw_name, &fattr );
         if (rc == OK) {
            /*
             * file exists. overwrite or append?
             */
            set_prompt( block7, prompt_line );
            switch (get_oa( )) {
               case A_OVERWRITE :
                  change_mode( g_status.rw_name, prompt_line );
                  /*
                   * writing block to
                   */
                  combine_strings( buff, block8, g_status.rw_name, "'" );
                  s_output( buff, prompt_line, 0, g_display.message_color );
                  rc = hw_save( g_status.rw_name, file, file->block_br,
                                file->block_er, block_type );
                  if (rc == ERROR)
                     /*
                      * could not write block
                      */
                     error( WARNING, prompt_line, block9 );
                  break;
               case A_APPEND :
                  /*
                   * appending block to
                   */
                  combine_strings( buff, block10, g_status.rw_name, "'" );
                  s_output( buff, prompt_line, 0, g_display.message_color );
                  rc = hw_append( g_status.rw_name, file, file->block_br,
                                  file->block_er, block_type );
                  if (rc == ERROR)
                     /*
                      * could not append block
                      */
                     error( WARNING, prompt_line, block11 );
                  break;
               case AbortCommand :
               default :
                  rc = ERROR;
                  break;
            }
         } else if (rc != ERROR) {
            /*
             * writing block to
             */
            combine_strings( buff, block12, g_status.rw_name, "'" );
            s_output( buff, prompt_line, 0, g_display.message_color );
            if (hw_save( g_status.rw_name, file, file->block_br, file->block_er,
                         block_type ) == ERROR) {
               /*
                * could not write block
                */
               error( WARNING, prompt_line, block9 );
               rc = ERROR;
            }
         }
      }
      restore_screen_line( 0, prompt_line, line_buff );
   } else
      rc = ERROR;
   return( rc );
}


/*
 * Name:    block_print
 * Class:   primary editor function
 * Purpose: Print an entire file or the currently marked block.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   With the added Critical Error Handler routine, let's fflush
 *          the print buffer first.
 */
int  block_print( WINDOW *window )
{
char answer[MAX_COLS];          /* entire file or just marked block? */
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
int  col;
int  func;
int  prompt_line;
line_list_ptr block_start;   /* start of block in file */
file_infos *file;
int  block_type;
char *p;
int  len;
int  bc;
int  ec;
int  last_c;
long lbegin;
long lend;
long l;
int  color;
int  rc;

   color = g_display.message_color;
   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   prompt_line = window->bottom_line;
   save_screen_line( 0, prompt_line, line_buff );
   /*
    * print entire file or just marked block?
    */

   assert( strlen( block13 ) < MAX_COLS );

   strcpy( answer, block13 );
   col = strlen( answer );
   s_output( answer, prompt_line, 0, color );
   eol_clear( col, prompt_line, g_display.text_color );
   xygoto( col, prompt_line );
   func = col = 0;
   while (col != 'f' && col != 'F' && col != 'b' && col != 'B' &&
          func != AbortCommand) {
      col = getkey( );
      func = getfunc( col );
      if (col == ESC  ||  func == AbortCommand)
         rc = ERROR;
   }

   if (rc == OK) {
      /*
       * if everything is everything, flush the printer before we start
       *   printing.  then, check the critical error flag after the flush.
       */
      fflush( stdprn );
      if (ceh.flag == ERROR)
         rc = ERROR;
   }

   if (rc != ERROR) {
      file = window->file_info;
      block_type  = NOTMARKED;
      if (col == 'f' || col == 'F') {
         block_start = file->line_list;
         lend =   l  = file->length;
      } else {
         check_block( );
         if (g_status.marked == TRUE) {
            file        = g_status.marked_file;
            block_start = file->block_start;
            block_type  = file->block_type;
            lend =   l  = file->block_er + 1l - file->block_br;
         } else
            rc = ERROR;
      }

      if (rc != ERROR) {
         eol_clear( 0, prompt_line, color );
         /*
          * printing line   of    press control-break to cancel.
          */
         s_output( block14, prompt_line, 0, color );
         ltoa( l, answer, 10 );
         s_output( answer, prompt_line, 25, color );
         xygoto( 14, prompt_line );
         if (block_type == BOX || block_type == STREAM) {
            bc = file->block_bc;
            ec = file->block_ec;
            last_c = ec + 1 - bc;
         }
         p = g_status.line_buff;
         lbegin = 1;
         for (col=OK; l>0 && col == OK && !g_status.control_break; l--) {
            ltoa( lbegin, answer, 10 );
            s_output( answer, prompt_line, 14, color );
            g_status.copied = FALSE;
            if (block_type == BOX) {
               load_box_buff( p, block_start, bc, ec, ' ' );
               len = last_c;
            } else if (block_type == STREAM && lbegin == 1) {
               len = block_start->len;
               detab_a_line( block_start->line, &len );
               if (bc > len)
                  len = 0;
               else {
                  if (lbegin == lend) {
                     load_box_buff( p, block_start, bc, ec, ' ' );
                     len = last_c;
                  } else {
                     len = len - bc;
                     g_status.copied = TRUE;

                     assert( len >= 0 );
                     assert( len < MAX_LINE_LENGTH );

                     _fmemcpy( p, block_start->line + bc, len );
                  }
               }
            } else if (block_type == STREAM && l == 1L) {
               copy_line( block_start );
               detab_linebuff( );
               len = g_status.line_buff_len;
               if (len > ec + 1)
                  len = ec + 1;
            } else {
               copy_line( block_start );
               len = g_status.line_buff_len;
            }

            assert( len >= 0 );
            assert( len < MAX_LINE_LENGTH );

            *(p+len) = '\r';
            ++len;
            *(p+len) = '\n';
            ++len;
            if (fwrite( p, sizeof( char ), len, stdprn ) < (unsigned)len ||
                ceh.flag == ERROR)
               col = ERROR;
            block_start = block_start->next;
            ++lbegin;
         }
         g_status.copied = FALSE;
         if (ceh.flag != ERROR)
            fflush( stdprn );
         else
            rc = ERROR;
      }
   }
   g_status.copied = FALSE;
   restore_screen_line( 0, prompt_line, line_buff );
   return( rc );
}


/*
 * Name:    get_block_fill_char
 * Class:   helper function
 * Purpose: get the character to fill marked block.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          c: address of character to fill block
 */
int  get_block_fill_char( WINDOW *window, int *c )
{
char answer[MAX_COLS];
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int col;
int prompt_line;
int rc;

   rc = OK;
   prompt_line = window->bottom_line;
   save_screen_line( 0, prompt_line, line_buff );
   /*
    * enter character to file block (esc to exit)
    */

   assert( strlen( block15 ) < MAX_COLS );

   strcpy( answer, block15 );
   s_output( answer, prompt_line, 0, g_display.message_color );
   col = strlen( answer );
   eol_clear( col, prompt_line, g_display.text_color );
   xygoto( col, prompt_line );
   col = getkey( );
   if (col >= 256)
      rc = ERROR;
   else
      *c = col;
   restore_screen_line( 0, prompt_line, line_buff );
   return( rc );
}


/*
 * Name:    get_block_numbers
 * Class:   helper function
 * Purpose: get the starting number and increment
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 *          block_num: address of number to start numbering
 *          block_inc: address of number to add to block_num
 *          just:      left or right justify numbers in block?
 */
int  get_block_numbers( WINDOW *window, long *block_num, long *block_inc,
                        int *just )
{
char answer[MAX_COLS];
int prompt_line;
register int rc;
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int col;

   prompt_line = window->bottom_line;

   /*
    * don't assume anything on starting number - start w/ null string.
    */
   answer[0] = '\0';
   /*
    * enter starting number
    */
   rc = get_name( block16, prompt_line, answer, g_display.message_color );
   if (answer[0] == '\0')
      rc = ERROR;
   if (rc != ERROR) {
      *block_num = atol( answer );

      /*
       * assume increment is 1
       */
      answer[0] = '1';
      answer[1] = '\0';
      /*
       * enter increment
       */
      rc = get_name( block17, prompt_line, answer, g_display.message_color );
      if (answer[0] == '\0')
         rc = ERROR;
      if (rc != ERROR) {
         *block_inc = atol( answer );

         /*
          * now, get left or right justification.  save contents of screen
          *  in a buffer, then write contents of buffer back to screen when
          *  we get through w/ justification.
          */
         save_screen_line( 0, prompt_line, line_buff );
         /*
          * left or right justify (l/r)?
          */

         assert( strlen( block18 ) < MAX_COLS );

         strcpy( answer, block18 );
         s_output( answer, prompt_line, 0, g_display.message_color );
         col = strlen( answer );
         eol_clear( col, prompt_line, g_display.text_color );
         xygoto( col, prompt_line );
         rc = get_lr( );
         if (rc != ERROR) {
            *just = rc;
            rc = OK;
         }
         restore_screen_line( 0, prompt_line, line_buff );
      }
   }

   /*
    * if everything is everything then return code = OK.
    */
   return( rc );
}


/*
 * Name:    block_trim_trailing
 * Class:   primary editor function
 * Purpose: Trim trailing space in a LINE block.
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Use copy_line and un_copy_line to do the work.
 */
int  block_trim_trailing( WINDOW *window )
{
int prompt_line;
int rc;
line_list_ptr p;             /* pointer to block line */
file_infos *file;
WINDOW *sw, s_w;
long er;
int  trailing;               /* save trailing setting */

   /*
    * make sure block is marked OK and that this is a LINE block
    */
   prompt_line = window->bottom_line;
   entab_linebuff( );
   rc = un_copy_line( window->ll, window, TRUE );
   check_block( );
   if (rc != ERROR && g_status.marked == TRUE) {

      trailing = mode.trailing;
      mode.trailing = TRUE;
      file = g_status.marked_file;
      if (file->block_type != LINE) {
         /*
          * can only trim trailing space in line blocks
          */
         error( WARNING, prompt_line, block21 );
         return( ERROR );
      }

      /*
       * initialize everything
       */
      sw = g_status.window_list;
      for (; ptoul( sw->file_info ) != ptoul( file );)
         sw = sw->next;
      if (mode.do_backups == TRUE) {
         file->modified = TRUE;
         rc = backup_file( sw );
      }
      dup_window_info( &s_w, sw );

      /*
       * set window to invisible so the un_copy_line function will
       * not display the lines while trimming.
       */
      s_w.visible = FALSE;

      p  = file->block_start;
      er = file->block_er;
      s_w.rline = file->block_br;
      for (; rc == OK && s_w.rline <= er  &&  !g_status.control_break; s_w.rline++) {

         /*
          * use the line buffer to trim space.
          */
         copy_line( p );
         rc = un_copy_line( p, &s_w, TRUE );
         p = p->next;
      }

      /*
       * IMPORTANT:  we need to reset the copied flag because the cursor may
       * not necessarily be on the last line of the block.
       */
      g_status.copied = FALSE;
      file->dirty = GLOBAL;
      mode.trailing = trailing;
      show_avail_mem( );
   }
   return( rc );
}


/*
 * Name:    block_email_reply
 * Class:   primary editor function
 * Purpose: insert the standard replay character '>' at beginning of line
 * Date:    June 5, 1992
 * Passed:  window:  pointer to current window
 * Notes:   it is customary to prepend "> " to the initial text and
 *             ">" to replies to replies to etc...
 */
int  block_email_reply( WINDOW *window )
{
int prompt_line;
int add;
int len;
int rc;
char *source;    /* source for block move to make room for c */
char *dest;      /* destination for block move */
line_list_ptr p;                     /* pointer to block line */
file_infos *file;
WINDOW *sw, s_w;
long er;

   /*
    * make sure block is marked OK and that this is a LINE block
    */
   prompt_line = window->bottom_line;
   entab_linebuff( );
   rc = un_copy_line( window->ll, window, TRUE );
   check_block( );
   if (rc != ERROR  &&  g_status.marked == TRUE) {
      file = g_status.marked_file;
      if (file->block_type != LINE) {
         /*
          * can only reply line blocks
          */
         error( WARNING, prompt_line, block25 );
         return( ERROR );
      }

      /*
       * find a window that points to the file with a marked block.
       */
      sw = g_status.window_list;
      for (; ptoul( sw->file_info ) != ptoul( file );)
         sw = sw->next;
      if (mode.do_backups == TRUE) {
         file->modified = TRUE;
         rc = backup_file( sw );
      }

      /*
       * use a local window structure to do the dirty work.  initialize
       *   the local window structure to the beginning of the marked
       *   block.
       */
      dup_window_info( &s_w, sw );

      /*
       * set s_w to invisible so the un_copy_line function will
       * not display the lines while doing block stuff.
       */
      s_w.visible = FALSE;
      s_w.rline = file->block_br;
      p  = file->block_start;
      er = file->block_er;

      /*
       * for each line in the marked block, prepend the reply character(s)
       */
      for (; rc == OK  &&  s_w.rline <= er  &&  !g_status.control_break;
                                                             s_w.rline++) {

         /*
          * put the line in the g_status.line_buff.  use add to count the
          *   number of characters to insert at the beginning of a line.
          *   the original reply uses "> ", while replies to replies use ">".
          */
         copy_line( p );
         if (*(p->line) == '>')
            add = 1;
         else
            add = 2;

         /*
          * see if the line has room to add the ">" character.  if there is
          *   room, move everything down to make room for the
          *   reply character(s).
          */
         len = g_status.line_buff_len;
         if (len + add < MAX_LINE_LENGTH) {
            source = g_status.line_buff;
            dest = source + add;

            assert( len >= 0 );
            assert( len < MAX_LINE_LENGTH );
            assert( len + add < MAX_LINE_LENGTH );

            memmove( dest, source, len );
            *source = '>';
            if (add > 1)
              *(source+1) = ' ';
            g_status.line_buff_len = len + add;
            rc = un_copy_line( p, &s_w, TRUE );
         }
         p = p->next;
         g_status.copied = FALSE;
      }

      /*
       * IMPORTANT:  we need to reset the copied flag because the cursor may
       * not necessarily be on the last line of the block.
       */
      g_status.copied = FALSE;
      file->dirty = GLOBAL;
      show_avail_mem( );
   }
   return( OK );
}


/*
 * Name:    block_convert_case
 * Class:   primary editor function
 * Purpose: convert characters to lower case, upper case, strip hi bits,
 *          or e-mail functions
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 */
int  block_convert_case( WINDOW *window )
{
int  len;
int  block_type;
line_list_ptr begin;
register file_infos *file;
WINDOW *sw;
long number;
long er;
unsigned int count;
int  bc, ec;
int  block_len;
int  rc;
void (*char_func)( text_ptr, unsigned int );

   /*
    * make sure block is marked OK
    */
   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   rc = OK;
   check_block( );
   if (g_status.marked == TRUE) {

      /*
       * set char_func() to the required block function in tdeasm.c
       */
      switch (g_status.command) {
         case BlockUpperCase  :
            char_func = upper_case;
            break;
         case BlockLowerCase  :
            char_func = lower_case;
            break;
         case BlockRot13      :
            char_func = rot13;
            break;
         case BlockFixUUE     :
            char_func = fix_uue;
            break;
         case BlockStripHiBit :
            char_func = strip_hi;
            break;
         default :
            return( ERROR );
      }

      file  = g_status.marked_file;
      file->modified = TRUE;
      if (mode.do_backups == TRUE) {
         sw = g_status.window_list;
         for (; ptoul( sw->file_info ) != ptoul( file );)
            sw = sw->next;
         rc = backup_file( sw );
      }

      if (rc == OK) {
         block_type = file->block_type;
         ec = file->block_ec;

         begin  = file->block_start;

         er = file->block_er;
         block_len = ec + 1 - file->block_bc;
         for (number=file->block_br; number <= er; number++) {
            begin->dirty = TRUE;
            count = len = begin->len;
            bc = 0;
            if (block_type == STREAM) {
               if (number == file->block_br) {
                  bc = file->block_bc;
                  if (len < file->block_bc) {
                     count = 0;
                     bc = len;
                  }
               }
               if (number == file->block_er) {
                  if (ec < len)
                     ec = len;
                  count = ec - bc + 1;
               }
            } else if (block_type == BOX) {
               bc = file->block_bc;
               count =  len >= ec ? block_len : len - bc;
            }
            if (len > bc) {

               assert( count < MAX_LINE_LENGTH );
               assert( bc >= 0 );
               assert( bc < MAX_LINE_LENGTH );

               (*char_func)( begin->line+bc, count );
            }
            begin = begin->next;
         }

         /*
          * IMPORTANT:  we need to reset the copied flag because the cursor may
          * not necessarily be on the last line of the block.
          */
         g_status.copied = FALSE;
         file->dirty = GLOBAL;
      }
   } else
      rc = ERROR;
   return( rc );
}


/*
 * Name:    upper_case
 * Purpose: To convert all lower case characters to upper characters
 * Date:    June 5, 1991
 * Passed:  s:    the starting point
 *          count: number of characters to convert
 * Returns: none
 * Notes:   xor 0x20 with lower case to get upper case.  yes, I know
 *           the toupper( ) macro or function is faster, but let's
 *           let make it easy for users to modify for non-English alphabets.
 *          this routine only handles the English alphabet.  modify as
 *           needed for other alphabets.
 */
void upper_case( text_ptr s, size_t count )
{
   if (s != NULL) {
      for (; count > 0; s++, count-- ) {
         if (*s >= 'a'  &&  *s <= 'z')
            *s ^= 0x20;
      }
   }
}


/*
 * Name:    lower_case
 * Purpose: To convert all upper case characters to lower characters
 * Date:    June 5, 1991
 * Passed:  s:    the starting point
 *          count: number of characters to convert
 * Returns: none
 * Notes:   or upper case with 0x20 to get lower case.  yes, I know
 *           the tolower( ) macro or function is faster, but let's
 *           let make it easy for users to modify for non-English alphabets. 
 *          this routine only handles the English alphabet.  modify as
 *           needed for other alphabets.
 */
void lower_case( text_ptr s, size_t count )
{
   if (s != NULL) {
      for (; count > 0; s++, count-- ) {
         if (*s >= 'A'  &&  *s <= 'Z')
            *s |= 0x20;
      }
   }
}


/*
 * Name:    rot13
 * Purpose: To rotate all alphabet characters by 13
 * Date:    June 5, 1991
 * Passed:  s:    the starting point
 *          count: number of characters to convert
 * Returns: none
 * Notes:   simple rot13
 *          i really don't know how to handle rot13 for alphabets
 *           other than English.
 */
void rot13( text_ptr s, size_t count )
{
register size_t c;

   c = *s;
}


/*
 * Name:    fix_uue
 * Purpose: To fix EBCDIC ==> ASCII translation problem
 * Date:    June 5, 1991
 * Passed:  s:    the starting point
 *          count: number of characters to convert
 * Returns: none
 * Notes:   to fix the EBCDIC to ASCII translation problem, three characters
 *           need to be changed,  0x5d -> 0x7c, 0xd5 -> 0x5b, 0xe5 -> 0x5d
 */
void fix_uue( text_ptr s, size_t  count )
{
   if (s != NULL) {
      for (; count > 0; s++, count-- ) {
         switch (*s) {
            case 0x5d :
               *s = 0x7c;
               break;
            case 0xd5 :
               *s = 0x5b;
               break;
            case 0xe5 :
               *s = 0x5d;
               break;
            default :
               break;
         }
      }
   }
}


/*
 * Name:    strip_hi
 * Purpose: To strip bit 7 from characters
 * Date:    June 5, 1991
 * Passed:  s:    the starting point, which should be normalized to a segment
 *          count: number of characters to strip (size_t)
 *                 count should not be greater than MAX_LINE_LENGTH
 * Returns: none
 * Notes:   this function is useful on WordStar files.  to make a WordStar
 *           file readable, the hi bits need to be anded off.
 *          incidentally, soft CRs and LFs may be converted to hard CRs
 *           and LFs.  this function makes no attempt to split lines
 *           when soft returns are converted to hard returns.
 */
void strip_hi( text_ptr s, size_t count )
{
   if (s != NULL) {
      for (; count > 0; s++, count-- ) {
         if (*s >= 0x80)
            *s &= 0x7f;
      }
   }
}
