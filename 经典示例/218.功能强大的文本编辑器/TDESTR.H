


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <assert.h>
#if defined( __MSC__ )
   #include <malloc.h>          /* for memory allocation */
   #if defined( toupper )
       #undef toupper
   #endif
#else
   #include <alloc.h>           /* for memory allocation */
#endif


/*
 * defines for the inline assembler.
 */
#if defined( __MSC__ )
   #define  ASSEMBLE   _asm
#else
   #define  ASSEMBLE   asm
#endif


#define MAX_COLS            80  /* widest screen ever used */
#define MAX_LINES           24  /* highest screen ever used */
#define BUFF_SIZE         1042  /* buffer size for lines */
#define MAX_LINE_LENGTH   1040  /* longest line allowed in file */
#define FNAME_LENGTH        45  /* maximum file name length in lite bar */
#define NO_MARKERS           3  /* maximum no. of markers */
#define UNDO_STACK_LEN     200  /* number of lines in undo stack */

/*
 * when we read in a file, lets try to match the size of the read buffer
 *   with some multiple of a hardware or software cache that may be present.
 */
#define READ_LENGTH             1024
#define DEFAULT_BIN_LENGTH      64


#define REGX_SIZE               200     /* maximum number of nodes in nfa */


/*
 * general defines.
 */
#define ERROR             (-1)  /* abnormal termination */
#define OK                   0  /* normal termination */
#define TRUE                 1  /* logical true */
#define FALSE                0  /* logical false */

#define MAX_KEYS           256  /* number of special keys recognized by TDE */
#define MAX_TWO_KEYS       128  /* number of two key-combos allowed by TDE  */
#define STROKE_LIMIT      1024  /* number of key strokes in playback buffer */


#define STACK_UNDERFLOW      1  /* code for underflowing macro stack */
#define STACK_OVERFLOW       2  /* code for overflowing macro stack */
#define SAS_P               20  /* number of sas pointers to tokens */
#define NUM_FUNCS          139

#define NUM_COLORS          14  /* number of color fields in TDE */

/*
 * special cases for keyboard mapping -  see bottom of main.c
 */
#define RTURN           262           /* Return key = 262 */
#define ESC             258           /* Escape key = 258 */
#define CONTROL_BREAK   269           /* Control-Break = 269 */


/*
 * The following defines are used by the "error" function to indicate
 *  how serious the error is.
 */
#define WARNING         1    /* user must acknowledge, editor continues */
#define FATAL           2    /* editor aborts - very rare! */
#define INFO            3    /* display message, acknowledge, continue */


/*
 * define the type of block marked by user and block actions
 */
#define NOTMARKED       0    /* block type undefined */
#define BOX             1    /* block marked by row and column */
#define LINE            2    /* block marked by begin and end lines */
#define STREAM          3    /* block marked by begin and end characters */

#define MOVE            1
#define DELETE          2
#define COPY            3
#define KOPY            4
#define FILL            5
#define OVERLAY         6
#define NUMBER          7
#define SWAP            8

#define LEFT            1
#define RIGHT           2

#define ASCENDING       1
#define DESCENDING      2


/*
 * three types of ways to update windows
 */
#define LOCAL           1
#define NOT_LOCAL       2
#define GLOBAL          3

#define CURLINE         1
#define NOTCURLINE      2


/*
 * search/replace flags.
 */
#define BOYER_MOORE     0
#define REG_EXPRESSION  1

#define CLR_SEARCH      0
#define WRAPPED         1
#define SEARCHING       2
#define REPLACING       3
#define NFA_GAVE_UP     4

#define IGNORE          1
#define MATCH           2

#define PROMPT          1
#define NOPROMPT        2

#define FORWARD         1
#define BACKWARD        2

#define BEGIN           1
#define END             2


#define BEGINNING       1
#define CURRENT         2

/*
 * word wrap flag.
 */
#define NO_WRAP         0
#define FIXED_WRAP      1
#define DYNAMIC_WRAP    2

