/*


#include "tdestr.h"
#include "common.h"
#include "define.h"
#include "tdefunc.h"

#include <bios.h>               /* for direct BIOS keyboard input */


/*******************************************************/
/*                                                     */
/*  1) initialize the video and keyboard structures    */
/*                                                     */
/*******************************************************/


/*
 *   BIOS Data Areas
 *
 *   See:
 *
 *      IBM Corp., _Technical Reference:  Personal Computer AT_,
 *      Boca Raton, Florida, 1984, IBM part no. 1502494,
 *      pp 5-29 thru 5-32.
 *
 *  These addresses, variable names, types, and descriptions are directly
 *   from the IBM AT Technical Reference manual, BIOS listing, copyright IBM.
 *
 *   Address  Name           Type   Description
 *   0x0449   CRT_MODE       Byte   Current CRT mode
 *   0x044a   CRT_COLS       Word   Number columns on screen
 *   0x044c   CRT_LEN        Word   length of regen buffer, video buffer, bytes
 *   0x044e   CRT_START      Word   Starting address of regen buffer
 *   0x0450   CURSOR_POSN    Word   cursor for each of up to 8 pages.
 *   0x0460   CURSOR_MODE    Word   current cursor mode setting.
 *   0x0462   ACTIVE_PAGE    Byte   Current page
 *   0x0463   ADDR_6845      Word   base address for active display card
 *   0x0465   CRT_MODE_SET   Byte   current mode of display card
 *   0x0466   CRT_PALETTE    Byte   overscan color for CGA, EGA, and VGA.
 *   0x0467   io_rom_init    Word   Pointer to optional i/o rom init routine
 *   0x0469   io_rom_seg     Word   Pointer to io rom segment
 *   0x046b   intr_flag      Byte   Flag to indicate an interrupt happened
 *   0x046c   timer_low      Word   Low word of timer count
 *   0x046e   timer_high     Word   High word of timer count
 *   0x0470   timer_ofl      Byte   Timer has rolled over since last count
 *   0x0471   bios_break     Byte   Bit 7 = 1 if Break Key has been hit
 *   0x0472   reset_flag     Word   Word = 1234h if keyboard reset underway
 *   0x0484   ROWS           Byte   Number of displayed character rows - 1
 *   0x0485   POINTS         Word   Height of character matrix
 *   0x0487   INFO           Byte   EGA and VGA display data
 *   0x0488   INFO_3         Byte   Configuration switches for EGA and VGA
 *   0x0489   flags          Byte   Miscellaneous flags
 *   0x0496   kb_flag_3      Byte   Additional keyboard flag
 *   0x048A   DCC            Byte   Display Combination Code
 *   0x04A8   SAVE_PTR       Dword  Pointer to BIOS save area
 */
void video_config( struct vcfg *cfg )
{
#pragma pack( 1 )    /* Use pragma to force packing on byte boundaries. */

struct LOWMEMVID
{
   char     vidmode;           /* 0x449 */
   unsigned scrwid;            /* 0x44A */
   unsigned scrlen;            /* 0x44C */
   unsigned scroff;            /* 0x44E */
   struct   LOCATE
   {
      unsigned char col;
      unsigned char row;
   } csrpos[8];                /* 0x450 */
   struct   CURSIZE
   {
      unsigned char end;
      unsigned char start;
   } csrsize;                  /* 0x460 */
   char      page;             /* 0x462 */
   unsigned  addr_6845;        /* 0x463 */
   char      crt_mode_set;     /* 0x465 */
   char      crt_palette;      /* 0x466 */
   char      system_stuff[29]; /* 0x467 */
   char      rows;             /* 0x484 */
   unsigned  points;           /* 0x485 */
   char      ega_info;         /* 0x487 */
   char      info_3;           /* 0x488 */
   char      skip[13];         /* 0x489 */
   char      kb_flag_3;        /* 0x496 */
} vid;
struct LOWMEMVID _far *pvid = &vid;
#pragma pack( )    /* revert to previously defined pack pragma. */

union REGS in, out;
unsigned char temp, active_display;

   /*
    * Move system information into our video structure.
    */
   _fmemmove( pvid, (void far *)0x00000449l, sizeof( vid ) );

   cfg->rescan = FALSE;
   in.x.ax =  0x1a00;
   int86( VIDEO_INT, &in, &out );
   temp = out.h.al;
   active_display = out.h.bl;
   if (temp == 0x1a && (active_display == 7 || active_display == 8))
      g_display.adapter = VGA;
   else {
      in.h.ah =  0x12;
      in.h.bl =  0x10;
      int86( VIDEO_INT, &in, &out );
      if (out.h.bl != 0x10) {         /* EGA */
         if (vid.ega_info & 0x08)
            g_display.adapter = vid.addr_6845 == 0x3d4 ? CGA : MDA;
         else
            g_display.adapter = EGA;
      } else if (vid.addr_6845 == 0x3d4)
         g_display.adapter = CGA;
      else
         g_display.adapter = MDA;
   }

   if (g_display.adapter == CGA || g_display.adapter == EGA) {
      if (g_display.adapter == CGA)
         cfg->rescan = TRUE;
      g_display.insert_cursor = mode.cursor_size == SMALL_INS ? 0x0607 : 0x0407;
      g_display.overw_cursor = mode.cursor_size == SMALL_INS ? 0x0407 : 0x0607;
   } else {
      g_display.insert_cursor = mode.cursor_size == SMALL_INS ? 0x0b0c : 0x070b;
      g_display.overw_cursor = mode.cursor_size == SMALL_INS ? 0x070b : 0x0b0c;
   }

   cfg->mode = vid.vidmode;
   if (vid.addr_6845 == 0x3D4) {
      cfg->color = TRUE;
      cfg->videomem = (void far *)0xb8000000l;
   } else {
      cfg->color = FALSE;
      cfg->videomem = (void far *)0xb0000000l;
   }

   /*
    * let save the overscan color
    */
   g_display.old_overscan = vid.crt_palette;


   /*
    * Get keyboard type.  Since there is no interrupt that determines
    * keyboard type, use this method.  Look at bit 4 in keyboard flag3.
    * This method is not always foolproof on clones.
    */
   if ((vid.kb_flag_3 & 0x10) != 0)
      mode.enh_kbd = TRUE;
   else
      mode.enh_kbd = FALSE;
   if (mode.enh_kbd == FALSE)
      simulate_enh_kbd( 1 );
}


