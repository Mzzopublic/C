

#include "tdestr.h"             /* tde types */
#include "common.h"
#include "define.h"
#include "tdefunc.h"


/*
 *              keystroke record functions
 */

/*
 * Name:    record_on_off
 * Purpose: save keystrokes in keystroke buffer
 * Date:    April 1, 1992
 * Passed:  window:  pointer to current window
 * Notes:   -1 in .next field indicates the end of a recording
 *          -1 in .key field indicates the initial, unassigned macro key
 *          STROKE_LIMIT+1 in .next field indicates an unused space.
 */
int  record_on_off( WINDOW *window )
{
register int next;
int  prev;
int  line;
int  key;
int  func;
char line_buff[(MAX_COLS+2)*2]; /* buffer for char and attribute  */

   mode.record = !mode.record;
   if (mode.record == TRUE) {
      line = window->bottom_line;
      show_avail_strokes( );
      save_screen_line( 0, line, line_buff );
      /*
       * press key that will play back recording
       */
      set_prompt( main11, line );

      /*
       * get the candidate macro key and look up the function assigned to it.
       */
      key = getkey( );
      func = getfunc( key );

      /*
       * the key must be an unused, recognized function key or a function
       *  key assigned to a previously defined macro.  we also need room
       *  in the macro structure.
       */
      if (key <= 256 || (func != 0 && func != PlayBack)) {
         /*
          * cannot assign a recording to this key
          */
         error( WARNING, line, main12 );
         mode.record = FALSE;
      } else if (g_status.stroke_count == 0) {
         /*
          * no more room in recording buffer
          */
         error( WARNING, line, main13 );
         mode.record = FALSE;
      } else {

         /*
          * everything is everything so far, just check for a prev macro
          */
         prev = OK;
         if (func == PlayBack) {
            /*
             * overwrite recording (y/n)?
             */
            set_prompt( main14, line );
            if (get_yn( ) == A_NO) {
               prev = ERROR;
               mode.record = FALSE;
            }
         }
         if (prev == OK) {
            g_status.recording_key = key;
            next = macro.first_stroke[key-256];

            /*
             * if key has already been assigned to a macro, clear macro def.
             */
            if (next != STROKE_LIMIT+1) {
               do {
                  prev = next;
                  next = macro.strokes[next].next;
                  macro.strokes[prev].key  = MAX_KEYS+1;
                  macro.strokes[prev].next = STROKE_LIMIT+1;
                  ++g_status.stroke_count;
               } while (next != -1);
               show_avail_strokes( );
            }

            /*
             * find the first open space and initialize
             */
            for (next=0; macro.strokes[next].next != STROKE_LIMIT+1;)
               next++;
            macro.first_stroke[key-256] = next;
            macro.strokes[next].key  = -1;
            macro.strokes[next].next = -1;
            key_func.key[key-256] = PlayBack;
            /*
             * recording
             */
            s_output( main15, g_display.mode_line, 22,
                      g_display.mode_color | 0x80 );
         }
      }
      restore_screen_line( 0, line, line_buff );
   }

   /*
    * the flashing "Recording" and the stroke count write over the modes.
    *  when we get thru defining a macro, redisplay the modes.
    */
   if (mode.record == FALSE) {
      memset( line_buff, ' ', 36 );
      line_buff[36] = '\0';
      s_output( line_buff, g_display.mode_line, 22, g_display.mode_color );
      show_tab_modes( );
      show_indent_mode( );
      show_sync_mode( );
      show_search_case( );
      show_wordwrap_mode( );

      /*
       * let's look at the macro.  if the first .key of the macro is
       *  still -1, which is the initial unassigned key in a macro, reset
       *  the macro so other keys may be assigned to this node.
       */
      key = g_status.recording_key;
      if (key != 0) {
         next = macro.first_stroke[key-256];
         if (macro.strokes[next].key == -1) {
            macro.strokes[next].key  = MAX_KEYS+1;
            macro.strokes[next].next = STROKE_LIMIT+1;
            macro.first_stroke[key-256] = STROKE_LIMIT+1;
            if (getfunc( key ) == PlayBack)
               key_func.key[key-256] = 0;
         }
      }
      g_status.recording_key = 0;
   }
   return( OK );
}


