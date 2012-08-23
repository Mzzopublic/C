

displays g_display;

status_infos g_status;

boyer_moore_type bm;

boyer_moore_type sas_bm;

CEH ceh;

SORT sort;

DIFF diff;

MACRO_STACK macro_stack[MAX_KEYS];      /* small, simple stack for calls */

REGX_INFO regx;

REGX_INFO sas_regx;

NFA_TYPE nfa;

NFA_TYPE sas_nfa;


/*
 *   those who use special accented characters as part of normal character set
 *     in text do not particularly care for a straight ASCII sort sequence.
 *
 *   this modified ASCII sorting sequence for special accent characters is
 *     useful with Enlish, Esperanto, and French.
 */
SORT_ORDER sort_order = {
               /* ignore case */
   { '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
     '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
     '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
     '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
     ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
     '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     ':', ';', '<', '=', '>', '?',
     '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[',
     '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
     'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
     'x', 'y', 'z', '{', '|', '}', '~', '', 'c', 'u', 'e', 'a', 'a', 'a',
     'a', 'c', 'e', 'e', 'e', 'i', 'i', 'i', 'a', 'a', 'e', 'a', 'a', 'o',
     'o', 'o', 'u', 'u', 'y', 'o', 'u', 'õ', 'ú', 'ù', 'û', 'ü', 'a', 'i',
     'o', 'u', 'n', 'n', '¶', 'ß', '®', '©', '™', '´', '¨', '≠', 'Æ', 'Ø',
     '∞', '±', '≤', '≥', '¥', 'µ', '∂', '∑', '∏', 'π', '∫', 'ª', 'º', 'Ω',
     'æ', 'ø', '¿', '¡', '¬', '√', 'ƒ', '≈', '∆', '«', '»', '…', ' ', 'À',
     'Ã', 'Õ', 'Œ', 'œ', '–', '—', '“', '”', '‘', '’', '÷', '◊', 'ÿ', 'Ÿ',
     '⁄', '€', '‹', '›', 'ﬁ', 'ﬂ', '‡', '·', '‚', '„', '‰', 'Â', 'Ê', 'Á',
     'Ë', 'È', 'Í', 'Î', 'Ï', 'Ì', 'Ó', 'Ô', '', 'Ò', 'Ú', 'Û', 'Ù', 'ı',
     'ˆ', '˜', '¯', '˘', '˙', '˚', '¸', '˝', '˛', 'ˇ' },
               /* match case */
   { '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
     '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
     '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
     '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
     ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
     '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     ':', ';', '<', '=', '>', '?',
     '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[',
     '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
     'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
     'x', 'y', 'z', '{', '|', '}', '~', '', 'C', 'u', 'e', 'a', 'a', 'a',
     'a', 'c', 'e', 'e', 'e', 'i', 'i', 'i', 'A', 'A', 'E', 'a', 'A', 'o',
     'o', 'o', 'u', 'u', 'y', 'O', 'U', 'õ', 'ú', 'ù', 'û', 'ü', 'a', 'i',
     'o', 'u', 'n', 'N', '¶', 'ß', '®', '©', '™', '´', '¨', '≠', 'Æ', 'Ø',
     '∞', '±', '≤', '≥', '¥', 'µ', '∂', '∑', '∏', 'π', '∫', 'ª', 'º', 'Ω',
     'æ', 'ø', '¿', '¡', '¬', '√', 'ƒ', '≈', '∆', '«', '»', '…', ' ', 'À',
     'Ã', 'Õ', 'Œ', 'œ', '–', '—', '“', '”', '‘', '’', '÷', '◊', 'ÿ', 'Ÿ',
     '⁄', '€', '‹', '›', 'ﬁ', 'ﬂ', '‡', '·', '‚', '„', '‰', 'Â', 'Ê', 'Á',
     'Ë', 'È', 'Í', 'Î', 'Ï', 'Ì', 'Ó', 'Ô', '', 'Ò', 'Ú', 'Û', 'Ù', 'ı',
     'ˆ', '˜', '¯', '˘', '˙', '˚', '¸', '˝', '˛', 'ˇ' },
};