/*
 * date and time formats
 */
#define MM_DD_YY        0
#define DD_MM_YY        1
#define YY_MM_DD        2
#define MM_DD_YYYY      3
#define DD_MM_YYYY      4
#define YYYY_MM_DD      5

#define _12_HOUR        0
#define _24_HOUR        1


/*
 * used in interrupt 0x21 function xx for checking file status
 */
#define EXIST           0
#define WRITE           2
#define READ            4
#define READ_WRITE      6

#define NORMAL          0x00
#define READ_ONLY       0x01
#define HIDDEN          0x02
#define SYSTEM          0x04
#define VOLUME_LABEL    0x08
#define SUBDIRECTORY    0x10
#define ARCHIVE         0x20

/*
 * critical error def's
 */
#define RETRY           1
#define ABORT           2
#define FAIL            3


/*
 * flags used for opening files to write either in binary or text mode.
 * crlf is for writing files in text mode - Operating System converts
 * lf to crlf automatically on output.  in binary mode, lf is not translated.
 */
#define NATIVE          1
#define CRLF            2
#define LF              3
#define BINARY          4
#define TEXT            5

#define OVERWRITE       1
#define APPEND          2

/*
 * characters used in tdeasm.c to display eol and column pointer in ruler
 */
#define EOL_CHAR        0x11
#define RULER_PTR       0x19
#define RULER_FILL      0x2e
#define RULER_TICK      0x04
#define LM_CHAR         0xb4
#define RM_CHAR_RAG     0x3c
#define RM_CHAR_JUS     0xc3
#define PGR_CHAR        0x14


/*
 * character used two separate vertical screens
 */
#define VERTICAL_CHAR   0xba


/*
 * cursor size
 */
#define SMALL_INS       0
#define BIG_INS         1


/*
 * possible answers to various questions - see get_yn, get_ynaq and get_oa
 */
#define A_YES           1
#define A_NO            2
#define A_ALWAYS        3
#define A_QUIT          4
#define A_ABORT         5
#define A_OVERWRITE     6
#define A_APPEND        7

/*
 * The following defines specify which video attributes give desired
 *  effects on different display devices.
 * REVERSE is supposed to be reverse video - a different background color,
 *  so that even a blank space can be identified.
 * HIGH is supposed to quickly draw the user's eye to the relevant part of
 *  the screen, either for a message or for matched text in find/replace.
 * NORMAL is supposed to be something pleasant to look at for the main
 *  body of the text.
 */
#define VIDEO_INT       0x10

#define HERC_REVERSE    0x70
#define HERC_UNDER      0x01
#define HERC_NORMAL     0x07
#define HERC_HIGH       0x0f

#define COLOR_HEAD      0x4b
#define COLOR_TEXT      0x07
#define COLOR_DIRTY     0x02
#define COLOR_MODE      0x17
#define COLOR_BLOCK     0x71
#define COLOR_MESSAGE   0x0f
#define COLOR_HELP      0x1a
#define COLOR_DIAG      0x0e
#define COLOR_EOF       0x09
#define COLOR_CURL      0x0f
#define COLOR_RULER     0x02
#define COLOR_POINTER   0x0a
#define COLOR_OVRS      0x00

#define COLOR_80        3
#define MONO_80         7

#define VGA             3
#define EGA             2
#define CGA             1
#define MDA             0


#define SAS_DELIMITERS  " \n"

/*
 * let's explicitly treat characters as unsigned.
 */
typedef unsigned char far * text_ptr;


/*
 * "s_line_list" contains contains struct defs for a node of text.
 */
typedef struct s_line_list {
   text_ptr line;                       /* pointer to line */
   int      len;                        /* length of line */
   int      dirty;                      /* boolean - dirty line indicator */
   struct s_line_list far *next;        /* next line in doubly linked list */
   struct s_line_list far *prev;        /* prev line in doubly linked list */
} line_list_struc;

typedef line_list_struc far *line_list_ptr;


