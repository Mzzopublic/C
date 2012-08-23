/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1992
 *
 * This file contains all user prompts in TDE.  Prompts were gathered into
 *  one file to make the job of translating English into other languages
 *  as easy as possible.
 */


char *cb      = "Control-Break pressed";


/*
 * block.c
 */
char *block1  = "a block is already defined in another file";

char *ltol    = "Error: line would be too long.";
char *block2  = "can only fill box blocks";
char *block3a = "can only number box blocks";
char *block3b = "can only swap box blocks";
char *block4  = "not enough memory for block";
char *block5  = "can only overlay box blocks";

char *block6  = "File name: ";
char *block7  = "File exists. Overwrite or Append? (o/a): ";
char *block8  = "writing block to '";
char *block9  = "could not write block";
char *block10 = "appending block to '";
char *block11 = "could not append block";
char *block12 = "writing block to '";

char *block13 = "Print file or block? (f/b): ";
char *block14 = "Printing line         of         Press Control-Break to cancel.";

char *block15 = "Enter character to fill block (ESC to exit): ";

char *block16 = "Enter starting number: ";
char *block17 = "Enter increment: ";
char *block18 = "Left or Right justify numbers in block (l/r)? ";

char *block20 = "can only expand tabs in line blocks";
char *block26 = "can only expand tabs in line blocks";

char *block21 = "can only trim trailing space in line blocks";

char *block22a = "Phase 1) Quicksort";
char *block22b = "Phase 2) Insertion sort";
char *block23 = "Can only sort Box blocks";
char *block24 = "Box block not marked";

char *block25 = "can only e-mail reply in line blocks";


/*
 * critical error handler
 */
char *critt1  = "N/A";


/*
 * diff
 */
char *diff_prompt1 = "DIFF:  Enter first window number and letter (e.g. 1a) : ";
char *diff_prompt2 = "DIFF:  Enter next window number and letter (e.g. 2a) : ";
char *diff_prompt3 = "DIFF:  Start diff at (B)eginning of file or (C)urrent position? (b/c) ";
char *diff_prompt4 = "DIFF:  Diffed until EOF(s).  No diff's were found.  Press a key.";
char *diff_prompt5 = "DIFF windows not defined:  ";
char *diff_prompt6a = "window '";
char *diff_prompt6b = "' not visible or does not exist  ";
char *diff_prompt7a = "DIFF:  Ignore leading spaces (y/n)?  ";
char *diff_prompt7b = "DIFF:  Ignore all space (y/n)?  ";
char *diff_prompt7c = "DIFF:  Ignore blank lines (y/n)?  ";
char *diff_prompt7d = "DIFF:  Ignore end of line (useful with reformatted paragraphs) (y/n)?  ";

char *diff_message = "diffing...";
char *diff_blank   = "          ";


/*
 * dir list
 */
char *dir1 = "Search path or pattern : ";
char *dir2 = "Invalid path or file name";
char *stardotstar = "*.*";
char *dir3 = "Out of memory";

char *dir4 = "Selected file : ";
char *dir5 = "    File size : ";
char *dir6 = "File count : ";

char *dir7 = "Cursor keys move.   Enter selects file or new directory";

/*
 * ed.c
 */
char *ed1  = "line too long to add";
char *ed2  = "cannot insert more characters";
char *ed3  = "no more room to add";

char *ed4  = "cannot combine lines";

char *ed5  = "cannot duplicate line";

char *ed7a = "Logical tab interval: ";
char *ed7b = "Physical (file) tab interval: ";
char *ed8  = "tab size too long";

char *ed9  = "Enter new left margin (must be less than right margin) : ";
char *ed10 = "Left margin out of range.";

char *ed11a = "Justify right margin?  (y/n): ";
char *ed11 = "Enter new right margin (must be greater than left margin) : ";
char *ed12 = "Right margin out of range.";

char *ed13 = "Enter paragraph margin (must be less than right margin) : ";
char *ed14 = "Paragraph margin out of range.";

char *ed15 = "File name to edit : ";

char *ed16 = "Macro execution halted:  Internal stack overflow.";
char *ed17 = "Macro execution halted:  Internal stack underflow.";

char *paused1 = "Paused:";
char *paused2 = "  Press ESC to halt macro   ";

/*
 * findrep.c
 */
char *find1  = "Options:  (P)rompt before replace   (N)o prompt (p/n): ";
char *find2  = "(R)eplace  (S)kip  (E)xit  (r/s/e)";
char *find3  = "Search has wrapped.  (C)ontinue or (Q)uit? (c/q): ";

char *find4  = "String to find: ";
char *find5a = "string \"";
char *find5b = "\" not found ";
char *find6  = "find pattern not defined";


char *find7[] = { "          ",
                  "wrapped...",
                  "searching ",
                  "replacing ",
                  "nfa choked"  };


char *find8  = "string not found";
char *find9  = "String to find: ";
char *find10 = "Replacement:    ";

char *find11 = "Line number: ";
char *find12 = "must be in the range 1 - ";


/*
 * hwind.c
 */
