
#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"
#include "define.h"


/*
 * Name:    my_malloc
 * Purpose: malloc from the far heap
 * Date:    April 1, 1993
 * Passed:  mem:  pointer to memory to free in far heap
 *          rc:   pointer to return code
 * Notes:   set the return code only if an ERROR occured with malloc.
 *           returning a NULL pointer is not neccessarily an ERROR.
 */
void far * my_malloc( size_t size, int *rc )
{
void far *mem;

   assert( size < MAX_LINE_LENGTH );

   if (size == 0)

      /*
       * if 0 bytes are requested, return NULL
       */
      mem = NULL;
   else {

#if defined( __MSC__ )
      mem = _fmalloc( size );
#else
      mem = farmalloc( size );
#endif

      /*
       * if malloc failed, return NULL and an ERROR.
       */
      if (mem == NULL)
         *rc = ERROR;
   }
   return( mem );
}


/*
 * Name:    my_free
 * Purpose: free memory from the far heap
 * Date:    April 1, 1993
 * Passed:  mem:  pointer to memory to free in far heap
 */
void my_free( void far *mem )
{
   assert( mem != NULL );

#if defined( __MSC__ )
   _ffree( mem );
#else
   farfree( mem );
#endif
}


/*
 * Name:    my_findfirst
 * Purpose: find the first file matching a pattern using DOS interrupt
 * Date:    January 6, 1992
 * Passed:  dta:    disk transfer address
 *          path:   path to search for files
 *          f_attr: attributes of files to search for
 * Notes:   return codes for my_findfirst:
 *             0  no error
 *             2  file is invalid or does not exist
 *             3  path is invalid or does not exist
 *            18  no matching directory entry was found
 *            -1  check the critical error flag for critical errors
 */
int  my_findfirst( DTA far *dta, char far *path, int f_attr )
{
void far *old_dta;
void far *new_dta;
int  rc;

   new_dta = (void far *)dta;

   ASSEMBLE {

/*
; save the old dta
*/
        mov     ah, 0x2f                /* DOS get dta */
        int     0x21                    /* DOS interrupt */
        mov     WORD PTR old_dta, bx    /* save OFFSET of old DTA */
        mov     ax, es
        mov     WORD PTR old_dta+2, ax  /* save SEGMENT of old DTA */

/*
; set the new dta
*/
        push    ds                      /* save ds */
        mov     dx, WORD PTR new_dta    /* get OFFSET of new dta */
        mov     ax, WORD PTR new_dta+2  /* get SEGMENT of new dta */
        mov     ds, ax                  /* put it in ds */
        mov     ah, 0x1a                /* DOS set dta */
        int     0x21                    /* DOS interrupt */
        pop     ds                      /* get back ds */

/*
; find first matching file
*/
        push    ds                      /* save ds */
        mov     cx, WORD PTR f_attr     /* file attributes to search for */
        mov     dx, WORD PTR path       /* get OFFSET of path */
        mov     ax, WORD PTR path+2     /* get SEGMENT of path */
        mov     ds, ax                  /* put it in ds */
        mov     ah, 0x4e                /* DOS find first file */
        int     0x21                    /* DOS interrupt */
        pop     ds                      /* get back ds */

/*
; save the return code
*/
        jc      an_error                /* carry is set if an error occured */
        xor     ax, ax                  /* zero out ax, return OK if no error */
   }
an_error:

   ASSEMBLE {
        mov     WORD PTR rc, ax         /* save the return code */

/*
; get back old dta
*/
        push    ds                      /* save ds */
        mov     dx, WORD PTR old_dta    /* get OFFSET of old dta */
        mov     ax, WORD PTR old_dta+2  /* get SEGMENT of old dta */
        mov     ds, ax                  /* put it in ds */
        mov     ah, 0x1a                /* DOS set dta */
        int     0x21                    /* DOS interrupt */
        pop     ds                      /* get back ds */
   }
   if (ceh.flag == ERROR)
      rc = ERROR;
   return( rc );
}


/*
 * Name:    my_findnext
 * Purpose: find the next file matching a pattern using DOS interrupt
 * Date:    January 6, 1992
 * Passed:  dta:  disk transfer address
 * Notes:   my_findfirst() MUST be called before calling this function.
 *          return codes for my_findnext (see DOS tech ref manuals):
 *             0  no error
 *             2  path is invalid or does not exist
 *            18  no matching directory entry was found
 *            -1  check the critical error flag for critical errors
 */
int  my_findnext( DTA far *dta )
{
void far *old_dta;
void far *new_dta;
int  rc;

   new_dta = (void far *)dta;

   ASSEMBLE {

/*
; save the old dta
*/
        mov     ah, 0x2f                /* DOS get dta */
        int     0x21                    /* DOS interrupt */
        mov     WORD PTR old_dta, bx    /* save OFFSET of old DTA */
        mov     ax, es
        mov     WORD PTR old_dta+2, ax  /* save SEGMENT of old DTA */

/*
; set the new dta
*/
        push    ds                      /* save ds */
        mov     dx, WORD PTR new_dta    /* get OFFSET of new dta */
        mov     ax, WORD PTR new_dta+2  /* get SEGMENT of new dta */
        mov     ds, ax                  /* put it in ds */
        mov     ah, 0x1a                /* DOS set dta */
        int     0x21                    /* DOS interrupt */
        pop     ds                      /* get back ds */

/*
; find next matching file
*/
        mov     ah, 0x4f                /* DOS find first file */
        int     0x21                    /* DOS interrupt */

/*
; save the return code
*/
        jc      an_error                /* carry is set if an error occured */
        xor     ax, ax                  /* zero out ax, return OK if no error */
   }
an_error:

   ASSEMBLE {
        mov     WORD PTR rc, ax         /* save the return code */

/*
; get back old dta
*/
        push    ds                      /* save ds */
        mov     dx, WORD PTR old_dta    /* get OFFSET of old dta */
        mov     ax, WORD PTR old_dta+2  /* get SEGMENT of old dta */
        mov     ds, ax                  /* put it in ds */
        mov     ah, 0x1a                /* DOS set dta */
        int     0x21                    /* DOS interrupt */
        pop     ds                      /* get back ds */
   }
   if (ceh.flag == ERROR)
      rc = ERROR;
   return( rc );
}
