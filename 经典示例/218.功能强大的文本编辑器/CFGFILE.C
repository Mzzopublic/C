/*
 * This module contains all the routines needed to redefine key, colors, and
 *   modes via a configuration file.
 *
 * Program Name:  tdecfg
 * Author:        Frank Davis
 * Date:          June 5, 1992
 */


#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tdecfg.h"
#include "cfgfile.h"


/*
 * reference the structures in config files.
 */
extern struct vcfg cfg;
extern FILE *tde_exe;                  /* FILE pointer to tde.exe */
extern KEY_FUNC key_func;
extern MACRO macros;
extern COLORS temp_colours;
extern MODE_INFO in_modes;
extern long keys_offset;
extern long two_key_offset;
extern long macro_offset;
extern long color_offset;
extern long mode_offset;
extern long sort_offset;



char line_in[2000];             /* line buffer */
int  stroke_count;              /* global variable for macro strokes */
unsigned int line_no;           /* global variable for line count */
int modes[NUM_MODES];


TWO_KEY two_key_list;

SORT_ORDER sort_order;


/*
 * Name:    tdecfgfile
 * Date:    June 5, 1992
 * Notes:   read in a configuration file
 */
void tdecfgfile( void )
{
FILE *config;
char fname[80];
int  rc;

   /*
    * prompt for the configuration file name.
    */
   cls( );
   xygoto( 0, 3 );
   puts( "Enter configuration file name, e.g. tde.cfg  :" );
   gets( fname );

   if (strlen( fname ) != 0) {
      rc = OK;
      if ((rc = access( fname, EXIST )) != 0) {
         puts( "\n\n Error: File not found." );
         getkey( );
         rc = ERROR;
      } else if ((config = fopen( fname, "r" )) == NULL ) {
         puts( "\n\nError: Cannot open configuration file." );
         getkey( );
         rc = ERROR;
      }

      /*
       * if everything is everthing so far, get the current editor settings.
       */
      if (rc == OK) {
         fseek( tde_exe, keys_offset, SEEK_SET );
         fread( (void *)&key_func, sizeof(KEY_FUNC), 1, tde_exe );
         fseek( tde_exe, two_key_offset, SEEK_SET );
         fread( (void *)&two_key_list, sizeof(TWO_KEY), 1, tde_exe );
         fseek( tde_exe, macro_offset, SEEK_SET );
         fread( (void *)&macros, sizeof(MACRO), 1, tde_exe );
         fseek( tde_exe, color_offset, SEEK_SET );
         fread( (void *)&temp_colours, sizeof(COLORS), 1, tde_exe );
         fseek( tde_exe, mode_offset, SEEK_SET );
         fread( (void *)&in_modes, sizeof( MODE_INFO ), 1, tde_exe );
         fseek( tde_exe, sort_offset, SEEK_SET );
         fread( (void *)&sort_order, sizeof( SORT_ORDER ), 1, tde_exe );

         stroke_count = get_stroke_count( );

         /*
          * put the current modes into an array.  it's easier to work
          *   with them in an array.  This is from cfgmode.c.
          */
         modes[Ins]         = in_modes.insert;
         modes[Ind]         = in_modes.indent;
         modes[PTAB]        = in_modes.ptab_size;
         modes[LTAB]        = in_modes.ltab_size;
         modes[Smart]       = in_modes.smart_tab;
         modes[Write_Z]     = in_modes.control_z;
         modes[Crlf]        = in_modes.crlf;
         modes[Trim]        = in_modes.trailing;
         modes[Eol]         = in_modes.show_eol;
         modes[WW]          = in_modes.word_wrap;
         modes[Left]        = in_modes.left_margin;
         modes[Para]        = in_modes.parg_margin;
         modes[Right]       = in_modes.right_margin;
         modes[Size]        = in_modes.cursor_size;
         modes[Backup]      = in_modes.do_backups;
         modes[Ruler]       = in_modes.ruler;
         modes[Date]        = in_modes.date_style;
         modes[Time]        = in_modes.time_style;
         modes[InflateTabs] = in_modes.inflate_tabs;
         modes[Initcase]    = in_modes.search_case;
         modes[JustRM]      = in_modes.right_justify;

         line_no = 1;
         while (!feof( config )) {
            if (fgets( line_in, 1500, config ) == NULL)
               break;
            parse_line( line_in );
            ++line_no;
         }

         /*
          *  if we changed any modes, reset them b4 we write them to tde.exe.
          */
         in_modes.insert        = modes[Ins];
         in_modes.indent        = modes[Ind];
         in_modes.ptab_size     = modes[PTAB];
         in_modes.ltab_size     = modes[LTAB];
         in_modes.smart_tab     = modes[Smart];
         in_modes.control_z     = modes[Write_Z];
         in_modes.crlf          = modes[Crlf];
         in_modes.trailing      = modes[Trim];
         in_modes.show_eol      = modes[Eol];
         in_modes.word_wrap     = modes[WW];
         in_modes.left_margin   = modes[Left];
         in_modes.parg_margin   = modes[Para];
         in_modes.right_margin  = modes[Right];
         in_modes.cursor_size   = modes[Size];
         in_modes.do_backups    = modes[Backup];
         in_modes.ruler         = modes[Ruler];
         in_modes.date_style    = modes[Date];
         in_modes.time_style    = modes[Time];
         in_modes.inflate_tabs  = modes[InflateTabs];
         in_modes.search_case   = modes[Initcase];
         in_modes.right_justify = modes[JustRM];

         fseek( tde_exe, keys_offset, SEEK_SET );
         fwrite( (void *)&key_func, sizeof(KEY_FUNC), 1, tde_exe );
         fseek( tde_exe, two_key_offset, SEEK_SET );
         fwrite( (void *)&two_key_list, sizeof(TWO_KEY), 1, tde_exe );
         fseek( tde_exe, macro_offset, SEEK_SET );
         fwrite( (void *)&macros, sizeof(MACRO), 1, tde_exe );
         fseek( tde_exe, color_offset, SEEK_SET );
         fwrite( (void *)&temp_colours, sizeof(COLORS), 1, tde_exe );
         fseek( tde_exe, mode_offset, SEEK_SET );
         fwrite( (void *)&in_modes, sizeof( MODE_INFO ), 1, tde_exe );
         fseek( tde_exe, sort_offset, SEEK_SET );
         fwrite( (void *)&sort_order, sizeof( SORT_ORDER ), 1, tde_exe );
         fclose( config );
         printf( "\n\n    Configuration file read.  Press a key to continue :" );
         getkey( );
      }
   }
   cls( );
}


