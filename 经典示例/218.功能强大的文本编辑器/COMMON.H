/*******************  start of original comments  ********************/
/*
 * Written by Douglas Thomson (1989/1990)
 *
 * This source code is released into the public domain.
 */
/*********************  end of original comments   ********************/


/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 *
 * This file contains all the external structure declarations common
 * to all the editor modules.   Being stupid, I can't remember which
 * extern prompts or structures are used by which file.  let's combine
 * them in one file, so I don't have to remember.
 */

/*
 * Some of variables are used again and again throughout the editor.
 * Gather them into logical structures and make them global to all
 * modules.
 */
extern displays g_display;

extern status_infos g_status;

extern boyer_moore_type bm;

extern boyer_moore_type sas_bm;

extern REGX_INFO regx;

extern REGX_INFO sas_regx;

extern NFA_TYPE nfa;

extern NFA_TYPE sas_nfa;

extern mode_infos mode;

extern KEY_FUNC key_func;

extern MACRO macro;

extern CEH ceh;

extern SORT sort;

extern SORT_ORDER sort_order;

extern DIFF diff;

extern MACRO_STACK macro_stack[];      /* small, simple stack for calls */

extern TWO_KEY two_key_list;

extern COLORS colour;

extern char *regx_help[];

extern int (* (do_it[NUM_FUNCS]))( WINDOW * );

/*
 * extern definitions for all prompts
 */

extern char *cb;

extern char *block1;
extern char *ltol;
extern char *block2;
extern char *block3a;
extern char *block3b;
extern char *block4;
extern char *block5;
extern char *block6;
extern char *block7;
extern char *block8;
extern char *block9;
extern char *block10;
extern char *block11;
extern char *block12;
extern char *block13;
extern char *block14;
extern char *block15;
extern char *block16;
extern char *block17;
extern char *block18;
extern char *block20;
extern char *block21;
extern char *block22a;
extern char *block22b;
extern char *block23;
extern char *block24;
extern char *block25;
extern char *block26;


extern char *critt1;


extern char *diff_prompt1;
extern char *diff_prompt2;
extern char *diff_prompt3;
extern char *diff_prompt4;
extern char *diff_prompt5;
extern char *diff_prompt6a;
extern char *diff_prompt6b;
extern char *diff_prompt7a;
extern char *diff_prompt7b;
extern char *diff_prompt7c;
extern char *diff_prompt7d;

extern char *diff_message;
extern char *diff_blank;


extern char *dir1;
extern char *dir2;
extern char *stardotstar;
extern char *dir3;
extern char *dir4;
extern char *dir5;
extern char *dir6;
extern char *dir7;


extern char *ed1;
extern char *ed2;
extern char *ed3;
extern char *ed4;
extern char *ed5;

extern char *ed7a;
extern char *ed7b;
extern char *ed8;
extern char *ed9;
extern char *ed10;
extern char *ed11;
extern char *ed12;
extern char *ed13;
extern char *ed14;
extern char *ed15;
extern char *ed16;
extern char *ed17;

extern char *paused1;
extern char *paused2;


extern char *find1;
extern char *find2;
extern char *find3;
extern char *find4;
extern char *find5a;
extern char *find5b;
extern char *find6;
extern char *find7[];
extern char *find8;
extern char *find9;
extern char *find10;
extern char *find11;
extern char *find12;


extern char *smart;
extern char *fixed;
extern char *intab;
extern char *outtab;
extern char *tabs;
extern char *indent;
extern char *blank;
extern char *ignore;
extern char *match;
extern char *sync_off;
extern char *sync_on;
extern char *ww_mode[];
extern char *eol_mode[];


extern char *main1;
extern char *main2;
extern char *main3;
extern char *main4;
extern char *main5;
extern char *main6;
extern char *main7a;
extern char *main7b;
extern char *main8a;
extern char *main8b;
extern char *main9;
extern char *main10a;
extern char *main10b;
extern char *main11;
extern char *main12;
extern char *main13;
extern char *main14;
extern char *main15;

extern char *main18;
extern char *main19;
extern char *main20;
extern char *main21;
extern char *main22;


extern char *reg1;
extern char *reg2;
extern char *reg3;
extern char *reg4;
extern char *reg5;
extern char *reg6;
extern char *reg7;
extern char *reg8;
extern char *reg9;
extern char *reg10;
extern char *reg11;


extern char *utils1;

extern char *utils3;
extern char *utils4;
extern char *utils5;

extern char *utils6;
extern char *utils7a;
extern char *utils7b;
extern char *utils8;
extern char *utils9;
extern char *utils10;
extern char *utils11;
extern char *utils12;
extern char *utils13;
extern char *utils14;
extern char *utils15;

extern char *utils16;

extern char *win1;
extern char *win2;
extern char *win3;
extern char *win4;
extern char *win5;
extern char *win6;
extern char *win7;
extern char *win8;
extern char *win9;
extern char *win16a;
extern char *win16b;
extern char *win17;
extern char *win18;
extern char *win19;


extern char *ww1;
