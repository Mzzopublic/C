/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 *
 * This file contains all prototypes for every function in tde.  It should
 * be included in every source code module.
 *
 * I'm so stupid, I can't keep up with which functions are used in which
 * files.  Let's gather all prototypes into one file, so I don't have
 * to remember too much.
 */


/*************************  function prototypes in block.c *************/
int  mark_block( WINDOW * );
int  unmark_block( WINDOW * );
void restore_marked_block( WINDOW *, int );
int  prepare_block( WINDOW *, line_list_ptr, int );
int  pad_dest_line( WINDOW *, file_infos *, line_list_ptr );
int  move_copy_delete_overlay_block( WINDOW * );
void do_line_block( WINDOW *,  WINDOW *,  int,  file_infos *,  file_infos *,
                    line_list_ptr,  line_list_ptr,  line_list_ptr,
                    line_list_ptr,  long,  long,  int * );
void do_stream_block( WINDOW *,  WINDOW *,  int,  file_infos *,
                      file_infos *,  line_list_ptr,  line_list_ptr,
                      line_list_ptr,  line_list_ptr,
                      long,  long,  long,  int,  int,  int,  int * );
void do_box_block( WINDOW *,  WINDOW *,  int,  file_infos *,  file_infos *,
                   line_list_ptr, line_list_ptr,  long,  long, long,
                   long,  long,  int,  int, int,
                   int,  int,  int,  int,  int * );
void load_box_buff( char *, line_list_ptr, int, int, char );
int  copy_buff_2file( WINDOW *, char *, line_list_ptr, int, int, int );
void block_fill( char *, int, int );
void number_block_buff( char *, int, long, int );
void restore_cursors( file_infos * );
int  delete_box_block( WINDOW *, line_list_ptr, int, int );
void check_block( void );
void find_begblock( file_infos * );
void find_endblock( file_infos * );
int  block_write( WINDOW * );
int  block_print( WINDOW * );
int  get_block_fill_char( WINDOW *, int * );
int  get_block_numbers( WINDOW *, long *, long *, int * );
int  block_trim_trailing( WINDOW * );
int  block_email_reply( WINDOW * );
int  block_convert_case( WINDOW * );
void upper_case( text_ptr, size_t );
void lower_case( text_ptr, size_t );
void rot13( text_ptr, size_t );
void fix_uue( text_ptr, size_t );
void strip_hi( text_ptr, size_t );
/************************************************************************/


/**********************  function prototypes in console.c  **************/
void video_config( struct vcfg *cfg );
int  getkey( void );
int  waitkey( int );
int  getfunc( int );
int  two_key( WINDOW * );
void flush_keyboard( void );
void xygoto( int, int );
void update_line( WINDOW * );
void c_output( int, int, int, int );
void s_output( char far *, int, int, int );
void eol_clear( int, int, int );
void window_eol_clear( WINDOW *, int );
void hlight_line( int, int, int, int );
void cls( void );
void set_cursor_size( int );
void set_overscan_color( int );
void save_screen_line( int, int, char * );
void restore_screen_line( int, int, char * );
/************************************************************************/


/*************************  function prototypes in diff.c  **************/
int  define_diff( WINDOW * );
int  repeat_diff( WINDOW * );
int  differ( int, int, int );
int  skip_leading_space( text_ptr, int );
line_list_ptr skip_eol( line_list_ptr, int *, int *, long *, long * );
void show_diff_window( WINDOW * );
int  verify_number( char *, int * );
int  verify_letter( char *, int *, WINDOW ** );
/************************************************************************/


/*************************  function prototypes in dirlist.c *************/
int  dir_help( WINDOW * );
void far *my_calloc( unsigned, unsigned );
int  validate_path( char *, char * );
int  list_and_pick( char *, char *, WINDOW * );
void setup_directory_window( DIRECTORY *, int );
void recalculate_dir( DIRECTORY *, FTYPE *, int );
void write_directory_list( FTYPE *, DIRECTORY );
int  select_file( FTYPE *, char *, DIRECTORY * );
void shell_sort( FTYPE *, int );
/************************************************************************/