/*********************************************/
/*                                           */
/*  2) keyboard i/o routines, PC specific    */
/*                                           */
/*********************************************/


/*
 * The next function was written by Tom Waters, twaters@relay.nswc.navy.mil, and
 * modified extensively by Frank Davis.
 *
 * "I use ANSI.SYS to redefine a few of my function keys and this causes a
 * problem when getch() is used in a program.  For example, instead of returning
 * 321 for the F7, I get the redefined string inserted in the text. So, instead
 * of using getch(), I use the following function ..."
 *
 * Tom, thanks for the info and solution.  I'm sure your function will be
 * be appreciated by everyone with ANSI key defs, Frank.
 */

/*
 * Name:    getkey
 * Purpose: use bios to get keyboard input (getch has trouble w/ ANSI
 *          redefined keys)
 * Date:    July 2, 1991
 * Modified:July 12, 1991, Frank Davis - accept ALT-xxx keyboard entry
 *          September 10, 1991, Frank Davis - add support for Ctrl+Up, etc...
 * Passed:  None
 * Notes:   Uses the BIOS to read the next keyboard character.  Service 0x00
 *           is the XT keyboard read.  Service 0x10 is the extended keyboard
 *           read.  Test for a bunch of special cases.  Allow the user to enter
 *           any ASCII or Extended ASCII code as a normal text characters.
 *
 *          Control @ is defined as 0.  we need to do a special test
 *           for this key, otherwise it's interpretted as an Alt key.  It's
 *           the only "regular" Control key that returns 0 in AL and the scan
 *           byte in AH.  The "regular" Control keys are those in the range
 *           0-31 and they return the Control character in AL and the scan
 *           code in AH.  All of the Alt + keys return 0 in AL and
 *           the scan code in ah.
 *
 *          This function was written for US keyboards.  It may have to be
 *           modified for other keyboards, eg. Belgium, Canada, Czech,
 *           Slovak, Denmark, France, Germany, etc...
 *
 *          if Ctrl-Break is pressed, it returns 0xffff as the key pressed.
 *           let's set it to CONTROL_BREAK == 269 and do nothing.
 */