char *smart   = "S";
char *fixed   = "F";
char *intab   = "I";
char *outtab  = "D";
char *tabs    = "Tabs=";

char *indent  = "Indent";
char *blank   = "      ";

char *ignore = "Ignore";
char *match  = "Match ";

char *sync_off = "    ";
char *sync_on  = "Sync";

char *ww_mode[]  = { "  ",
                     "FW",
                     "DW" };

char *eol_mode[] = { "Native",
                     "CRLF  ",
                     "LF    ",
                     "Binary" };

/*
 * main.c
 */
char *main1   = "Fatal error: ";
char *main2   = "Warning: ";
char *main3   = ": press a key";

char *main4   = "out of memory";

char *main5   = "negative move - contact Frank Davis";

char *main6   = "File is write protected.  Overwrite anyway? (y/n): ";

char *main7a  = "File '";
char *main7b  = "'not found or error loading file";

char *main8a  = "file '";
char *main8b  = "'too big.  FILE WILL BE TRUNCATED IF SAVED!";

char *main9   = "error reading file '";

char *main10a = "file '";
char *main10b = "'too big";

char *main11  = "Press the key that will play back this recording : ";
char *main12  = "Cannot assign a recording to this key";
char *main13  = "No more room in recording buffer";
char *main14  = "Overwrite recording assigned to this key? (y/n) : ";

char *main15  = "Recording";

char *main18  = "  Avail strokes = ";

char *main19  = "Name for macro file name: ";
char *main20  = "Overwrite existing file? (y/n): ";

char *main21  = "Search path or file name for macro file : ";
char *main22  = "Invalid path or file name";


/*
 * regx.c
 */
char *reg1   = "Regular expression search (F1 = help): ";
char *reg2   = "unmatched open paren ";
char *reg3   = "unmatched close paren ";
char *reg4   = "char \'\\\' at end of string is not escaped";
char *reg5   = "class is not defined properly";
char *reg6   = "unmatched open bracket";
char *reg7   = "out of heap for class definition";
char *reg8   = "operator *, + , or ? error";
char *reg9   = "unmatched close bracket ";
char *reg10  = "incomplete range in character class ";
char *reg11  = "Can't parse two operators in a row ";


char *regx_help[] = {
"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",
"³   c = char    x = string    r,s = regular expression ³",
"ÃÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄ´",
"³  c      ³  any non-operator character  ³  Felis      ³",
"³  \\c     ³  c literally and C escapes   ³  catus\\.    ³",
"³  \\:c    ³  predefined macro            ³  \\:a+.?\\(   ³",
"³  .      ³  any character but newline   ³  c.t        ³",
"³  <      ³  beginning of word           ³  <cat       ³",
"³  >      ³  end of word                 ³  <cat>      ³",
"³  ^      ³  beginning of line           ³  ^cat>      ³",
"³  $      ³  end of line                 ³  cat$       ³",
"³  [x]    ³  any character in x          ³  [a-z0-9]   ³",
"³  [^x]   ³  any character not in x      ³  [^AEIOU]   ³",
"³  r*     ³  zero or more r's            ³  ca*t       ³",
"³  r+     ³  one or more r's             ³  ca[b-t]+   ³",
"³  r?     ³  zero or one r               ³  c.?t       ³",
"³  rs     ³  r followed by s             ³  ^$         ³",
"³  r|s    ³  either r or s               ³  kitty|cat  ³",
"³  (r)    ³  r                           ³  (c)?(a+)t  ³",
"ÀÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",
NULL
};


/*
 * utils.c
 */
char *utils1  = "line buffer overflow - rest of line ignored";
char *utils3  = "buffer full, part line truncated";

char *utils4  = "Sort (A)scending or (D)escending (a/d)? : ";

char *utils5  = "Replace (F)orward or (B)ackward (f/b)? : ";

char *utils6  = "Saving '";
char *utils7a = "file  '";
char *utils7b = "' is Read Only";
char *utils8  = "cannot write to '";

char *utils9  = "New file name: ";

char *utils10 = "Overwrite existing file? (y/n): ";

char *utils11 = "Error writing file or disk full";

char *utils12 = "Abandon changes? (y/n): ";

char *utils13 = "Marker   not set in this file";

char *utils14 = "Enter new file attributes, \"AaSsHhRr\" : ";
char *utils15 = "New file attributes not set";

char *utils16 = "No dirty lines found";

/*
 * window.c
 */
char *win1 = "move cursor up first";

char *win2 = "move cursor right first";
char *win3 = "move cursor left first";

char *win4 = "Press Up or Down to change window size.  Press Return when done.";
char *win5 = "can not resize vertical window";
char *win6 = "can not resize top window";

char *win7 = "cannot close current window";

char *win8 = "Invalid path or file name ";
char *win9 = "No more files to load ";

char *win16a = "Enter search string:  ";
char *win16b = "Enter regular expression search string:  ";
char *win17 = "Enter file names to search:  ";

char *win18 = "Press the <Grep> key for more files or any other key to quit:  ";

char *win19 = "Searching:  ";

/*
 * wordwrap.c
 */
char *ww1  = "line would be too long.";