/*************************  function prototypes in ed.c *****************/
int  insert_newline( WINDOW * );
int  insert_overwrite( WINDOW * );
int  join_line( WINDOW * );
int  dup_line( WINDOW * );
int  word_delete( WINDOW * );
int  back_space( WINDOW * );
int  line_kill( WINDOW * );
int  char_del_under( WINDOW * );
int  eol_kill( WINDOW * );
int  undo_line( WINDOW * );
int  undo( WINDOW * );
int  beg_next_line( WINDOW *window );
int  next_line( WINDOW *window );
int  home( WINDOW * );
int  goto_eol( WINDOW * );
int  goto_top( WINDOW * );
int  goto_bottom( WINDOW * );
int  set_tabstop( WINDOW * );
void show_line_col( WINDOW * );
void show_asterisk( WINDOW * );
int  toggle_overwrite( WINDOW * );
int  toggle_smart_tabs( WINDOW * );
int  toggle_indent( WINDOW * );
int  set_left_margin( WINDOW * );
int  set_right_margin( WINDOW * );
int  set_paragraph_margin( WINDOW * );
int  toggle_ww( WINDOW * );
int  toggle_crlf( WINDOW * );
int  toggle_trailing( WINDOW * );
int  toggle_z( WINDOW * );
int  toggle_eol( WINDOW * );
int  toggle_search_case( WINDOW * );
int  toggle_sync( WINDOW * );
int  toggle_ruler( WINDOW * );
int  toggle_tabinflate( WINDOW * );
void sync( WINDOW * );
void editor( void );
void display_dirty_windows( WINDOW * );
void show_dirty_window( WINDOW * );
/************************************************************************/


/*************************  function prototypes in file.c  **************/
int  hw_fattrib( char * );
int  change_mode( char *, int );
int  write_file( char *, int, file_infos *, long, long, int );
int  hw_save( char *, file_infos *, long, long, int );
int  hw_append( char *, file_infos *, long, long, int );
int  load_file( char *, file_infos *, int *, int );
void insert_node( file_infos *, line_list_ptr, line_list_ptr );
int  show_file_2big( char *, int, line_list_ptr, text_ptr );
int  backup_file( WINDOW * );
int  edit_file( char *, int, int );
int  edit_another_file( WINDOW * );
int  edit_next_file( WINDOW * );
int  search_and_seize( WINDOW * );
int  attempt_edit_display( char *, int, int, int );
void make_backup_fname( file_infos * );
int  file_file( WINDOW * );
int  save_file( WINDOW * );
int  save_backup( WINDOW * );
int  write_to_disk( WINDOW *, char * );
int  save_as_file( WINDOW * );
int  change_fattr( WINDOW * );
int  get_fattr( char far *, int * );
int  set_fattr( char far *, int );
int  get_current_directory( char far *, int );
int  set_current_directory( char far * );
/************************************************************************/


/*************************  function prototypes in findrep.c ************/
int  get_replacement_flags( int );
int  ask_replace( WINDOW *, int * );
int  ask_wrap_replace( WINDOW *, int * );
void do_replace( WINDOW *, int );
int  find_string( WINDOW * );
void build_boyer_array( void );
void build_forward_skip( boyer_moore_type * );
void build_backward_skip( boyer_moore_type * );
int  calculate_forward_md2( char *, int );
int  calculate_backward_md2( char *, int );
line_list_ptr forward_boyer_moore_search( WINDOW *, long *, int * );
line_list_ptr search_forward( line_list_ptr, long *, unsigned * );
line_list_ptr backward_boyer_moore_search( WINDOW *, long *, int * );
line_list_ptr search_backward( line_list_ptr, long *, unsigned * );
void show_search_message( int, int );
void bin_offset_adjust( WINDOW *, long );
void find_adjust( WINDOW *, line_list_ptr, long, int );
int  replace_string( WINDOW * );
int  replace_and_display( WINDOW *, line_list_ptr, long, int, int *, int *, int );
line_list_ptr scan_forward( long *, int *, line_list_ptr, char, char, int * );
line_list_ptr scan_backward( long *, int *, line_list_ptr, char, char, int * );
int  match_pair( WINDOW * );
/************************************************************************/


