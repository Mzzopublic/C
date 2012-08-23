/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 *
 * Set up default dispatch table.
 *
 * If you want to change the default key assignments - it's really easy.  All
 * you have to do is set the appropriate array element to the desired function
 * and then recompile the source code.  The available functions are in the
 * define.h file.
 *
 * The key code returned by pressing a function key is added to 256.  This is
 * done because it makes it easy to allow the ASCII and Extended ASCII
 * characters to get thru as normal characters and to trap the function keys
 * because they are greater than 256.  Once a function is detected, 256 is
 * subtracted from the key code, which maps the function key codes to
 * 0-256 (MAX_KEYS).  See function getfunc( ) in console.c.
 *
 * Look in the back of the MSC 6.0a manual or a MASM manual to find out what
 * the extended codes are for the various key combinations.  For example,
 * ALT+M = 50.  Add 50 to 256 which equals 306.
 *
 * **************************************************************************
 * **** In tde, version 1.4, I added support for assigning characters    ****
 * **** to Control and Alt keys.  This allows those who need various     ****
 * **** accent and dot characters to assign any ASCII or Extended ASCII  ****
 * **** character to unused function keys.                               ****
 * **************************************************************************
 *
 * **************************************************************************
 * **** In tde, version 1.5, I took out support for assigning characters ****
 * **** to Control and Alt keys.  Macros may now be assigned to function ****
 * **** keys, which will accomplish the same task.                       ****
 * **************************************************************************
 *
 * If you change the default key assignments, you should also change the help
 * screen in help.h to show what function the new keys are assigned to.
 *
 * The insert_overwrite function is assigned the index of 0.  If the key
 * code is less than 256 then a character is written to the file.  DO NOT
 * assign any function to 256, it is used to call insert_overwrite( ) and
 * add text characters to the file.  DO NOT put an index to it in the
 * cfgkeys utility.
 *
 * See the end of this file for default two-key assignments.
 */