/*
 * 作用: 把键盘命令放到缓存中
 * 参数: line: 要提示显示的行
 * 注意: 如果next值是-1，表明到了记录的末尾；
 *       如果next值是STROKE_LIMIT+1表示空间已经用完
 */
void record_keys( int line )
{
register int next;
register int prev;
int  key;
int  func;

   if (mode.record == TRUE) {
      if (g_status.stroke_count == 0)
         /*
          * 宏记录已经没有空间记录更多的操作
          */
         error( WARNING, line, main13 );
      else {
         key = g_status.key_pressed;
         func = getfunc( key );
         if (func != RecordMacro && func != SaveMacro && func != LoadMacro &&
             func != ClearAllMacros) {

            /*
             * 如果next值是-1，表明到了记录的末尾
             */
            next = macro.first_stroke[g_status.recording_key - 256];
            if (macro.strokes[next].next != STROKE_LIMIT+1) {
               while (macro.strokes[next].next != -1)
                  next = macro.strokes[next].next;
            }
            prev = next;

            /*
             * 找到一个空间来记录当前的操作
             */
            if (macro.strokes[next].key != -1) {
               for (; next < STROKE_LIMIT &&
                            macro.strokes[next].next != STROKE_LIMIT+1;)
                  next++;
               if (next == STROKE_LIMIT) {
                  for (next=0; next < prev &&
                               macro.strokes[next].next != STROKE_LIMIT+1;)
                     next++;
               }
            }
            if (next == prev && macro.strokes[prev].key != -1)
               /*
                * 记录缓存没有记录
                */
               error( WARNING, line, main13 );
            else {
            /*
             * 如果我们正在记录第一个宏节点，那么next == prev
             */
               macro.strokes[prev].next = next;
               macro.strokes[next].next = -1;
               macro.strokes[next].key  = key;
               g_status.stroke_count--;
               show_avail_strokes( );
            }
         }
      }
   }
}


/*
 * Name:    show_avail_strokes
 * Purpose: show available free key strokes in lite bar at bottom of screen
 * Date:    April 1, 1992
 */
void show_avail_strokes( void )
{
char strokes[MAX_COLS];

   s_output( main18, g_display.mode_line, 33, g_display.mode_color );
   itoa( g_status.stroke_count, strokes, 10 );
   s_output( "      ", g_display.mode_line, 51, g_display.mode_color );
   s_output( strokes, g_display.mode_line, 51, g_display.mode_color );
}


/*
 * Name:    save_strokes
 * Purpose: save strokes to a file
 * Date:    April 1, 1992
 * Passed:  window:  pointer to current window
 */
int  save_strokes( WINDOW *window )
{
FILE *fp;                       /* file to be written */
char name[MAX_COLS+2];          /* file name */
char line_buff[(MAX_COLS+1)*2]; /* buffer for char and attribute  */
register int rc;
int  prompt_line;
int  fattr;

   name[0] = '\0';
   prompt_line = window->bottom_line;
   save_screen_line( 0, prompt_line, line_buff );
   /*
    * name for macro file
    */
   if ((rc = get_name( main19, prompt_line, name,
                 g_display.message_color )) == OK  &&  *name != '\0') {

      /*
       * make sure it is OK to overwrite any existing file
       */
      rc = get_fattr( name, &fattr );
      if (rc == OK) {
         /*
          * overwrite existing file
          */
         set_prompt( main20, prompt_line );
         if (get_yn( ) != A_YES  ||  change_mode( name, prompt_line ) == ERROR)
            rc = ERROR;
      }
      if (rc != ERROR) {
         if ((fp = fopen( name, "wb" )) != NULL) {
            fwrite( &macro.first_stroke[0], sizeof(int), MAX_KEYS, fp );
            fwrite( &macro.strokes[0], sizeof(STROKES), STROKE_LIMIT, fp );
            fclose( fp );
         }
      }
   }
   restore_screen_line( 0, prompt_line, line_buff );
   return( OK );
}