/*************************  function prototypes in hwind.c **************/
void get_date( int *, int *, int *, int * );
void get_time( int *, int *, int *, int * );
void show_end_mem( void );
void show_end_text( WINDOW * );
void show_modes( void );
void show_file_count( int );
void show_window_count( int );
void show_avail_mem( void );
void show_tab_modes( void );
void show_indent_mode( void );
void show_search_case( void );
void show_sync_mode( void );
void show_wordwrap_mode( void );
void show_trailing( void );
void show_control_z( void );
void show_insert_mode( void );
void my_scroll_down( WINDOW * );
void combine_strings( char *, char *, char *, char * );
void make_ruler( WINDOW * );
void show_ruler( WINDOW * );
void show_ruler_char( WINDOW * );
void show_ruler_pointer( WINDOW * );
void show_all_rulers( void );
/************************************************************************/


/************************  function prototypes in macro.c  **************/
int  record_on_off( WINDOW * );
void record_keys( int );
void show_avail_strokes( void );
int  save_strokes( WINDOW * );
int  load_strokes( WINDOW * );
int  clear_macros( WINDOW * );
void connect_macros( void );
int  play_back( WINDOW * );
int  push_macro_stack( int );
int  pop_macro_stack( int * );
int  pause( WINDOW * );
/************************************************************************/


/*************************  function prototypes in main.c  **************/
void main( int, char *[] );
void error( int, int, char * );
#if defined( __MSC__ )
void interrupt far harmless( void );
void interrupt far ctrl_break( void );
#else
static int harmless( void );
#endif
void terminate( void );
void initialize( void );
void hw_initialize( void );
int  get_help( WINDOW * );
void show_credits( void );
/************************************************************************/


/*************************  function prototypes in port.c *************/
void far * my_malloc( size_t, int * );
void my_free( void far * );
int  my_findfirst( DTA far *, char far *, int );
int  my_findnext( DTA far * );
/************************************************************************/


/*************************  function prototypes in regx.c *************/
int  find_regx( WINDOW * );
line_list_ptr forward_regx_search( WINDOW *, long *, int * );
line_list_ptr regx_search_forward( line_list_ptr, long *, int * );
line_list_ptr backward_regx_search( WINDOW *, long *, int * );
line_list_ptr regx_search_backward( line_list_ptr, long *, int * );
int  nfa_match( void );
int  build_nfa( void );
int  expression( void );
int  term( void );
int  factor( void );
int  escape_char( int );
void emit_cnode( int, int, int, int );
void emit_nnode( int, int, int, int, int );
int  put_dq( int );
int  push_dq( int );
int  pop_dq( void );
int  dequeempty( void );
void init_nfa( void );
void regx_error( char * );
int  separator( int );
int  Kleene_star( int );
int  letter( int );
/**********************************************************************/


/*************************  function prototypes in sort.c *************/
int  sort_box_block( WINDOW *window );
void quick_sort_block( long, long, line_list_ptr, line_list_ptr );
void insertion_sort_block( long, long, line_list_ptr );
void load_pivot( line_list_ptr );
int  compare_pivot( line_list_ptr );
int  my_memcmp( text_ptr, text_ptr, int );
/************************************************************************/


/**********************  function prototypes in tab.c  ******************/
int  tab_key( WINDOW * );
int  backtab( WINDOW * );
int  next_smart_tab( WINDOW * );
int  prev_smart_tab( WINDOW * );
text_ptr entab( text_ptr, int );
void detab_linebuff( void );
void entab_linebuff( void );
text_ptr detab_a_line( text_ptr, int * );
int  detab_adjust_rcol( text_ptr, int );
int  entab_adjust_rcol( text_ptr, int, int );
int  block_expand_tabs( WINDOW * );
int  block_compress_tabs( WINDOW * );
/************************************************************************/