struct vcfg {
   int color;
   int rescan;
   int mode;
   int far *videomem;
};


typedef struct {
   char sig[8];                 /* signature, so we can find struct in .exe */
   int  clr[2][NUM_COLORS];     /* one array for mono and another for color */
} COLORS;


/*
 * structure for two key combos
 */
typedef struct {
   int parent_key;
   int child_key;
   int func;
} TWO_KEY_TYPE;


typedef struct {
   char sig[8];
   TWO_KEY_TYPE key[MAX_TWO_KEYS];
} TWO_KEY;


typedef struct {
   char sig[8];
   unsigned char key[MAX_KEYS];
} KEY_FUNC;


/*
 * structure used in directory list.
 */
typedef struct {
   char fname[14];              /* file name */
   long fsize;                  /* file size in bytes */
} FTYPE;


/*
 * stuff we need to know about how to display a directory listing.
 */
typedef struct {
   int  row;                    /* absolute row to display dir box */
   int  col;                    /* absolute column to display dir box */
   int  wid;                    /* absolute number of columns in dir box */
   int  hgt;                    /* absolute number of rows in dir box */
   int  max_cols;               /* number of columns of files in box */
   int  max_lines;              /* number of lines of files in box */
   int  cnt;                    /* file count */
   int  cols;                   /* logical number of columns in list */
   int  lines;                  /* logical number of rows in list */
   int  prow;                   /* logical number of rows in partial row */
   int  vcols;                  /* number of virtual columns, if any */
   int  nfiles;                 /* number of files on screen */
   int  avail;                  /* number of available slots for files */
   int  select;                 /* current file under cursor */
   int  flist_col[5];           /* offset from col to display each column */
} DIRECTORY;


typedef struct {
   int  pattern_length;                 /* number of chars in pattern */
   int  search_defined;                 /* search pattern defined? */
   unsigned char pattern[MAX_COLS];     /* search pattern */
   int  forward_md2;                    /* forward mini-delta2 */
   int  backward_md2;                   /* backward mini-delta2 */
   char skip_forward[256];              /* boyer array for forward searches */
   char skip_backward[256];             /* boyer array for back searches */
} boyer_moore_type;


/*
 * "mode_infos" contain the editor mode variables.  The configuration
 *  utility modifies this structure to custimize the start-up tde
 *  configuration
 */
typedef struct {
   char sig[8];                 /* signature, so we can find struct in .exe */
   int  color_scheme;           /* color to start out with */
   int  sync;                   /* sync the cursor movement command? */
   int  sync_sem;               /* sync the cursor movement command? */
   int  record;                 /* are we recording keystrokes? */
   int  insert;                 /* in insert mode? */
   int  indent;                 /* in auto-indent mode? */
   int  ptab_size;              /* physical tab stops */
   int  ltab_size;              /* logical tab stops */
   int  smart_tab;              /* smart tab mode on or off? */
   int  inflate_tabs;           /* inflate tabs?  T or F */
   int  search_case;            /* consider case? IGNORE or MATCH */
   int  enh_kbd;                /* type of keyboard */
   int  cursor_size;            /* insert cursor big or small? */
   char *eof;                   /* message to display at end of file */
   int  control_z;              /* write ^Z - t or f */
   int  crlf;                   /* <cr><lf> toggle CRLF or LF */
   int  trailing;               /* remove trailing space? T or F */
   int  show_eol;               /* show lf at eol? T or F */
   int  word_wrap;              /* in word wrap mode? */
   int  left_margin;            /* left margin */
   int  parg_margin;            /* column for 1st word in paragraph */
   int  right_margin;           /* right margin */
   int  right_justify;          /* boolean, justify right margin?  T or F */
   int  format_sem;             /* format semaphore */
   int  undo_max;               /* max number of lines in undo stack */
   int  do_backups;             /* create backup or ".bak" files? T or F */
   int  ruler;                  /* show ruler at top of window? T or F */
   int  date_style;             /* date style for date and time stamp */
   int  time_style;             /* time style for date and time stamp */
} mode_infos;