/*
 * Name:    load_strokes
 * Purpose: load strokes from a file
 * Date:    April 1, 1992
 * Passed:  window:  pointer to current window
 * Notes:   show the user a file pick list.  I can never remember macro
 *           file names or the directory in which they hide.  might as well
 *           give the user a file pick list.
 */
int  load_strokes( WINDOW *window )
{
register FILE *fp;      /* file to be read */
char dname[MAX_COLS];   /* directory search pattern */
char stem[MAX_COLS];    /* directory stem */
register int rc;

   dname[0] = '\0';
   /*
    * search path for macro file
    */
   if (get_name( main21, window->bottom_line, dname,
                 g_display.message_color ) == OK  &&  *dname != '\0') {
      if (validate_path( dname, stem ) == OK) {
         rc = list_and_pick( dname, stem, window );

         /*
          * if everything is everything, load in the file selected by user.
          */
         if (rc == OK) {
            if ((fp = fopen( dname, "rb" )) != NULL && ceh.flag != ERROR) {
               fwrite( &macro.first_stroke[0], sizeof(int), MAX_KEYS, fp );
               fwrite( &macro.strokes[0], sizeof(STROKES), STROKE_LIMIT, fp );
               fclose( fp );
            }
            if (ceh.flag == OK)
               connect_macros( );
         }
      } else
         /*
          * invalid path or file name
          */
         error( WARNING, window->bottom_line, main22 );
   }
   return( OK );
}


/*
 * Name:    clear_macro
 * Purpose: reset all macro buffers, pointers, functions.
 * Date:    April 1, 1992
 * Notes:   reset the available macro stroke count.  reset all fields in
 *           macro structure.  clear any keys assigned to macros in the
 *           function assignment array.
 */
int  clear_macros( WINDOW *arg_filler )
{
register int i;

   g_status.stroke_count = STROKE_LIMIT;
   for (i=0; i<STROKE_LIMIT; i++) {
      macro.strokes[i].next = STROKE_LIMIT+1;
      macro.strokes[i].key  = MAX_KEYS+1;
   }
   for (i=0; i<MAX_KEYS; i++) {
      macro.first_stroke[i] = STROKE_LIMIT+1;
      if (key_func.key[i] == PlayBack)
         key_func.key[i] = 0;
   }
   return( OK );
}


/*
 * Name:    connect_macros
 * Purpose: hook up all (if any) macros to the function key definition table
 * Date:    April 1, 1992
 * Notes:   we need to connect all macro definitions to the key definition
 *           table in the startup routine or when we read in a new macro
 *           definition file.  the predefined func assignments take
 *           precedence over macro definitions.
 */
void connect_macros( void )
{
register int i;

   /*
    * reset the key function assignment array.  initially, no keys may be
    *  assigned to a macro.
    */
   for (i=0; i<MAX_KEYS; i++)
      if (key_func.key[i] == PlayBack)
         key_func.key[i] = 0;

   /*
    * now, find out how many free keystrokes are in the macro structure.
    */
   g_status.stroke_count = 0;
   for (i=0; i<STROKE_LIMIT; i++)
      if (macro.strokes[i].next == STROKE_LIMIT+1)
         ++g_status.stroke_count;

   /*
    * go thru the first stroke list to see if any key has been assigned to
    *  a macro and connect the macro def to the key.  predefined function
    *  assignments take precedence over macros.
    */
   for (i=0; i<MAX_KEYS; i++) {
      if (macro.first_stroke[i] != STROKE_LIMIT+1)
         if (key_func.key[i] == 0)
            key_func.key[i] = PlayBack;
   }
}


/*
 *              keystroke play back functions
 */


/*
 * Name:    play_back
 * Purpose: play back a series of keystrokes assigned to key
 * Date:    April 1, 1992
 * Notes:   go thru the macro key list playing back the recorded keystrokes.
 *          to let macros call other macros, we have to 1) save the next
 *           keystroke of the current macro in a stack, 2) execute the
 *           the called macro, 3) pop the key that saved by the calling
 *           macro, 4) continue executing the macro, beginning with the
 *           key we just popped.
 *          use a local stack to store keys.  currently, there is room
 *           for 256 keys -- should be enough room for most purposes.
 */