/*************************  function prototypes in utils.c **************/
int  myiswhitespc( int );
void check_virtual_col( WINDOW *, int, int );
void copy_line( line_list_ptr );
int  un_copy_line( line_list_ptr,  WINDOW * , int );
int  un_copy_tab_buffer( line_list_ptr, WINDOW * );
void load_undo_buffer( file_infos *, text_ptr, int );
void set_prompt( char *, int );
int  get_name( char *, int, char *, int );
int  get_sort_order( WINDOW * );
int  get_replace_direction( WINDOW * );
int  get_yn( void );
int  get_lr( void );
int  get_bc( void );
int  get_oa( void );
void show_eof( WINDOW * );
void display_current_window( WINDOW * );
int  redraw_screen( WINDOW * );
void redraw_current_window( WINDOW * );
void show_changed_line( WINDOW * );
void show_curl_line( WINDOW * );
void dup_window_info( WINDOW *, WINDOW * );
void adjust_windows_cursor( WINDOW *, long );
int  first_non_blank( text_ptr, int );
int  find_end( text_ptr, int );
int  is_line_blank( text_ptr, int );
int  page_up( WINDOW * );
int  page_down( WINDOW * );
int  scroll_down( WINDOW * );
int  scroll_up( WINDOW * );
int  pan_up( WINDOW * );
int  pan_down( WINDOW * );
void show_window_header( WINDOW * );
void show_window_number_letter( WINDOW * );
void show_window_fname( WINDOW * );
void show_crlf_mode( WINDOW * );
void show_size( WINDOW * );
int  quit( WINDOW * );
int  move_up( WINDOW * );
int  move_down( WINDOW * );
int  prepare_move_down( WINDOW * );
int  move_left( WINDOW * );
int  move_right( WINDOW * );
int  pan_left( WINDOW * );
int  pan_right( WINDOW * );
int  word_left( WINDOW * );
int  word_right( WINDOW * );
int  next_dirty_line( WINDOW * );
int  prev_dirty_line( WINDOW * );
int  center_window( WINDOW * );
int  horizontal_screen_right( WINDOW * );
int  horizontal_screen_left( WINDOW * );
int  goto_top_file( WINDOW * );
int  goto_end_file( WINDOW * );
int  goto_line( WINDOW * );
int  set_marker( WINDOW * );
int  goto_marker( WINDOW * );
int  date_time_stamp( WINDOW * );
int  add_chars( char *, WINDOW * );
/************************************************************************/


/*************************  function prototypes in window.c *************/
int  initialize_window( void );
void choose_window( char *, WINDOW * );
int  next_window( WINDOW * );
int  prev_window( WINDOW * );
int  split_horizontal( WINDOW * );
int  split_vertical( WINDOW * );
void show_vertical_separator( WINDOW * );
int  size_window( WINDOW * );
int  zoom_window( WINDOW * );
int  next_hidden_window( WINDOW * );
void setup_window( WINDOW * );
void finish( WINDOW * );
int  create_window( WINDOW **, int, int, int, int, file_infos * );
/************************************************************************/


/**********************  function prototypes in wordwrap.c **************/
int  find_left_margin( line_list_ptr, int );
void word_wrap( WINDOW * );
int  format_paragraph( WINDOW *window );
void combine_wrap_spill( WINDOW *, int, int, int, int, int );
void justify_right_margin( WINDOW *, line_list_ptr, int, int, int );
void remove_spaces( int );
int  find_word( text_ptr, int, int );
int  flush_left( WINDOW * );
int  flush_right( WINDOW * );
int  flush_center( WINDOW * );
/************************************************************************/


/*************************  function prototypes in tdeasm.c *************/
unsigned long  ptoul( void far * );
text_ptr tabout( text_ptr, int * );
/*****************************************************/


/***********  function prototype for simul101.asm *************/
void far simulate_enh_kbd( int );
/*****************************************************/


/***********  function prototype for criterr.asm *************/
void far install_ceh( void far * );
/*****************************************************/


/***********  function prototype for criterr.c   *************/
int  far crit_err_handler( void );
void show_error_screen( int, int );
void save_area( char far * );
void restore_area( char far * );
/*****************************************************/
