
char *sample_screen[] = {
"ÚÄÄÄÄÄÄÄ´ Sample Help Screen ÃÄÄÄÄÄÄÄÄ¿ ÚÄÄÄÄÄÄÄÄ´ Sample Text Screen ÃÄÄÄÄÄÄÄÄ¿",
"³ * Enter, ,  move to next field    ³ ³ File Header    Size = xxx    Line:Col³",
"³ *  and \x1a change foreground         ³ ³....´..1........2.......3..Ã....³",
"³ * PgUp and PgDn change background   ³ ³ Regular text is this color.  text    ³",
"³                                     ³ ³ The cursor line is this color.       ³",
"³    F2 = Load default colors         ³ ³ Dirty lines are this color.          ³",
"³    F3 = Quit                        ³ ³ Block area.  Line, stream, and box   ³",
"³   F10 = Write colors to tde.exe     ³ ³ blocks are marked in this color.     ³",
"³                                     ³ ³ <====== End of File ======>          ³",
"³                                     ³ ³ Warning Messages                     ³",
"³             SAMPLE.FIL              ³ ³ Mode/Information         wrapped...  ³",
"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",
NULL
};


char *field_screen[] = {
" Help screen [   ]        Warning Messages [   ]           Ruler line     [   ] ",
" File Header [   ]        Mode/Information [   ]           Ruler pointer  [   ] ",
" Text Area   [   ]        wrapped...       [   ]           Block color    [   ] ",
" Cursor Line [   ]        End of File      [   ]           Hilited File   [   ] ",
" Dirty Line  [   ]        Overscan         [   ]                                ",
NULL
};

struct {
   int line;
   int col;
   int color;
   void (*show_me)( void );
} fields[NUM_COLORS] = {
   { 12, 13, 0 }, { 13, 13, 0 }, { 14, 13, 0 }, { 15, 13, 0 }, { 16, 13, 0 },
   { 12, 43, 0 }, { 13, 43, 0 }, { 14, 43, 0 }, { 15, 43, 0 }, { 16, 43, 0 },
   { 12, 74, 0 }, { 13, 74, 0 }, { 14, 74, 0 }, { 15, 74, 0 }
};

#define HERC_REVERSE    0x70
#define HERC_UNDER      0x01
#define HERC_NORMAL     0x07
#define HERC_HIGH       0x0F

#define COLOR_HEAD      0x4b
#define COLOR_TEXT      0x07
#define COLOR_DIRTY     0x07
#define COLOR_MODE      0x17
#define COLOR_BLOCK     0x71
#define COLOR_MESSAGE   0x0f
#define COLOR_HELP      0x1a
#define COLOR_WRAP      0x0e
#define COLOR_EOF       0x09
#define COLOR_CURL      0x0f
#define COLOR_RULER     0x02
#define COLOR_POINTER   0x0a
#define COLOR_OVRS      0x00


#define HEAD            0
#define TEXT            1
#define DIRTY           2
#define MODE            3
#define BLOCK           4
#define WARNING         5
#define HELP            6
#define WRAP            7
#define CEOF            8
#define CURL            9
#define RULER           10
#define RULER_PTR       11
#define HILITED_FILE    12
#define OVERSCAN        13