/*
 * Name:    parse_line
 * Purpose: real work horse of the configuration utility, figure out what
 *          we need to do with each line of the config file.
 * Date:    June 5, 1992
 * Passed:  line:  line that contains the text to parse
 */
void parse_line( char *line )
{
char key[1042];         /* buffer to hold any token that we parse */
char *residue;          /* pointer to next item in line, if it exists */
int key_no;             /* index into key array */
int parent_key;         /* 1st of two-combination keys */
int color;              /* color field */
int mode_index;         /* index in mode array */
int func_no;            /* function number we want to assign to a key */
int color_no;           /* attribute we want to assign to a color field */
int mode_no;            /* mode number we want to assign to a mode */
int found;              /* boolean, did we find a valid key, color, or mode? */
int i;

   /*
    * find the first token and put it in key.  residue points to next token.
    */
   residue = parse_token( line, key );
   if (*key != '\0' && *key != ';') {
      if (strlen( key ) > 1) {
         /*
          * try to find a valid key
          */
         found = FALSE;
         key_no = search( key, valid_keys, AVAIL_KEYS-1 );
         if (key_no != ERROR) {
            /*
             * find the function assignment
             */
            found = TRUE;
            if (residue != NULL) {
               residue = parse_token( residue, key );

               /*
                * if this is not a comment, find the function to assign
                *   to key.  clear any previous macro or key assignment.
                */
               if (*key != '\0' && *key != ';') {
                  func_no = search( key, valid_func, NUM_FUNC );
                  if (func_no != ERROR) {
                     clear_previous_twokey( key_no );
                     clear_previous_macro( key_no );
                     key_func.key[key_no] = func_no;
                     if (func_no == PlayBack)
                        parse_macro( key_no, residue );
                  } else {
                     parent_key = key_no;

                     /*
                      * was the second key one letter?
                      */
                     if (strlen( key ) == 1) {
                        key_no = *key;
                        residue = parse_token( residue, key );
                        if (*key != '\0' && *key != ';') {
                           func_no = search( key, valid_func, NUM_FUNC );
                           if (func_no != ERROR && func_no != PlayBack) {
                              if (insert_twokey( parent_key+256, key_no,
                                                          func_no ) == ERROR) {
                                 printf( "==> %s", line_in );
                                 printf( "Out of room for two-key: line %u  : function %s\n",
                                      line_no, key );
                              }
                           } else {
                              printf( "==> %s", line_in );
                              if ( func_no == ERROR)
                                 printf( "Unrecognized function: line %u  : function %s\n",
                                         line_no, key );
                              else
                                 printf( "Cannot assign a macro to two-keys: line %u  : function %s\n",
                                         line_no, key );
                           }
                        }
                     } else {
                        residue = parse_token( residue, key );
                        key_no = search( key, valid_keys, AVAIL_KEYS-1 );
                        if (key_no != ERROR && *key != '\0' && *key != ';') {
                           func_no = search( key, valid_func, NUM_FUNC );
                           if (func_no != ERROR && func_no != PlayBack) {
                              if (insert_twokey( parent_key+256, key_no+256,
                                                        func_no )  == ERROR) {
                                 printf( "==> %s", line_in );
                                 printf( "Out of room for two-key: line %u  : function %s\n",
                                      line_no, key );
                              }
                           } else {
                              printf( "==> %s", line_in );
                              if ( func_no == ERROR)
                                 printf( "Unrecognized function: line %u  : function %s\n",
                                         line_no, key );
                              else
                                 printf( "Cannot assign a macro to two-keys: line %u  : function %s\n",
                                         line_no, key );
                           }
                        } else {
                           printf( "==> %s", line_in );
                           printf( "Unrecognized function: line %u  : function %s\n",
                                   line_no, key );
                        }
                     }
                  }
               }
            }
         }

         /*
          * valid key not found, now try a valid color
          */
         if (!found) {
            color = search( key, valid_colors, (NUM_COLORS * 2) - 1 );
            if (color != ERROR) {
               if (*key == 'm')
                  i = 0;
               else
                  i = 1;
               found = TRUE;
               if (residue != NULL) {
                  residue = parse_token( residue, key );

                  /*
                   * we found a color field and attribute.  now, make sure
                   *   everything is everything before we assign the attribute
                   *   to the color field.
                   */
                  if (*key != '\0' && *key != ';') {
                     color_no = atoi( key );
                     if (color_no >= 0 && color_no <= 127)
                        temp_colours.clr[i][color] = color_no;
                     else {
                        printf( "==> %s", line_in );
                        printf( "Color number out of range: line %u  : number %s\n",
                                 line_no, key );
                     }
                  }
               }
            }
         }

         /*
          * valid color not found, now try a valid mode
          */
         if (!found) {
            mode_index = search( key, valid_modes, NUM_MODES-1 );
            if (mode_index != ERROR) {
               found = TRUE;

               /*
                * if we find a valid mode, we need to search different
                *   option arrays before we find a valid assignment.
                */
               if (residue != NULL) {
                  residue = parse_token( residue, key );
                  if (*key != '\0' && *key != ';') {
                     switch ( mode_index ) {
                        case Ins         :
                        case Ind         :
                        case Smart       :
                        case Trim        :
                        case Eol         :
                        case Backup      :
                        case Ruler       :
                        case InflateTabs :
                        case JustRM      :
                           mode_no = search( key, off_on, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Off/On error: " );
                           }
                           break;
                        case LTAB     :
                        case PTAB     :
                           mode_no = atoi( key );
                           if (mode_no > 520 || mode_no < 1) {
                              mode_no = ERROR;
                              printf( "==> %s", line_in );
                              printf( "Tab error: " );
                           }
                           break;
                        case Left    :
                           mode_no = atoi( key );
                           if (mode_no < 1 || mode_no > modes[Right]) {
                              mode_no = ERROR;
                              printf( "==> %s", line_in );
                              printf( "Left margin error: " );
                           } else
                              --mode_no;
                           break;
                        case Para    :
                           mode_no = atoi( key );
                           if (mode_no < 1 || mode_no > modes[Right]) {
                              mode_no = ERROR;
                              printf( "==> %s", line_in );
                              printf( "Paragraph margin error: " );
                           } else
                              --mode_no;
                           break;
                        case Right   :
                           mode_no = atoi( key );
                           if (mode_no < modes[Left] || mode_no > 1040) {
                              mode_no = ERROR;
                              printf( "==> %s", line_in );
                              printf( "Right margin error: " );
                           } else
                              --mode_no;
                           break;
                        case Crlf    :
                           mode_no = search( key, valid_crlf, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "CRLF or LF error: " );
                           }
                           break;
                        case WW      :
                           mode_no = search( key, valid_wraps, 2 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Word wrap error: " );
                           }
                           break;
                        case Size    :
                           mode_no = search( key, valid_cursor, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Cursor size error: " );
                           }
                           break;
                        case Write_Z :
                           mode_no = search( key, valid_z, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Control Z error: " );
                           }
                           break;
                        case Date    :
                           mode_no = search( key, valid_dates, 5 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Date format error: " );
                           }
                           break;
                        case Time    :
                           mode_no = search( key, valid_times, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Time format error: " );
                           }
                           break;
                        case Initcase    :
                           mode_no = search( key, init_case_modes, 1 );
                           if (mode_no == ERROR) {
                              printf( "==> %s", line_in );
                              printf( "Initial Case Mode error: " );
                           }
                           break;
                        case Match   :
                           for (i=0; i<256; i++)
                              sort_order.match[i] = (char)i;
                           new_sort_order( key, sort_order.match );
                           break;
                        case Ignore  :
                           for (i=0; i<256; i++)
                              sort_order.ignore[i] = (char)i;
                           for (i=65; i<91; i++)
                              sort_order.ignore[i] = (char)(i + 32);
                           new_sort_order( key, sort_order.ignore );
                           break;
                     }
                     if (mode_no != ERROR)
                        modes[mode_index] = mode_no;
                     else
                        printf( " line = %u  :  unknown mode = %s\n",
                              line_no, key );
                  }
               }
            }
         }
         if (!found) {
            printf( "==> %s", line_in );
            printf( "Unrecognized editor setting: line %u  :  %s\n", line_no, key );
         }
      }
   }
}