int getkey( void )
{
unsigned key, num_lock, control, shift;
register scan;
register unsigned lo;

/*
 * this code is used during testing to check the amount of memory
 *    in the near heap.
 *
 * char buff[MAX_COLS];
 * ultoa( _memavl( ), buff, 10 );
 * s_output( "h=       ", g_display.mode_line, 37, g_display.mode_color );
 * s_output( buff, g_display.mode_line, 39, g_display.mode_color );
 */

   /*
    * lets spin on waitkey until the user presses a key.  waitkey polls
    *  the keyboard and returns 0 when a key is waiting.
    */
   while (waitkey( mode.enh_kbd ));

   /*
    *  _bios_keybrd == int 16.  It returns the scan code in ah, hi part of key,
    *   and the ascii key code in al, lo part of key.  If the character was
    *   entered via ALT-xxx, the scan code, hi part of key, is 0.
    */
   if (mode.enh_kbd) {
      key = _bios_keybrd( 0x10 );
      lo  = _bios_keybrd( 0x12 );

      /*
       * couple of special cases:
       *   on the numeric keypad, some keys return 0xe0 in the lo byte.
       *   1) if user enters Alt-224, then the hi byte == 0 and lo byte == 0xe0.
       *   we need to let this get thru as an Extended ASCII char.  2) although
       *   we could make the cursor pad keys do different functions than the
       *   numeric pad cursor keys, let's set the 0xe0 in the lo byte to zero
       *   and forget about support for those keys.
       */
      if ((key & 0x00ff) == 0x00e0 && (key & 0xff00) != 0)
         key = key & 0xff00;
   } else {
      key = _bios_keybrd( 0 );
      lo  = _bios_keybrd( 2 );
   }
   num_lock = lo & 0x20;
   control  = lo & 0x04;
   shift    = lo & 0x03;
   scan = (key & 0xff00) >> 8;
   lo = key & 0X00FF;

   if (lo == 0) {
      /*
       * special case for Control @, which is defined as 0 or NULL.
       */
      if (scan == 3)
         lo = 430;

      /*
       * when first byte is 0, map it above 256, so that we can
       *  let ALT-xxx keys map to their 'natural' place.  In
       *  otherwords, use 0-255 as normal text characters and
       *  those >= 256 are function keys.
       */
      else
         lo = scan | 0x100;

   /*
    * now test for Control-Break.  let's set this to do nothing in the
    *  editor.  manually map Control-Break to 269 - DO NOT assign
    *  any function to 269.
    */
   } else if (key == 0xffff)
      lo = CONTROL_BREAK;


   /*
    * Pressing Control+BackSpace generates the 0x7f character.  Instead of
    * 0x7f, make lo the ASCII backspace character.  If anyone wants the
    * 0x7f character, then they can enter it via ALT+xxx.
    */
   if (scan == 14 && lo == 0x7f)
      lo = 8;

   /*
    * At the bottom of page 195 in MASM 6.0 ref manual, "..when the keypad
    *  ENTER and / keys are read through the BIOS interrupt 16h, only E0h
    *  is seen since the interrupt only gives one-byte scan codes."
    */
   else if (scan == 0xe0) {
      /*
       * plain Grey Enter
       */
      if (lo == 13 && !shift)
         lo = 285;
      /*
       * shift Grey Enter
       */
      else if (lo == 13)
         lo = 298;
      /*
       * control Grey Enter
       */
      else if (lo == 10)
         lo = 299;
   }

   /*
    *  let's massage all of the control key combinations.
    */
   if (lo < 32) {

      /*
       * My machine at home is sorta weird.  On every machine that I've
       *  tested at the awffice, the ALT-xxx combination returns 0 for the
       *  scan byte and xxx for the ASCII code.  My machine returns 184 (decimal)
       *  as the scan code?!?!?  I added the next two lines for my machine at
       *  home.  I wonder if someone forgot to zero out ah for Alt keypad entry
       *  when they wrote my bios?
       */
      if (scan > 0x80)
         scan = 0;

      /*
       * Separate the ESC key from the ^[ key.  The scan code for the ESC
       *  key is 1.  Map this to a different index into the key function
       *  array just in case someone wants to define ESC or ^[ to different
       *  functions.  BTW, ESC and ^[ return the same ASCII code, 27.
       */
      else if (scan == 1) {
         if (shift)
            lo = 259;
         else if (control)
            lo = 260;
         else
            lo = 258;
      }

      /*
       * Scan code for Enter = 28.  Separate the various Enter keys.
       */
      else if (scan == 28) {
         if (shift)
            lo = 263;
         else if (control)
            lo = 264;
         else
            lo = 262;
      }

      /*
       * Scan code for Backspace = 14.  Separate the various BackSpace keys.
       */
      else if (scan == 14) {
         if (shift)
            lo = 266;
         else if (control)
            lo = 267;
         else
            lo = 265;
      }

      /*
       * Scan code for Tab = 15.  Separate the tab key.
       */
      else if (scan == 15) {
         lo = 268;
      }

      /*
       * if scan code is not 0, then a Control key was pressed.  Map
       *  those keys to the WordStar commands.
       */
      else if (scan > 0)
         lo += 430;

   } else if (!num_lock) {
      switch (scan) {
         /*
          * scan code for grey - == 74.  if num_lock is not toggled, assign it
          *  to the scroll line up function.
          */
         case 74 :
            lo = 423;
            break;

         /*
          * scan code for grey + == 78.  if num_lock is not toggled, assign it
          *  to the scroll line down function.  if shift grey + then stationary
          *  scroll down.
          */
         case 78 :
            lo = 424;
            break;
      }
   }


   /*
    * let's set up for the Shift+Alt and Control+Alt keys.
    *  With these key combinations, we can do the International keyboard
    *  stuff, see the Microsoft MS DOS 5.0 manual pages 623-637.
    */
   if (lo > 256 && (shift || control)) {

      /*
       * add 55 to Ctrl+Left thru Ctrl+Home when the shift key is pressed.
       *  this is not part of the International keyboard stuff, just a way
       *  to assign the horizontal scroll left and right funcs to cursor keys.
       */
      if (shift) {
         if (lo >= 371 && lo <= 374)
            lo += 55;

         /*
          * if shift is down, map alt 1! thru alt =+ to shift alt 1! thru alt =+
          */
         else if (lo >= 376 && lo <= 387)
            lo += 86;

         /*
          * internation keyboard stuff
          */
         else if (lo >= 272 && lo <= 309)
            lo += 202;
      }
   }

   /*
    * map the enter key to line feed.
    */
   if (lo == 10  &&  scan != 0)
      lo = 425;
   return( lo );
}


/*
 * Name:    waitkey
 * Purpose: call the BIOS keyboard status subfunction
 * Date:    October 31, 1992
 * Passed:  enh_keyboard:  boolean - TRUE if 101 keyboard, FALSE otherwise
 * Returns: 1 if no key ready, 0 if key is waiting
 * Notes:   lets get the keyboard status so we can spin on this function until
 *           the user presses a key.  some OS replacements for DOS want
 *           application programs to poll the keyboard instead of rudely
 *           sitting on the BIOS read key function.
 */
int  waitkey( int enh_keyboard )
{
   return( _bios_keybrd( enh_keyboard ? 0x11 : 0x01 ) == 0 ? 1 : 0 );
}


/*
 * Name:    getfunc
 * Purpose: get the function assigned to key c
 * Date:    July 11, 1991
 * Passed:  c:  key just pressed
 * Notes:   key codes less than 256 or 0x100 are not assigned a function.
 *           The codes in the range 0-255 are ASCII and extended ASCII chars.
 */
int getfunc( int c )
{
register int i = c;
int  key_found;

   if (!g_status.key_pending) {
      i = c;
      if (i <= 256)
         i = 0;
      else
         i = key_func.key[i-256];
   } else {

      /*
       * allow the user to enter two-key combinations
       */
      key_found = FALSE;
      for (i=0; i < MAX_TWO_KEYS; i++) {
         if (g_status.first_key == two_key_list.key[i].parent_key &&
                               c == two_key_list.key[i].child_key) {
            i = two_key_list.key[i].func;
            key_found = TRUE;
            break;
         }
      }
      if (key_found == FALSE)
         i = ERROR;
   }
   return( i );
}


/*
 * Name:    two_key
 * Purpose: set the two_key flag and prompt for the next key.
 * Date:    April 1, 1992
 * Notes:   this function accepts two key commands.
 */