/*
 * "displays" contain all the status information about what attributes are
 *  used for what purposes, which attribute is currently set, and so on.
 * The editor only knows about one physical screen.
 */
typedef struct {
   int nlines;                  /* lines on display device */
   int ncols;                   /* columns on display device */
   int line_length;             /* length of longest line */
   int mode_line;               /* line to display editor modes - fmd */
   int head_color;              /* file header color */
   int text_color;              /* text area color */
   int dirty_color;             /* text area color */
   int mode_color;              /* mode line color - footer */
   int block_color;             /* hilited area of blocked region */
   int message_color;           /* color of editor messages */
   int help_color;              /* color of help screen */
   int diag_color;              /* color for diagnostics in mode line */
   int eof_color;               /* color for end of file line */
   int curl_color;              /* color of cursor line */
   int ruler_color;             /* color of ruler line */
   int ruler_pointer;           /* color of ruler pointer */
   int hilited_file;            /* color of current file in dir list */
   int overscan;                /* color of overscan color */
   int old_overscan;            /* color of old overscan   */
   int adapter;                 /* VGA, EGA, CGA, or MDA? */
   unsigned int overw_cursor;   /* hi part = top scan line, lo part = bottom */
   unsigned int insert_cursor;  /* hi part = top scan line, lo part = bottom */
   char far *display_address;   /* address of display memory */
} displays;


/*
 * "WINDOW" contains all the information that is unique to a given
 *  window.
 */
typedef struct s_window {
   struct s_file_infos *file_info;       /* file in window */
   line_list_ptr ll;            /* pointer to current node in linked list */
   int  ccol;                   /* column cursor logically in */
   int  rcol;                   /* column cursor actually in */
   int  bcol;                   /* base column to start display */
   int  cline;                  /* line cursor logically in */
   long rline;                  /* real line cursor in */
   long bin_offset;             /* offset, in bytes from beg of bin file */
   int  top_line;               /* top line in window */
   int  bottom_line;            /* bottom line in window */
   int  vertical;               /* boolean. is window split vertically? */
   int  start_col;              /* starting column on physical screen */
   int  end_col;                /* ending column on physical screen */
   int  page;                   /* no. of lines to scroll for one page */
   int  visible;                /* window hidden or visible */
   int  letter;                 /* window letter */
   int  ruler;                  /* show ruler in this window? */
   char ruler_line[MAX_COLS+2]; /* ruler for this window */
   struct s_window *next;       /* next window in doubly linked list */
   struct s_window *prev;       /* previous window in doubly linked list */
} WINDOW;


/*
 * struct for find first and find next functions.  see DOS technical ref
 * manuals for more info on DTA structures.
 */
typedef struct {
   char          reserved[21];
   unsigned char attrib;
   unsigned      time;
   unsigned      date;
   long          size;
   char          name[13];
} DTA;


/*
 * "status_infos" contain all the editor status information that is
 *  global to the entire editor (i.e. not dependent on the file or
 *  window)
 */
