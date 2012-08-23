/*
 * cursor size
 */
#define SMALL_INS       0
#define BIG_INS         1


/*
 * flags used for opening files in binary or text mode.
 *   CRLF is for opening files in text mode - Operating System converts
 *   LF to CRLF automatically.  in binary mode, LF is not translated.
 */
#define CRLF            1
#define LF              2


char *mode_screen[] = {
"                      Default editor modes ",
" ",
"            Insert/Overwrite mode: ",
"                      Indent mode: ",
"                Physical Tab Size: ",
"                 Logical Tab Size: ",
"                   Smart tab mode: ",
"                ^Z at end of file: ",
"       End of line on file output: ",
"                   Trailing space: ",
"    Display end of line character: ",
"                   Word wrap mode: ",
"                      Left margin: ",
"                 Paragraph margin: ",
"                     Right margin: ",
"               Insert cursor size: ",
"         Create .bak backup files: ",
"           Ruler at top of window: ",
"                       Date style: ",
"                       Time style: ",
"         Initial search\\sort case: ",
"  ",
"  \x1b, \x1a = change values   \x18, \x19 = move cursor   F3 = Quit   F10 = Save and Quit",
NULL
};


struct {
   int line;
   int col;
   int mode;
   void (*show_me)( void );
} mode[19] = {
   {  3, 36, 0, show_insert_mode },
   {  4, 36, 0, show_indent_mode },
   {  5, 36, 0, show_ptabsize },
   {  6, 36, 0, show_ltabsize },
   {  7, 36, 0, show_smart_mode },
   {  8, 36, 0, show_controlz },
   {  9, 36, 0, show_eol_out },
   { 10, 36, 0, show_trail },
   { 11, 36, 0, show_eol_display },
   { 12, 36, 0, show_ww },
   { 13, 36, 0, show_left },
   { 14, 36, 0, show_para },
   { 15, 36, 0, show_right },
   { 16, 36, 0, show_cursor_size },
   { 17, 36, 0, show_backup_mode },
   { 18, 36, 0, show_ruler_mode },
   { 19, 36, 0, show_date_style },
   { 20, 36, 0, show_time_style },
   { 21, 36, 0, show_initcase }
};
