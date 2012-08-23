

#include "tdestr.h"
#include "common.h"
#include "define.h"
#include "tdefunc.h"


/*
 * Name:    dir_help
 * Purpose: To prompt the user and list the directory contents
 * Date:    February 13, 1992
 * Passed:  window:  pointer to current window
 */
int  dir_help( WINDOW *window )
{
char dname[MAX_COLS+2]; /* directory search pattern */
char stem[MAX_COLS+2];  /* directory stem */
char drive[_MAX_DRIVE]; /* splitpath drive buff */
char dir[_MAX_DIR];     /* splitpath dir buff */
char fname[_MAX_FNAME]; /* splitpath fname buff */
char ext[_MAX_EXT];     /* splitpath ext buff */
int  rc;
int  file_mode;
int  bin_length;
int  prompt_line;

   if (window != NULL) {
      entab_linebuff( );
      if (un_copy_line( window->ll, window, TRUE ) == ERROR)
         return( ERROR );
      prompt_line = window->bottom_line;
   } else
      prompt_line = g_display.nlines;

   /*
    * search path or pattern
    */
   dname[0] = '\0';
   rc = get_name( dir1, prompt_line, dname, g_display.message_color );

   if (rc == OK) {
      if (validate_path( dname, stem ) == OK) {
         rc = list_and_pick( dname, stem, window );

         /*
          * if everything is everything, load in the file selected by user.
          */
         if (rc == OK) {
            file_mode = TEXT;
            bin_length = 0;
            _splitpath( dname, drive, dir, fname, ext );
            if (stricmp( ext, ".exe" ) == 0  ||  stricmp( ext, ".com" ) == 0) {
               file_mode = BINARY;
               bin_length = g_status.file_chunk;
            }
            if (window != NULL)
               attempt_edit_display( dname, LOCAL, file_mode, bin_length );
            else
               attempt_edit_display( dname, GLOBAL, file_mode, bin_length );
         }
      } else
         /*
          * invalid path or file name
          */
         error( WARNING,
                window != NULL ? window->bottom_line : g_display.nlines, dir2 );
   }
   return( rc );
}


/*
 * Name:    validate_path
 * Purpose: make sure we got a valid search pattern or subdirectory
 * Date:    February 13, 1992
 * Passed:  dname: search path entered by user
 *          stem:  directory stem is returned
 * Returns: successful or not
 * Notes:   we need to validate the search path or pattern.  if the search
 *            pattern is valid, then we need to get the search stem.
 *          the user may enter a subdirectory or some kind of search pattern.
 *             if the user enters a subdirectory, then there are a few things
 *             we need to take care of  1) find out if the subdirectory is
 *             the root, 2) append a '\' to the subdirectory so we can create
 *             a search pattern for the subdirectory, 3) don't append '\' to
 *             the root, it already has a '\'.
 *          if the user enters a search pattern, then we need to dissect the
 *             search path.  we must create a stem from the search pattern.
 */
