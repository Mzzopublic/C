

char *greatest_composer_ever = "EDITOR";


#include "tdestr.h"             
#include "common.h"
#include "define.h"
#include "help.h"
#include "tdefunc.h"


#include <dos.h>                /* 对文件重命名需要调用这个库 */
#include <bios.h>               /* 直接取得键盘的输入要需要调用这个库 */
#include <io.h>                 /* 取得文件的属性值需要调用这个库 */
#include <fcntl.h>              /* 打开文件的时候标志 */
#if defined( __MSC__ )
   #include <errno.h>
   #include <sys\types.h>       /* 定义了常量和诸如时间格式的数据类型 */
#endif
#include <sys\stat.h>           /* 定义了文件状态位等常量和全局数据 */

#if defined( __MSC__ )
void (interrupt far *old_control_c)( ); /* 为老的ctrl-c设置变量 */
void (interrupt far *old_int1b)( );     /* 为老的中断int 1b设置变量 */
#endif


/*
 * 原来的检查control-break标志位
 */
static int s_cbrk;


/*
 * 函数作用:	处理系统命令行调用主要的编辑函数。
 * 参数：		argc:  命令行参数的个数
 *				argv:  命令行参数
 */
void main( int argc, char *argv[] )
{
#if defined( __MSC__ )
   union REGS inregs, outregs;
#endif

   g_status.found_first = FALSE;
   g_status.arg         = 1;
   g_status.argc        = argc;
   g_status.argv        = argv;

   /*
    * control-break的中断对于我们程序没有破坏，所以我们把对它的检查关闭。
    */
#if defined( __MSC__ )
   inregs.h.ah = 0x33;
   inregs.h.al = 0;
   intdos( &inregs, &outregs );
   s_cbrk = outregs.h.dl;
   old_control_c = _dos_getvect( (unsigned)0x23 );
   _dos_setvect( 0x23, harmless );
   old_int1b = _dos_getvect( (unsigned)0x1b );
   _dos_setvect( 0x1b, ctrl_break );
   inregs.h.ah = 0x33;
   inregs.h.al = 1;
   inregs.h.dl = 0;
   intdos( &inregs, &outregs );
#else
   s_cbrk = getcbrk( );
   ctrlbrk( harmless );
   setcbrk( 0 );
#endif


   /*
    * 为程序建立一个简单的关键错误处理器
    */
   install_ceh( &ceh );
   ceh.flag = OK;

   initialize( );
   editor( );
   terminate( );
}


/*
 * Name:    error
 * Purpose: To report an error, and usually make the user type <ESC> before
 *           continuing.
 * Date:    June 5, 1991
 * Passed:  kind:   an indication of how serious the error was:
 *                      WARNING: continue after pressing a key
 *                      FATAL:   abort the editor
 *          line:    line to display message
 *          message: string to be printed
 * Notes:   Show user the message and ask for a key if needed.
 */
void error( int kind, int line, char *message )
{
char buff[MAX_COLS+2];          /* somewhere to store error before printing */
char line_buff[(MAX_COLS+2)*2]; /* buffer for char and attribute  */

   /*
    * tell the user what kind of an error it is
    */
   switch (kind) {
      case FATAL:
         /*
          * fatal error
          */
         assert( strlen( main1 ) < MAX_COLS );
         strcpy( buff, main1 );
         break;
     case WARNING:
     default:
         /*
          * warning
          */
         assert( strlen( main2 ) < MAX_COLS );
         strcpy( buff, main2 );
         break;
   }

   /*
    * prepare the error message itself
    */
   strcat( buff, message );

   /*
    * tell the user how to continue editing if necessary
    */
   if (kind == WARNING)
      /*
       * press a key
       */
      strcat( buff, main3 );

   /*
    * output the error message
    */
   save_screen_line( 0, line, line_buff );
   set_prompt( buff, line );

   if (kind == FATAL) {
      /*
       * no point in making the user type <ESC>, since the program is
       *  about to abort anyway...
       */
      terminate( );
      exit( 1 );
   }

   getkey( );
   restore_screen_line( 0, line, line_buff );
   if (g_status.wrapped) {
      g_status.wrapped = FALSE;
      show_search_message( CLR_SEARCH, g_display.mode_color );
   }
}


