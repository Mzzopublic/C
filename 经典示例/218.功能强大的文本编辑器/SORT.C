
#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * Name:    sort_box_block
 * Purpose: sort lines according to text in marked BOX block
 * Date:    June 5, 1992
 * Passed:  window:  pointer to current window
 * Notes:   quick sort and insertion sort the lines in the BOX buff according
 *           to stuff in a box block.
 */
int  sort_box_block( WINDOW *window )
{
int  prompt_line;
int  block_type;
line_list_ptr ll;
register file_infos *file;
WINDOW *sw;
int  rc;
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */

   /*
    * make sure block is marked OK
    */
   rc = OK;
   prompt_line = window->bottom_line;
   entab_linebuff( );
   if (un_copy_line( window->ll, window, TRUE ) == ERROR)
      return( ERROR );
   check_block( );
   if (g_status.marked == TRUE) {
      file  = g_status.marked_file;
      block_type = file->block_type;
      if (block_type == BOX) {
         /*
          * sort ascending or descending?
          */
         rc = get_sort_order( window );
         if (rc != ERROR) {
            file->modified = TRUE;
            if (mode.do_backups == TRUE) {
               sw = g_status.window_list;
               for (; ptoul( sw->file_info ) != ptoul( file );)
                  sw = sw->next;
               backup_file( sw );
            }

            /*
             * figure the width of the block.
             */
            sort.block_len = file->block_ec + 1 - file->block_bc;

            /*
             * save the prompt line and print the quicksort message.
             */
            save_screen_line( 0, prompt_line, line_buff );
            eol_clear( 0, prompt_line, g_display.text_color );
            set_prompt( block22a, prompt_line );

            /*
             * set up the sort structure.
             */
            sort.bc  = g_status.marked_file->block_bc;
            sort.ec  = g_status.marked_file->block_ec;
            sort.order_array = (mode.search_case == IGNORE) ?
                                    sort_order.ignore : sort_order.match;

            /*
             * save the previous node for use with insertion sort.
             */
            ll = file->block_start->prev;
            quick_sort_block( file->block_br, file->block_er,
                              file->block_start, file->block_end );

            /*
             * get back previous node and clean up list with insertion
             *   sort.
             */
            if (ll == NULL)
               ll = file->line_list;
            else
               ll = ll->next;
            set_prompt( block22b, prompt_line );
            insertion_sort_block( file->block_br, file->block_er, ll );

            /*
             * housekeeping.  mark the file as dirty and restore the
             *   cursors, which are scrambled during the sort.
             */
            file->dirty = GLOBAL;
            restore_cursors( file );
            restore_screen_line( 0, prompt_line, line_buff );
         }
      } else {
         /*
          * can only sort box blocks
          */
         error( WARNING, prompt_line, block23 );
         rc = ERROR;
      }
   } else {
      /*
       * box not marked
       */
      error( WARNING, prompt_line, block24 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    quick_sort_block
 * Purpose: sort lines according to text in marked BOX block
 * Date:    Jaunary 10, 1993
 * Passed:  low:        starting line in box block
 *          high:       ending line in a box block
 *          low_node:   starting node in box block
 *          high_node:  ending node in box block
 * Notes:   Quicksort lines in the BOX block according to keys in
 *           a box block.
 *          because the median of three method is used to find the partion
 *           node,  high - low  should be greater than or equal to 2.
 *          with end recursion removal and sorting the smallest sublist
 *           first, our stack only needs room for log2 (N+1)/(M+2) nodes.
 *           a stack size of 24 can reliably handle almost 500 million lines.
 */
void quick_sort_block( long low, long high, line_list_ptr low_node,
                       line_list_ptr high_node )
{
long low_rline_stack[24];
long high_rline_stack[24];
line_list_ptr low_node_stack[24];
line_list_ptr high_node_stack[24];
long low_count;
long high_count;
long count;
line_list_ptr low_start;
line_list_ptr low_head;
line_list_ptr low_tail;
line_list_ptr high_end;
line_list_ptr high_head;
line_list_ptr high_tail;
line_list_ptr equal_head;
line_list_ptr equal_tail;
line_list_ptr walk_node;
line_list_ptr median_node;
int  i;
int  stack_pointer;

   assert( low_node->len != EOF);
   assert( high_node->len != EOF);

   stack_pointer = 0;
   for (;;) {

      /*
       * being that a median-of-three is used as the partition algorithm,
       *  we probably need to have at least 2 nodes in each sublist.  I
       *  chose a minimum of 25 nodes as a SWAG (scientific wild ass guess).
       *  a simple insertion sort mops the list after quicksort finishes.
       */
      while (high - low > 25) {

         assert( high >= 1 );
         assert( low  >= 1 );
         assert( low  <= high );

         /*
          * start the walk node at the head of the list and walk to the
          *  middle of the sublist.
          */
         walk_node  = low_node;
         count = (high - low) / 2;
         for (; count > 0; count--)
            walk_node = walk_node->next;

         /*
          * now, find the median of the low, middle, and high node.
          *
          * being that I am subject to error, let's assert that we really
          *  did find the median-of-three.
          */
         load_pivot( low_node );
         if (compare_pivot( walk_node ) < 0) {
            low_head   = walk_node;
            median_node = low_node;
         } else {
            low_head   = low_node;
            median_node = walk_node;
         }
         high_head = high_node;
         load_pivot( median_node );
         if (compare_pivot( high_node ) < 0) {
            high_head   = median_node;
            median_node = high_node;
         }
         load_pivot( median_node );
         if (compare_pivot( low_head ) > 0) {
            low_tail    = median_node;
            median_node = low_head;
            low_head    = low_tail;
         }

         load_pivot( median_node );

         assert( compare_pivot( low_head ) <= 0 );
         assert( compare_pivot( high_head ) >= 0 );

         /*
          * now, walk again from the head of the list comparing nodes and
          *  use the first occurrence of the median node as the partition.
          */
         walk_node = low_node;
         for (i = 0; ; walk_node = walk_node->next) {
            if (compare_pivot( walk_node ) == 0)
               break;
            i = 1;
         }

         /*
          * initialize pointers and counters for this partition.
          */
         low_start  = low_node->prev;
         high_end   = high_node->next;
         low_head   = low_tail  = NULL;
         high_head  = high_tail = NULL;
         low_count  = high_count = 0;

         /*
          * setup the first occurrence of the median node as a "fat pivot"
          *  sublist.  there are two cases to consider 1) the first
          *  occurrence of the median node is the first element in the
          *  sublist, i == 0, or 2) the first occurrence of the median node
          *  is somewhere in the sublist.
          */
         if (i == 0)
            walk_node = equal_head = equal_tail = low_node;
         else {
            equal_head = equal_tail = walk_node;
            equal_head->next->prev = equal_head->prev;
            equal_head->prev->next = equal_head->next;
            equal_head->next = low_node;
            walk_node = equal_head;
         }
         load_pivot( equal_head );

         /*
          * PARTITION:
          *  put all nodes less than the pivot on the end of the low list.
          *  put all nodes equal to the pivot on the end of the equal list.
          *  put all nodes greater than the pivot on the end of the high list.
          */
         for (count=low+1; count <= high; count++) {
            walk_node = walk_node->next;
            i = compare_pivot( walk_node );
            if (i > 0) {
               if (high_head == NULL)
                  high_head = high_tail = walk_node;
               else {
                  high_tail->next = walk_node;
                  walk_node->prev = high_tail;
                  high_tail = walk_node;
               }

               /*
                * keep a count of the number of nodes in the high list.
                */
               ++high_count;
            } else if (i < 0) {
               if (low_head == NULL)
                  low_head = low_tail = walk_node;
               else {
                  low_tail->next = walk_node;
                  walk_node->prev = low_tail;
                  low_tail = walk_node;
               }

               /*
                * keep a count of the number of nodes in the low list
                */
               ++low_count;
            } else {
               equal_tail->next = walk_node;
               walk_node->prev = equal_tail;
               equal_tail = walk_node;
            }
         }

         assert( low_count >= 0 );
         assert( low_count < high - low );
         assert( high_count >= 0 );
         assert( high_count < high - low );

         /*
          * we just partitioned the sublist into low, equal, and high
          *  sublists.  now, let's put the lists back together.
          */
         if (low_count > 0) {
            low_head->prev = low_start;
            if (low_start != NULL)
               low_start->next = low_head;
            else
               g_status.marked_file->line_list = low_head;
            low_tail->next = equal_head;
            equal_head->prev = low_tail;
         } else {
            equal_head->prev = low_start;
            if (low_start != NULL)
               low_start->next = equal_head;
            else
               g_status.marked_file->line_list = equal_head;
         }
         if (high_count > 0) {
            high_head->prev = equal_tail;
            equal_tail->next = high_head;
            high_tail->next = high_end;
            high_end->prev  = high_tail;
         } else {
            equal_tail->next = high_end;
            high_end->prev   = equal_tail;
         }

         /*
          * now, lets look at the low list and the high list.  save the node
          *  pointers and counters of the longest sublist on the stack.
          *  then, quicksort the shortest sublist.
          */
         if (low_count > high_count) {

            /*
             * if fewer than 25 nodes in the high count, don't bother to
             *  push the stack -- sort the low list.
             */
            if (high_count > 25) {
               low_rline_stack[stack_pointer]  = low;
               high_rline_stack[stack_pointer] = low + low_count - 1;
               low_node_stack[stack_pointer]   = low_head;
               high_node_stack[stack_pointer]  = low_tail;
               ++stack_pointer;
               low       = high - high_count + 1;
               high      = high;
               low_node  = high_head;
               high_node = high_tail;
            } else {
               low       = low;
               high      = low + low_count - 1;
               low_node  = low_head;
               high_node = low_tail;
            }
         } else {

            /*
             * if fewer than 25 nodes in the low count, don't bother to
             *  push the stack -- sort the high list.
             */
            if (low_count > 25) {
               low_rline_stack[stack_pointer]  = high - high_count + 1;
               high_rline_stack[stack_pointer] = high;
               low_node_stack[stack_pointer]   = high_head;
               high_node_stack[stack_pointer]  = high_tail;
               ++stack_pointer;
               low       = low;
               high      = low + low_count - 1;
               low_node  = low_head;
               high_node = low_tail;
            } else {
               low       = high - high_count + 1;
               high      = high;
               low_node  = high_head;
               high_node = high_tail;
            }
         }

         assert( stack_pointer < 24 );
      }

      /*
       * now that we have sorted the smallest sublist, we need to pop
       *  the long sublist(s) that were pushed on the stack.
       */
      --stack_pointer;
      if (stack_pointer < 0)
         break;
      low       = low_rline_stack[stack_pointer];
      high      = high_rline_stack[stack_pointer];
      low_node  = low_node_stack[stack_pointer];
      high_node = high_node_stack[stack_pointer];
   }
}


/*
 * Name:    insertion_sort_block
 * Purpose: sort small partitions passed in by qsort
 * Date:    January 10, 1993
 * Passed:  low:          starting line in box block
 *          high:         ending line in a box block
 *          first_node:   first linked list node to sort
 * Notes:   Insertion sort the lines in the BOX buff according to stuff in
 *           a box block.
 */
void insertion_sort_block( long low, long high, line_list_ptr first_node )
{
long down;                      /* relative line number for insertion sort */
long pivot;                     /* relative line number of pivot in block */
long count;
line_list_ptr pivot_node;       /* pointer to actual text in block */
line_list_ptr down_node;        /* pointer used to compare text */
text_ptr key;
int  dirty_flag;
int  len;

   /*
    * make sure we have more than 1 line to sort.
    */
   if (low < high) {

      count = (int)(high - low) + 1;
      pivot_node = first_node->next;
      for (pivot=1; pivot < count; pivot++) {
         load_pivot( pivot_node );
         key = pivot_node->line;
         len = pivot_node->len;
         dirty_flag = pivot_node->dirty;
         down_node = pivot_node;
         for (down=pivot-1; down >= 0; down--) {
            /*
             * lets keep comparing the keys until we find the hole for
             *  pivot.
             */
            if (compare_pivot( down_node->prev ) > 0) {
               down_node->line = down_node->prev->line;
               down_node->len = down_node->prev->len;
               down_node->dirty = down_node->prev->dirty;
            } else
               break;
            down_node = down_node->prev;
         }
         down_node->line = key;
         down_node->len  = len;
         down_node->dirty = (char)dirty_flag;
         pivot_node = pivot_node->next;
      }
   }
}


/*
 * Name:    load_pivot
 * Purpose: load pivot point for insertion sort
 * Date:    June 5, 1992
 * Passed:  text:  line that contains the pivot
 */
void load_pivot( line_list_ptr node )
{
   sort.pivot_ptr = node->line;
   sort.pivot_len = node->len;
}


/*
 * Name:    compare_pivot
 * Purpose: compare pivot string with text string
 * Date:    June 5, 1992
 * Passed:  text:  pointer to current line
 * Notes:   the sort structure keeps track of the pointer to the pivot line
 *           and the pivot line length.
 */
int  compare_pivot( line_list_ptr node )
{
register int len;
register int bc;
int  rc;
int  left_over;

   len = node->len;
   bc  = sort.bc;

   assert( bc >= 0 );
   assert( len >= 0 );

   /*
    * is the current line length less than beginning column?  if so, just
    *  look at the length of the pivot line.  no need to compare keys.
    */
   if (len < bc+1) {
      if (sort.pivot_len < bc+1)
         return( 0 );
      else
         return( sort.direction == ASCENDING ?  -1 : 1 );

   /*
    * is the pivot line length less than beginning column?  if so, just
    *  look at the length of the current line.  no need to compare keys.
    */
   } else if (sort.pivot_len < bc+1) {
      if (len < bc+1)
         return( 0 );
      else
         return( sort.direction == ASCENDING ?  1 : -1 );
   } else {

      /*
       * if lines are of equal length or greater than the ending column,
       *  then lets consider them equal.
       */
      if (len == sort.pivot_len)
         left_over = 0;
      else if (len > sort.ec  &&  sort.pivot_len > sort.ec)
         left_over = 0;
      else {

         /*
          * if one line does not extend thru the ending column, give
          *  preference to the longest key.
          */
         if (sort.direction == ASCENDING)
            left_over =  len > sort.pivot_len ? 1 : -1;
         else
            left_over =  len > sort.pivot_len ? -1 : 1;
      }

      /*
       * only need to compare up to length of the key in the pivot line.
       */
      if (len > sort.pivot_len)
         len = sort.pivot_len;
      len = len - bc;
      if (len > sort.block_len)
         len = sort.block_len;

      assert( len > 0 );

      if (sort.direction == ASCENDING)
         rc = my_memcmp( node->line + bc, sort.pivot_ptr + bc, len );
      else
         rc = my_memcmp( sort.pivot_ptr + bc, node->line + bc, len );

      /*
       * if keys are equal, let's see if one key is longer than the other.
       */
      if (rc == 0)
         rc = left_over;
      return( rc );
   }
}


/*
 * Name:    my_memcmp
 * Purpose: compare strings using ignore or match case sort order
 * Date:    October 31, 1992
 * Passed:  s1:  pointer to string 1
 *          s2:  pointer to string 2
 *          len: number of characters to compare
 * Notes:   let's do our own memcmp, so we can sort languages that use
 *           extended characters as part of their alphabet.
 */
int  my_memcmp( text_ptr s1, text_ptr s2, int len )
{
unsigned char *p;
register int c;

   assert( len >= 0 );
   assert( len < MAX_LINE_LENGTH );
   assert( s1 != NULL );
   assert( s2 != NULL );

   if (len == 0)
      return( 0 );

   p = sort.order_array;

   /*
    * the C comparison function is equivalent to the assembly version;
    *  however, once the assembly routine initializes, it is much faster
    *  than the C version.
    */
   if (len < 10) {
      for (;len > 0  &&  (c = (int)p[*s1] - (int)p[*s2]) == 0;
                                              s1++, s2++, len--);
      return( c );
   } else {

      ASSEMBLE {

   /*
   ; Register strategy:
   ;   ax  == *s1
   ;   dx  == *s2
   ;   ds:[si]  == s1
   ;   es:[bx]  == s2
   ;   bp       == sort.order_array
   ;   [bp+di]  == p[*s1]  or  p[*s2]
   ;   cx       == len
   ;
   ;  CAVEAT:  sort.order_array is assumed to be located in the stack segment
   */

        push    ds                      /* push required registers */
        push    si
        push    di
        push    bp

        xor     ax, ax                  /* zero ax */
        mov     cx, WORD PTR len        /* keep len in cx */
        cmp     cx, 0                   /* len <= 0? */
        jle     get_out                 /* yes, get out */

        mov     bx, WORD PTR s2         /* load our registers */
        mov     ax, WORD PTR s2+2
        mov     es, ax                  /* es:[bx] = s2 */
        mov     si, WORD PTR s1
        mov     ax, WORD PTR s1+2
        mov     ds, ax                  /* ds:[si] = s1 */
        mov     bp, p                   /* [bp] = p */
        xor     ax, ax                  /* zero out ax */
        xor     dx, dx                  /* zero out dx */
      }
top:

   ASSEMBLE {
        mov     al, BYTE PTR ds:[si]    /* al == *s1 */
        mov     di, ax
        mov     al, BYTE PTR [bp+di]    /* al == p[*s1] */
        mov     dl, BYTE PTR es:[bx]    /* dl == *s2 */
        mov     di, dx
        mov     dl, BYTE PTR [bp+di]    /* dl == p[*s2] */
        sub     ax, dx                  /* ax == p[*s1] - p[*s2] */
        jne     get_out
        inc     bx
        inc     si
        dec     cx
        cmp     cx, 0
        jg      top                     /* ax keeps the return value */
      }
get_out:

   ASSEMBLE {
        pop     bp                      /* pop the registers we pushed */
        pop     di
        pop     si
        pop     ds                      /* ax keeps the return value */
      }
   }
}