/*
 * Name:    parse_token
 * Purpose: given an input line, find the first token
 * Date:    June 5, 1992
 * Passed:  line:  line that contains the text to parse
 *          token:   buffer to hold token
 * Returns: pointer in line to start next token search.
 * Notes:   assume tokens are delimited by spaces.
 */
char *parse_token( char *line, char *token )
{
   /*
    * skip over any leading spaces.
    */
   while (*line == ' ')
      ++line;

   /*
    * put the characters into the token array until we run into a space
    *   or the terminating '\0';
    */
   while (*line != ' ' && *line != '\0' && *line != '\n')
      *token++ = *line++;
   *token = '\0';

   /*
    * return what's left on the line, if anything.
    */
   if (*line != '\0' && *line != '\n')
      return( line );
   else
      return( NULL );
}


/*
 * Name:    search
 * Purpose: binary search a CONFIG_DEFS structure
 * Date:    June 5, 1992
 * Passed:  token:  token to search for
 *          list:   list of valid tokens
 *          num:    number of valid tokens in list
 * Returns: value of token assigned to matching token.
 * Notes:   do a standard binary search.
 *          instead of returning mid, lets return the value of the token
 *          assigned to mid.
 */
int  search( char *token, CONFIG_DEFS list[], int num )
{
int bot;
int mid;
int top;
int rc;

   bot = 0;
   top = num;
   while (bot <= top) {
      mid = (bot + top) / 2;
      rc = stricmp( token, list[mid].key );
      if (rc == 0)
         return( list[mid].key_index );
      else if (rc < 0)
         top = mid - 1;
      else
         bot = mid + 1;
   }
   return( ERROR );
}