typedef struct {
   WINDOW *current_window;      /* current active window */
   struct s_file_infos *current_file; /* current active file */
   struct s_file_infos *file_list; /* all active files */
   WINDOW *window_list;         /* all active windows */
   int  window_count;           /* number of windows - displayed and hidden */
   int  file_count;             /* number of files currently open */
   int  arg;                    /* current argument pointer */
   int  found_first;            /* have we found the first file? */
   int  argc;                   /* argument count */
   char **argv;                 /* argument variables - same as *argv[] */
   char path[MAX_COLS];         /* path for files on command line */
   DTA  dta;                    /* disk transfer address for find next */
   int  file_mode;              /* mode to open sas files, TEXT or BINARY */
   int  file_chunk;             /* if opened BINARY, bytes per line */
   int  sas_defined;            /* has search and seize been defined */
   int  sas_search_type;        /* if defined, Boyer-Moore or reg expression? */
   int  sas_arg;                /* current argument pointer */
   int  sas_argc;               /* argument count */
   char **sas_argv;             /* argument variables - same as *argv[] */
   char sas_tokens[MAX_COLS];   /* storage for file tokens */
   char sas_path[MAX_COLS];     /* path for files on search list */
   char *sas_arg_pointers[SAS_P]; /* array of pointers to tokens in sas_path */
   int  sas_found_first;        /* have we found the first file? */
   DTA  sas_dta;                /* disk transfer address for find next */
   long sas_rline;              /* line number of sas find */
   int  sas_rcol;               /* column number of sas find */
   int  sas_mode;               /* mode to open sas files, TEXT or BINARY */
   int  sas_chunk;              /* if opened BINARY, bytes per line */
   line_list_ptr sas_ll;        /* linked list node pointing to line */
   int  marked;                 /* has block been marked? */
   int  prompt_line;            /* line to display cursor */
   int  prompt_col;             /* column to display cursor */
   struct s_file_infos *marked_file;  /* pointer to file w/ marked block */
   char rw_name[MAX_COLS];      /* name of last file read or written */
   char pattern[MAX_COLS];      /* last search pattern */
   char subst[MAX_COLS];        /* last substitute text */
   int  replace_flag;           /* prompt or noprompt b4 replacing */
   int  replace_defined;        /* replace defined ? */
   int  overlap;                /* overlap between pages for page up etc */
   line_list_ptr buff_node;     /* address of node in line_buff */
   int  copied;                 /* has line been copied to file? */
   char line_buff[BUFF_SIZE*2+8]; /* for currently edited line */
   char tabout_buff[BUFF_SIZE+8]; /* for exanding tabs */
   int  line_buff_len;          /* length of line in the line_buff */
   int  tabout_buff_len;        /* length of line in the tabout_buff */
   int  command;                /* function of key just entered */
   int  key_pressed;            /* key pressed by user */
   int  key_pending;            /* is two-key command waiting for next key? */
   int  first_key;              /* first key of the two-key sequence */
   int  wrapped;                /* is the wrapped message on mode line? */
   int  stop;                   /* stop indicator */
   int  control_break;          /* control break pressed? */
   int  recording_key;          /* key we are assigning keystrokes to */
   int  stroke_count;           /* free keys in stroke buffer */
   int  macro_next;             /* pointer to next key in macro */
   int  macro_executing;        /* flag set if macro is executing */
   int  mstack_pointer;         /* pointer to macro stack */
   int  current_macro;          /* index of currently executing macro */
   int  screen_display;         /* screen display off or on? */
} status_infos;


/*
 * marker structure used to save file positions.
 */
typedef struct {
   long rline;                  /* real line of marker */
   int  rcol;                   /* real column of marker */
   int  ccol;                   /* logical column of marker */
   int  bcol;                   /* base column of marker */
   int  marked;                 /* boolean:  has this marker been set? */
} MARKER;


/*
 * "file_infos" contain all the information unique to a given file
 */
typedef struct s_file_infos {
   line_list_ptr line_list;    /* pointer to first node in linked list */
   line_list_ptr line_list_end; /* pointer to last node in linked list */
   line_list_ptr undo_top;     /* pointer to top node in undo stack */
   line_list_ptr undo_bot;     /* pointer to last node in undo stack */
   int  undo_count;            /* number of lines in undo stack */
   MARKER marker[NO_MARKERS];  /* number of file markers */
   long length;                /* number of lines in file */
   int  modified;              /* file has been modified since save? */
   int  dirty;                 /* file in window modified? */
   int  new_file;              /* is current file new? */
   int  backed_up;             /* has ".bak" file been created? */
   int  crlf;                  /* when read, did lines end CRLF or LF? */
   int  open_mode;             /* opened in BINARY or TEXT */
   char file_name[MAX_COLS];   /* name of current file being edited */
   char backup_fname[MAX_COLS];/* name of backup of current file */
   int  block_type;            /* block type - line or box */
   line_list_ptr block_start;  /* beginning block position */
   line_list_ptr block_end;    /* ending block position */
   int  block_bc;              /* beginning column */
   long block_br;              /* beginning row */
   int  block_ec;              /* ending column */
   long block_er;              /* ending row */
   int  file_no;               /* file number */
   int  ref_count;             /* no. of windows referring to file */
   int  next_letter;           /* next window letter */
   unsigned int  file_attrib;  /* file attributes (rwx etc) */
   struct s_file_infos *next;  /* next file in doubly linked list */
   struct s_file_infos *prev;  /* previous file in doubly linked list */
} file_infos;


