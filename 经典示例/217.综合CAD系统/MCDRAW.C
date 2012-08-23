/*
 * MICROCAD Drawing utility
 *
 * This is a "boiled down" set of routines to draw a MICROCAD drawing file.
 * The main purpose of this program is to provide a starting point, should
 * you wish to incorporate code to display a MICROCAD drawing file in one
 * of your own programs. Most of this file should be useable without much
 * modification.
 *
 * Copyright 1992-1993 Dave Dunfield
 * All rights reserved.
 *
 * Permission granted for personal (non-commercial) use only.
 *
 * Compile with: cc mcdraw -fop
 */
#include <stdio.h>

/* Misc constants */
#define	VERTICAL		480		/* Screen height */
#define	HORIZONTAL		640		/* Screen width */
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

/* Global storage */
char drawing[32000], font[4608], vmode;
unsigned dpos;

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
 * Main program - read files and draw image
 */
main(argc, argv)
	int argc;
	char *argv[];
{
	unsigned i;
	FILE *fp;

	if(argc < 2)
		abort("\nUse: MCDRAW <drawing file> [font file]\n\nCopyright 1992-1993 Dave Dunfield\nAll rights reserved.\n");

	/* Read in the font file */
	concat(drawing, (argc > 2) ? argv[2] : "MICROCAD", ".FNT");
	fp = fopen(drawing, "rbvq");
	fread(font, sizeof(font), 1,fp);
	fclose(fp);

	/* Read in drawing file */
	concat(drawing, argv[1], ".DWG");
	fp = fopen(drawing, "rbvq");
	fread(drawing, PBYTES,1, fp);
	i = fread(drawing, sizeof(drawing),1, fp);
	fclose(fp);

	/* Zero unused portion of drawing file */
	while(i < sizeof(drawing))
		drawing[i++] = 0;

	/* Setup PC video screen */
	if(!init_video())
		abort("VGA required!");
	video_mode(0x11);

	/* Draw all the objects */
	dpos = 0;
	while(!draw_object(0, 0));

	/* Wait for carriage-return and exit */
	fgets(drawing, 100, stdin);
	video_mode(vmode);
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
 * Scale a value by a fraction, using a 32 bit intermediate result,
 * and round up/down to nearest integer ressult.
 */
scale(value, mul, div)
{
 asm		MOV		AX,8[BP]		; Get value
 asm		MUL		WORD PTR 6[BP]	; Multiply to 32 bit product
 asm		MOV		BX,4[BP]		; Get divisor
 asm		DIV		BX				; Divide back to 16 bit result
 asm		SHR		BX,1			; /2 for test
 asm		JZ		scale1			; Special case (/1)
 asm		INC		DX				; .5 rounds up
 asm		SUB		BX,DX			; Set 'C' if remainder > half
 asm		ADC		AX,0			; Increment result to scale
scale1:
}

/*
 * Initialize the video display
 */
init_video()
{
; First, determine current mode for later
	 asm	MOV		AH,0Fh			; Get video mode
	 asm	INT		10h				; Call BIOS
	 asm	MOV		DGRP:_vmode,AL	; Save video mode
; Check for VGA present
	 asm	MOV		AX,1A00h		; Get display code
	 asm	INT		10h				; Call BIOS
	 asm	CMP		AL,1Ah			; VGA supported?
	 asm	MOV		AL,-1			; Assume yes
	 asm	JZ		initv1			; It does exist
	 asm	XOR		AX,AX			; Report failure
initv1:
}

/*
 * Set active video mode & clear screen
 */
video_mode(mode)
{
	 asm	MOV		AL,4[BP]		; Get mode
	 asm	XOR		AH,AH			; Func 0 - set mode
	 asm	INT		10h				; Issue mode
}

/*
 * Set a graphic pixel.
 */
set_pixel(x, y) 
{
	asm	MOV		DX,4[BP]		; Get Y coordinate
	asm	CMP		DX,VERTICAL		; In range?
	asm	JAE		noset			; No, skip it
	asm	MOV		CX,6[BP]		; Get X coordinate
	asm	CMP		CX,HORIZONTAL	; In range?
	asm	JAE		noset			; No, skip it
	asm	MOV		AH,0Ch			; Write pixel function
	asm	MOV		AL,0fh			; Pixel SET attribute
	asm	XOR		BH,BH			; Zero page
	asm	INT		10h				; Call int 16
noset:
}
