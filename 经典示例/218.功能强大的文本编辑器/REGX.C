

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * types of nodes in the NFA.
 *
 * let's use the node naming convention in Ken Thompson's reg ex paper.
 */
#define CNODE           0
#define NNODE           1

#define SCAN            -1

/*
 * types of terminals in NNODEs.
 *
 * starting with simple ASCII terminals, it's easy to build in other types of
 *  terminals, e.g. wild chars, BOL, EOL, and character classes.  actually,
 *  we can easily build ANSI C ctype library function NNODEs.
 */
#define STRAIGHT_ASCII  0
#define IGNORE_ASCII    1
#define WILD            2
#define BOL             3
#define EOL             4
#define CLASS           5
#define NOTCLASS        6
#define WHITESPACE      7
#define ALPHA           8
#define UPPER           9
#define LOWER           10
#define ALPHANUM        11
#define DECIMAL         12
#define HEX             13
#define BOW             14
#define EOW             15


/*
 * types of terminals in CNODEs.
 */
#define START           0
#define ACCEPT          1
#define OR_NODE         2
#define JUXTA           3
#define CLOSURE         4
#define ZERO_OR_ONE     5


int  lookahead;
int  regx_rc;
int  reg_len;
int  parser_state;
char class_bits[32];
int  bit[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
int  c1;
int  c2;
int  ttype;
int  regx_error_line;

NFA_TYPE  *nfa_pointer;

int  nfa_status;
int  search_len;
int  search_col;
text_ptr search_string;

int  queued_states[REGX_SIZE+2];
int  deque[REGX_SIZE*2];
int  dq_head;
int  dq_tail;
int  stacked_node_count;
int  reset_count;


/*
 * Name:    find_regx
 * Purpose: To set up and find a regular expression.
 * Date:    June 5, 1993
 * Passed:  window:  pointer to current window
 * Notes:   Keep the regular expression until changed.
 */
int  find_regx( WINDOW *window )
{
int  direction;
int  new_string;
char pattern[MAX_COLS];  /* text to be found */
long found_line;
long bin_offset;
line_list_ptr ll;
register WINDOW *win;  /* put window pointer in a register */
int  rc;
int  old_rcol;
int  rcol;

   switch (g_status.command) {
      case FindRegX :
         new_string = TRUE;
         direction  = FORWARD;
         break;
      case RepeatFindRegX :
         new_string =  regx.search_defined != OK ? TRUE : FALSE;
         direction  = FORWARD;
         break;
      case RepeatFindRegXBackward :
         new_string =  regx.search_defined != OK ? TRUE : FALSE;
         direction  = BACKWARD;
         break;
      default :
         new_string = 0;
         direction  = 0;
         assert( FALSE );
         break;
   }

   win = window;
   entab_linebuff( );
   if (un_copy_line( win->ll, win, TRUE ) == ERROR)
      return( ERROR );

   regx_error_line = win->bottom_line;

   /*
    * get search text, using previous as default
    */
   rc = OK;
   if (new_string == TRUE) {
      *pattern = '\0';
      if (regx.search_defined == OK) {

         assert( strlen( (char *)regx.pattern ) < MAX_COLS );

         strcpy( pattern, (char *)regx.pattern );
      }

      /*
       * string to find:
       */
      if (get_name( reg1, win->bottom_line, pattern,
                    g_display.message_color ) != OK  ||  *pattern == '\0')
         return( ERROR );
      regx.search_defined = OK;

      assert( strlen( pattern ) < MAX_COLS );

      strcpy( (char *)regx.pattern, pattern );
      rc = build_nfa( );
      if (rc == ERROR)
         regx.search_defined = ERROR;
   }

   if (regx.search_defined == OK) {
      old_rcol = win->rcol;
      if (mode.inflate_tabs)
         win->rcol = entab_adjust_rcol( win->ll->line, win->ll->len, win->rcol);
      update_line( win );
      show_search_message( SEARCHING, g_display.diag_color );
      bin_offset = win->bin_offset;
      if (direction == FORWARD) {
         if ((ll = forward_regx_search( win, &found_line, &rcol )) != NULL) {
            if (g_status.wrapped && g_status.macro_executing)
               rc = ask_wrap_replace( win, &new_string );
            if (rc == OK)
               find_adjust( win, ll, found_line, rcol );
            else
               win->bin_offset = bin_offset;
         }
      } else {
         if ((ll = backward_regx_search( win, &found_line, &rcol )) != NULL) {
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
      else {
         if (nfa_status == OK)
            show_search_message( CLR_SEARCH, g_display.mode_color );
         else
            g_status.wrapped = TRUE;
      }
      if (ll == NULL) {
         /*
          * string not found
          */
         if (mode.inflate_tabs)
            win->rcol = old_rcol;
         combine_strings( pattern, find5a, (char *)regx.pattern, find5b );
         error( WARNING, win->bottom_line, pattern );
         rc = ERROR;
      }
      show_curl_line( win );
      make_ruler( win );
      show_ruler( win );
   } else {
      /*
       * find pattern not defined
       */
      error( WARNING, win->bottom_line, find6 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    forward_regx_search
 * Purpose: search forward for regular expression
 * Date:    June 5, 1993
 * Passed:  window:  pointer to current window
 *          rline:   pointer to real line counter
 *          rcol:    pointer to real column variable
 * Returns: position in file if found or NULL if not found
 * Notes:   Start searching from cursor position to end of file.  If we hit
 *           end of file without a match, start searching from the beginning
 *           of file to cursor position.  (do wrapped searches)
 */
line_list_ptr forward_regx_search( WINDOW *window, long *rline, int *rcol )
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
   *rline = win->rline;
   i = win->rcol + 1;
   ll = win->ll;
   len = ll->len;
   if (i > len  &&  len != EOF) {
      ll = ll->next;
      ++*rline;
      i = 0;
   }
   if (i < 0)
      i = 0;

   *rcol = i;
   ll = regx_search_forward( ll, rline, rcol );

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
      ll = regx_search_forward( win->file_info->line_list, rline, rcol );

      if (ll == win->ll  &&  *rcol >= win->rcol)
         ll = NULL;

      if (win->ll->next != NULL)
         win->ll->next->len = end;
   }
   flush_keyboard( );

   if (ll != NULL)
      bin_offset_adjust( win, *rline );
   return( ll );
}


/*
 * Name:    regx_search_forward
 * Purpose: search forward for pattern using nfa
 * Date:    June 5, 1993
 * Passed:  ll:     pointer to current node in linked list
 *          rline:  pointer to real line counter
 *          col:    column in ll->line to begin search
 * Returns: position in file if found or NULL if not found
 * Notes:   run the nfa machine on each character in each line
 */
line_list_ptr regx_search_forward( line_list_ptr ll, long *rline, int  *col )
{
   if (ll->len == EOF)
      return( NULL );
   else {
      switch (g_status.command) {
         case DefineRegXGrep  :
         case RepeatGrep :
            nfa_pointer = &sas_nfa;
            stacked_node_count = sas_regx.node_count;
            break;
         case FindRegX  :
         case RepeatFindRegX :
            nfa_pointer = &nfa;
            stacked_node_count = regx.node_count;
            break;
         default :
            assert( FALSE );
            break;
      }
      nfa_status = OK;
      search_string = ll->line;
      search_len = ll->len;
      search_col = *col;
      reset_count = stacked_node_count * sizeof(int);
      for (; !g_status.control_break;) {
         for (; search_col <= search_len; search_col++) {
            if (nfa_match( ) != ERROR) {
               *col = search_col;
               return( ll );
            }
         }
         ++*rline;
         ll = ll->next;
         search_string = ll->line;
         if (ll->len == EOF)
            return( NULL );
         search_len = ll->len;
         search_col = 0;
      }
      return( NULL );
   }
}


/*
 * Name:    backward_regx_search
 * Purpose: search backward for regular expression
 * Date:    June 5, 1993
 * Passed:  window:  pointer to current window
 *          rline:   pointer to real line counter
 *          rcol:    pointer to real column variable
 * Returns: position in file if found or NULL if not found
 * Notes:   Start searching from cursor position to end of file.  If we hit
 *           end of file without a match, start searching from the beginning
 *           of file to cursor position.  (do wrapped searches)
 */
line_list_ptr backward_regx_search( WINDOW *window, long *rline, int *rcol )
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
   ll = regx_search_backward( ll, rline, rcol );

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
      ll = regx_search_backward( ll->prev, rline, rcol );

      if (ll == win->ll  &&  *rcol <= win->rcol)
         ll = NULL;

      if (win->ll->prev != NULL)
         win->ll->prev->len = end;
   }
   flush_keyboard( );

   if (ll != NULL)
      bin_offset_adjust( win, *rline );
   return( ll );
}


/*
 * Name:    regx_search_backward
 * Purpose: search backward for pattern using regular expression
 * Date:    June 5, 1993
 * Passed:  ll:     pointer to current node in linked list
 *          rline:  pointer to real line counter
 *          col:    column in ll->line to begin search
 * Returns: position in file if found or NULL if not found
 * Notes:   run the nfa machine on each character in each line
 */
line_list_ptr regx_search_backward( line_list_ptr ll, long *rline, int *col )
{
   if (ll == NULL)
      return( NULL );
   if (ll->len == EOF)
      return( NULL );
   else {
      nfa_pointer = &nfa;
      stacked_node_count = regx.node_count;

      search_string = ll->line;
      search_len = ll->len;
      search_col = *col;
      reset_count = stacked_node_count * sizeof(int);
      while (!g_status.control_break) {
         for (; search_col >= 0; search_col--) {
            if (nfa_match( ) != ERROR) {
               *col = search_col;
               return( ll );
            }
         }
         --*rline;
         ll = ll->prev;
         if (ll == NULL)
            return( NULL );
         if (ll->len == EOF)
            return( NULL );
         search_string = ll->line;
         search_col = search_len = ll->len;
      }
      return( NULL );
   }
}


/******************************  NFA Recognizer  *********************/


/*
 * Name:    nfa_match
 * Purpose: try to recognize a pattern
 * Date:    June 5, 1993
 * Passed:  none, but modifies local global nfa recognizer.
 * Returns: length of recognized pattern if found or ERROR if not found.
 */
int  nfa_match( void )
{
register int c;
int  state;
int  j;
int  n1;
int  rc;

   j = search_col;
   c  =  j < search_len  ?  search_string[j]  :  EOF;
   state = nfa_pointer->next1[0];
   dq_head = 0;
   dq_tail = 0;
   memset( queued_states, 0, reset_count );
   put_dq( SCAN );

   while (state) {
      if (state == SCAN) {
         memset( queued_states, 0, reset_count );
         j++;
         put_dq( SCAN );
         c  =  j < search_len  ?  search_string[j]  :  EOF;
      } else if (nfa_pointer->node_type[state] == NNODE) {
         n1 = nfa_pointer->next1[state];
         rc = OK;
         switch (nfa_pointer->term_type[state]) {
            case STRAIGHT_ASCII :
               if (nfa_pointer->c[state] == c)
                  rc = put_dq( n1 );
               break;
            case IGNORE_ASCII   :
               if (nfa_pointer->c[state] == tolower( c ))
                  rc = put_dq( n1 );
               break;
            case WILD           :
               if (j < search_len)
                  rc = put_dq( n1 );
               break;
            case BOL            :
               if (j == 0) {
                  rc = put_dq( n1 );
                  if (deque[dq_tail] == SCAN)
                     --j;
               }
               break;
            case EOL            :
               if (j == search_len) {
                  rc = put_dq( n1 );
                  if (deque[dq_tail] == SCAN)
                     --j;
               }
               break;
            case CLASS          :
               if (c != EOF  &&  nfa_pointer->class[state][c/8] & bit[c%8])
                  rc = put_dq( n1 );
               break;
            case NOTCLASS       :
               if (c != EOF  &&  !(nfa_pointer->class[state][c/8] & bit[c%8]))
                  rc = put_dq( n1 );
               break;
            case WHITESPACE     :
               if (c == ' '  ||  c == '\t')
                  rc = put_dq( n1 );
               break;
            case ALPHA          :
               if (c != EOF  &&  isalpha( c ))
                  rc = put_dq( n1 );
               break;
            case UPPER          :
               if (c != EOF  &&  isupper( c ))
                  rc = put_dq( n1 );
               break;
            case LOWER          :
               if (c != EOF  &&  islower( c ))
                  rc = put_dq( n1 );
               break;
            case ALPHANUM       :
               if (c != EOF  &&  isalnum( c ))
                  rc = put_dq( n1 );
               break;
            case DECIMAL        :
               if (c != EOF  &&  isdigit( c ))
                  rc = put_dq( n1 );
               break;
            case HEX            :
               if (c != EOF  &&  isxdigit( c ))
                  rc = put_dq( n1 );
               break;
            case BOW            :
               if (c != EOF) {
                  if (!myiswhitespc( c )) {
                     if (j == 0) {
                        rc = put_dq( n1 );
                        if (deque[dq_tail] == SCAN)
                           --j;
                     } else if (j > 0) {
                        if (myiswhitespc( search_string[j-1] )) {
                           rc = put_dq( n1 );
                           if (deque[dq_tail] == SCAN)
                              --j;
                        }
                     }
                  }
               }
               break;
            case EOW            :
               if (c == EOF) {
                  if (search_len > 0) {
                     if (!myiswhitespc( search_string[search_len-1] )) {
                        rc = put_dq( n1 );
                        if (deque[dq_tail] == SCAN)
                           --j;
                     }
                  }
               } else {
                  if (myiswhitespc( c )  &&  j > 0) {
                     if (!myiswhitespc( search_string[j-1] )) {
                        rc = put_dq( n1 );
                        if (deque[dq_tail] == SCAN)
                           --j;
                     }
                  }
               }
               break;
            default             :
               assert( FALSE );
               break;
         }
         if (rc == ERROR)
            return( 0 );
      } else {
         assert( nfa_pointer->node_type[state] == CNODE );

         n1 = nfa_pointer->next1[state];
         if (push_dq( n1 ) == ERROR)
            return( 0 );

         if (n1 != nfa_pointer->next2[state])
            if (push_dq( nfa_pointer->next2[state] ) == ERROR)
               return( 0 );
      }
      if (dequeempty( )  ||  j > search_len)
         return( ERROR );
      state = pop_dq( );
   }
   return( j - search_col );
}


/*
 * Name:    put_dq
 * Purpose: queue future states
 * Date:    June 5, 1993
 * Passed:  v:  state to queue
 * Returns: none, but modifies local global deque variables.  future
 *           states are queued in the head.
 * Notes:   do not add any duplicate states to the head of the deque.
 */
int  put_dq( int v )
{
   if (queued_states[v+1] == 0) {
      ++queued_states[v+1];
      deque[dq_head] = v;
      dq_head--;
      if (dq_head < 0)
         dq_head = REGX_SIZE - 1;
      if (dq_tail == dq_head) {
         nfa_status = ERROR;
         show_search_message( NFA_GAVE_UP, g_display.diag_color );
         return( ERROR );
      }
   }
   return( OK );
}


/*
 * Name:    push_dq
 * Purpose: push state on deque
 * Date:    June 5, 1993
 * Passed:  v:  state to push
 * Returns: whether stack is OK or if stack overflowed - ERROR.  present
 *           states are stacked.
 */
int  push_dq( int v )
{
   ++dq_tail;
   if (dq_tail == dq_head) {
      nfa_status = ERROR;
      show_search_message( NFA_GAVE_UP, g_display.diag_color );
      return( ERROR );
   } else {
      if (dq_tail > REGX_SIZE - 1)
         dq_tail = 0;
      deque[dq_tail] = v;
      return( OK );
   }
}


/*
 * Name:    pop_dq
 * Purpose: pop next state on dq
 * Date:    June 5, 1993
 * Passed:  none, but looks at local global nfa recognizer
 * Returns: state on top of deque and decrements stack pointer
 */
int  pop_dq( void )
{
register int v;

   v = deque[dq_tail];
   dq_tail--;
   if (dq_tail < 0)
      dq_tail = REGX_SIZE - 1;
   return( v );
}


/*
 * Name:    dequeempty
 * Purpose: any room on dq?
 * Date:    June 5, 1993
 * Passed:  none, but looks at local global nfa recognizer
 * Returns: boolean, TRUE if empty.
 * Notes:   the deque is a circular array where future states are
 *           queued in the head and present states are pushed in the tail.
 */
int  dequeempty( void )
{
   if (dq_tail > dq_head)
      return( dq_tail - dq_head == 1 );
   else
      return( dq_tail + REGX_SIZE - dq_head == 1 );
}


/***************************  NFA Compiler  **************************/


/*
 * Name:    build_nfa
 * Purpose: initialize nfa and call the compiler
 * Date:    June 5, 1993
 * Passed:  none, but looks at local global pattern.
 * Returns: whether or not an ERROR occured
 * Notes:   sets up the initial variables for the compiler.  builds
 *          initial and acceptor states for the nfa after compiler finishes.
 */
int  build_nfa( void )
{
   regx_rc = OK;

   init_nfa( );
   lookahead = 0;
   reg_len   = strlen( (char *)regx.pattern );
   parser_state = 1;

   nfa.next1[0] = expression( );
   if (regx_rc == OK) {
      emit_cnode( 0, START, nfa.next1[0], nfa.next1[0] );
      emit_cnode( parser_state, ACCEPT, 0, 0 );
      regx.node_count = parser_state + 2;
   }

   if (g_status.command == DefineRegXGrep) {
      memcpy( &sas_nfa, &nfa, sizeof(NFA_TYPE) );
      memcpy( &sas_regx, &regx, sizeof(REGX_INFO) );
   }
   return( regx_rc );
}


/*
 * Name:    expression
 * Purpose: break reg ex into terms and expressions
 * Date:    June 5, 1993
 * Passed:  none, but looks at local global pattern.
 * Returns: none
 * Notes:   because recursion can go fairly deep, almost all variables
 *          were made local global.  no need to save most of this stuff
 *          on the stack.
 */
int  expression( void )
{
int t1;
int t2;
int r;

   t1 = term( );
   r = t1;
   if (regx.pattern[lookahead] == '|') {
      lookahead++;
      parser_state++;
      r = t2 = parser_state;
      parser_state++;
      emit_cnode( t2, OR_NODE, expression( ), t1 );
      emit_cnode( t2-1, JUXTA, parser_state, parser_state );

      /*
       * the OR_NODE seems to need a path from the previous node
       */
      if (nfa.node_type[t1] == CNODE)
         t1 = min( nfa.next1[t1], nfa.next2[t1] );
      nfa.next1[t1-1] = t2;
      if (nfa.node_type[t1-1] == NNODE)
         nfa.next2[t1-1] = t2;
   }
   return( r );
}


/*
 * Name:    term
 * Purpose: break reg ex into factors and terms
 * Date:    June 5, 1993
 * Passed:  none, but looks at local global pattern.
 * Returns: none
 */
int  term( void )
{
int r;

   r = factor( );
   if (regx.pattern[lookahead] == '('  ||  letter( regx.pattern[lookahead] ))
      term( );
   else if (Kleene_star( regx.pattern[lookahead] ))
      regx_error( reg11 );
   return( r );
}


/*
 * Name:    factor
 * Purpose: add NNODEs and CNODEs to local global nfa
 * Date:    June 5, 1993
 * Passed:  none
 * Returns: none, but modifies local global nfa.
 * Notes:   this function does most of the compiling.  it recognizes all
 *          NNODEs, predefined macros, escape characters, and operators.
 */
int  factor( void )
{
int t1;
int t2;
int r;
int c;
int paren;

   t1 = parser_state;
   c = regx.pattern[lookahead];
   if (c == '(') {
      lookahead++;
      t2 = expression( );
      if (regx.pattern[lookahead] == ')') {
         lookahead++;
         paren = TRUE;
      } else

         /*
          * unmatched open parens
          */
         regx_error( reg2 );
   } else if (letter( c )) {
      paren = FALSE;
      switch (c) {
         case ']' :

            /*
             * unmatched close bracket
             */
            regx_error( reg9 );
            break;
         case '.' :
            ttype = WILD;
            break;
         case ',' :
            ttype = WHITESPACE;
            break;
         case '^' :
            ttype = BOL;
            break;
         case '$' :
            ttype = EOL;
            break;
         case '<' :
            ttype = BOW;
            break;
         case '>' :
            ttype = EOW;
            break;
         case '\\' :
            ++lookahead;
            ttype =  mode.search_case == IGNORE ? IGNORE_ASCII : STRAIGHT_ASCII;
            if (lookahead != '\0') {
               if (regx.pattern[lookahead] != ':')
                  c = escape_char( regx.pattern[lookahead] );

               /*
                * predefined unix-like macros.
                */
               else {
                  c = regx.pattern[lookahead+1];
                  if (c != '\0') {
                     switch (c) {
                        case 'a' :
                           ++lookahead;
                           ttype = ALPHANUM;
                           break;
                        case 'b' :
                           ++lookahead;
                           ttype = WHITESPACE;
                           break;
                        case 'c' :
                           ++lookahead;
                           ttype = ALPHA;
                           break;
                        case 'd' :
                           ++lookahead;
                           ttype = DECIMAL;
                           break;
                        case 'h' :
                           ++lookahead;
                           ttype = HEX;
                           break;
                        case 'l' :
                           ++lookahead;
                           ttype = LOWER;
                           break;
                        case 'u' :
                           ++lookahead;
                           ttype = UPPER;
                           break;
                        default :
                           c = escape_char( regx.pattern[lookahead] );
                           break;
                     }
                  } else
                     c = escape_char( regx.pattern[lookahead] );
               }
            } else
               regx_error( reg4 );
            break;
         case '[' :
            memset( class_bits, 0, sizeof(char) * 32 );
            ++lookahead;
            if (lookahead != '\0') {
               c = regx.pattern[lookahead];
               if (c == '^') {
                  ++lookahead;
                  ttype = NOTCLASS;
               } else
                  ttype = CLASS;

               c1 = regx.pattern[lookahead];
               do {
                  class_bits[c1/8]  |=  bit[c1%8];
                  if (c1 != '\0')
                     ++lookahead;
                  if (regx.pattern[lookahead] == '-') {
                     ++lookahead;
                     c2 = regx.pattern[lookahead];
                     if (c2 != '\0') {

                        /*
                         * just in case the hi for the range is given first,
                         *  switch c1 and c2,  e.g. [9-0].
                         */
                        if (c2 < c1) {
                           c  = c2;
                           c2 = c1;
                           c1 = c;
                        }

                        for (c=c1; c <= c2; c++)
                           class_bits[c/8] |= bit[c%8];

                        if (regx.pattern[lookahead] != '\0')
                           ++lookahead;
                     } else
                        regx_error( reg10 );
                  }
                  c1 = regx.pattern[lookahead];
               } while (c1  != '\0'  &&  c1 != ']');

               if (c1 == '\0')
                  regx_error( reg5 );
            } else
               regx_error( reg6 );
            break;
         default :
            if (mode.search_case == IGNORE) {
               c = tolower( c );
               ttype = IGNORE_ASCII;
            } else
               ttype = STRAIGHT_ASCII;
      }
      emit_nnode( parser_state, ttype, c, parser_state+1, parser_state+1 );
      if (ttype == CLASS  ||  ttype == NOTCLASS) {
         nfa.class[parser_state] = calloc( 32, sizeof(char) );
         if (nfa.class[parser_state] != NULL)
            memcpy( nfa.class[parser_state], class_bits, sizeof( char )*32 );
         else
            regx_error( reg7 );
      }
      t2 = parser_state;
      lookahead++;
      parser_state++;
   } else if (c == '\0')
      return( 0 );
   else {
      if (c == '*'  ||  c == '+'  ||  c == '?')
         regx_error( reg8 );
      else if (c  ==  ')')
         regx_error( reg3 );
      else
         regx_error( reg2 );
   }

   c = regx.pattern[lookahead];
   switch (c) {
      case '*' :
         emit_cnode( parser_state, CLOSURE, parser_state+1, t2 );
         r = parser_state;
         if (nfa.node_type[t1] == CNODE)
            t1 = min( nfa.next1[t1], nfa.next2[t1] );
         nfa.next1[t1-1] = parser_state;
         if (nfa.node_type[t1-1] == NNODE)
            nfa.next2[t1-1] = parser_state;
         lookahead++;
         parser_state++;
         paren = FALSE;
         break;
      case '+' :
         if (paren == TRUE) {
            emit_cnode( parser_state, JUXTA, parser_state+2, parser_state+2 );
            parser_state++;
         }

         emit_cnode( parser_state, JUXTA, t2, t2 );
         r = parser_state;
         parser_state++;

         if (paren == FALSE) {
            nfa.next1[t2] = parser_state;
            if (nfa.node_type[t2] == NNODE)
               nfa.next2[t2] = parser_state;
         }

         emit_cnode( parser_state, CLOSURE, parser_state+1, t2 );
         if (nfa.node_type[t1] == CNODE)
            t1 = min( nfa.next1[t1], nfa.next2[t1] );
         nfa.next1[t1-1] = r;
         if (nfa.node_type[t1-1] == NNODE)
            nfa.next2[t1-1] = r;
         parser_state++;
         lookahead++;
         paren = FALSE;
         break;
      case '?' :
         emit_cnode( parser_state, JUXTA, parser_state+2, parser_state+2 );
         parser_state++;
         r = parser_state;
         emit_cnode( parser_state, ZERO_OR_ONE, parser_state+1, t2 );
         if (nfa.node_type[t1] == CNODE)
            t1 = min( nfa.next1[t1], nfa.next2[t1] );
         nfa.next1[t1-1] = parser_state;
         if (nfa.node_type[t1-1] == NNODE)
            nfa.next2[t1-1] = parser_state;
         parser_state++;
         lookahead++;
         paren = FALSE;
         break;
      default  :
         r = t2;
         break;
   }

   /*
    * close parens seem to need a JUXTA node to gather all reg ex's
    *  to a common point.
    */
   if (paren) {
      emit_cnode( parser_state, JUXTA, parser_state+1, parser_state+1 );
      parser_state++;
   }
   return( r );
}


/*
 * Name:    escape_char
 * Purpose: recognize escape and C escape sequences
 * Date:    June 5, 1993
 * Passed:  let:  letter to escape
 * Returns: escaped letter
 */
int  escape_char( int let )
{
   switch (let) {
      case '0' :
         let = 0x00;
         break;
      case 'a' :
         let = 0x07;
         break;
      case 'b' :
         let = 0x08;
         break;
      case 'n' :
         let = 0x0a;
         break;
      case 'r' :
         let = 0x0d;
         break;
      case 't' :
         let = 0x09;
         break;
      default  :
         break;
   }
   return( let );
}


/*
 * Name:    emit_cnode
 * Purpose: add a null node to our pattern matching machine
 * Date:    June 5, 1993
 * Passed:  index:  current node in nfa
 *          ttype:  terminal type - CLOSURE, OR, JUXTA, etc...
 *          n1:     pointer to next state, path for lambda transitions
 *          n2:     pointer to other next state, usually a NNODE
 * Returns: none, but modifies local global nfa.
 */
void emit_cnode( int index, int ttype, int n1, int n2 )
{
   assert( index >= 0);
   assert( index < REGX_SIZE );

   nfa.node_type[index] = CNODE;
   nfa.term_type[index] = ttype;
   nfa.c[index] = 0;
   nfa.next1[index] = n1;
   nfa.next2[index] = n2;
}


/*
 * Name:    emit_nnode
 * Purpose: add a to our pattern matching machine
 * Date:    June 5, 1993
 * Passed:  index:  current node in nfa
 *          ttype:  terminal type - EOL, ASCII, etc...
 *          c:      letter this node recognizes
 *          n1:     pointer to next state
 *          n2:     pointer to other next state, which can be same as n1
 * Returns: none, but modifies local global nfa.
 */
void emit_nnode( int index, int ttype, int c, int n1, int n2 )
{
   assert( index >= 0);
   assert( index < REGX_SIZE );

   nfa.node_type[index] = NNODE;
   nfa.term_type[index] = ttype;
   nfa.c[index] = c;
   nfa.next1[index] = n1;
   nfa.next2[index] = n2;
}


/*
 * Name:    init_nfa
 * Purpose: set local global nfa to NULL state
 * Date:    June 5, 1993
 * Passed:  none
 */
void init_nfa( void )
{
int i;

   for (i=0; i < REGX_SIZE; i++) {
      nfa.node_type[i] = NNODE;
      nfa.term_type[i] = 0;
      nfa.c[i] = 0;
      nfa.next1[i] = 0;
      nfa.next2[i] = 0;
      if (nfa.class[i] != NULL)
         free( nfa.class[i] );
      nfa.class[i] = NULL;
   }
}


/*
 * Name:    regx_error
 * Purpose: display reg ex error message and set reg ex error code
 * Date:    June 5, 1993
 * Passed:  line:  line to display error
 * Returns: none, but sets reg ex return code to error.
 */
void regx_error( char *line )
{
   error( WARNING, regx_error_line, line );
   regx_rc = ERROR;
}


/*
 * Name:    separator
 * Purpose: determine if character is a reg ex separator
 * Date:    June 5, 1993
 * Passed:  let:  letter to look at
 * Returns: whether or not 'let' is a separator
 */
int  separator( int let )
{
   return( let == 0  ||  let == ')'  ||  let == '|' );
}


/*
 * Name:    Kleene_star
 * Purpose: determine if character is a reg ex operator
 * Date:    June 5, 1993
 * Passed:  let:  letter to look at
 * Returns: whether or not 'let' is a letter
 */
int  Kleene_star( int let )
{
   return( let == '*'  ||  let == '+'  ||  let == '?' );
}


/*
 * Name:    letter
 * Purpose: determine if character is a recognized reg ex character
 * Date:    June 5, 1993
 * Passed:  let:  letter to look at
 * Returns: whether or not 'let' is a letter.
 */
int  letter( int let )
{
   return( !separator( let )  &&  !Kleene_star( let ) );
}