int  two_key( WINDOW *arg_filler )
{
   s_output( "Next Key..", g_display.mode_line, 67, g_display.diag_color );
   g_status.key_pending = TRUE;
   g_status.first_key = g_status.key_pressed;
   return( OK );
}


/*
 * Name:    flush_keyboard
 * Purpose: flush keys from the keyboard buffer
 * Date:    June 5, 1993
 * Passed:  enh_keyboard:  boolean - TRUE if 101 keyboard, FALSE otherwise
 * Returns: 1 if no key ready, 0 if key is waiting
 * Notes:   lets get the keyboard status so we can spin on this function until
 *           the user presses a key.  some OS replacements for DOS want
 *           application programs to poll the keyboard instead of rudely
 *           sitting on the BIOS read key function.
 */
void flush_keyboard( void )
{
   if (mode.enh_kbd) {
      while (!waitkey( mode.enh_kbd ))
         _bios_keybrd( 0x10 );
   } else {
      while (!waitkey( mode.enh_kbd ))
         _bios_keybrd( 0 );
   }
}


/*********************************************/
/*                                           */
/*  3) video output routines, PC specific    */
/*                                           */
/*********************************************/


/*
 * Name:    xygoto
 * Purpose: To move the cursor to the required column and line.
 * Date:    September 28, 1991
 * Passed:  col:    desired column (0 up to max)
 *          line:   desired line (0 up to max)
 * Notes;   use standard BIOS video interrupt 0x10 to set the set.
 */
void xygoto( int col, int line )
{
union REGS inregs, outregs;

   inregs.h.ah = 2;
   inregs.h.bh = 0;
   inregs.h.dh = (unsigned char)line;
   inregs.h.dl = (unsigned char)col;
   int86( 0x10, &inregs, &outregs );
}


/*
 * Name:    update_line
 * Purpose: Display the current line in window
 * Date:    June 5, 1991
 * Passed:  window:  pointer to current window
 * Notes:   Show string starting at column zero and if needed blank rest
 *           of line.  Put max_col in cx and count down.  When we run into
 *           len, cx contains number of columns to blank out.  Use the
 *           fast 'rep stosw' to clear the end of line.
 *          The C routine was probably fast enough, but let's do some
 *           assembly because it's so fun.
 *          To handle line lengths longer than 255 characters,
 *           block begin and end columns were changed from real
 *           to virtual columns in this display routine.
 */
