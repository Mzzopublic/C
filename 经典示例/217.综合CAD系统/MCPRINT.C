/*
 * MICROCAD Print Utility
 *
 * Copyright 1992-1993 Dave Dunfield
 * All rights reserved.
 *
 * Permission granted for personal (non-commercial) use only.
 *
 * Compile command: cc mcprint -fop
 */
#include <stdio.h>

/* Screen dimensions */
#define	VERTICAL		480		/* Maximum screen height */
#define	HORIZONTAL		640		/* Maximum screen width */

#define	BUFSIZE			38400	/* Size of video buffer */

#define	ARC_RES			64		/* Resolution of each ARC quadrant */

#define	PBYTES			11		/* # of prefix bytes */

/* Drawing buffer codes */
#define	LINE			0x01	/* Object is a LINE */
#define	BOX				0x02	/* Object is a BOX */
#define	CIRCLE			0x03	/* Object is a CIRCLE */
#define	TEXT			0x04	/* Object is a TEXT string */
#define	ARC				0x05	/* Object is an ARC */
#define	GROUP			0x06	/* Object is a GROUP of objects */
#define	ACOPY			0x07	/* Copy an object (absolute) */
#define	RCOPY			0x08	/* Copy an object (relative) */

/* Drawing storage variables */
unsigned char drawing[32000];
unsigned dpos;

/* Misc. variables */
char font[4608], dfile[65], ffile[65], *device = "LPT1";
int vbseg, printer = -1;

/*
 * Printer information: The variable 'printers' contains the names of
 * All supported printer types. To add a new printer, add its name to
 * this list, add its 'xyorder' flag to the 'xyorders' variable below,
 * and add code to the 'switch' statement at the end of 'main()' to
 * output the drawing.
 */
char *printers[] = {
	"LASERJET",
	"EPSON" }

/*
 * The 'xyorder' flag controls how the bitmap image is generated.
 *
 * If xyorder is non-zero, the bitmap will be organized such that each
 * byte contains bits from the same COLUMN. This is suitable for direct
 * output to a "raster" type printer such as the LaserJet.
 *
 * If xyorder is zero, the bitmap will be organized such that each
 * byte contains bits from the same ROW. This is suitable for
 * direct output to a "dot matrix" type printer such as the EPSON.
 *
 * NOTE: The terms COLUMN and ROW are used with reference to the drawing,
 *       which is printed SIDEWAYS. Therefore, the terms are reversed with
 *       respect to the physical page layout of the printer.
 */
char xyorder, xyorders[] = {
	-1,		/* Laserjet uses COLUMN alignment */
	0 }		/* EPSON uses ROW alignment */

/* 
 * Sine table for drawing ARC's
 */
unsigned sine[] = {
	    0,  1608,  3216,  4821,  6424,  8022,  9616, 11204,
	12785, 14359, 15924, 17479, 19024, 20557, 22078, 23586,
	25079, 26557, 28020, 29465, 30893, 32302, 33692, 35062,
	36410, 37736, 39040, 40320, 41575, 42806, 44011, 45190,
	46341, 47464, 48559, 49624, 50660, 51665, 52639, 53581,
	54491, 55368, 56212, 57022, 57797, 58538, 59243, 59913,
	60547, 61144, 61705, 62228, 62714, 63162, 63571, 63943,
	64276, 64571, 64826, 65043, 65220, 65358, 65457, 65516 };

/*
 * Main program - process commands at the highest level
 */