KEY_FUNC key_func = {
     "$  keys",             /* signature for the .exe file */
/* 256 */
     0,                     /* DO NOT assign this to any function */
     0,                     /* Alt + ESC  -  101 Keyboard */
     AbortCommand,          /* ESC    - manually map to 258 */
     0,                     /* #ESC   - manually map to 259 */
/* 260 */                   
     ClearAllMacros,        /* ^ESC    - mannuall map to 260 */
     0,                     /* no key assigned here */
     Rturn,                 /*  Enter  - manually map to 262 */
     NextLine,              /* #Enter  - manually map to 263 */
     BegNextLine,           /* ^Enter  - manually map to 264 */
     BackSpace,             /*  BackSpace - manually map to 265 */
     BackSpace,             /* #BackSpace - manually map to 266 */
     0,                     /* ^BackSpace - manually map to 267 */
     Tab,                   /*  Tab       - manually map to 268 */
     0,                     /* Control-Break - manually map to 269
                             *    DO NOT assign any function to Control-Break.
                             *    It is used to stop run away functions.
                             */
/* 270 */                   
     0,                     /* Alt + Backspace  - 101 keyboard */
     BackTab,               /* Shift + Tab */
     Quit,                  /* Alt-Q  272 */
     BlockToFile,           /* Alt-W  273 */
     BlockExpandTabs,       /* Alt-E  274 */
     ToggleRuler,           /* Alt-R  275 */
     BlockTrimTrailing,     /* Alt-T  276 */
     UndoDelete,            /* ALT-Y  277 */
     UnMarkBlock,           /* Alt-U  278 */
     ToggleIndent,          /* Alt-I  279 */
/* 280 */                   
     OverlayBlock,          /* Alt-O  280 */
     PrintBlock,            /* Alt-P  281 */
     0,                     /* Alt-[     - 101 keyboard */
     0,                     /* Alt-]     - 101 keyboard */
     0,                     /* Alt-Enter - 101 keyboard */
     0,                     /* manually map Grey Enter to this hole */
     AddLine,               /* Alt-A  286 */
     SortBoxBlock,          /* Alt-S  287 */
     DeleteLine,            /* Alt-D  288 */
     FillBlock,             /* Alt-F  289 */
/* 290 */                   
     DeleteBlock,           /* Alt-G  290 */
     Help,                  /* Alt-H  291 */
     JoinLine,              /* Alt-J  292 */
     KopyBlock,             /* Alt-K  293 */
     MarkLine,              /* Alt-L  294 */
     0,                     /* Alt-;: - 101 keyboard */
     0,                     /* Alt-'" - 101 keyboard */
     0,                     /* Alt-`~ - 101 keyboard */
     0,                     /* manually map Shift+Grey Enter to this hole */
     0,                     /* manually map Control+Greay Enter to this hole */
/* 300 */                   
     ToggleZ,               /* Alt-Z  300 */
     MarkStream,            /* Alt-X  301 */
     CopyBlock,             /* Alt-C  302 */
     ToggleWordWrap,        /* Alt-V  303 */
     MarkBox,               /* Alt-B  304 */
     NumberBlock,           /* Alt-N  305 */
     MoveBlock,             /* Alt-M  306 */
     BlockUpperCase,        /* Alt-<  added for 83/84 keyboards, fmd */
     BlockLowerCase,        /* Alt->  added for 83/84 keyboards, fmd */
     BlockStripHiBit,       /* ALt-?  added for 83/84 keyboards, fmd */
/* 310 */                   
     0,                     /*  hole in key codes, not used */
     0,                     /*  hole in key codes, not used */
     0,                     /*  hole in key codes, not used */
     0,                     /*  hole in key codes, not used */
     0,                     /*  hole in key codes, not used */
     Help,                  /* F1     315 */
     Save,                  /* F2     316 */
     Quit,                  /* F3     317 */
     File,                  /* F4     318 */
     RepeatFindForward1,    /* F5     319 */
/* 320 */                   
     RepeatFindBackward1,   /* F6     320 */
     RepeatFindRegX,        /* F7     321 */
     SplitVertical,         /* F8     322 */
     SplitHorizontal,       /* F9     323 */
     NextWindow,            /* F10    324 */
     0,                     /*  hole in key codes, not used */
     0,                     /*  hole in key codes, not used */
     BegOfLine,             /* Home   327 */
     LineUp,                /* Up     328 */
     ScreenUp,              /* PgUp   329 */
/* 330 */                   
     0,                     /*  hole in key codes, not used */
     CharLeft,              /* Left   331 */
     CenterWindow,          /* Center 332 */
     CharRight,             /* Right  333 */
     0,                     /*  hole in key codes, not used */
     EndOfLine,             /* End    335 */
     LineDown,              /* Down   336 */
     ScreenDown,            /* PgDn   337 */
     ToggleOverWrite,       /* Ins    338 */
     DeleteChar,            /* Del    339 */
/* 340 */                   
     SaveMacro,             /* Shift F1  */
     SaveAs,                /* Shift F2  */
     LoadMacro,             /* Shift F3  */
     EditFile,              /* Shift F4  */
     FindForward,           /* Shift F5  */
     FindBackward,          /* Shift F6  */
     FindRegX,              /* Shift F7  */
     ReplaceString,         /* Shift F8  */
     SizeWindow,            /* Shift F9  */
     PreviousWindow,        /* Shift F10 */
/* 350 */                   
     ToggleSync,            /* Control F1 */
     ToggleEol,             /* Control F2 */
     ToggleCRLF,            /* Control F3 */
     ToggleTrailing,        /* Control F4 */
     ToggleSearchCase,      /* Control F5 */
     SetLeftMargin,         /* Control F6 */
     SetRightMargin,        /* Control F7 */
     SetParagraphMargin,    /* Control F8 */
     ZoomWindow,            /* Control F9  */
     NextHiddenWindow,      /* Control F10 */
/* 360 */                   
     DirList,               /* Alt-F1   360 */
     SetFileAttributes,     /* Alt-F2   361 */
     RecordMacro,           /* Alt-F3   362 */
     EditNextFile,          /* Alt-F4   363 */
     NextDirtyLine,         /* Alt-F5   364 */
     PrevDirtyLine,         /* Alt-F6   365 */
/*     FormatParagraph,   */    /* Alt-F7   366 */
     RepeatFindRegXBackward,/* Alt-F7   366 */
     LeftJustify,           /* Alt-F8   367 */
     RightJustify,          /* Alt-F9   368 */
     CenterJustify,         /* Alt-F10  369 */
/* 370 */                   
     0,                     /*  hole in key codes, not used */
     WordLeft,              /* Control Left  371 */
     WordRight,             /* Control Right 372 */
     BotOfScreen,           /* Control End   373 */
     EndOfFile,             /* Control PgUp  374 */
     TopOfScreen,           /* Control Home  375 */
     SetMark1,              /* Alt-1! */
     SetMark2,              /* Alt-2@ */
     SetMark3,              /* Alt-3# */
     0,                     /* Alt-4$ */
/* 380 */                   
     0,                     /* Alt-5% */
     0,                     /* Alt-6^ */
     0,                     /* Alt-7& */
     0,                     /* Alt-8* */
     0,                     /* Alt-9( */
     0,                     /* Alt-0) */
     DelEndOfLine,          /* Alt '-'  386      */
     DuplicateLine,         /* Alt '='  387      */
     TopOfFile,             /* Control Pgup  388 */
     RepeatDiff,            /* F11          - 101 keyboard */
/* 390 */                   
     RepeatGrep,            /* F12          - 101 keyboard */
     DefineDiff,            /* Shift F11    - 101 keyboard */
     DefineGrep,            /* Shift F12    - 101 keyboard */
     0,                     /* Control F11  - 101 keyboard */
     DefineRegXGrep,        /* Control F12  - 101 keyboard */
     0,                     /* Alt F11      - 101 keyboard */
     RepeatFindRegX,        /* Alt F12      - 101 keyboard */
     ScrollUpLine,          /* Control Up */
     PanUp,                 /* Control Grey - */
     CenterLine,            /* Control Center */
/* 400 */                   
     PanDn,                 /* Control Grey + */
     ScrollDnLine,          /* Control Down   */
     0,                     /* Control Ins    */
     StreamDeleteChar,      /* Control Del    */
     SetTabs,               /* Control TAB    */
     0,                     /* Control Grey /  */
     0,                     /* Control Grey *  */
     0,                     /* Alt + Grey Home  */
     PanUp,                 /* Alt + Grey Up    */
     HorizontalScreenLeft,  /* Alt + Grey PgUp  */
/* 410 */                   
     0,                     /*  hole in key codes, not used */
     PanLeft,               /* Alt + Grey Left  */
     0,                     /*  hole in key codes, not used */
     PanRight,              /* Alt + Grey Right */
     0,                     /*  hole in key codes, not used */
     0,                     /* Alt + Grey End   */
     PanDn,                 /* Alt + Grey Down  */
     HorizontalScreenRight, /* Alt + Grey PgDn  */
     0,                     /* Alt + Grey Ins   */
     0,                     /* Alt + Grey Del   */
/* 420 */                   
     0,                     /* Alt + Grey /     */
     ToggleSmartTabs,       /* Alt + Tab        */
     0,                     /* Alt + Grey Enter */
     ScrollUpLine,          /* Shift Grey - (Num Lock off), fmd key */
     ScrollDnLine,          /* Shift Grey + (Num Lock off), fmd  key */
     Rturn,                 /* Hard code Enter to 425 - DO NOT let users
                             * change, used in the reassignment of
                             * ALT-013 <lf> */
     PanLeft,               /* hard code - control+shift+left, fmd key  */
     PanRight,              /* hard code - control+shift+right, fmd key */
     0,                     /* no keys are assigned here */
     0,                     /* no keys are assigned here */

   /*
    * Add 430 to keycodes 0-31 if ALT key is not pressed.  This effectively
    * remaps the Control code sequence to 430-461.
    */

/* 430 */
     DateTimeStamp,         /* Control @ - remapped in main.c */
     WordLeft,              /* Control a */
     FormatText,            /* Control b */
     ScreenDown,            /* Control c */
     CharRight,             /* Control d */
     LineUp,                /* Control e */
     WordRight,             /* Control f */
     DeleteChar,            /* Control g */
     BackSpace,             /* Control h */
     Tab,                   /* Control i */
     Help,                  /* Control j */
     TwoCharKey,            /* Control k */
     0,                     /* Control l */
     Rturn,                 /* Control m */
     AddLine,               /* Control n */
     0,                     /* Control o */
     Pause,                 /* Control p */
     0,                     /* Control q */
     ScreenUp,              /* Control r */
     CharLeft,              /* Control s */
     WordDelete,            /* Control t */
     UndoDelete,            /* Control u */
     ToggleOverWrite,       /* Control v */
     ScrollUpLine,          /* Control w */
     LineDown,              /* Control x */
     DeleteLine,            /* Control y */
     ScrollDnLine,          /* Control z */
     AbortCommand,          /* Control [ */
     RedrawScreen,          /* Control \ */
     ParenBalance,          /* Control ] */
/* 460 */
     0,                     /* Control ^     460 */
     SplitLine,             /* Control _     461 */
     GotoMark1,             /* Shift Alt 1!  462 */
     GotoMark2,             /* Shift Alt 2@  463 */
     GotoMark3,             /* Shift Alt 3#  464 */
     0,                     /* Shift Alt 4$  465 */
     0,                     /* Shift Alt 5%  466 */
     0,                     /* Shift Alt 6^  467 */
     0,                     /* Shift Alt 7&  468 */
     0,                     /* Shift Alt 8*  469 */
/* 470 */
     0,                     /* Shift Alt 9(  470 */
     0,                     /* Shift Alt 0)  471 */
     0,                     /* Shift Alt -_  472 */
     0,                     /* Shift Alt +=  473 */
     DefineGrep,            /* Shift Alt-Q   474 */
     RepeatGrep,            /* Shift Alt-W   475 */
     0,                     /* Shift Alt-E   476 */
     0,                     /* Shift Alt-R   477 */
     ToggleTabInflate,      /* Shift Alt-T   478 */
     0,                     /* Shift ALT-Y   479 */
/* 480 */
     0,                     /* Shift Alt-U   480 */
     BlockIndentTabs,       /* Shift Alt-I   481 */
     0,                     /* Shift Alt-O   482 */
     0,                     /* Shift Alt-P   483 */
     0,                     /* Shift Alt-[     - 101 keyboard */
     0,                     /* Shift Alt-]     - 101 keyboard */
     0,                     /* Shift Alt-Enter - 101 keyboard */
     0,                     /* hole in the key codes here */
     0,                     /* Shift Alt-A   488 */
     SwapBlock,             /* Shift Alt-S   489 */
/* 490 */
     RepeatDiff,            /* Shift Alt-D   490 */
     0,                     /* Shift Alt-F   491 */
     0,                     /* Shift Alt-G   492 */
     0,                     /* Shift Alt-H   493 */
     JumpToLine,            /* Shift Alt-J   494 */
     0,                     /* Shift Alt-K   495 */
     0,                     /* Shift Alt-L   496 */
     0,                     /* Shift Alt-;: - 101 keyboard */
     0,                     /* Shift Alt-'" - 101 keyboard */
     0,                     /* Shift Alt-`~ - 101 keyboard */
/* 500 */
     0,                     /* hole in the key codes here */
     0,                     /* see pages 194-195 of MASM man. for more info */
     0,                     /* Shift Alt-Z   502 */
     0,                     /* Shift Alt-X   503 */
     BlockCompressTabs,     /* Shift Alt-C   504 */
     0,                     /* Shift Alt-V   505 */
     0,                     /* Shift Alt-B   506 */
     0,                     /* Shift Alt-N   507 */
     0,                     /* Shift Alt-M   508 */
     BlockRot13,            /* Shift Alt-<  added for 83/84 keyboards, fmd */
/* 510 */
     BlockFixUUE,           /* Shift Alt->  added for 83/84 keyboards, fmd */
     BlockEmailReply        /* Shift ALt-?  added for 83/84 keyboards, fmd */
};


/*
 * there are MAX_TWO_KEY == 128 combinations for user customization.
 *   the first entry is the first key and the second entry is the next
 *   key.  the function assigned to the two-key combination is the last
 *   entry.
 *
 * the first key entry MUST be a recognized function key.  the second key
 *   can be any any ASCII, Extended ASCII, or function key.  the second key
 *   may even be a repeat of the first function key.
 */
TWO_KEY two_key_list = {
   "$twokey",
   { { _CNTRL_K, 'q', Quit },       /* example */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 6 */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 26 */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 51 */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 76 */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 101 */
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
   { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },  /* 126 */
   { 0, 0, 0 }, { 0, 0, 0 } }
};