void update_line( WINDOW *window )
{
text_ptr text;      /* current character of orig begin considered */
char far *screen_ptr;
int  off;
int  attr;
int  line;
int  col;
int  bcol;
int  bc;
int  ec;
int  normal;
int  block;
int  max_col;
int  block_line;
int  len;
int  show_eol;
int  c;
long rline;
file_infos *file;

   if (window->rline > window->file_info->length || window->ll->len == EOF
             || !g_status.screen_display)
      return;
   file = window->file_info;
   max_col = window->end_col + 1 - window->start_col;
   line = window->cline;
   normal = window->ll->dirty == FALSE ? g_display.text_color : g_display.dirty_color;
   block = g_display.block_color;
   show_eol = mode.show_eol;

   /*
    * set the screen pointer to physical screen memory.
    *       160 = 80 chars + 80 attr  for each line
    */
   screen_ptr = g_display.display_address;
   off = line * 160 + window->start_col * 2;

   /*
    * figure which line to display.
    * actually, we could be displaying any line in any file.  we display
    *  the line_buffer only if window->ll == g_status.buff_node
    *  and window->ll-line has been copied to g_status.line_buff.
    */
   text = window->ll->line;
   len  = window->ll->len;
   if (g_status.copied && ptoul( window->ll ) == ptoul( g_status.buff_node )) {
      text = (text_ptr)g_status.line_buff;
      len  = g_status.line_buff_len;
   }
   if (mode.inflate_tabs)
      text = tabout( text, &len );

   /*
    * lets look at the base column.  if the line to display is shorter
    *  than the base column, then set text to eol and we can't see the
    *  eol either.
    */
   bc = window->bcol;
   if (bc > 0) {
      if (text == NULL) {
         show_eol = FALSE;
         len = 0;
      } else {
         if ((col = len) < bc) {
            bc = col;
            show_eol = FALSE;
         }
         text += bc;
         len  -= bc;
      }
   }

   /*
    * for display purposes, set the line length to screen width if line
    *  is longer than screen.  our assembly routine uses bytes and the
    *  the line length can be longer than a byte.
    */
   if (len > max_col)
      len = max_col;

   bcol = window->bcol;
   rline = window->rline;
   if (file->block_type && rline >= file->block_br && rline <= file->block_er)
      block_line = TRUE;
   else
      block_line = FALSE;

   /*
    * do this if 1) a box block is marked, or 2) a stream block begins
    *  and ends on the same line.
    */
   if (block_line == TRUE && (file->block_type == BOX ||
         (file->block_type == STREAM &&
         rline == file->block_br && rline == file->block_er))) {

      /*
       * start with the bc and ec equal to physical block marker.
       */
      bc = file->block_bc;
      ec = file->block_ec;
      if (ec < bcol || bc >= bcol + max_col)
         /*
          * we can't see block if ending column is less than the base col or
          *  the beginning column is greater than max_col.
          */
         ec = bc = max_col + 1;
      else if (ec < bcol + max_col) {
         /*
          * if the ec is less than the max column, make ec relative to
          *  base column then figure the bc.
          */
         ec = ec - bcol;
         if (bc < bcol)
            bc = 0;
         else
            bc = bc - bcol;
      } else if (bc < bcol + max_col) {
         /*
          * if the bc is less than the max column, make bc relative to
          *  base column then figure the ec.
          */
         bc = bc - bcol;
         if (ec > bcol + max_col)
            ec = max_col;
         else
            ec = ec - bcol;
      } else if (bc < bcol  &&  ec >= bcol + max_col) {
         /*
          * if the block is wider than the screen, make bc start at the
          *  logical begin and make ec end at the logical end of the
          *  window.
          */
         bc = 0;
         ec = max_col;
      }


   ASSEMBLE {
        /*
        ; Register strategy:
        ;   bl == beginning column
        ;   bh == ending column
        ;   dl == normal text attribute
        ;   dh == block attribute
        ;   cl == current virtual column
        ;   ch == maximum columns in window
        ;   ES:DI == screen pointer (destination)
        ;   DS:SI == text pointer (source)
        ;   [bp+show_eol]  == access for local C variable
        */


        push    ds                      /* MUST save ds - push it on stack */
        push    si                      /* save si on stack */
        push    di                      /* save di on stack */

/*
;
; set up local register variables
;
*/
        mov     ax, WORD PTR bc         /* get beginning column */
        mov     bl, al                  /* keep it in bl */
        mov     ax, WORD PTR ec         /* get ending column */
        mov     bh, al                  /* keep it in bh */
        mov     ax, WORD PTR normal     /* get normal attribute */
        mov     dl, al                  /* keep it in dl */
        mov     ax, WORD PTR block      /* get block attribute */
        mov     dh, al                  /* keep it in dh */
        mov     ax, WORD PTR max_col    /* get max number columns on screen */
        mov     ch, al                  /* keep it in ch */
        xor     cl, cl                  /* col = 0, keep col in cl */
/*
;
; load screen and text pointer
;
*/
        mov     di, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     di, WORD PTR off                /* add offset of line */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     si, WORD PTR text       /* load OFFSET of text ptr */
        mov     ax, WORD PTR text+2     /* load SEGMENT of text ptr */
        mov     ds, ax                  /* move segment of text in ds */
        cmp     si, 0                   /* is offset of text ptr == NULL? */
        jne     not_null                /* no, output string */
        cmp     ax, 0                   /* is segment of text ptr == NULL? */
        je      dspl_eol                /* yes, clear end of line */
   }
not_null:

   ASSEMBLE {
        cmp     cl, ch                  /* is col == max_col? */
        jge     getout                  /* yes, thru with line */
        cmp     cl, BYTE PTR len        /* is col == len? */
        je      dspl_eol                /* yes, must check block past eol */
   }
top:

   ASSEMBLE {
        lodsb                   /* get next char in string */
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      ch_out1         /* yes, show char and normal attribute */
        cmp     cl, bh          /* is col > ec? (greater than ending col) */
        jg      ch_out1         /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - block attribute */
   }
ch_out1:

   ASSEMBLE {
        stosw                   /* now show char on screen w/ attribute */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        jge     getout          /* yes, thru with line */
        cmp     cl, BYTE PTR len        /* is col == len? */
        jl      top             /* yes, must check block past eol */
   }
dspl_eol:

   ASSEMBLE {
        cmp     WORD PTR show_eol, FALSE        /* look at the show_eol flag */
        je      block_eol       /* show_eol flag is FALSE, blank line */
        mov     al, EOL_CHAR    /* load some eol indicator */
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      ch_out2         /* yes, show char and normal attribute */
        cmp     cl, bh          /* is col > ec? (greater than ending col) */
        jg      ch_out2         /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - show block attribute */
   }
ch_out2:

   ASSEMBLE {
        stosw                   /* write eol and attribute to screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        je      getout          /* yes, we're done */
   }
block_eol:

   ASSEMBLE {
        mov     al, ' '         /* clear rest of line w/ spaces */
   }
b1:

   ASSEMBLE {
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      ch_out3         /* yes, show char and normal attribute */
        cmp     cl, bh          /* is col > ec? (greater than ending col) */
        jg      ch_out3         /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - show block attribute */
   }
ch_out3:

   ASSEMBLE {
        stosw                   /* write blank and attribute to screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        jl      b1              /* while less output block */
   }
getout:

   ASSEMBLE {
        pop     di
        pop     si
        pop     ds
      }

/*
      screen_ptr += off;
      bcol = window->start_col;
      for (col=0; col < max_col; col++, bcol++) {
         attr = normal;
         if (col >= bc && col <= ec)
            attr = block;
         if (col < len)
            c = *text++;
         else if (col == len && show_eol)
            c = EOL_CHAR;
         else
            c = ' ';
         *screen_ptr++ = c;
         *screen_ptr++ = attr;

         c_output( c, bcol, line, attr );
      }
*/
   } else if (block_line == TRUE && file->block_type == STREAM &&
              (rline == file->block_br || rline == file->block_er)) {
      if (rline == file->block_br)
         bc = file->block_bc;
      else {
         bc = file->block_ec + 1;
         ec = normal;
         normal = block;
         block = ec;
      }
      if (bc < bcol)
         bc = 0;
      else if (bc < bcol + max_col)
         bc = bc - bcol;
      else
         bc = max_col + 1;


   ASSEMBLE {
        /*
        ; Register strategy:
        ;   bl == beginning column
        ;   bh == relative line length
        ;   dl == normal text attribute
        ;   dh == block attribute
        ;   cl == current virtual column
        ;   ch == maximum columns in window
        ;   ES:DI == screen pointer (destination)
        ;   DS:SI == text pointer (source)
        ;   [bp+show_eol]  == access for local C variable
        */

        push    ds                      /* MUST save ds - push it on stack */
        push    si                      /* save si on stack */
        push    di                      /* save di on stack */

/*
;
; set up local register variables
;
*/
        mov     ax, WORD PTR bc         /* get beginning column */
        mov     bl, al                  /* keep it in bl */
        mov     ax, WORD PTR len        /* get relative line length */
        mov     bh, al                  /* keep it in bh */
        mov     ax, WORD PTR normal     /* get normal attribute */
        mov     dl, al                  /* keep it in dl */
        mov     ax, WORD PTR block      /* get block attribute */
        mov     dh, al                  /* keep it in dh */
        mov     ax, WORD PTR max_col    /* get max number columns on screen */
        mov     ch, al                  /* keep it in ch */
        xor     cl, cl                  /* col = 0, keep col in cl */
/*
;
; load screen and text pointer
;
*/
        mov     di, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     di, WORD PTR off                /* add offset of line */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     si, WORD PTR text       /* load OFFSET of text ptr */
        mov     ax, WORD PTR text+2     /* load SEGMENT of text ptr */
        mov     ds, ax                  /* move segment of text in ds */
        cmp     si, 0                   /* is offset of text ptr == NULL? */
        jne     nott_null               /* no, output string */
        cmp     ax, 0                   /* is segment of text ptr == NULL? */
        je      ddspl_eol               /* yes, clear end of line */
   }
nott_null:

   ASSEMBLE {
        cmp     cl, ch          /* is col == max_col? */
        je      ggetout         /* yes, thru with line */
        cmp     cl, bh          /* is col == len? */
        je      ddspl_eol       /* yes, check eol display */
   }
ttop:

   ASSEMBLE {
        lodsb                   /* get next char in string */
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      str_out1        /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - show block attribute */
   }
str_out1:

   ASSEMBLE {
        stosw                   /* else show char on screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        je      ggetout         /* yes, thru with line */
        cmp     cl, bh          /* is col == len? */
        jl      ttop            /* yes, check eol display */
   }
ddspl_eol:

   ASSEMBLE {
        cmp     WORD PTR show_eol, FALSE        /* look at the show_eol flag */
        je      stream_eol      /* show_eol flag is FALSE, blank line */
        mov     al, EOL_CHAR    /* load some eol indicator */
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      str_out2        /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - show block attribute */
   }
str_out2:

   ASSEMBLE {
        stosw                   /* write blank and attribute to screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        jge     ggetout         /* yes, we're done */
   }
stream_eol:

   ASSEMBLE {
        mov     al, ' '         /* clear rest of line w/ spaces */
   }
c1:

   ASSEMBLE {
        mov     ah, dl          /* assume normal attribute */
        cmp     cl, bl          /* is col < bc? (less than beginning col) */
        jl      str_out3        /* yes, show char and normal attribute */
        mov     ah, dh          /* must be in a block - show block attribute */
   }
str_out3:

   ASSEMBLE {
        stosw                   /* write blank and attribute to screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        jl      c1              /* while less output block */
   }
ggetout:

   ASSEMBLE {
        pop     di
        pop     si
        pop     ds
      }

/*
      screen_ptr += off;
      bcol = window->start_col;
      for (col=0; col < max_col; col++, bcol++) {
         attr = normal;
         if (col >= bc && col <= ec)
            attr = block;
         if (col < len)
            c = *text++;
         else if (col == len && show_eol)
            c = EOL_CHAR;
         else
            c = ' ';
         *screen_ptr++ = c;
         *screen_ptr++ = attr;
         c_output( c, bcol, line, attr );
      }
*/
   } else {
      if (block_line)
         attr = block;
      else
         attr = normal;

      assert( len >= 0 );
      assert( len <= MAX_COLS );

   ASSEMBLE {
        /*
        ; Register strategy:
        ;   bl == normal text attribute
        ;   bh == relative line length
        ;   cl == current virtual column
        ;   ch == maximum columns in window
        ;   ES:DI == screen pointer (destination)
        ;   DS:SI == text pointer (source)
        ;   [bp+show_eol]  == access for local C variable
        */

        mov     dx, ds          /* MUST save ds - keep it in dx */
        push    di              /* save di on stack */
        push    si              /* save si on stack */


        mov     bx, WORD PTR attr               /* keep attribute in bl */
        mov     ax, WORD PTR len                /* get normalized len */
        mov     bh, al                          /* keep len in bh */
        mov     cx, WORD PTR max_col            /* get max_col in cx */
        mov     ch, cl                          /* keep it in ch */
        xor     cl, cl                          /* zero out cl */
        mov     di, WORD PTR screen_ptr         /* load OFFST of screen ptr */
        add     di, WORD PTR off                /* add offset of line */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     si, WORD PTR text       /* load OFFSET of text ptr */
        mov     ax, WORD PTR text+2     /* load SEGMENT of text ptr */
        mov     ds, ax                  /* move segment of text in ds */
        cmp     si, 0                   /* is offset of pointer == NULL? */
        jne     nnot_null               /* no, output string */
        cmp     ax, 0                   /* is segment of pointer == NULL? */
        je      normeol                 /* yes, then clear rest of line */
   }
nnot_null:

   ASSEMBLE {
        mov     ah, bl                  /* get attribute */
        cmp     cl, ch          /* compare count with max columns */
        jge     getoutt         /* yes, thru with line */
        cmp     cl, bh          /* compare count with len */
        je      normeol         /* yes, clear end of line */
   }
topp:

   ASSEMBLE {
        lodsb                   /* get next char in string */
        stosw                   /* else show char on screen */
        inc     cl              /* ++col, count up to max_column */
        cmp     cl, ch          /* compare count with max columns */
        jge     getoutt         /* yes, thru with line */
        cmp     cl, bh          /* compare count with len */
        jl      topp            /* yes, clear end of line */
   }
normeol:

   ASSEMBLE {
        cmp     WORD PTR show_eol, FALSE        /* look at the show_eol flag */
        je      clreol          /* show_eol flag is FALSE, blank line */
        mov     al, EOL_CHAR    /* load some eol indicator */
        mov     ah, bl          /* assume normal attribute */
        stosw                   /* write blank and attribute to screen */
        inc     cl              /* ++col */
        cmp     cl, ch          /* is col == max_col? */
        jge     getoutt         /* yes, we're done */
   }
clreol:

   ASSEMBLE {
        mov     ah, bl          /* get attribute */
        mov     al, ' '         /* clear eol with ' ' */
        sub     ch, cl          /* find number of columns left */
        mov     cl, ch          /* put number of column left in cl */
        xor     ch, ch          /* clear ch */
        rep     stosw           /* count is in cx - set rest of line to ' ' */
   }
getoutt:

   ASSEMBLE {
        pop     si
        pop     di
        mov     ds, dx
      }

/*
      screen_ptr += off;
      bcol = window->start_col;
      for (col=0; col < max_col; col++, bcol++) {
         attr = normal;
         if (col < len)
            c = *text++;
         else if (col == len && show_eol)
            c = EOL_CHAR;
         else
            c = ' ';
         *screen_ptr++ = c;
         *screen_ptr++ = attr;
         c_output( c, bcol, line, attr );
      }
*/
   }
}