mode_infos mode = {
   "$ modes",           /* signature for mode structure, 8 chars */
   0,                   /* initial color scheme */
   FALSE,               /* sync mode? */
   TRUE,                /* sync semaphore must be TRUE - DO NOT CHANGE */
   FALSE,               /* macro recording mode must be FALSE - DO NOT CHANGE */
   TRUE,                /* insert mode */
   TRUE,                /* indent mode */
   8,                   /* tab size */
   8,                   /* tab size */
   TRUE,                /* default smart tab mode */
   FALSE,               /* inflate tabs? */
   IGNORE,              /* sort case */
   FALSE,               /* enhanced keyboard flag - set in console.c */
   SMALL_INS,           /* default cursor size == small insert */
   "<=======   End of File   =======>",       /* eof message */
   FALSE,               /* default FALSE = do not write ^Z at end of file */
   NATIVE,              /* default write <cr><lf> at eol */
   TRUE,                /* default remove trailing space on edited lines */
   FALSE,               /* default show eol character is off */
   NO_WRAP,             /* default word wrap mode is off */
   0,                   /* default left margin - add 1 to margins for display */
   2,                   /* default paragraph begin */
   71,                  /* default right margin */
   FALSE,               /* default justify right margin */
   FALSE,               /* format paragraph/text semaphore - DO NOT CHANGE */
   UNDO_STACK_LEN,      /* number lines in undo buffer */
   FALSE,               /* default backup mode */
   TRUE,                /* default ruler mode */
   MM_DD_YY,            /* default date style */
   _12_HOUR             /* default time style */
};


/*
 * Default color settings.  Incidentally, I'm color blind (mild red-green) and
 * the default colors look fine to me, Frank.
 */
COLORS colour = {
   "$colors",
   { { HERC_REVERSE, HERC_NORMAL, HERC_UNDER, HERC_REVERSE, HERC_REVERSE,
     HERC_HIGH, HERC_NORMAL, HERC_NORMAL, HERC_HIGH, HERC_HIGH, HERC_HIGH,
     HERC_REVERSE, HERC_REVERSE, HERC_NORMAL },
   { COLOR_HEAD, COLOR_TEXT, COLOR_DIRTY, COLOR_MODE, COLOR_BLOCK,
     COLOR_MESSAGE, COLOR_HELP, COLOR_DIAG, COLOR_EOF, COLOR_CURL, COLOR_RULER,
     COLOR_POINTER, COLOR_TEXT, COLOR_OVRS } }
};


/*
 * do_it is an array of pointers to functions that return int with an argument
 * that is a pointer to a window.  Is that right???
 */