int  validate_path( char *dname, char *stem )
{
int  rc;
DTA  dta;               /* temp disk transfer struct for findfirst, etc. */
int  fattr;
int  i;
int  len;
char *p;
char temp[MAX_COLS+2];  /* directory stem */

   /*
    * if path name is void then the current working directory is implied.
    */
   if (dname[0] == '\0') {

      assert( strlen( stardotstar ) < MAX_COLS );

      strcpy( dname, stardotstar );
      stem[0] = '\0';
      rc = OK;
   } else {

      /*
       * get the attributes of the search pattern, so we can determine if
       * this is a pattern or subdirectory.
       */
      rc = get_fattr( dname, &fattr );

      if (rc == OK && (fattr & SUBDIRECTORY)) {
         assert( strlen( dname ) < MAX_COLS );
         strcpy( stem, dname );

         /*
          * if this is the root directory ( \ ), don't append '\' to it.
          * user entered a subdirectory - append *.* to get contents of
          * subdirectory.
          */
         len = strlen( stem );
         if (stem[len-1] != '\\') {
            strcat( stem, "\\" );
            strcat( dname, "\\" );
         }
         strcat( dname, stardotstar );

      /*
       * not a subdirectory.  let's see if any files match the search
       * pattern.
       */
      } else if (rc != ERROR) {
         if ((rc = my_findfirst( &dta, dname, NORMAL | READ_ONLY | HIDDEN |
                              SYSTEM | SUBDIRECTORY | ARCHIVE )) == OK) {

            /*
             * copy dname to "temp" so we can use "temp" to find the stem.
             *    we need to search the pattern backwards to figure the stem.
             */

            assert( strlen( dname ) < MAX_COLS );

            strcpy( temp, dname );
            len = strlen( dname );
            for (i=len,p=temp+len; i>=0; i--) {
               /*
                *  if we run into the '\' or the ':', then we got a stem.
                */
               if (*p == '\\' || *p == ':') {
                  p = temp + i;
                  *(p+1) = '\0';
                  break;
               /*
                * if we're at the beginning of the string, stem == '\0'
                */
               } else if (i == 0) {
                  *p = '\0';
                  break;
               }
               --p;
            }

            assert( strlen( temp ) < MAX_COLS );

            strcpy( stem, temp );
         } else
            rc = ERROR;

      /*
       * user did not enter a valid subdirectory name or search pattern.
       */
      } else
         rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    list_and_pick
 * Purpose: To show matching file names and let user pick a file
 * Date:    February 13, 1992
 * Passed:  dname:  directory search pattern
 *          stem:   stem of directory search pattern
 *          window:  pointer to current window
 * Returns: return code from pick.  rc = OK, then edit a new file.
 * Notes:   real work routine of this function.  save the cwd and let the
 *           user search upwards or downwards thru the directory structure.
 *          since we are doing DOS directory functions, we need to check the
 *           return code after each DOS call for critical errors.
 */
int  list_and_pick( char *dname, char *stem, WINDOW *window )
{
int  rc;
DTA  dta;               /* disk transfer address for findfirst */
DIRECTORY dir;          /* contains all info for dir display */
unsigned int cnt;       /* number of matching files */
FTYPE *flist, *p;       /* pointer to list of matching files */
char cwd[MAX_COLS];     /* save the current working directory in this buff */
char dbuff[MAX_COLS];   /* temporary directory buff */
char prefix[MAX_COLS];  /* directory prefix  */
int  change_directory = FALSE;
int  stop;
int  len;
int  drive;

   /*
    * Some algorithms alloc the maximum possible number of files in
    *  a directory, eg. 256 or 512.  Let's count the number of matching
    *  files so we know egxactly how much memory to request from calloc.
    *  Depending on the op system, disk media, disk format, or version of DOS,
    *  the max number of files may vary, anyway, also, additionally.
    */
   rc = my_findfirst( &dta, dname, NORMAL | READ_ONLY | HIDDEN | SYSTEM |
                                SUBDIRECTORY | ARCHIVE );
   if (rc != ERROR) {
      for (cnt=1; (rc = my_findnext( &dta )) == OK;)
         ++cnt;
      flist = (FTYPE *)calloc( cnt, sizeof(FTYPE) );
   } else
      flist = NULL;
   if (rc != ERROR && flist != NULL) {

      stop = FALSE;
      /*
       * If user entered drive name in search pattern, find out the drive and
       *  directory stem.
       */
      if (stem[1] == ':') {

         /*
          * If the second character of the search pattern is a ':', the
          *  the first character of the pattern should be the drive.
          *  Convert drive to lower case and get a numerical representation.
          * CAVEAT:  In DOS v 2.x, there may be up to 63 logical drives.
          *   my algorithm may blow up if the number of logical drives
          *   is greater than 'Z'.
          * For DOS >= 3, the number of drives is limited to 26, I think.
          */
         drive = stem[0];
         if (drive < 'a')
            drive += 32;
         drive = drive - 'a' + 1;
         rc = get_current_directory( dbuff, drive );
         if (rc == ERROR)
            stop = TRUE;
         else {

            /*
             * Put drive letter, ':', and '\' in front of current directory.
             */
            prefix[0] = (char)(drive - 1 + 'a');
            prefix[1] = ':';
            prefix[2] = '\\';
            prefix[3] = '\0';
            assert( strlen( prefix ) + strlen( dbuff ) < MAX_COLS );
            strcpy( cwd, prefix );
            strcat( cwd, dbuff );
         }

      /*
       * else get current directory from default drive
       */
      } else {

         /*
          * 0 = default drive.
          */
         drive = 0;
         rc = get_current_directory( dbuff, drive );
         if (rc == ERROR)
            stop = TRUE;
         else {

            /*
             * Put a '\' in front of the current directory.
             */
            prefix[0] = '\\';
            prefix[1] = '\0';

            assert( strlen( prefix ) + strlen( dbuff ) < MAX_COLS );

            strcpy( cwd, prefix );
            strcat( cwd, dbuff );
         }
      }

      while (stop == FALSE) {
         /*
          * If we had enough memory, find all matching file names.  Append
          *  '\\' at the end of subdirectory names so user will know if
          *  name is a directory.  Might as well find everything, because
          *  i also forget subdirectory names, too.
          *
          * when we get here, we have already done: 1) my_findfirst and
          *  my_findnext, 2) counted the number of matching files, and
          *  3) allocated space.
          */
         p = flist;
         cnt = 0;

         rc = my_findfirst( &dta, dname, NORMAL | READ_ONLY | HIDDEN | SYSTEM |
                                 SUBDIRECTORY | ARCHIVE );
         if (rc != ERROR) {

            /*
             * p is pointer that walks down the file info structure.
             *  save the file name, file size, and directory character,
             *  if needed, for each matching file we find.
             */

            assert( strlen( dta.name ) < 14 );

            strcpy( p->fname, dta.name );
            p->fsize = dta.size;
            if (dta.attrib & SUBDIRECTORY)
               strcat( p->fname, "\\" );
            for (cnt=1; (rc = my_findnext( &dta )) == OK; ) {
               ++p;

               assert( strlen( dta.name ) < 14 );

               strcpy( p->fname, dta.name );
               p->fsize = dta.size;
               if (dta.attrib & SUBDIRECTORY)
                  strcat( p->fname, "\\" );
               cnt++;
            }
         }

         if (rc != ERROR) {
            shell_sort( flist, cnt );

            /*
             * figure out number of rows, cols, etc... then display dir list
             */
            setup_directory_window( &dir, cnt );
            write_directory_list( flist, dir );

            /*
             * Let user select file name or another search directory.
             *  Save the choice in dbuff.  rc == OK if user selected file or dir.
             */
            rc = select_file( flist, stem, &dir );

            assert( strlen( flist[dir.select].fname ) < MAX_COLS );

            strcpy( dbuff, flist[dir.select].fname );
         }

         /*
          *  give memory back.
          */
         free( flist );

         if (rc == ERROR)
            stop = TRUE;
         else {
            len = strlen( dbuff );

            /*
             * If the last character in a file name is '\' then let's
             *  do a dir on selected directory.  See the matching
             *  else when the user selects a file.
             */
            if (dbuff[len-1] == '\\') {

               /*
                * Stem has subdirectory path.  dbuff has selected path.
                * Create a new dname with stem and dbuff.
                */

               assert( strlen( stem ) + strlen( dbuff ) < MAX_COLS );

               strcpy( dname, stem );
               strcat( dname, dbuff );
               len = strlen( dname );
               strcpy( dbuff, dname );

               /*
                * The last character in dbuff is '\', because we append the
                *  '\' to every directory entry in the file list.  Replace
                *  it with a NULL char then we will have a valid path name.
                */
               dbuff[len-1] = '\0';

               /*
                * now let's change to the selected subdirectory.
                */
               rc = set_current_directory( dbuff );
               if (rc == OK) {

                  /*
                   * Every time we change directories, we need to get the
                   *  current directory so we will be sure to have the
                   *  correct path.
                   */
                  rc = get_current_directory( dbuff, drive );
                  if (rc == OK) {

                     assert( strlen( prefix ) + strlen( dbuff ) < MAX_COLS );

                     strcpy( dname, prefix );
                     strcat( dname, dbuff );
                     change_directory = TRUE;
                  }
               }

               /*
                * Validate the new path and allocate memory for the
                *  matching files.
                */
               if (rc == OK)
                  rc = validate_path( dname, stem );
               if (rc == OK) {
                  rc = my_findfirst( &dta, dname, NORMAL | READ_ONLY | HIDDEN |
                                  SYSTEM | SUBDIRECTORY | ARCHIVE );
                  if (rc != ERROR) {
                     for (cnt=1; (rc = my_findnext( &dta )) == OK;)
                        ++cnt;
                     flist = (FTYPE *)calloc( cnt, sizeof(FTYPE) );
                  }
               }
               if (flist == NULL || rc == ERROR) {
                  stop = TRUE;
                  rc = ERROR;
               }
            } else {

               /*
                * user selected a file.  store fname in dname and return.
                */
               rc = OK;
               stop = TRUE;

               assert( strlen( stem ) + strlen( dbuff ) < MAX_COLS );

               strcpy( dname, stem );
               strcat( dname, dbuff );
            }
         }
      }

      /*
       * Go back to the current directory if needed.
       */
      if (change_directory)
         set_current_directory( cwd );
      if (window != NULL)
         redraw_screen( window );
   } else {
      /*
       * out of memory
       */
      error( WARNING,  window != NULL ? window->bottom_line : g_display.nlines,
             dir3 );
      rc = ERROR;
   }
   return( rc );
}


/*
 * Name:    setup_directory_window
 * Purpose: set number of rows and cols in directory window
 * Date:    February 13, 1992
 * Passed:  dir: pointer to directory structure
 *          cnt: number of files
 * Notes:   set up stuff we need to know about how to display files.
 */
void setup_directory_window( DIRECTORY *dir, int cnt )
{
int  i;
int  wid;
char temp[MAX_COLS];    /* line to output */

   /*
    * setup the fixed vars used in dir display.
    *    dir->col =      physical upper left column of dir screen
    *    dir->row =      physical upper left row or line of dir screen
    *    dir->wid =      width of physical screen
    *    dir->hgt =      height of physical screen
    *    dir->max_cols   number of columns of files in dir screen
    *    dir->max_lines  number of lines of files in each column in dir screen
    *    dir->cnt        number of files in list
    */
   dir->col = 3;
   dir->row = 5;
   wid = dir->wid = 72;
   dir->hgt = 16;
   dir->max_cols = 5;
   dir->max_lines = 9;
   dir->cnt = cnt;

   /*
    * Find out how many lines in each column are needed to display
    *  matching files.
    */
   dir->lines = dir->cnt / dir->max_cols + (dir->cnt % dir->max_cols ? 1 : 0);
   if (dir->lines > dir->max_lines)
      dir->lines = dir->max_lines;

   /*
    * Find out how many columns of file names we need.
    */
   dir->cols = dir->cnt / dir->lines + (dir->cnt % dir->lines ? 1 : 0);
   if (dir->cols > dir->max_cols)
      dir->cols = dir->max_cols;


   /*
    * Find the maximun number of file names we can display in help screen.
    */
   dir->avail = dir->lines * dir->cols;

   /*
    * Now find the number of file names we do have on the screen.  Every
    *  time we slide the "window", we have to calculate a new nfiles.
    */
   dir->nfiles = dir->cnt > dir->avail ? dir->avail : dir->cnt;

   /*
    * A lot of times, the number of matching files will not fit evenly
    *  in our help screen.  The last column on the right will be partially
    *  filled, hence the variable name prow (partial row).  When there are
    *  more file names than can fit on the screen, we have to calculate
    *  prow every time we slide the "window" of files.
    */
   dir->prow = dir->lines - (dir->avail - dir->nfiles);

   /*
    * Find out how many "virtual" columns of file names we have.  If
    *  all the files can fit in the dir screen, there will be no
    *  virtual columns.
    */
   if (dir->cnt < dir->avail)
      dir->vcols = 0;
   else
      dir->vcols =  (dir->cnt - dir->avail) / dir->max_lines +
                   ((dir->cnt - dir->avail) % dir->max_lines ? 1 : 0);

   /*
    * Find the physical display column in dir screen.
    */
   dir->flist_col[0] = dir->col + 2;
   for (i=1; i<dir->max_cols; i++)
      dir->flist_col[i] = dir->flist_col[i-1] + 14;

   /*
    * Now, draw the borders of the dir screen.
    */
   for (i=0; i < dir->hgt; i++) {
      if (i == 0 || i == dir->hgt-1) {
         memset( temp, 'Ä', wid );
         temp[wid] = '\0';
         if (i == 0) {
            temp[0] = 'Ú';
            temp[wid-1] = '¿';
         } else {
            temp[0] = 'À';
            temp[wid-1] = 'Ù';
         }
      } else {
         memset( temp, ' ', wid );
         temp[wid] = '\0';
         temp[0] = temp[wid-1] = '³';
      }
      s_output( temp, dir->row+i, dir->col, g_display.help_color );
   }

   /*
    * Write headings in help screen.
    */
   s_output( dir4, dir->row+1, dir->col+3, g_display.help_color );
   s_output( dir5, dir->row+2, dir->col+3, g_display.help_color );
   s_output( dir6, dir->row+2, dir->col+44, g_display.help_color );
   s_output( dir7, dir->row+14, dir->col+8, g_display.help_color );
}


/*
 * Name:    write_directory_list
 * Purpose: given directory list, display matching files
 * Date:    February 13, 1992
 * Passed:  flist: pointer to list of files
 *          dir:   directory display structure
 * Notes:   blank out the previous file name and display the new one.
 */
void write_directory_list( FTYPE *flist, DIRECTORY dir )
{
FTYPE *p, *top;
int  i;
int  j;
int  k;
int  end;
int  line;
int  col;
int  color;

   color = g_display.help_color;
   top = flist;
   for (i=0; i < dir.lines; ++i) {
      p = top;
      end = FALSE;
      for (j=0; j < dir.cols; ++j) {
         col = dir.flist_col[j];
         line = i + dir.row + 4;

         /*
          * We need to blank out all lines and columns used to display
          *  files, because there may be some residue from a previous dir
          */
         s_output( "            ", line, col, color );
         if (!end) {
            s_output( p->fname, line, col, color );
            p += dir.lines;
            k = p - flist;
            if (k >= dir.nfiles)
               end = TRUE;
         }
      }
      ++top;
   }
}


/*
 * Name:    select_file
 * Purpose: To let user select a file from dir list
 * Date:    February 13, 1992
 * Passed:  flist: pointer to list of files
 *          stem:  base directory
 *          dir:   directory display stuff
 * Notes:   let user move thru the file names with the cursor keys
 */
int  select_file( FTYPE *flist, char *stem, DIRECTORY *dir )
{
int  ch;                /* input character from user */
int  func;              /* function of character input by user */
int  fno;               /* index into flist of the file under cursor */
int  goodkey;           /* is key a recognized function key? */
int  r;                 /* current row of cursor */
int  c;                 /* current column of cursor */
int  offset;            /* offset into file list */
int  stop;              /* stop indicator */
int  stem_len;          /* stem length */
int  color;             /* color of help screen */
int  file_color;        /* color of current file */
int  change;            /* boolean, hilite another file? */
int  oldr;              /* old row */
int  oldc;              /* old column */
char asize[20];         /* ascii file size */
char blank[20];         /* blank out file names */

   /*
    * initial everything.
    */
   memset( blank, ' ', 12 );
   blank[12] = '\0';
   c = r = 1;
   ch = fno = offset = 0;
   color = g_display.help_color;
   file_color = g_display.hilited_file;
   goodkey = TRUE;
   stop = FALSE;
   stem_len = strlen( stem );
   s_output( stem, dir->row+1, dir->col+19, color );
   s_output( flist[fno].fname, dir->row+1, dir->col+19+stem_len, color );
   ltoa( flist[fno].fsize, asize, 10 );
   s_output( blank, dir->row+2, dir->col+19, color );
   s_output( asize, dir->row+2, dir->col+19, color );
   itoa( dir->cnt,  asize, 10 );
   s_output( blank, dir->row+2, dir->col+57, color );
   s_output( asize, dir->row+2, dir->col+57, color );
   xygoto( (c-1)*14+dir->col+2, r+dir->row+3 );
   hlight_line( (c-1)*14+dir->col+2, r+dir->row+3, 12, file_color );
   change = FALSE;
   while (stop == FALSE) {
      oldr = r;
      oldc = c;
      ch = getkey( );
      func = getfunc( ch );

      /*
       * User may have redefined the Enter and ESC keys.  Make the Enter key
       *  perform a Rturn in this function. Make the ESC key do an AbortCommand.
       */
      if (ch == RTURN)
         func = Rturn;
      else if (ch == ESC)
         func = AbortCommand;

      switch (func) {
         case Rturn       :
         case NextLine    :
         case BegNextLine :
            stop = TRUE;
            break;
         case AbortCommand :
            stop = TRUE;
            break;
         case LineUp :
            if (r > 1) {
               change = TRUE;
               --r;
            } else {
               r = dir->lines;
               change = TRUE;
               if (offset == 0 || c > 1) {
                  if (c > 1)
                     --c;
               } else if (dir->vcols > 0 && offset > 0 && c == 1) {
                  /*
                   * recalculate the dir display stuff.
                   */
                  offset -= dir->lines;
                  recalculate_dir( dir, flist, offset );
               }
            }
            goodkey = TRUE;
            break;
         case LineDown :
            if (r < dir->prow) {
               change = TRUE;
               ++r;
            } else if (r < dir->lines && c != dir->cols) {
               change = TRUE;
               ++r;
            } else {
               change = TRUE;
               r = 1;
               if (offset == dir->vcols * dir->lines || c < dir->cols) {
                  if (c < dir->cols)
                     ++c;
               } else if (dir->vcols > 0 && offset < dir->vcols * dir->lines &&
                         c == dir->cols) {
                  offset += dir->lines;
                  recalculate_dir( dir, flist, offset );
               }
            }
            goodkey = TRUE;
            break;
         case CharLeft :
            if (offset == 0 || c > 1) {
               if (c > 1) {
                  change = TRUE;
                  --c;
               }
            } else if (dir->vcols > 0 && offset > 0 && c == 1) {
               change = TRUE;

               /*
                * recalculate the dir display stuff.
                */
               offset -= dir->lines;
               recalculate_dir( dir, flist, offset );
            }
            goodkey = TRUE;
            break;
         case CharRight :
            if (offset == dir->vcols * dir->lines || c < dir->cols) {
               if (c < dir->cols) {
                  change = TRUE;
                  ++c;
                  if (c == dir->cols) {
                     if ( r > dir->prow)
                        r = dir->prow;
                  }
               }
            } else if (dir->vcols > 0 && offset < dir->vcols * dir->lines &&
                         c == dir->cols) {
               change = TRUE;
               offset += dir->lines;
               recalculate_dir( dir, flist, offset );
               if (r > dir->prow)
                  r = dir->prow;
            }
            goodkey = TRUE;
            break;
         case BegOfLine :
            change = TRUE;
            c = r = 1;
            goodkey = TRUE;
            break;
         case EndOfLine :
            change = TRUE;
            r = dir->prow;
            c = dir->cols;
            goodkey = TRUE;
            break;
         case ScreenDown :
            change = TRUE;
            r = (c == dir->cols) ? r = dir->prow : dir->lines;
            goodkey = TRUE;
            break;
         case ScreenUp :
            change = TRUE;
            r = 1;
            goodkey = TRUE;
            break;
         default :
            break;
      }
      if (goodkey) {
         s_output( blank, dir->row+1, dir->col+19+stem_len, color );
         fno = offset + (c-1)*dir->lines + (r-1);
         s_output( flist[fno].fname, dir->row+1, dir->col+19+stem_len, color );
         ltoa( flist[fno].fsize, asize, 10 );
         s_output( blank, dir->row+2, dir->col+19, color );
         s_output( asize, dir->row+2, dir->col+19, color );
         xygoto( (c-1)*14+dir->col+2, r+dir->row+3 );
         goodkey = FALSE;
         if (change) {
            hlight_line( (oldc-1)*14+dir->col+2, oldr+dir->row+3, 12, color );
            hlight_line( (c-1)*14+dir->col+2, r+dir->row+3, 12, file_color );
            change = FALSE;
         }
      }
   }
   dir->select = fno;
   return( func == AbortCommand ? ERROR : OK );
}


/*
 * Name:    recalculate_dir
 * Purpose: To recalcute dir structure when cursor goes ahead or behind screen
 * Date:    February 13, 1992
 * Passed:  dir:    pointer to file structure
 *          flist:  pointer to file structure
 *          offset: number of files from beginning of flist
 * Notes:   Find new number of files on the screen.  Then, find out
 *           how many files names are in the last column.
 */
void recalculate_dir( DIRECTORY *dir , FTYPE *flist, int offset )
{
register int off;

   off = offset;
   dir->nfiles = (dir->cnt - off) > dir->avail ? dir->avail :
                (dir->cnt - off);
   dir->prow = dir->lines - (dir->avail - dir->nfiles);
   write_directory_list( flist+off, *dir );
}


/*
 * Name:    shell_sort
 * Purpose: To sort file names
 * Date:    February 13, 1992
 * Passed:  flist: pointer to file structure
 *          cnt:   number of files to sort
 * Notes:   this implementation of Shellsort is based on the one by Robert
 *           Sedgewick on page 109, _Algorithms in C_.
 */
void shell_sort( FTYPE *flist, int cnt )
{
int  i;
register int j;
register int inc;
FTYPE temp;
FTYPE *fl;

   if (cnt > 1) {
      fl = flist;

      /*
       * figure the increments, per Donald Knuth, _Sorting and Searching_, and
       *  Robert Sedgewick, _Algorithms in C_.
       */
      j = cnt / 9;
      for (inc=1; inc <= j; inc = 3 * inc + 1);

      /*
       * now, Shellsort the directory file names.
       */
      for (; inc > 0; inc /= 3) {
         for (i=inc; i < cnt; i++) {
            j = i;
            memcpy( &temp, fl+j, sizeof(FTYPE) );
            while (j >= inc && memcmp( fl[j-inc].fname, temp.fname, 14 ) > 0) {
               memcpy( fl+j, fl+j-inc, sizeof(FTYPE) );
               j -= inc;
            }
            memcpy( fl+j, &temp, sizeof(FTYPE) );
         }
      }
   }
}