/*
 * Name:    c_output
 * Purpose: Output one character on prompt lines
 * Date:    June 5, 1991
 * Passed:  c:     character to output to screen
 *          col:   col to display character
 *          line:  line number to display character
 *          attr:  attribute of character
 * Returns: none
 */
void c_output( int c, int col, int line, int attr )
{
void far *screen_ptr;
int  off;

   screen_ptr = (void far *)g_display.display_address;
   off = line * 160 + col * 2;

   ASSEMBLE {
        mov     bx, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     bx, WORD PTR off                /* add offset of line:col */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     cx, WORD PTR attr       /* get attribute */
        mov     ah, cl                  /* put in ah */
        mov     cx, WORD PTR c          /* get character */
        mov     al, cl                  /* put in al */
        mov     WORD PTR es:[bx], ax    /* show char on screen */
   }

/*
   screen_ptr = g_display.display_address + line * 160 + col * 2;
   *screen_ptr++ = c;
   *screen_ptr = attr;
*/
}


/*
 * Name:    s_output
 * Purpose: To output a string
 * Date:    June 5, 1991
 * Passed:  s:     string to output
 *          line:  line to display
 *          col:   column to begin display
 *          attr:  color to display string
 * Notes:   This function is used to output most strings not part of file text.
 */
void s_output( char far *s, int line, int col, int attr )
{
void far *screen_ptr;
int  off;
int  max_col;

   max_col = g_display.ncols;
   screen_ptr = (void far *)g_display.display_address;
   off = line * 160 + col * 2;

   ASSEMBLE {
        push    ds              /* save ds on stack */
        push    di              /* save di on stack */
        push    si              /* save si on stack */

        mov     bx, WORD PTR attr               /* keep attribute in bx */
        mov     cx, WORD PTR col                /* put cols in cx */
        mov     dx, WORD PTR max_col            /* keep max_col in dx */
        mov     di, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     di, WORD PTR off                /* add offset of line:col */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     si, WORD PTR s  /* load offset of string ptr */
        or      si, si          /* is it == NULL? */
        je      getout          /* yes, no output needed */
        mov     ax, WORD PTR s+2        /* load segment of string ptr */
        or      ax, ax          /* is pointer == NULL? */
        je      getout          /* yes, no output needed */
        mov     ds, ax          /* load segment of text in ds */
        mov     ah, bl          /* put attribute in AH */
   }
top:

   ASSEMBLE {
        cmp     cx, dx          /* col < max_cols? */
        jge     getout          /* no, thru with line */
        lodsb                   /* get next char in string - put in al */
        or      al, al          /* is it '\0' */
        je      getout          /* yes, end of string */
        cmp     al, 0x0a        /* is it '\n'? */
        je      getout          /* yes, end of string */
        stosw                   /* else show attr + char on screen (ah + al) */
        inc     cx              /* col++ */
        jmp     SHORT top       /* get another character */
   }
getout:

   ASSEMBLE {
        pop     si              /* get back si */
        pop     di              /* get back di */
        pop     ds              /* get back ds */
   }

/*
   screen_ptr = g_display.display_address + line * 160 + col * 2;
   max_col = g_display.ncols;
   while (*s && col < max) {
      *screen_ptr++ = *s++;
      *screen_ptr++ = attr;
   }
*/
}