/*
 * Name:    parse_macro
 * Purpose: separate literals from keys in a macro definition
 * Date:    June 5, 1992
 * Passed:  macro_key:  key that we are a assigning a macro to
 *          residue:    pointer to macro defs
 * Notes:   for each token in macro def, find out if it's a literal or a
 *             function key.
 *          a literal begins with a ".  to put a " in a macro def, precede
 *             a " with a ".
 */
void parse_macro( int macro_key, char *residue )
{
int  rc;
char literal[1042];
char *l;
int  key_no;

   /*
    * reset any previous macro def.
    */
   initialize_macro( macro_key );
   while (residue != NULL) {
      /*
       * skip over any leading spaces.
       */
      while (*residue == ' ')
         ++residue;

      /*
       * done if we hit a comment
       */
      if (*residue == ';')
         residue = NULL;

      /*
       * check for a literal.
       */
      else if (*residue == '\"') {
         rc = parse_literal( macro_key, residue, literal, &residue );
         if (rc == OK) {
            l = literal;
            while (*l != '\0'  &&  rc == OK) {
               rc = record_keys( macro_key, *l );
               ++l;
            }
         } else {
            printf( "==> %s", line_in );
            printf( "Literal not recognized: line %u  : literal  %s\n", line_no, literal );
         }

      /*
       * check for a function key.
       */
      } else {
         residue = parse_token( residue, literal );
         key_no = search( literal, valid_keys, AVAIL_KEYS );
         if (key_no != ERROR)
            record_keys( macro_key, key_no+256 );
         else {
            printf( "==> %s", line_in );
            printf( "Unrecognized key: line %u  : key %s\n", line_no, literal );
         }
      }
   }
   check_macro( macro_key );
}


