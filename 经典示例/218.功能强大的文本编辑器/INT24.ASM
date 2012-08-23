;
; Sometimes, things may go wrong that are outside the control of an application
; program.  When an application program needs to access system resources, lots
; of things can go wrong:  hard disk errors, lost network connections, printing
; a file before turning on the printer, etc....  These types of errors are
; known as Critical Errors.  The operating system doesn't know what to do in
; these situations, so it asks the application program for advice.  Ideally,
; the application program should be prepared to handle these situations.  After
; a critical error occurs in MSDOS, an application program will be asked to
; Abort, Retry, or Fail the operation.
;
; In this critical error handler (CEH), we never let the user return
; the Abort choice for the standard Abort, Retry, Fail prompt.  When the
; user selects Abort, all pending work and the application program are
; terminated.  Users can get very frustrated when they can't save their work
; or if they loose everything because an application can't recover from
; a critical error.
;
;------------------------  special case in 2.21  -------------------------
; A couple of days ago, I tried to edit a file on our network drive
; running Novell 3.11.  The file was corrupt and there was no way to
; get out of the TDE critical error handler except by Ctrl-Alt-Del,
; aka Vulcan nerve pinch.  I added the Abort choice to the CEH to keep
; TDE out of an infinite loop.  Don't returning Abort as a critical
; error leave the workstation in an unstable state or something?
; I still lose all pending work with Abort, but at least I can exit TDE.
; Shouldn't Novell recognize the Fail option in addition to Abort and Retry?
;---------------------------------  end  ---------------------------------
;
; The simple critical error handler (CEH) in TDE is designed to be memory
; model independent.  When we first install this CEH, pass in the FAR address
; of a CEH structure.  Save the address of CEH structure in the code segment
; of our assembly CEH replacement.  When a critical error occurs, load in the
; address of our CEH structure and save the return codes in the structure.
; Call the prompt functions in criterr.c, then return from interrupt.
;
; Let's set the error flag only if a Fail or Ignore condition is input by
; the user.  The Ignore option is not explicity supported by this algorithm,
; but it is returned for DOS versions less than 3.0.  The Fail option is
; not available for DOS versions less than 3.0.  If the user selects the
; Retry option, set the ceh.flag to OK, because there is no error.  By
; selecting Retry, the user just wants to retry the DOS operation, which is
; not necessarily an error condition.
;
; See:
;
;   Programmer's Reference Manual, Microsoft Corporation, Redmond,
;     Washington, 1986, Document No. 410630014-320-003-1285, pp. 1-20 thru
;     1-21, pp. 1-34 thru 1-38, p 1-99, pp. 1-121 thru 1-124, pp. 1-216 thru
;     1-218, pp. 2-1 thru 2-30.
;
;   Ray Duncan, _Advanced MS-DOS_, Microsoft Press, Redmond, Washington,
;     1986, ISBN 0-914845-77-2, pp 89-97, pp 130-133.
;
;
; Assembler flags:
;
;      QuickAssembler:   qcl /c int24.asm
;            MASM 6.0:   ml /c /Cp /Zm int24.asm
;
; Editor name:   TDE, the Thomson-Davis Editor.
; Author:        Frank Davis
; Date:          June 5, 1991, version 1.0
; Date:          July 29, 1991, version 1.1
; Date:          October 5, 1991, version 1.2
; Date:          January 20, 1992, version 1.3
; Date:          February 17, 1992, version 1.4
; Date:          April 1, 1992, version 1.5
; Date:          June 5, 1992, version 2.0
; Date:          October 31, 1992, version 2.1
;
; This code is released into the public domain, Frank Davis.  You may
; distribute it freely.

;typedef struct {         typedef from tdestr.h
;   int  flag;
;   int  ecode;
;   int  rw;
;   int  drive;
;   int  extended;
;   int  class;
;   int  action;
;   int  locus;
;   int  dattr;
;   char dname[10];
;} CEH;