/*
 * Name:    harmless
 * Purpose: Do nothing when control-C is pressed
 * Date:    June 5, 1991
 * Notes:   Interrupt 23, the Control-C handler, is a MS DOS system function.
 *            Since we want to use Control-C as a regular function key,
 *            let's do absolutely nothing when Control-C is pressed.
 */
#if defined( __MSC__ )
void interrupt far harmless( void )
#else
static int harmless( void )
#endif
{
}


/*
 * Name:    ctrl_break
 * Purpose: Set our control-break flag when control-break is pressed.
 * Date:    June 5, 1992
 * Notes:   Control-break is a little different from Control-C.  When
 *           Control-C is pressed, MS DOS processes it as soon as possible,
 *           which may be quite a while.  On the other hand, when
 *           Control-break is pressed on IBM and compatibles, interrupt 0x1b
 *           is generated immediately.  Since an interrupt is generated
 *           immediately, we can gain control of run-away functions, like
 *           recursive macros, by checking our Control-break flag.
 */
void interrupt far ctrl_break( void )
{
   g_status.control_break = TRUE;
}


/*
 * 作用: 释放我们所有的动态结构和加载的结构
 */
void terminate( void )
{
union REGS inregs, outregs;
register WINDOW     *wp;        /* 来搜索窗口的寄存器 */
WINDOW              *w;         /* 释放窗口 */
register file_infos *fp;        /* 来搜索文件的寄存器  */
file_infos          *f;         /* 释放文件 */
int                 i;

   /*
    * 重建control-break检测
    */
#if defined( __MSC__ )
   _dos_setvect( 0x1b, old_int1b );
   _dos_setvect( 0x23, old_control_c );
   inregs.h.ah = 0x33;
   inregs.h.al = 1;
   inregs.h.dl = (char)s_cbrk;
   intdos( &inregs, &outregs );
#else
   setcbrk( s_cbrk );
#endif

   /*
    * 如果还有没有释放的文件结构，那么释放他们。
    */
   fp = g_status.file_list;
   while (fp != NULL) {
      f  = fp;
      fp = fp->next;
      free( f );
   }

   /*
    * 如果还有没有释放的窗口结构，那么释放他们。
    */
   wp = g_status.window_list;
   while (wp != NULL) {
      w  = wp;
      wp = wp->next;
      free( w );
   }


   /*
    * 释放在nfa中的字符类
    */
   for (i=0; i < REGX_SIZE; i++) {
      if (sas_nfa.class[i] == nfa.class[i]  &&  nfa.class[i] != NULL)
         free( nfa.class[i] );
      else if (sas_nfa.class[i] != NULL)
         free( sas_nfa.class[i] );
      else if (nfa.class[i] != NULL)
         free( nfa.class[i] );
   }


   /*
    * 重新设置光标的大小并且卸载83/84键盘工具集
    */
   set_cursor_size( mode.cursor_size == SMALL_INS ? g_display.insert_cursor :
                                                    g_display.overw_cursor );
   if (mode.enh_kbd == FALSE)
      simulate_enh_kbd( 0 );

   /*
    * 重建扫描颜色
    */
   if (g_display.adapter != MDA)
      set_overscan_color( g_display.old_overscan );
}


/*

 * 作用: 初始化不依赖硬件的屏幕状态信息
 *			并且调用硬件出世户程序来获得和硬件相关的信息
 
 * 返回: [g_status and g_display]
 * 注意: 这个函数假定g_status和g_display开始时储存的都是\0。
 */
