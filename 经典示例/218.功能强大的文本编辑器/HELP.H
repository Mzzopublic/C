/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 *
 * This info is displayed in the intro screen and the pop-up help screen
 * when F1 is pressed.
 */

char *credit_screen[] = {
"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",
"³                                                    ³",
"³            TDE, the Thomson-Davis Editor           ³",
"³                    Version 3.1                     ³",
"³                  August 29, 1993                   ³",
"³                                                    ³",
"³                                                    ³",
"³      This program is released into the public      ³",
"³   domain.  You may use and distribute it freely.   ³",
"³                                                    ³",
"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",
NULL
};


char *help_screen[] = {
"$  help",
"ÄÄÄÄÄ Word Processing ÄÄÄÄÄÄÄÂ #-Shift   @-Alt   ^-Ctrl ÂÄÄÄÄ File commands ÄÄÄÄ",
"  @V  toggle word wrap       ÃÄÄÄÄÄ Block commands ÄÄÄÄÄ´@F1 dir list   @F2 attr",
"^F6/^F7  left/right margin   ³ @B  mark box block       ³ F2 save    #F2 save as",
"  ^F8  set paragraph margin  ³ @L  mark line block      ³ F3 quit     F4 file   ",
"  ^B   WordStar format pargr ³ @X  mark stream block    ³#F4 edit F  @F4 next F ",
"  @F8  left justify line     ³ @U  unmark block         ³#F12/^F12 define Grep  ",
"  @F9  right justify line    ³ @M  move    @C  copy     ³ F12 Repeat Grep       ",
" @F10  center justify line   ³ @K  kopy    @G  group delÃÄÄÄÄÄÄÄ Toggles ÄÄÄÄÄÄÄ",
"                             ³ @F  fill    @O  overlay  ³ ^F1  tog cursor sync  ",
"ÄÄÄÄÄ Window Commands ÄÄÄÄÄÄÄ´ @S  sort    @N  number   ³ ^F2  eol/tab display  ",
" F8 vertical     #F9 resize  ³#@S  swap BOX w/cursor    ³ ^F3  CRLF | LF at eol ",
" F9 horizontal   ^F9 zoom    ³ @<  up case  @>  lo case ³ ^F4  tog trim trailing",
"F10 next        #F10 previous³ @?  hi bit  #@?  e-mail  ³ ^F5  IGNORE/MATCH case",
"   ^F10  next hidden window  ³#@<  rot13   #@>  fix uue ³  @R  ruler   @I indent",
"ÄÄÄÄÄÄÄ Misc commands ÄÄÄÄÄÄÄ´ @T  trim trailing space  ³  @Z  tog ^Z at eof    ",
" @A  add L     ^2  Date stamp³ @P  print file/block     ³ #@T  tog inflate tabs ",
" @J  join L    ^]  paren bal ³ @W  write block to file  ³^tab  set ltabs/ptabs  ",
" @=  dup L     @-  kill eol  ³ @E  block expand tabs    ³@tab  tog smart tabs   ",
" @D  delete L     @1-@3  mark³#@C  block compress tabs  ÃÄÄ Find/Replace/Diff ÄÄ",
" ^_  split L    #@1-#@3  goto³#@I  block indent tabs    ³ #F5/#F6  define find  ",
"ESC  undo L      ^\\  redraw  ÃÄÄÄÄÄÄÄÄÄ Macros ÄÄÄÄÄÄÄÂÄÙ F5/F6 next/prev find  ",
" ^U  undelete L  ^T  word del³  @F3  record on/off    ³ #F7 define reg exp      ",
" Enter  Emacs L              ³  #F1  save    #F3 load ³ F7/@F7 next/prev reg exp",
"#Enter  next L       @Q  Quit³   ^P  pause            ³ #F11/F11 def/rep diff   ",
"^Enter  beg next L   @H  Help³ ^ESC  clear macros     ³ #F8 replace string      ",
NULL
};