flag            EQU     0
ecode           EQU     2
rw              EQU     4
drive           EQU     6
extended        EQU     8
class           EQU     10
action          EQU     12
locus           EQU     14
dattr           EQU     16
dname           EQU     18

; see any DOS tech ref manual for the format of device headers.
;
dev_attr_word   EQU     4
char_dev_name   EQU     10

;
; external C routine in criterr.c
;
        EXTRN   _crit_err_handler:FAR


_TEXT   SEGMENT WORD PUBLIC 'CODE'
        ASSUME  cs:_TEXT, ds:NOTHING, es:NOTHING
        public  _install_ceh


;
; Prototype this function as far in the C header file so it may be used easily
; with any memory model.  See the last section in tdefunc.h for more info.
;
_install_ceh    PROC    FAR
        jmp     initialize

ceh_pointer     DW      ?,?     ; pointer to critical error handler structure
dos_version     DW      ?       ; what version of DOS are we working with?

start:
;
;  the first thing we need to do is turn interrupts back on.  interrupts
;  are disabled when the handler receives control.
;
;  segment register strategy:
;      use ds to access variables in the code segment and the device header
;      use es to access the ceh structure.
;
        sti                     ; turn interrupts back on
        push    bx              ; push required registers
        push    cx
        push    dx
        push    ds
        push    es

        ASSUME  ds:_TEXT, es:NOTHING    ; put cs in ds and tell assembler
                                        ; ds references code, _TEXT seg
        mov     dx, cs
        mov     ds, dx

        mov     dx, di                          ; put error code in dx
        xor     dh, dh                          ; high part of di is undef'ed
        mov     bx, WORD PTR ceh_pointer+2      ; get SEGMENT of ceh struc
        mov     es, bx                          ; put it in es
        mov     di, WORD PTR ceh_pointer        ; get OFFSET of ceh struc
        mov     WORD PTR es:[di].ecode, dx      ; save error code
        mov     dl, ah                          ; move error stuff to dl
        and     dl, 1                           ; 1 == write error
        mov     WORD PTR es:[di].rw, dx         ; save read/write error code

        mov     dl, al                          ; put drive letter in dl
        mov     WORD PTR es:[di].drive, dx      ; save drive number
        test    ah, 80h                 ; was this a character or block dev
        jz      drive_err               ; if fail, then block error
        mov     ds, bp                  ; put SEGMENT of device header in ds
        mov     dx, WORD PTR [si].dev_attr_word ; see DOS tech ref, get dev attr
        test    dx, 8000h               ; was there a prob w/ FAT
        jz      drive_err               ; if bit 15 == 0, drive error - FAT prob
        mov     WORD PTR es:[di].dattr, 1       ; 1 == character device
        mov     bx, di                  ; save OFFSET of CEH struct in bx
        add     di, dname               ; load destination of char dev name
        add     si, char_dev_name       ; char dev
        mov     cx, 8                   ; char name is 8 bytes
        rep     movsb                   ; move 8 bytes of driver name
        xor     al, al                  ; zero out al
        stosb                           ; store a terminating NULL
        mov     di, bx                  ; get back the OFFSET of CEH struct
        jmp     SHORT get_extended_error
        ALIGN   2
drive_err:
        mov     WORD PTR es:[di].dattr, 0       ; 0 == disk drive
get_extended_error:
        mov     dx, cs                  ; put cs back into ds
        mov     ds, dx
        mov     ax, WORD PTR ds:dos_version     ; get DOS version
        or      ax, ax                  ; is it 0?
        jz      no_ext_avail            ; skip extended error