void initialize( void )
{
int i;

   /*
    * 首先做硬件初始化
    */
   hw_initialize( );

   /*
    * 初始化编辑器的模式，指针和计数器
    */
   bm.search_defined        = ERROR;
   sas_bm.search_defined    = ERROR;
   g_status.sas_defined     = ERROR;
   g_status.sas_search_type = ERROR;

   regx.search_defined      = ERROR;
   sas_regx.search_defined  = ERROR;

   if (mode.undo_max < 2)
      mode.undo_max = 2;

   g_status.marked_file = NULL;
   g_status.current_window = NULL;
   g_status.current_file = NULL;
   g_status.window_list = NULL;
   g_status.file_list = NULL;
   g_status.buff_node = NULL;

   g_status.window_count = 0;
   g_status.file_count = 0;
   g_status.line_buff_len = 0;
   g_status.tabout_buff_len = 0;
   g_status.command = 0;
   g_status.key_pressed = 0;
   g_status.sas_rcol  = 0;
   g_status.sas_rline = 0;
   g_status.recording_key = 0;

   g_status.key_pending = FALSE;
   g_status.found_first = FALSE;
   g_status.sas_found_first = FALSE;
   g_status.copied = FALSE;
   g_status.wrapped = FALSE;
   g_status.marked = FALSE;
   g_status.macro_executing = FALSE;
   g_status.replace_defined = FALSE;

   g_status.screen_display = TRUE;

   g_status.file_chunk = DEFAULT_BIN_LENGTH;

   g_status.sas_tokens[0] = '\0';
   g_status.path[0] = '\0';
   g_status.sas_path[0] = '\0';
   g_status.rw_name[0] = '\0';
   g_status.pattern[0] = '\0';
   g_status.subst[0] = '\0';


   /*
    * 设置翻页后依然要显示出的当前的页中的行号。
    */
   g_status.overlap = 1;


   /*
    * 初始化nfa中的节点。
    */
   for (i=0; i < REGX_SIZE; i++) {
      sas_nfa.node_type[i] = nfa.node_type[i] = 0;
      sas_nfa.term_type[i] = nfa.term_type[i] = 0;
      sas_nfa.c[i] = nfa.c[i] = 0;
      sas_nfa.next1[i] = nfa.next1[i] = 0;
      sas_nfa.next2[i] = nfa.next2[i] = 0;
      sas_nfa.class[i] = nfa.class[i] = NULL;
   }

   /*
    * 没有在执行宏
    */
   connect_macros( );


   /*
    * 清屏
    */
   cls( );
   show_credits( );
}


/*
 * Name:    hw_initialize
 * Purpose: To initialize the display ready for editor use.
 * Date:    June 5, 1991
 */
void hw_initialize( void )
{
struct vcfg cfg;       /* defined in .h */
register int *clr;

   /*
    * set up screen size
    */
   g_display.ncols     = MAX_COLS;
   g_display.nlines    = MAX_LINES - 1;
   g_display.mode_line = MAX_LINES;
   g_display.line_length = MAX_LINE_LENGTH;

   /*
    * work out what kind of display is in use, and set attributes and
    *  display address accordingly. Note that this will only work with
    *  close IBM compatibles.
    */

   video_config( &cfg );
   g_display.display_address = (char far *)cfg.videomem;

   /*
    * Use an integer pointer to go thru the color array for setting up the
    * various color fields.
    */
   clr =  cfg.color == FALSE ? &colour.clr[0][0] : &colour.clr[1][0];

   g_display.head_color    = *clr++;
   g_display.text_color    = *clr++;
   g_display.dirty_color   = *clr++;
   g_display.mode_color    = *clr++;
   g_display.block_color   = *clr++;
   g_display.message_color = *clr++;
   g_display.help_color    = *clr++;
   g_display.diag_color    = *clr++;
   g_display.eof_color     = *clr++;
   g_display.curl_color    = *clr++;
   g_display.ruler_color   = *clr++;
   g_display.ruler_pointer = *clr++;
   g_display.hilited_file  = *clr++;
   g_display.overscan      = *clr;

   /*
    * set the overscan color.
    * in terminate( ), the overscan color is returned to old state.
    */
   if (g_display.adapter != MDA)
      set_overscan_color( g_display.overscan );
}


/*
 * Name:    get_help
 * Purpose: save the screen and display key definitions
 * Date:    June 5, 1991
 * Notes:   This routine is dependent on the length of the strings in the
 *          help screen.  To make it easy to load in a new help screen,
 *          the strings are assumed to be 80 characters long followed by
 *          the '\0' character.  It is assumed each that string contains
 *          exactly 81 characters.
 */
int  get_help( WINDOW *window )
{
register char *help;
register int line;

   xygoto( -1, -1 );
   help = help_screen[1];
   for (line=0; help != NULL; line++) {
      s_output( help, line, 0, g_display.help_color );
      help = help_screen[line+2];
   }
   line = getkey( );
   redraw_screen( window );
   return( OK );
}


/*
 * Name:    show_credits
 * Purpose: display authors
 * Date:    June 5, 1991
 */
void show_credits( void )
{
register char *credit;
int  line;

   xygoto( -1, -1 );
   credit = credit_screen[0];
   for (line=0; credit != NULL; ) {
      s_output( credit, line+2, 11, g_display.text_color );
      credit = credit_screen[++line];
   }
}