/*
 * structure for recording and playing back one keystroke.
 */
typedef struct {
  int key;      /* key assinged to this node, which may be text or function */
  int next;     /* pointer to next node in macro def */
} STROKES;


/*
 * structure for the macro buffer.
 */
typedef struct {
   char sig[8];                         /* signature, easy to find in .exe */
   int  first_stroke[MAX_KEYS];         /* pointer to first key in macro */
   STROKES strokes[STROKE_LIMIT];       /* buffer to hold key strokes */
} MACRO;


/*
 * structure for the local macro stack in macro processor.
 */
typedef struct {
   int key;                             /* key to begin execution in macro */
   int macro;                           /* macro we were executing */
} MACRO_STACK;


/*
 * structure for critical error handler.  check the flag for errors on
 * each I/O call.
 */
typedef struct {
   int  flag;                   /* 0 = no error, -1 = error */
   int  code;                   /* error code from di */
   int  rw;                     /* read or write operation? */
   int  drive;                  /* drive number of error */
   int  extended;               /* extended error code, func 0x59 */
   int  class;                  /* error class */
   int  action;                 /* suggested action from DOS */
   int  locus;                  /* locus of error: drive, network, etc... */
   int  dattr;                  /* device attribute, 0 = drive, 1 = serial */
   char dname[10];              /* device name */
} CEH;


/*
 * structure for sort strings
 */
typedef struct {
   text_ptr pivot_ptr;
   int  direction;
   unsigned char *order_array;
   int  block_len;
   int  pivot_len;
   int  bc;
   int  ec;
} SORT;


typedef struct {
   unsigned char ignore[256];
   unsigned char match[256];
} SORT_ORDER;


/*
 * structure for diff.  let's only diff two windows at a time.
 */
typedef struct {
   int       defined;           /* initially FALSE */
   int       leading;           /* skip leading spaces t/f */
   int       all_space;         /* skip all space t/f */
   int       blank_lines;       /* skip blank lines t/f */
   int       ignore_eol;        /* skip end of line t/f */
   WINDOW    *w1;               /* pointer to first diff window */
   WINDOW    *w2;               /* pointer to second diff window */
   line_list_ptr  d1;           /* pointer to text in window 1 */
   line_list_ptr  d2;           /* pointer to text in window 2 */
   long      rline1;            /* line number in window 1 */
   long      rline2;            /* line number in window 2 */
   long      bin_offset1;       /* binary offset in window 1 */
   long      bin_offset2;       /* binary offset in window 2 */
} DIFF;


/*
 * structures for regular expression searches.
 */
typedef struct {
   int  pattern_length;                 /* number of chars in pattern */
   int  search_defined;                 /* search pattern defined? */
   int  node_count;                     /* number of nodes in the nfa */
   unsigned char pattern[MAX_COLS];     /* search pattern */
} REGX_INFO;


typedef struct {
   int  node_type[REGX_SIZE];
   int  term_type[REGX_SIZE];
   int  c[REGX_SIZE];
   char *class[REGX_SIZE];
   int  next1[REGX_SIZE];
   int  next2[REGX_SIZE];
} NFA_TYPE;


typedef struct parse_tree {
   int node_type;
   int leaf_char;
   char *leaf_string;
   struct parse_tree *r_pt;
   struct parse_tree *l_pt;
} PARSE_TREE;