main(argc, argv)
	int argc;
	char *argv[];
{
	unsigned i, j;
	char *ptr;
	FILE *fp, *pfp;

	/* Set default file names */
	*dfile = 0;
	concat(ffile, "MICROCAD.FNT");

	/* Parse command line arguments */
	for(i=1; i < argc; ++i) {
		ptr = argv[i];
		switch((toupper(*ptr++) << 8) | toupper(*ptr++)) {
			case 'F=' :		/* Specify alternate FONT file */
				concat(ffile, ptr, ".FNT");
				break;
			case 'D=' :		/* Specify output device */
				device = ptr;
				break;
			case '?' << 8:	/* Command line help */
			case '/?' :
			case '-?' :
				goto help;
			default:		/* Assume drawing filename or printer*/
				if(!*dfile) {	/* Need filename? */
					concat(dfile, argv[i], ".DWG");
					break; }
				for(ptr = argv[i]; *ptr; ++ptr)	/* Insure name uppercase */
					*ptr = toupper(*ptr);
				for(j=0; j < (sizeof(printers)/2); ++j) {	/* Lookup name */
					if(strbeg(printers[j], argv[i])) {
						if(printer != -1) {
							printf("Ambiguous printer name!\n");
							goto help; }
						printer = j; } }
				if(printer == -1) {
					printf("Unknown printer name!\n");
					goto help; } } }

	/* If insufficent parameters, or help request... output info */
	if((printer == -1) || !*dfile) {
		printf("Drawing name and printer type required!\n");
	help:
		printf("\nUse: MCPRINT <drawing file> <printer type> [F=font file] [D=device]\n");
		printf("\nCopyright 1992-1993 Dave Dunfield\nAll rights reserved.\n\n");
		printf("Available printers are:\n");
		for(j=0; j < (sizeof(printers)/2); ++j)
			printf("   %s\n", printers[j]);
		exit(-1); }

	/* Read in "font" file */
	fp = fopen(ffile, "rbvq");
	fread(font, sizeof(font), fp);
	fclose(fp);

	/* Read in the drawing file */
	fp = fopen(dfile, "rbvq");
	fread(drawing, PBYTES, fp);
	i = fread(drawing, sizeof(drawing), fp);
	fclose(fp);

	/* Zero unused portion of drawing file */
	while(i < sizeof(drawing))
		drawing[i++] = 0;

	/* Open printer device, and set for binary output */
	set_raw(pfp = fopen(device, "wbvq"));

	/* Allocate buffer for printer */
	printf("Initalize... ");
	if(!(vbseg = alloc_seg((BUFSIZE+15)/16)))
		abort("Not enough memory!");

	/* Zero the initial bitmap in the buffer */
	for(i=0; i < BUFSIZE; ++i)
		poke(vbseg, i, 0);

	/* Draw all the objects */
	printf("Draw objects... ");
	xyorder = xyorders[printer];
	dpos = 0;
	while(!draw_object(0, 0));

	printf("Print... ");

	/* Add code to this switch statement to support additional printers */
	switch(printer) {
		case 0 :		/* Laserjet raster format */
			fprintf(pfp,"\x1B*t75R");			/* Set 75 Dots per inch */
			fprintf(pfp,"\x1B*r0A");			/* Enable raster graphics */
			for(dpos=i=0; i < HORIZONTAL; ++i) {
				fprintf(pfp,"\x1B*b60W");		/* Transfer graphics line */
				for(j=0; j < (VERTICAL/8); ++j)
					putc(peek(vbseg, dpos++), pfp); }
			fprintf(pfp,"\x1B*rB\f");			/* End raster graphics */
			break;
		case 1 :		/* EPSON dot-matrix format */
			for(i=0; i < (HORIZONTAL/8); ++i) {
				fprintf(pfp,"\x1BK\xE0\x01");	/* Graphics: 480 dots */
				for(j=0; j < BUFSIZE; j += (HORIZONTAL/8))
					putc(peek(vbseg, i + j), pfp);
				fprintf(pfp,"\r\x1BJ\x18"); }	/* Advance 8 lines */
		}
	
	/* Close the output file & we are all done */
	fclose(pfp);
	printf("Done.");
}

/*
 * Draws an object from the drawing file
 */
draw_object(xoffset, yoffset)
	int xoffset, yoffset;
{
	int x, y, i, j, k, l;
	char buffer[80], *ptr;

	i = drawing[dpos++];
	x = dvalue() + xoffset;
	y = dvalue() + yoffset;

	switch(i) {
		case LINE :
			line(x, y, x+dvalue(), y+dvalue());
			break;
		case BOX :
			box(x, y, x+dvalue(), y+dvalue());
			break;
		case CIRCLE :
			circle(x, y, dvalue());
			break;
		case TEXT :
			i = dvalue();
			ptr = buffer;
			do
				*ptr++ = j = drawing[dpos++];
			while(j);
			text(buffer, x, y, i);
			break;
		case ARC:
			arc(x, y, dvalue(), dvalue(), dvalue());
			break;
		case GROUP :
			i = dvalue();
			j = dpos;
			while((dpos - j) < i)
				draw_object(x, y);
			break;
		case RCOPY :
			i = dpos - 5;
			i += dvalue();
			goto gocopy;
		case ACOPY :
			i = dvalue();
		gocopy:
			j = dpos;
			dpos = i+1;
			k = dvalue();
			l = dvalue();
			dpos = i;
			draw_object(x - k, y - l);
			dpos = j;
			break;
		default:
			abort("Corrupt drawing file!");
		case 0 :
			return -1; }
	return 0;
}

/*
 * Draw a line from point (x1, y1) to (x2, y2)
 */
line(x1, y1, x2, y2)
	int x1, y1, x2, y2;
{
	int i, w, h;
	/* If 'X' is greater, increment through 'X' coordinate */
	if((w = abs(x1 - x2)) >= (h = abs(y1 - y2))) {
		if(x1 > x2) {
			i = x1;
			x1 = x2;
			x2 = i;
			i = y1;
			y1 = y2;
			y2 = i; }
		if(y1 < y2) {
			for(i=0; i < w; ++i)
				set_pixel(x1+i, y1+scale(i, h, w)); }
		else {
			for(i=0; i < w; ++i)
				set_pixel(x1+i, y1-scale(i, h, w)); } }
	/* If 'Y' is greater, increment through 'Y' coordinate */
	else {
		if(y1 > y2) {
			i = x1;
			x1 = x2;
			x2 = i;
			i = y1;
			y1 = y2;
			y2 = i; }
		if(x1 < x2) {
			for(i=0; i < h; ++i)
				set_pixel(x1+scale(i, w, h), y1+i); }
		else {
			for(i=0; i < h; ++i)
				set_pixel(x1-scale(i, w, h), y1+i); } }

	set_pixel(x2, y2);
}

