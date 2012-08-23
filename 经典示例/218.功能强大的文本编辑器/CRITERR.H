/*
 * Most of this info is regurgitated stuff from IBM and Microsoft Technical
 * Reference manuals and the Microsoft Knowledge Base.  The error messages
 * are copyright by either Microsoft or IBM.
 *
 * See:
 *
 *   Microsoft Corporation, _Programmer's Reference Manual_, 1986,
 *    Redmond, Washingtion, Document No. 410630014-320-003-1285,
 *    pp. 1-20 thru 1-21, 1-37 thru 1-38, 1-216 thru 1-218.
 *
 *   IBM Corporation, _Disk Operating System: Technical Reference_, 1985,
 *    Boca Raton, Florida, pp. 6-14 thru 6-21, 6-38 thru 6-44.
 *
 *   Microsoft Knowledge Base, "Extended Error Code Information",
 *    Microsoft Corporation, Redmond, Wash., 1992, Document Number: Q74463,
 *    Publication Date:  March 24, 1993.
 *
 *
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             April 1, 1992
 *
 * This program is released into the public domain, Frank Davis.
 *   You may distribute it freely.
 */

char *criterr_screen[] = {
"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",
"³                  tde critical error handler                       ³",
"³                                                                   ³",
"³    Error code:                                                    ³",
"³     Operation:                                                    ³",
"³         Drive:                                                    ³",
"³ Extended Code:                                                    ³",
"³   Error Class:                                                    ³",
"³         Locus:                                                    ³",
"³   Device Type:                                                    ³",
"³   Device Name:                                                    ³",
"³                                                                   ³",
"³            Please enter action:  (Q)uit or (R)etry?               ³",
"³               (ONLY AS A LAST RESORT -- (A)bort)                  ³",
"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",
NULL
};


/*
 * codes from register di
 */
char *error_code[] = {
   "Attempt to write on write-protected disk",    /* 0 */
   "Unknown unit",
   "Drive not ready",
   "Unknown command",
   "Data error (CRC)",
   "Bad drive request structure length",          /* 5 */
   "Seek error",
   "Unknown media type",
   "Sector not found",
   "Printer out of paper",
   "Write fault",                                 /* 10 */
   "Read fault",
   "General failure"
};


/*
 * from bit 0 of ah
 */
char *operation[] = {
   "Read",
   "Write"
};


/*
 * extended error, error class, register bh.
 */
char *error_class[] = {
   "Unknown",
   "Out of Resource:  space, channels, etc...",         /* 1 */
   "Temporary Situation:  expected to end",
   "Authorization: permission problem",
   "Internal: error in system software",
   "Hardware Failure:  not the fault of tde",           /* 5 */
   "System Failure:  system software",
   "Application Program Error",
   "Not Found:  file/item not found",
   "Bad Format:  invalid format, type",
   "Locked:  file/item locked",                         /* 10 */
   "Media:  wrong disk, bad spot, etc...",
   "Already Exists:  collision with existing",
   "Unknown"
};


/*
 * extended error, locus, register ch
 */
char *locus[] = {
   "Unknown",
   "Unknown",
   "Block Device (disk or disk emulator)",
   "Network",
   "Serial Device:  keyboard, printer, plotter, modem",
   "Memory:  random access memory"
};


/*
 * bit 7 of ah and device header when needed.
 */
char *device_type[] = {
   "Block",
   "Character"
};


/*
 * In int24.asm, check for errors greater than 88.  Set any extended error
 *   code above 88 to 0.
 */
char *ext_err[] = {
   "Frank has no idea what this error is",              /* 0 */
   "Function number invalid",
   "File not found",
   "Path not found",
   "Too many open files (no handles left)",
   "Access denied",
   "Handle invalid",
   "Memory control blocks destroyed",
   "Insufficient memory",
   "Memory block address invalid",
   "Environment invalid",                               /* 10 */
   "Format invalid",
   "Access code invalid",
   "Data invalid",
   "Unknown unit",
   "Invalid drive was specified",
   "Attempt to remove current directory",
   "Not same device",
   "No more files",
   "Attempt to write on write-protected disk",
   "Unknown unit",                                      /* 20 */
   "Drive not ready",
   "Unknown command",
   "Data error (CRC)",
   "Bad request structure length",
   "Seek error",
   "Unknown media type",
   "Sector not found",
   "Printer out of paper",
   "Write fault",
   "Read fault",                                        /* 30 */
   "General failure",
   "Sharing violation",
   "File-lock voilation",
   "Invalid disk change",
   "FCB unavailable",
   "Sharing buffer overflow",
   "37",
   "38",
   "39",
   "40",
   "41",
   "42",
   "43",
   "44",
   "45",
   "46",
   "47",
   "48",
   "49",
   "Unsupported network request",
   "Remote machine not listening",
   "Duplicate name on network",
   "Network name not found",
   "Network busy",
   "Device no longer exists on network",
   "NetBIOS command limit exceeded",
   "Network adapter hardware failure",
   "Incorrect response from network",
   "Unexpected network error",
   "Incompatible remote adapter",                       /* 60 */
   "Print queue full",
   "Not enough room for print file",
   "Print file was deleted",
   "Network name deleted",
   "Network access denied",
   "Incorrect network device type",
   "Network name not found",
   "Network name limit exceeded",
   "Net BIOS session limit exceeded",
   "Temporarily paused",                                /* 70 */
   "Network request not accepted",
   "Print or disk redirection is paused",
   "73",
   "74",
   "75",
   "76",
   "77",
   "78",
   "79",
   "File already exists",                               /* 80 */
   "Reserved",
   "Cannot make directory entry",
   "Fail on INT 24",
   "Too many redirections",
   "Duplicate redirection",
   "Invalid password",
   "Invalid parameter",
   "Network device fault"                               /* 88 */
};