/*
 * Name:    eol_clear
 * Purpose: To clear the line from col to max columns
 * Date:    June 5, 1991
 * Passed:  col:   column to begin clear
 *          line:  line to clear
 *          attr:  color to clear
 * Notes:   Basic assembly
 */
void eol_clear( int col, int line, int attr )
{
int  max_col;
void far *screen_ptr;
int  off;

   max_col = g_display.ncols;
   screen_ptr = (void far *)g_display.display_address;
   off = line * 160 + col * 2;

   ASSEMBLE {
        push    di                              /* save di on stack */

        mov     bx, WORD PTR attr               /* keep attribute in bx */
        mov     dx, WORD PTR col                /* put cols in dx */
        mov     cx, WORD PTR max_col            /* put max_col in cx */
        cmp     dx, cx                          /* max_cols < cols? */
        jge     getout                          /* no, thru with line */
        sub     cx, dx                          /* number of column to clear */
        mov     di, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     di, WORD PTR off                /* add offset of line:col */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     ah, bl                          /* get attribute in ah */
        mov     al, ' '                         /* store ' ' in al */
        rep     stosw                           /* clear to end of line */
   }
getout:

   ASSEMBLE {
        pop     di                              /* get back di from stack */
   }

/*
   for (; col < g_display.ncols; col++) {
      *p++ = ' ';
      *p++ = attr;
   }
*/
}