int  play_back( WINDOW *window )
{
int  key;
int  rc = OK;
int  popped;            /* flag is set when macro is popped */

   /*
    * if we are recording a macro, let's just return if we do a recursive
    *  definition.  Otherwise, we end up executing our recursive macro
    *  while we are defining it.
    */
   if (mode.record == TRUE && g_status.key_pressed == g_status.recording_key)
      rc = ERROR;
   else {

      /*
       * set the global macro flags, so other routines will know
       *  if a macro is executing.
       * set the stack_pointer to "empty" or -1.  initialize the popped
       *  flag to FALSE being that we haven't popped any thing off the stack,
       *  yet.
       */
      g_status.macro_executing = TRUE;
      g_status.mstack_pointer  = -1;
      popped = FALSE;
      rc = OK;
      while (rc == OK) {

         /*
          * the first time thru the loop, popped is FALSE.  some lint
          *  utilities may complain about key being used but not defined.
          */
         if (popped == FALSE) {

            /*
             * find the first keystroke in the macro.  when we pop the stack,
             *  all this stuff is reset by the pop -- do not reset it again.
             */
            g_status.macro_next = macro.first_stroke[g_status.key_pressed-256];
            g_status.current_macro = g_status.key_pressed;
            key = macro.strokes[g_status.macro_next].key;
         }
         popped = FALSE;
         if (key != MAX_KEYS+1  &&  key != -1) {
            do {

               /*
                * set up all editor variables as if we were entering
                *  keys from the keyboard.
                */
               window = g_status.current_window;
               display_dirty_windows( window );
               ceh.flag = OK;
               g_status.key_pressed = macro.strokes[g_status.macro_next].key;
               g_status.command = getfunc( g_status.key_pressed );
               if (g_status.wrapped  ||  g_status.key_pending) {
                  g_status.key_pending = FALSE;
                  g_status.wrapped = FALSE;
                  show_search_message( CLR_SEARCH, g_display.mode_color );
               }

               /*
                * while there are no errors or Control-Breaks, let's keep on
                *  executing a macro.  g_status.control_break is a global
                *  editor flag that is set in our Control-Break interrupt
                *  handler routine.
                */
               if (g_status.control_break == TRUE) {
                  rc = ERROR;
                  break;
               }

               /*
                * we haven't called any editor function yet.  we need
                *  to look at the editor command that is to be executed.
                *  if the command is PlayBack, we need to break out of
                *  this inner do loop and start executing the macro
                *  from the beginning (the outer do loop).
                *
                * if we don't break out now from a recursive macro, we will
                *  recursively call PlayBack and we will likely overflow
                *  the main (as opposed to the macro_stack) stack.
                */
               if (g_status.command == PlayBack) {

                  /*
                   * recursive macros are handled differently from
                   *  macros that call other macros.
                   * recursive macros - break out of this inner loop
                   *  and begin executing the macro from the beg of macro.
                   * standard macros - save the next instruction of this
                   *  macro on the stack and begin executing the called macro.
                   */
                  if (g_status.current_macro != g_status.key_pressed) {
                     if (push_macro_stack(
                                   macro.strokes[g_status.macro_next].next )
                                   != OK) {
                        error( WARNING, window->bottom_line, ed16 );
                        rc = ERROR;
                     }
                     g_status.macro_next =
                                macro.first_stroke[g_status.key_pressed-256];
                     g_status.current_macro = g_status.key_pressed;
                     key = macro.strokes[g_status.macro_next].key;

                     /*
                      * execute called macro at beginning of this do loop.
                      */
                     continue;
                  } else

                     /*
                      * recursive macro - break out of this inner loop
                      *  or else we may overflow the stack(s).
                      */
                     break;
               }


               /*
                * just as we assert before the main editor routine, let's
                *  assert in the macro function to make sure everything
                *  is everything.
                */
#if defined(  __MSC__ )
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
#endif


               if (g_status.command >= 0 && g_status.command < NUM_FUNCS)
                   rc = (*do_it[g_status.command])( window );
               g_status.macro_next =
                          macro.strokes[g_status.macro_next].next;
            } while (rc == OK  &&  g_status.macro_next != -1);

            /*
             * if we have come the end of a macro definition and there
             *  are no keys saved on the stack, we have finished our
             *  macro.  get out.
             */
            if (g_status.macro_next == -1 && g_status.mstack_pointer < 0)
               rc = ERROR;
            else if (rc != ERROR  &&  g_status.mstack_pointer >= 0) {

               /*
                * if this is a recursive macro, don't pop the stack
                *  because we didn't push.
                * for a standard macro, get back the next key in the
                *  calling macro.
                */
               if (g_status.current_macro != g_status.key_pressed) {
                  if (pop_macro_stack( &g_status.macro_next ) != OK) {
                     error( WARNING, window->bottom_line, ed17 );
                     rc = ERROR;
                  } else {
                     popped = TRUE;
                     key = macro.strokes[g_status.macro_next].key;
                  }
               }
            }
         }
      }
      g_status.macro_executing = FALSE;
   }
   return( OK );
}