int  (* (do_it[NUM_FUNCS]))( WINDOW * ) = {
   insert_overwrite,                /*   regular text keys          0  */
   get_help,                        /*   Help                       1  */
   insert_newline,                  /*   Rturn                      2  */
   next_line,                       /*   NextLine                   3  */
   beg_next_line,                   /*   BegNextLine                4  */
   move_down,                       /*   LineDown                   5  */
   move_up,                         /*   LineUp                     6  */
   move_right,                      /*   CharRight                  7  */
   move_left,                       /*   CharLeft                   8  */
   pan_right,                       /*   PanRight                   9  */
   pan_left,                        /*   PanLeft                   10  */
   word_right,                      /*   WordRight                 11  */
   word_left,                       /*   WordLeft                  12  */
   page_down,                       /*   ScreenDown                13  */
   page_up,                         /*   ScreenUp                  14  */
   goto_end_file,                   /*   EndOfFile                 15  */
   goto_top_file,                   /*   TopOfFile                 16  */
   goto_bottom,                     /*   BotOfScreen               17  */
   goto_top,                        /*   TopOfScreen               18  */
   goto_eol,                        /*   EndOfLine                 19  */
   home,                            /*   BegOfLine                 20  */
   goto_line,                       /*   JumpToLine                21  */
   center_window,                   /*   CenterWindow              22  */
   center_window,                   /*   CenterLine                23  */
   horizontal_screen_right,         /*   HorizontalScreenRight     24  */
   horizontal_screen_left,          /*   HorizontalScreenLeft      25  */
   scroll_down,                     /*   ScrollDnLine              26  */
   scroll_up,                       /*   ScrollUpLine              27  */
   pan_up,                          /*   PanUp                     28  */
   pan_down,                        /*   PanDn                     29  */
   toggle_overwrite,                /*   ToggleOverWrite           30  */
   toggle_smart_tabs,               /*   ToggleSmartTabs           31  */
   toggle_indent,                   /*   ToggleIndent              32  */
   toggle_ww,                       /*   ToggleWordWrap            33  */
   toggle_crlf,                     /*   ToggleCRLF                34  */
   toggle_trailing,                 /*   ToggleTrailing            35  */
   toggle_z,                        /*   ToggleZ                   36  */
   toggle_eol,                      /*   ToggleEol                 37  */
   toggle_sync,                     /*   ToggleSync                38  */
   toggle_ruler,                    /*   ToggleRuler               39  */
   toggle_tabinflate,               /*   ToggleTabInflate          40  */
   set_tabstop,                     /*   SetTabs                   41  */
   set_left_margin,                 /*   SetLeftMargin             42  */
   set_right_margin,                /*   SetRightMargin            43  */
   set_paragraph_margin,            /*   SetParagraphMargin        44  */
   format_paragraph,                /*   FormatParagraph           45  */
   format_paragraph,                /*   FormatText                46  */
   flush_left,                      /*   LeftJustify               47  */
   flush_right,                     /*   RightJustify              48  */
   flush_center,                    /*   CenterJustify             49  */
   tab_key,                         /*   Tab                       50  */
   backtab,                         /*   BackTab                   51  */
   match_pair,                      /*   ParenBalance              52  */
   back_space,                      /*   BackSpace                 53  */
   char_del_under,                  /*   DeleteChar                54  */
   char_del_under,                  /*   StreamDeleteChar          55  */
   line_kill,                       /*   DeleteLine                56  */
   eol_kill,                        /*   DelEndOfLine              57  */
   word_delete,                     /*   WordDelete                58  */
   insert_newline,                  /*   AddLine                   59  */
   insert_newline,                  /*   SplitLine                 60  */
   join_line,                       /*   JoinLine                  61  */
   dup_line,                        /*   DuplicateLine             62  */
   undo_line,                       /*   AbortCommand              63  */
   undo_line,                       /*   UndoLine                  64  */
   undo,                            /*   UndoDelete                65  */
   toggle_search_case,              /*   ToggleSearchCase          66  */
   find_string,                     /*   FindForward               67  */
   find_string,                     /*   FindBackward              68  */
   find_string,                     /*   RepeatFindForward1        69  */
   find_string,                     /*   RepeatFindForward2        70  */
   find_string,                     /*   RepeatFindBackward1       71  */
   find_string,                     /*   RepeatFindBackward2       72  */
   replace_string,                  /*   ReplaceString             73  */
   define_diff,                     /*   DefineDiff                74  */
   repeat_diff,                     /*   RepeatDiff                75  */
   mark_block,                      /*   MarkBlock                 76  */
   mark_block,                      /*   MarkLine                  77  */
   mark_block,                      /*   MarkStream                78  */
   unmark_block,                    /*   UnMarkBlock               79  */
   move_copy_delete_overlay_block,  /*   FillBlock                 80  */
   move_copy_delete_overlay_block,  /*   NumberBlock               81  */
   move_copy_delete_overlay_block,  /*   CopyBlock                 82  */
   move_copy_delete_overlay_block,  /*   KopyBlock                 83  */
   move_copy_delete_overlay_block,  /*   MoveBlock                 84  */
   move_copy_delete_overlay_block,  /*   OverlayBlock              85  */
   move_copy_delete_overlay_block,  /*   DeleteBlock               86  */
   move_copy_delete_overlay_block,  /*   SwapBlock                 87  */
   block_write,                     /*   BlockToFile               88  */
   block_print,                     /*   PrintBlock                89  */
   block_expand_tabs,               /*   BlockExpandTabs           90  */
   block_compress_tabs,             /*   BlockCompressTabs         91  */
   block_compress_tabs,             /*   BlockIndentTabs           92  */
   block_trim_trailing,             /*   BlockTrimTrailing         93  */
   block_convert_case,              /*   BlockUpperCase            94  */
   block_convert_case,              /*   BlockLowerCase            95  */
   block_convert_case,              /*   BlockRot13                96  */
   block_convert_case,              /*   BlockFixUUE               97  */
   block_email_reply,               /*   BlockEmailReply           98  */
   block_convert_case,              /*   BlockStripHiBit           99  */
   sort_box_block,                  /*   SortBoxBlock             100  */
   date_time_stamp,                 /*   DateTimeStamp            101  */
   edit_another_file,               /*   EditFile                 102  */
   dir_help,                        /*   DirList                  103  */
   file_file,                       /*   File                     104  */
   save_file,                       /*   Save                     105  */
   save_as_file,                    /*   SaveAs                   106  */
   change_fattr,                    /*   SetFileAttributes        107  */
   edit_next_file,                  /*   EditNextFile             108  */
   search_and_seize,                /*   DefineGrep               109  */
   search_and_seize,                /*   RepeatGrep               110  */
   redraw_screen,                   /*   RedrawScreen             111  */
   size_window,                     /*   SizeWindow               112  */
   split_horizontal,                /*   SplitHorizontal          113  */
   split_vertical,                  /*   SplitVertical            114  */
   next_window,                     /*   NextWindow               115  */
   prev_window,                     /*   PreviousWindow           116  */
   zoom_window,                     /*   ZoomWindow               117  */
   next_hidden_window,              /*   NextHiddenWindow         118  */
   set_marker,                      /*   SetMark1                 119  */
   set_marker,                      /*   SetMark2                 120  */
   set_marker,                      /*   SetMark3                 121  */
   goto_marker,                     /*   GotoMark1                122  */
   goto_marker,                     /*   GotoMark2                123  */
   goto_marker,                     /*   GotoMark3                124  */
   record_on_off,                   /*   RecordMacro              125  */
   play_back,                       /*   PlayBack                 126  */
   save_strokes,                    /*   SaveMacro                127  */
   load_strokes,                    /*   LoadMacro                128  */
   clear_macros,                    /*   ClearAllMacros           129  */
   pause,                           /*   Pause                    130  */
   quit,                            /*   Quit                     131  */
   next_dirty_line,                 /*   NextDirtyLine            132  */
   prev_dirty_line,                 /*   PrevDirtyLine            133  */
   find_regx,                       /*   FindRegX                 134  */
   find_regx,                       /*   RepeatFindRegX           135  */
   find_regx,                       /*   RepeatFindRegXBackward   137  */
   search_and_seize,                /*   DefineRegXGrep           138  */
   two_key                          /*   TwoCharKey               139  */
};

/*
 * set the 256 possible function keys to FS (First Stroke)
 * set the 1024 stroke buffer keys to SK (Stroke Key) and SN (Stroke Next)
 */
#define         FS      ((STROKE_LIMIT)+1)
#define         SK      ((MAX_KEYS)+1)
#define         SN      ((STROKE_LIMIT)+1)

MACRO macro = {
   "$macbuf",
               /* 16 rows of 16 = 256 = MAX_KEYS */
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
   FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS, FS,
                 /* 102.4 rows of 10 = 1024 = STROKE_LIMIT */
{{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN}, {SK, SN},
{SK, SN}, {SK, SN}, {SK, SN}, {SK, SN} }
};