/*
 * Name:    window_eol_clear
 * Purpose: To clear the line from start_col to end_col
 * Date:    June 5, 1991
 * Passed:  col:   column to begin clear
 *          line:  line to clear
 *          attr:  color to clear
 * Notes:   Basic assembly
 */
void window_eol_clear( WINDOW *window, int attr )
{
int  max_col;
void far *screen_ptr;
int  off;

   if (!g_status.screen_display)
      return;
   screen_ptr = (void far *)g_display.display_address;
   off = window->cline * 160 + window->start_col * 2;
   max_col = window->end_col + 1 - window->start_col;

   ASSEMBLE {
        push    di                              /* save di on stack */

        mov     bx, WORD PTR attr               /* keep attribute in bx */
        mov     cx, WORD PTR max_col            /* put max_col in cx */
        mov     di, WORD PTR screen_ptr         /* load OFFSET of screen ptr */
        add     di, WORD PTR off                /* add offset of line:col */
        mov     ax, WORD PTR screen_ptr+2       /* load SEGMENT of screen ptr */
        mov     es, ax
        mov     ah, bl                          /* get attribute in ah */
        mov     al, ' '                         /* store ' ' in al */
        rep     stosw                           /* clear to end of line */

        pop     di                              /* get back di from stack */
   }

/*
   for (; col < g_display.ncols; col++) {
      *p++ = ' ';
      *p++ = attr;
   }
*/
}


/*
 * Name:    hlight_line
 * Date:    July 21, 1991
 * Passed:  x:     column to begin hi lite
 *          y:     line to begin hi lite
 *          lgth:  number of characters to hi lite
 *          attr:  attribute color
 * Notes:   The attribute byte is the hi byte.
 */
void hlight_line( int x, int y, int lgth, int attr )
{
int  off;
void far *screen_ptr;

   screen_ptr = (void far *)g_display.display_address;
   off = y * 160 + 2 * x + 1;  /* add one - so it points to attribute byte */

   ASSEMBLE {
        push    di              /* save di */

        mov     cx, lgth        /* number of characters to change color */

        mov     di, WORD PTR screen_ptr /* get destination - video memory */
        add     di, off                 /* add offset */
        mov     ax, WORD PTR screen_ptr+2
        mov     es, ax
        mov     ax, attr        /* attribute */
   }
lite_len:

   ASSEMBLE {
        stosb                   /* store a BYTE */
        inc     di              /* skip over character to next attribute */
        loop    lite_len        /* change next attribute */
        pop     di              /* restore di */
   }
}


/*
 * Name:    cls
 * Purpose: clear screen
 * Date:    June 5, 1991
 * Notes:   Call the video BIOS routine to clear the screen.
 */
void cls( void )
{
int  line;

     line = g_display.nlines + 1;

   ASSEMBLE {
        xor     ch, ch                  /* starting row in ch = 0 */
        xor     cl, cl                  /* starting column in cl = 0 */
        mov     ax, WORD PTR line       /* get ending row */
        mov     dh, al                  /* put it in dh */
        mov     dl, 79                  /* ending column in dl = 79 */
        mov     bh, 7                   /* attribute in bh  = 7 (normal) */
        mov     al, 0                   /* get number of lines */
        mov     ah, 6                   /* get function number */
        push    bp                      /* some BIOS versions wipe out bp */
        int     0x10
        pop     bp
   }
}


/*
 * Name:    set_cursor_size
 * Purpose: To set cursor size according to insert mode.
 * Date:    June 5, 1991
 * Passed:  csize:  desired cursor size
 * Notes:   use the global display structures to set the cursor size
 */
void set_cursor_size( int csize )
{
   ASSEMBLE {
        mov     ah, 1                   /* function 1 - set cursor size */
        mov     cx, WORD PTR csize      /* get cursor size ch:cl == top:bot */
        push    bp
        int     VIDEO_INT               /* video interrupt = 10h */
        pop     bp
   }
}


/*
 * Name:    set_overscan_color
 * Purpose: To set overscan color
 * Date:    April 1, 1993
 * Passed:  color:  overscan color
 * Notes:   before setting the overscan color, the old overscan color
 *           needs to be saved so it can be restored.
 */
void set_overscan_color( int color )
{
   ASSEMBLE {
        mov     ah, 0x0b                /* function 0x0b */
        mov     bl, BYTE PTR color      /* get new overscan color */
        xor     bh, bh
        push    bp
        int     VIDEO_INT               /* video interrupt = 10h */
        pop     bp
   }
}


/*
 * Name:    save_screen_line
 * Purpose: To save the characters and attributes of a line on screen.
 * Date:    June 5, 1991
 * Passed:  col:    desired column, usually always zero
 *          line:   line on screen to save (0 up to max)
 *          screen_buffer:  buffer for screen contents, must be >= 160 chars
 * Notes:   Save the contents of the line on screen where prompt is
 *           to be displayed
 */
void save_screen_line( int col, int line, char *screen_buffer )
{
char far *p;

   p = g_display.display_address + line * 160 + col * 2;
   _fmemcpy( screen_buffer, p, 160 );
}


/*
 * Name:    restore_screen_line
 * Purpose: To restore the characters and attributes of a line on screen.
 * Date:    June 5, 1991
 * Passed:  col:    usually always zero
 *          line:   line to restore (0 up to max)
 *          screen_buffer:  buffer for screen contents, must be >= 160 chars
 * Notes:   Restore the contents of the line on screen where the prompt
 *           was displayed
 */
void restore_screen_line( int col, int line, char *screen_buffer )
{
char far *p;

   p = g_display.display_address + line * 160 + col * 2;
   _fmemcpy( p, screen_buffer, 160 );
}