;
; page 1-216, Programmer's Ref Man, 1986, function 59h pretty much wipes
; out all registers that don't return info.  function 59h is available
; for DOS >= 3.
;
        push    di
        push    ds
        push    es
        xor     bx, bx                  ; version indicator, 0 = current
        mov     ah, 59h                 ; function 59h == get extended err
        int     21h                     ; Standard DOS interrupt
        pop     es
        pop     ds
        pop     di
        cmp     ax, 88                          ; check for return code > 88
        ja      no_ext_avail                    ; if ax <= 88, we have info
        mov     WORD PTR es:[di].extended, ax   ; save ext err code
        xor     ax, ax                          ; zero out ax
        mov     al, bh                          ; get error class
        mov     WORD PTR es:[di].class, ax      ; save error class
        mov     al, bl                          ; get suggested action
        mov     WORD PTR es:[di].action, ax     ; save suggested action
        mov     al, ch                          ; get locus
        mov     WORD PTR es:[di].locus, ax      ; save locus
        jmp     SHORT prompt_user               ; now, ask user what to do
        ALIGN   2

no_ext_avail:
        xor     ax, ax                          ; zero out ax
        mov     WORD PTR es:[di].extended, ax   ; not avail
        mov     WORD PTR es:[di].class, ax      ; not avail
        mov     WORD PTR es:[di].action, ax     ; not avail
        mov     WORD PTR es:[di].locus, ax      ; not avail

prompt_user:
        push    di                      ; save OFFSET of CEH struct
        push    es                      ; save SEGMENT OF CEH struct
        push    ds                      ; save code segment in ds
        mov     ax, ss                  ; put stack segment in ds
        mov     ds, ax                  ; C routine expects stack seg in ds
        call    FAR PTR _crit_err_handler       ; return code is in ax
        pop     ds                      ; get back our registers
        pop     es
        pop     di

        mov     bx, WORD PTR ds:dos_version     ; get the DOS version
        or      bx, bx                          ; is bx == 0?
        jne     retry_or_fail                   ; DOS version is >= 3
        cmp     ax, 3                           ; did user try fail?
        jne     retry_or_fail                   ; cannot fail with DOS < 3
        mov     ax, 0                           ; change it to Ignore
        ALIGN   2

retry_or_fail:
        mov     dx, -1                          ; if we fail, store a -1
        cmp     ax, 1                           ; did user do a retry?
        jne     end_int_24                      ; if retry, change error code
        xor     dx, dx                          ;   reset error flag on retry

end_int_24:
        mov     WORD PTR es:[di].flag, dx       ; save ceh flag
        pop     es
        pop     ds
        pop     dx
        pop     cx
        pop     bx              ; pop registers
        iret                    ; return from interrupt

; ***********************************************************************
; prototype for _install_ceh in the C header file, tdefunc.h, is
;
;               void far install_ceh( void far * )
;
; The formal parameters are available on the stack.  Use bp register to
; access them.
;
; ***********************************************************************

initialize:
        push    bp              ; setup bp to access formal parameter
        mov     bp, sp

        push    ds              ; save ds

        ASSUME  ds:_TEXT        ; tell assembler ds references code, _TEXT seg
        mov     ax, cs
        mov     ds, ax          ; put cs in ds - required by function 25h, below

        mov     ah, 30h         ; function 30h == get DOS version
        int     21h             ; DOS interrupt
        xor     bx, bx          ; start out with zero for DOS version
        cmp     al, 3           ; compare major version
        jl      store_dos       ; if less than version 3, store 0
        mov     bx, 3           ; else store 3 for DOS >= 3
store_dos:
        mov     WORD PTR dos_version, bx        ; save DOS info for my int 24

        mov     ax, WORD PTR [bp+6]             ; load OFFSET of void FAR *
        mov     WORD PTR ceh_pointer, ax        ; save OFFSET of void FAR *
        mov     ax, WORD PTR [bp+8]             ; load SEGMENT of void FAR *
        mov     WORD PTR ceh_pointer+2, ax      ; save SEGMENT of void FAR *

        mov     dx, OFFSET start        ; get new offset of int 24
                                        ; we have already set ds above
        mov     ax, 2524h               ; use function 25 so int 24 points
        int     21h                     ;  to my critical error routine

        pop     ds              ; clean up
        pop     bp
        retf
_install_ceh            endp
_TEXT   ends
        end