/*
 * Name:    parse_literal
 * Purpose: get all letters in a literal
 * Date:    June 5, 1992
 * Passed:  macro_key:  key that we are a assigning a macro to
 *          line:       current line position
 *          literal:    buffer to hold literal
 *          residue:    pointer to next token in line
 * Notes:   a literal begins with a ".  to put a " in a macro def, precede
 *             a " with a ".
 */
int  parse_literal( int macro_key, char *line, char *literal, char **residue )
{
int quote_state = 1;    /* we've already seen one " before we get here */

   line++;
   /*
    * put the characters into the literal array until we run into the
    *   end of literal or terminating '\0';
    */
   while (*line != '\0' && *line != '\n') {
      if (*line != '\"')
         *literal++ = *line++;
      else {
         if (*(line+1) == '\"') {
            *literal++ = '\"';
            line++;
            line++;
         } else {
            line++;
            --quote_state;
            break;
         }
      }
   }
   *literal = '\0';

   /*
    * return what's left on the line, if anything.
    */
   if (*line != '\0' && *line != '\n')
      *residue = line;
   else
      *residue = NULL;
   if (quote_state != 0) {
      *residue = NULL;
      return( ERROR );
   } else
      return( OK );
}


/*
 * Name:    initialize_macro
 * Purpose: initialize the first key of a macro def
 * Date:    June 5, 1992
 * Passed:  macro_key:  key that we are a assigning a macro to
 * Notes:   this function is ported directly from tde.
 */