/*
 * Name:    push_macro_stack
 * Purpose: push the next key in a currently executing macro on local stack
 * Date:    October 31, 1992
 * Notes:   finally got tired of letting macros only "jump" and not call
 *           other macros.
 *          the first time in, stack_pointer is -1.
 */
int  push_macro_stack( int key )
{
   /*
    * first, make sure we have room to push the key.
    */
   if (g_status.mstack_pointer+1 < MAX_KEYS) {

      /*
       * increment the stack pointer and store the pointer to the next key
       *  of the currently executing macro.  store the currently executing
       *  macro, too.
       */
      ++g_status.mstack_pointer;
      macro_stack[g_status.mstack_pointer].key = key;
      macro_stack[g_status.mstack_pointer].macro = g_status.current_macro;
      return( OK );
   } else
      return( STACK_OVERFLOW );
}


/*
 * Name:    pop_macro_stack
 * Purpose: pop currently executing macro on local stack
 * Date:    October 31, 1992
 * Notes:   finally got tired of letting macros only "jump" and not "call"
 *           other macros.
 *          pop the macro stack.  stack pointer is pointing to last key saved
 *           on stack.
 */
int  pop_macro_stack( int *key )
{

   /*
    * before we pop the stack, make sure there is something in the stack.
    */
   if (g_status.mstack_pointer >= 0) {

      /*
       * pop the pointer to the next key and the current macro, then
       *  decrement the stack_pointer.
       */
      *key = macro_stack[g_status.mstack_pointer].key;
      g_status.current_macro = macro_stack[g_status.mstack_pointer].macro;
      --g_status.mstack_pointer;
      return( OK );
   } else
      return( STACK_UNDERFLOW );
}


/*
 * Name:    Pause
 * Purpose: Enter pause state for macros
 * Date:    June 5, 1992
 * Passed:  arg_filler:  argument to satify function prototype
 * Returns: ERROR if the ESC key was pressed, OK otherwise.
 * Notes:   this little function is quite useful in macro definitions.  if
 *          it is called near the beginning of a macro, the user may decide
 *          whether or not to stop the macro.
 */
int  pause( WINDOW *arg_filler )
{
int  c;

   /*
    * tell user we are paused.  the '|  0x80' turns on the blink attribute
    */
   s_output( paused1, g_display.mode_line, 23, g_display.mode_color | 0x80 );
   s_output( paused2, g_display.mode_line, 23+strlen( paused1 ),
             g_display.mode_color );

   /*
    * get the user's response and restore the mode line.
    */
   c = getkey( );
   show_modes( );
   if (mode.record == TRUE) {
      /*
       * if recording a macro, show recording message
       */
      s_output( main15, g_display.mode_line, 23, g_display.mode_color | 0x80 );
      show_avail_strokes( );
   }
   return( c == ESC ? ERROR : OK );
}