/*
 * Draw a box with opposite corners (x1, y1) to (x2, y2)
 */
box(x1, y1, x2, y2)
	int x1, y1, x2, y2;
{
	line(x1, y1, x2, y1);		/* Top */
	line(x1, y1+1, x1, y2-1);	/* Left side */
	line(x2, y1+1, x2, y2-1);	/* Right side */
	line(x1, y2, x2, y2);		/* Bottom */
}

/*
 * Draw a circle about point (x, y) of radius (r)
 */
circle(x, y, r)
	int x, y, r;
{
	int i, j, k, rs, lj;

	rs = (lj = r)*r;
	for(i=0; i <= r; ++i) {
		j = k = sqrt(rs - (i*i));
		do {
			set_pixel(x+i, y+j);
			set_pixel(x+i, y-j);
			set_pixel(x-i, y+j);
			set_pixel(x-i, y-j); }
		while(++j < lj);
		lj = k; }
}

/*
 * Draw an arc centered at (x, y), radius r at vectors v1, v2
 */
arc(x, y, r, v1, v2)
	int x, y, r;
	unsigned char v1, v2;
{
	int rs, i, j, ax, x1, y1, x2, y2;

	x2 = -1;
	rs = r*r;

	do {
		j = (ARC_RES-1) - (i = v1 & (ARC_RES-1));
		switch(v1 & (ARC_RES*3)) {
			case ARC_RES*0 :	/* Quadrant one */
				x1 = x + (ax = scale(r, sine[i], -1));
				y1 = y - sqrt(rs - (ax*ax));
				break;
			case ARC_RES*1 :	/* Quadrant two */
				x1 = x + (ax = scale(r, sine[j], -1));
				y1 = y + sqrt(rs - (ax*ax));
				break;
			case ARC_RES*2 :	/* Quadrant three */
				x1 = x - (ax = scale(r, sine[i], -1));
				y1 = y + sqrt(rs - (ax*ax));
				break;
			case ARC_RES*3 :	/* Quadrant four */
				x1 = x - (ax = scale(r, sine[j], -1));
				y1 = y - sqrt(rs - (ax*ax)); }
		if(x2 != -1)
			line(x2, y2, x1, y1);
		x2 = x1;
		y2 = y1; }
	while(v1++ != v2);
}

/*
 * Draw a text string on the screen at specified scale
 */
text(string, x, y, s)
	char *string;
	int x, y, s;
{
	unsigned i, j, b;
	unsigned char *ptr;

	y -= scale(24, s, 100);
	while(*string) {
		ptr = &font[(*string++ - ' ') * 48];

		for(i=0; i < 24; ++i) {
			b = (*ptr++ << 8) | *ptr++;
			for(j=0; j < 16; ++j) {
				if(b & 0x8000)
					set_pixel(x+scale(j,s,100), y+scale(i,s,100));
				b <<= 1; } }
		x += scale(18, s, 100); }
}

/*
 * Retrieve a 16 bit value from the drawing list
 */
dvalue()
{
	return (drawing[dpos++] << 8) | drawing[dpos++];
}

/*
 * Set a graphic pixel in the output buffer.
 */
set_pixel(x, y)
	unsigned x, y;
{
	unsigned byte;

	if((x < HORIZONTAL) && (y < VERTICAL)) {
		y = (VERTICAL-1) - y;	/* Flip because printer outputs from bottom */

		if(xyorder) {	/* COLUMN alignment */
			byte = (x * (VERTICAL/8)) + (y / 8);
			poke(vbseg, byte, peek(vbseg, byte) | (0x80 >> (y % 8))); }
		else {			/* ROM alignment */
			byte = (y * (HORIZONTAL/8)) + (x / 8);
			poke(vbseg, byte, peek(vbseg, byte) | (0x80 >> (x % 8))); } }
}

/*
 * Scale a value by a fraction, using a 32 bit intermediate result,
 * and round up/down to nearest integer ressult.
 */
scale(value, mul, div) asm
{
		MOV		AX,8[BP]		; Get value
		MUL		WORD PTR 6[BP]	; Multiply to 32 bit product
		MOV		BX,4[BP]		; Get divisor
		DIV		BX				; Divide back to 16 bit result
		SHR		BX,1			; /2 for test
		JZ		scale1			; Special case (/1)
		INC		DX				; .5 rounds up
		SUB		BX,DX			; Set 'C' if remainder > half
		ADC		AX,0			; Increment result to scale
scale1:
}

/*
 * Set a device into "raw" mode for pure binary output
 */
set_raw(fp) asm
{
		MOV		BX,4[BP]		; Get file pointer
		MOV		AX,4400h		; IOCTL:Get info
		MOV		BX,1[BX]		; Get DOS file handle
		INT		21h				; Ask DOS
		TEST	DL,80h			; Device?
		JZ		setr1			; Not a device...
		XOR		DH,DH			; Force DH=0
		OR		DL,20h			; Set RAM mode
		MOV		AX,4401h		; IOCTL:Set info
		INT		21h				; Ask DOS
setr1:
}