void initialize_macro( int macro_key )
{
register int next;
int  prev;

   next = macros.first_stroke[macro_key];

   /*
    * initialize the first key in a macro def
    */
   if (next != STROKE_LIMIT+1) {
      do {
         prev = next;
         next = macros.strokes[next].next;
         macros.strokes[prev].key  = MAX_KEYS+1;
         macros.strokes[prev].next = STROKE_LIMIT+1;
         ++stroke_count;
      } while (next != -1);
   }

   /*
    * find the first open space and initialize
    */
   for (next=0; macros.strokes[next].next != STROKE_LIMIT+1;)
      next++;
   macros.first_stroke[macro_key] = next;
   macros.strokes[next].key  = -1;
   macros.strokes[next].next = -1;
}


/*
 * Name:    clear_previous_macro
 * Purpose: clear any macro previously assigned to a key
 * Date:    June 5, 1992
 * Passed:  macro_key:  key that we are a assigning a macro to
 * Notes:   this function is ported directly from tde.
 */
void clear_previous_macro( int macro_key )
{
register int next;
int prev;

   next = macros.first_stroke[macro_key];

   /*
    * if key has already been assigned to a macro, clear macro def.
    */
   if (next != STROKE_LIMIT+1) {
      do {
         prev = next;
         next = macros.strokes[next].next;
         macros.strokes[prev].key  = MAX_KEYS+1;
         macros.strokes[prev].next = STROKE_LIMIT+1;
      } while (next != -1);
   }

   macros.first_stroke[macro_key] = STROKE_LIMIT+1;
}


/*
 * Name:    check_macro
 * Purpose: see if macro def has any valid key.  if not, clear macro
 * Date:    June 5, 1992
 * Passed:  macro_key:  key that we are a assigning a macro to
 * Notes:   this function is ported directly from tde.
 */
void check_macro( int macro_key )
{
register int next;
register int key;

   /*
    * see if the macro has any keystrokes.  if not, then wipe it out.
    */
   key = macro_key;
   if (key != 0) {
      next = macros.first_stroke[key];
      if (macros.strokes[next].key == -1) {
         macros.strokes[next].key  = MAX_KEYS+1;
         macros.strokes[next].next = STROKE_LIMIT+1;
         macros.first_stroke[key-256] = STROKE_LIMIT+1;
         if (key_func.key[key] == PlayBack)
            key_func.key[key] = 0;
      }
   }
}


/*
 * Name:    record_keys
 * Purpose: save keystrokes in keystroke buffer
 * Date:    June 5, 1992
 * Passed:  line: line to display prompts
 * Notes:   -1 in .next field indicates the end of a recording
 *          STROKE_LIMIT+1 in .next field indicates an unused space.
 *           Recall, return codes are for macros.  Since we do not allow
 *           keys to be assigned to macro functions, let's just return OK;
 */
int  record_keys( int macro_key, int key )
{
register int next;
register int prev;
int func;
int rc;

   rc = OK;
   if (stroke_count == 0) {
      printf( "==> %s", line_in );
      printf( "No more room in macro buffer:  line %u\n",
               line_no );
      rc = ERROR;
   } else {
      func = getfunc( key );
      if (func != RecordMacro && func != SaveMacro && func != LoadMacro &&
          func != ClearAllMacros) {

         /*
          * a -1 in the next field marks the end of the keystroke recording.
          */
         next = macros.first_stroke[macro_key];
         if (macros.strokes[next].next != STROKE_LIMIT+1) {
            while (macros.strokes[next].next != -1)
               next = macros.strokes[next].next;
         }
         prev = next;

         /*
          * now find an open space to record the current key.
          */
         if (macros.strokes[next].key != -1) {
            for (; next < STROKE_LIMIT &&
                         macros.strokes[next].next != STROKE_LIMIT+1;)
               next++;
            if (next == STROKE_LIMIT) {
               for (next=0; next < prev &&
                            macros.strokes[next].next != STROKE_LIMIT+1;)
                  next++;
            }
         }
         if (next == prev && macros.strokes[prev].key != -1) {
            rc = ERROR;
         } else {
         /*
          * next == prev if we are recording the initial macro node.
          */
            macros.strokes[prev].next = next;
            macros.strokes[next].next = -1;
            macros.strokes[next].key  = key;
            stroke_count--;
         }
      }
   }
   return( rc );
}


/*
 * Name:    new_sort_order
 * Purpose: change the sort order
 * Date:    October 31, 1992
 * Notes:   New sort oder starts at the @ sign
 */
void new_sort_order( unsigned char *residue, unsigned char *sort )
{
int i;

   sort += 33;
   for (i=33; *residue != '\0'  &&  *residue != '\n' && i <= 255; i++)
      *sort++ = *residue++;
}


/*
 * Name:    get_stroke_count
 * Purpose: count unassigned nodes in macro buff
 * Date:    June 5, 1992
 * Returns: number of strokes left in macro buffer.
 */
int  get_stroke_count( void )
{
int count = 0;
int i;

   for (i=0; i<STROKE_LIMIT; i++)
      if (macros.strokes[i].next == STROKE_LIMIT+1)
         ++count;
   return( count );
}



/*
 * Name:    getfunc
 * Purpose: get the function assigned to key c
 * Date:    June 5, 1992
 * Passed:  c:  key just pressed
 * Notes:   key codes less than 256 or 0x100 are not assigned a function.
 *          The codes in the range 0-255 are ASCII and extended ASCII chars.
 */
int  getfunc( int c )
{
register int i = c;

   if (i <= 256)
      i = 0;
   else
      i = key_func.key[i-256];
   return( i );
}


/*
 * Name:    clear_previous_twokey
 * Purpose: clear any previously assigned two-key combos
 * Date:    April 1, 1993
 * Passed:  macro_key:  key that we are clearing
 */
void clear_previous_twokey( int two_key )
{
int i;

   for (i=0; i < MAX_TWO_KEYS; i++) {
      if (two_key == two_key_list.key[i].parent_key) {
         two_key_list.key[i].parent_key = 0;
         two_key_list.key[i].child_key  = 0;
         two_key_list.key[i].func       = 0;
      }
   }
}


/*
 * Name:    insert_twokey
 * Purpose: find an open slot and insert the new two-key combination
 * Date:    April 1, 1993
 * Passed:  parent_key:  1st key
 *          child_key:   2nd key
 *          func_no:     function number to assign to this combo
 * Notes:   find the first avaible open slot.  clear any previously defined
 *          previous parent-child combo.
 */
int  insert_twokey( int parent_key, int child_key, int func_no )
{
register int i;
int  rc;

   for (i=0; i < MAX_TWO_KEYS; i++) {
      if (parent_key == two_key_list.key[i].parent_key) {
         if (child_key == two_key_list.key[i].child_key) {
            two_key_list.key[i].parent_key = 0;
            two_key_list.key[i].child_key  = 0;
            two_key_list.key[i].func       = 0;
         }
      }
   }
   for (i=0; i < MAX_TWO_KEYS; i++) {
      if (two_key_list.key[i].parent_key == 0) {
         two_key_list.key[i].parent_key = parent_key;
         two_key_list.key[i].child_key  = child_key;
         two_key_list.key[i].func       = func_no;
         break;
      }
   }
   rc = OK;
   if (i == MAX_TWO_KEYS)
      rc = ERROR;
   return( rc );
}
