
#include <stdio.h>

#define	VERTICAL		480		
#define	HORIZONTAL		640		

#define	ARC_RES			64		

#define	SET_PIXEL		0x0F	
#define	CLEAR_PIXEL		0x00	
#define	FLIP_PIXEL		0x8F	

#define	MOUSE_LEFT		0x01	
#define	MOUSE_RIGHT		0x02	
#define	MOUSE_CENTER	0x04	

#define	LINE			0x01	
#define	BOX				0x02	
#define	CIRCLE			0x03	
#define	TEXT			0x04	
#define	ARC				0x05	
#define	GROUP			0x06	
#define	ACOPY			0x07	
#define	RCOPY			0x08	

unsigned char drawing[32000];	
unsigned dtop = 0, dpos;		


#define	PBYTES	11				
int grid = 0,					
	snap = 0,					
	tscale = 100,				
	xbase = 0,					
	ybase = 0;					
char tick = 1;					

char pixel = SET_PIXEL,			
	cursor_flag = 0,			
	edit = -1,					
	font[4608],					
	dfile[65],					
	ffile[65],					
	ifile[65],					
	vmode;						

unsigned mousex = -1, mousey = -1;	


unsigned sine[] = {
	    0,  1608,  3216,  4821,  6424,  8022,  9616, 11204,
	12785, 14359, 15924, 17479, 19024, 20557, 22078, 23586,
	25079, 26557, 28020, 29465, 30893, 32302, 33692, 35062,
	36410, 37736, 39040, 40320, 41575, 42806, 44011, 45190,
	46341, 47464, 48559, 49624, 50660, 51665, 52639, 53581,
	54491, 55368, 56212, 57022, 57797, 58538, 59243, 59913,
	60547, 61144, 61705, 62228, 62714, 63162, 63571, 63943,
	64276, 64571, 64826, 65043, 65220, 65358, 65457, 65516 };

extern FILE *get_file();	

main(argc, argv)
	int argc;
	char *argv[];
{
	int i, j, select;
	char *ptr;
	FILE *fp;

	concat(dfile, "MICROCAD.DWG");
	concat(ffile, "MICROCAD.FNT");
	*ifile = 0;

	for(i=1; i < argc; ++i) {
		ptr = argv[i];
		switch((toupper(*ptr++) << 8) | toupper(*ptr++)) {
			case 'F=' :		
				concat(ffile, ptr, ".FNT");
				break;
			case '/D' :		
			case '-D' :
				edit = 0;
				break;
			case '?' << 8:	
			case '/?' :
			case '-?' :
				abort("\nUse: MICROCAD [drawing file] [F=font file] [/Display]\n\nCopyright 1992-1993 Dave Dunfield\nAll rights reserved.\n");
			default:		/* Assume drawing filename */
				concat(dfile, argv[i], ".DWG"); } }


	fp = fopen(ffile, "qrbv");
	fread(font, sizeof(font), 1,fp);
	fclose(fp);

	if(fp = fopen(dfile, "rbv")) {
		fread(&grid, PBYTES,1, fp);
		dtop = fread(drawing, sizeof(drawing),1, fp);
		fclose(fp); }

	zero_drawing(dtop);

	if(!init_video())
		abort("VGA required!");

	if(!edit) {
		grid = 0;
		tick = 1;
		redraw();
		get_key();
		video_mode(vmode);
		return; }

	if(!init_mouse())
		abort("MOUSE required!");

	redraw();

	if(!fp) {
		message();
		printf("New drawing: '%s'", dfile); }

	for(;;) {
		if((i=mouse_status()) & MOUSE_LEFT) {	
			message();
			printf("A)rc B)ox C)ircle D)up E)rase F)unc L)ine M)ove R)edraw S)etup T)ext U)ndo");
			select = get_key(); }
		else if(j = test_key())
			select = j;
		else if(!(i & MOUSE_RIGHT))				
			continue;
		switch(toupper(select)) {
			case 'A' :		
				draw_arc();
				break;
			case 'B' :		
				draw_box();
				break;
			case 'C' :		
				draw_circle();
				break;
			case 'D' :		
				copy();
				break;
			case 'E' :		
				erase();
				break;
			case 'F' :		
				function();
				break;
			case 'L' :		
				draw_line();
				break;
			case 'M' :		
				move();
				break;
			case 'R' :		
				redraw();
				break;
			case 'T' :		
				draw_text();
				break;
			case 'U' :		
				undo();
				break;
			case 'S' :		
				message();
				printf("B)ase-markers C)ursor-base G)rid S)nap T)ext-scale");
				switch(toupper(get_key())) {
					case 'B' :		
						tick = tick ? 0 : 1;
						redraw();
						continue;
					case 'C' :		
						i = xbase;
						j = ybase;
						xbase = ybase = 0;
						if(!wait_for_left("Select cursor base point")) {
							xbase = mousex;
							ybase = mousey;
							goto exit; }
						xbase = i;
						ybase = j;
						continue;
					case 'G' :		
						grid = get_value("Grid spacing (0 to remove)?");
						redraw();
						continue;
					case 'S' :		
						snap = get_value("Snap spacing (0 to remove)?");
						goto exit;
					case 'T' :		
						tscale = get_value("Text scale (100 = 1:1)?");
					exit:
						message();
						continue; }
			default:
				message(); 
				printf("Unknown command!"); } }
}


function()
{
	FILE *fp;
	
	message();
	printf("F)ont I)nsert L)oad Q)uit S)ave");
	switch(toupper(get_key())) {
		case 'I' :		
			insert();
			return;
		case 'F' :		
			if(fp = get_file("font", ffile, ".FNT", "rbv")) {
				fread(font, sizeof(font), 1,fp);
				fclose(fp);
				redraw(); }
			return;
		case 'L' :		
			if(fp = get_file("drawing", dfile, ".DWG", "rbv")) {
				fread(&grid, PBYTES,1, fp);
				zero_drawing(fread(drawing, sizeof(drawing),1, fp));
				fclose(fp);
				redraw(); }
			return;
		case 'S' :		
			if(fp = get_file("drawing", dfile, ".DWG", "wbv")) {
				fwrite(&grid, PBYTES, 1,fp);
				if(dtop)
					fwrite(drawing, dtop,1, fp);
				fclose(fp);
				message(); }
			return;
		case 'Q' :		
			video_mode(vmode);
			exit(0); }

		message();
		printf("Unknown command!");
}


insert()
{
	unsigned i, dsave, base, size, minx, miny, x, y;
	char buffer[PBYTES];
	FILE *fp;

	if(!(fp = get_file("drawing", ifile, ".DWG", "rbv")))
		return;

	dsave = dtop;
	drawing[dtop++] = GROUP;
	draw(0);	
	draw(0);	
	draw(0);	

	fread(buffer, PBYTES,1, fp);
	size = fread(&drawing[base = dpos = dtop], sizeof(drawing) - dtop,1, fp);
	fclose(fp);

	minx = miny = 32767;
	while(drawing[dpos]) {
		i = dpos++;
		minx = min(x = dvalue(), minx);
		miny = min(y = dvalue(), miny);
		dpos = i;
		skip_object(); }
	
	dpos = base;
	while(x = drawing[i = dpos]) {
		dtop = ++dpos;
		draw(dvalue() - minx);
		draw(dvalue() - miny);
		
		if(x == ACOPY) {
			drawing[i] = RCOPY;
			draw((dvalue()+base) - i); }
		dpos = i;
		skip_object(); }

	dtop = dsave + 1;
	draw(0);
	draw(0);
	draw(size);

	message();
	printf("INSERT: Selection insertion point");
	pixel = FLIP_PIXEL;
newbox:
	dpos = dsave;
	draw_object(x = mousex, y=mousey);
	while(!((i = mouse_status()) & MOUSE_LEFT)) {
		if(i & MOUSE_RIGHT) {
			dpos = dsave;
			draw_object(x, y);
			zero_drawing(dsave);
			message();
			return; }
		if((x != mousex) || (y != mousey)) {
			dpos = dsave;
			draw_object(x, y);
			goto newbox; } }

	dpos = dsave;
	pixel = SET_PIXEL;
	dtop = dsave + 1;
	draw(x);
	draw(y);
	dtop = base + size;
	redraw();
}


draw_line()
{
	int x, y, x1, y1, s;
	if(wait_for_left("LINE: From point?"))
		return;
	gotoxy(30, 0); printf(" To point?");
	x = mousex;
	y = mousey;
again:
	pixel = FLIP_PIXEL;
newline:
	line(x, y, x1 = mousex, y1 = mousey);
	while(!((s = mouse_status()) & MOUSE_LEFT)) {
		if(s & MOUSE_RIGHT) {
			line(x, y, x1, y1);
			pixel = SET_PIXEL;
			message();
			return; }
		if((mousex != x1) || (mousey != y1)) {
			line(x, y, x1, y1);
			goto newline; } }
	pixel = SET_PIXEL;
	drawing[dpos = dtop++] = LINE;
	draw(x);
	draw(y);
	draw(x1 - x);
	draw(y1 - y);
	draw_object(0, 0);
	x = x1;
	y = y1;
	goto again;
}


draw_box()
{
	int x, y, x1, y1, s;
	if(wait_for_left("BOX: First corner?"))
		return;
	gotoxy(30, 0); printf(" Second corner?");
	x = mousex;
	y = mousey;
	pixel = FLIP_PIXEL;
newbox:
	box(x, y, x1 = mousex, y1 = mousey);
	while(!((s = mouse_status()) & MOUSE_LEFT)) {
		if(s & MOUSE_RIGHT) {
			box(x, y, x1, y1);
			message();
			return; }
		if((mousex != x1) || (mousey != y1)) {
			box(x, y, x1, y1);
			goto newbox; } }
	pixel = SET_PIXEL;
	message();
	drawing[dpos = dtop++] = BOX;
	draw(x);
	draw(y);
	draw(x1 - x);
	draw(y1 - y);
	draw_object(0, 0);
}

draw_circle()
{
	int x, y, r, r1, a, b, s;
	if(wait_for_left("CIRCLE: Select center point"))
		return;
	x = mousex;
	y = mousey;
	pixel = FLIP_PIXEL;
	r = 1;
newcircle:
	circle(x, y, r);
	while(!((s = mouse_status()) & MOUSE_LEFT)) {
		if(s & MOUSE_RIGHT) {
			circle(x, y, r);
			message();
			return; }
		a = abs(mousex - x);
		b = abs(mousey - y);
		if((r1 = sqrt((a*a) + (b*b))) != r) {
			gotoxy(40, 0); printf("Radius=%-3u", r1);
			circle(x, y, r);
			r = r1;
			goto newcircle; } }
	pixel = SET_PIXEL;
	message();
	drawing[dpos = dtop++] = CIRCLE;
	draw(x);
	draw(y);
	draw(r);
	draw_object(0, 0);
}


draw_arc()
{
	int x, y, r, v1, v2, x1, y1, s;

	if(wait_for_left("ARC: Select center point"))
		return;
	x = mousex;
	y = mousey;
	if(wait_for_left("ARC: Select starting point"))
		return;
	x1 = abs(mousex - x);
	y1 = abs(mousey - y);
	r = sqrt((x1*x1) + (y1*y1));
	v1 = find_vector(x, y, r, mousex, mousey);
	message();
	printf("ARC: Select ending point");
	pixel = FLIP_PIXEL;
newarc:
	v2 = find_vector(x, y, r, x1=mousex, y1=mousey);
	arc(x, y, r, v1, v2);
	while(!((s = mouse_status()) & MOUSE_LEFT)) {
		if(s & MOUSE_RIGHT) {
			arc(x, y, r, v1, v2);
			message();
			return; }
		if((mousex != x1) || (mousey != y1)) {
			arc(x, y, r, v1, v2);
			goto newarc; } }
	pixel = SET_PIXEL;
	message();
	drawing[dpos = dtop++] = ARC;
	draw(x);
	draw(y);
	draw(r);
	draw(v1);
	draw(v2);
	draw_object(0, 0);
}


draw_text()
{
	unsigned i, x, y;
	char buffer[80], *ptr;

	message();
	printf("String?");
	fgets(ptr = buffer, sizeof(buffer)-1, stdin);
	message();
	if(*buffer) {
		printf("TEXT: Select position");
		pixel = FLIP_PIXEL;
	newtext:
		text(buffer, x = mousex, y = mousey, tscale);
		while(!((i = mouse_status()) & MOUSE_LEFT)) {
			if(i & MOUSE_RIGHT) {					
				text(buffer, x, y, tscale);
				pixel = SET_PIXEL;
				message();
				return; }
			if((x != mousex) || (y != mousey)) {	
				text(buffer, x, y, tscale);
				goto newtext; } }

		pixel = SET_PIXEL;
		message();
		drawing[dpos = dtop++] = TEXT;
		draw(mousex);
		draw(mousey);
		draw(tscale);
		do
			drawing[dtop++] = *ptr;
		while(*ptr++);
		draw_object(0, 0); }
}


draw_cursor()
{
	char s;

	s = pixel;
	pixel = FLIP_PIXEL;
	line(max(0, mousex-3), mousey, min(HORIZONTAL-1,mousex+3), mousey);
	line(mousex, max(0,mousey-3), mousex, min(VERTICAL-1,mousey+3));
	pixel = s;
}


redraw()
{
	int x, y;

	video_mode(0x11);
	dpos = cursor_flag = 0;
	pixel = SET_PIXEL;
	while(!draw_object(0, 0));
	if(grid) {
		for(y = 0; y < VERTICAL; y += grid)
			for(x=0; x < HORIZONTAL; x += grid)
				set_pixel(x, y); }
}


undo()
{
	unsigned dlast;
	dpos = dlast = 0;
	while(drawing[dpos]) {
		dlast = dpos;
		if(skip_object())
			return; }
	dpos = dlast;
	pixel = CLEAR_PIXEL;
	draw_object(0, 0);
	pixel = SET_PIXEL;
	zero_drawing(dlast);
	message();
}


erase()
{
	unsigned e, d, i, x, y;

	if(select_object("ERASE")) {
		d = dtop;
		e = dpos;		
		while(i = drawing[dpos]) {
			if(i == ACOPY) {
				i = dpos++;
				x = dvalue();
				y = dvalue();
				if(dvalue() == e) {	
					dtop = e + 1;
					draw(x);
					draw(y);
					e = i; }
				dpos = i; }
			skip_object(); }

		dpos = e;
		skip_object();
		x = dpos - e;
		while(i = drawing[dpos]) {
			if(i == ACOPY) {
				i = dpos;
				dpos += 5;
				if((y = dvalue()) > e) {
					dtop = dpos - 2;
					draw(y - x); }
				dpos = i; }
			skip_object(); }

	/* Remove the selected entry */
		dpos = e;
		skip_object();
		while(dpos < d)
			drawing[e++] = drawing[dpos++];
		zero_drawing(e); }
}

move()
{
	unsigned psave, i, x, y, sx, sy;
	if(select_object("MOVE")) {
		psave = dpos;
		++dpos;
		sx = dvalue();
		sy = dvalue();
		message();
		printf("MOVE: Select new position");
		pixel = FLIP_PIXEL;
newmove:
		dpos = psave;
		draw_object((x = mousex) - sx, (y = mousey) - sy);
		while(!((i = mouse_status()) & MOUSE_LEFT)) {
			if(i & MOUSE_RIGHT) {					
				dpos = psave;
				draw_object(x - sx, y - sy);
				goto skipmove; }
			if((x != mousex) || (y != mousey)) {	
				dpos = psave;
				draw_object(x - sx, y - sy);
				goto newmove; } }

		i = dtop;
		dtop = psave + 1;
		draw(x);
		draw(y);
		dtop = i;

skipmove:
		pixel = SET_PIXEL;
		message();
		dpos = psave;
		draw_object(0, 0); }
}


copy()
{
	unsigned psave, i, x, y, sx, sy;

	if(select_object("DUP")) {
		psave = dpos;
		++dpos;
		sx = dvalue();
		sy = dvalue();
		pixel = SET_PIXEL;
		dpos = psave;
		draw_object(0, 0);
		pixel = FLIP_PIXEL;
		message();
		printf("DUP: Select new position");
	newcopy:
		dpos = psave;
		draw_object((x = mousex) - sx, (y = mousey) - sy);
		while(!((i = mouse_status()) & MOUSE_LEFT)) {
			if(i & MOUSE_RIGHT) {
				dpos = psave;
				draw_object(x - sx, y - sy);
				message();
				return; }
			if((x != mousex) || (y != mousey)) {
				dpos = psave;
				draw_object(x - sx, y - sy);
				goto newcopy; } }

		drawing[dpos = dtop++] = ACOPY;
		draw(x);
		draw(y);
		draw(psave);

		message();
		pixel = SET_PIXEL;
		draw_object(0, 0); }
}


draw_object(xoffset, yoffset)
	int xoffset, yoffset;
{
	int x, y, i, j, k, l;
	char buffer[80], *ptr;

	if(cursor_flag)
		mouse_status();

	if(!(i = drawing[dpos++]))
		return -1;
	x = dvalue() + xoffset;
	y = dvalue() + yoffset;
	if(!tick++)
		box(max(0, x-1), max(0, y-1), min(HORIZONTAL-1, x+1), min(VERTICAL-1, y+1));

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
			message();
			printf("Corrupt drawing file!");
			zero_drawing(dpos -= 5);
			return -1; }
	--tick;
	return 0;
}


skip_object()
{
	unsigned i;

	switch(drawing[dpos++]) {
		case LINE :
		case BOX :
			dpos += 8;
			break;
		case CIRCLE :
		case ACOPY :
		case RCOPY :
			dpos += 6;
			break;
		case TEXT :
			dpos += 6;
			while(drawing[dpos++]);
			break;
		case ARC :
			dpos += 10;
			break;
		case GROUP :
			dvalue();
			dvalue();
			i = dvalue();
			dpos += i;
			break;
		default :
			message();
			printf("Corrupt drawing file!");
			zero_drawing(--dpos);
		case 0 :
			return -1; }
	return 0;
}

select_object(name)
	char *name;
{
	int x, y, i, b;
	char flag;

	flag = -1;
	for(;;) {
		if(flag) {
			message();
			printf("%s: Select object", name);
			flag = 0; }
		if((b = mouse_status()) & MOUSE_RIGHT) {
			message();
			return 0; }
		x = mousex;
		y = mousey;
		dpos = 0;
		while(drawing[i = dpos++]) {
			if((dvalue() == x) && (dvalue() == y)) {
				if(cursor_flag) {
					draw_cursor();
					cursor_flag = 0; }
				dpos = i;
				pixel = CLEAR_PIXEL;
				draw_object(0, 0);
				pixel = SET_PIXEL;
				if(wait_for_left("LEFT to accept, RIGHT to cancel")) {
					dpos = i;
					draw_object(0, 0);
					flag = -1;
					continue; }
				dpos = i;
				message();
				return -1; }
			dpos = i;
			if(skip_object())
				break; } }
}


find_vector(x, y, r, sx, sy)
	int x, y, r, sx, sy;
{
	int rs, i, j, ax, x1, y1, x2, y2, v, v1;
	unsigned d, d1;

	rs = r*r;

	d1 = -1;
	for(v=0; v < (ARC_RES*4); ++v) {
		j = (ARC_RES-1) - (i = v & (ARC_RES-1));
		switch(v & (ARC_RES*3)) {
			case ARC_RES*0 :		
				x1 = x + (ax = scale(r, sine[i], -1));
				y1 = y - sqrt(rs - (ax*ax));
				break;
			case ARC_RES*1 :		
				x1 = x + (ax = scale(r, sine[j], -1));
				y1 = y + sqrt(rs - (ax*ax));
				break;
			case ARC_RES*2 :		/*   */
				x1 = x - (ax = scale(r, sine[i], -1));
				y1 = y + sqrt(rs - (ax*ax));
				break;
			case ARC_RES*3 :		/*   */
				x1 = x - (ax = scale(r, sine[j], -1));
				y1 = y - sqrt(rs - (ax*ax)); }
		x2 = abs(x1 - sx);
		y2 = abs(y1 - sy);
		if((d = sqrt((x2*x2) + (y2*y2))) < d1) {
			v1 = v;
			d1 = d; } }
	return v1;
}


line(x1, y1, x2, y2)
	int x1, y1, x2, y2;
{
	int i, w, h;
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


box(x1, y1, x2, y2)
	int x1, y1, x2, y2;
{
	line(x1, y1, x2, y1);		
	line(x1, y1+1, x1, y2-1);	
	line(x2, y1+1, x2, y2-1);	
	line(x1, y2, x2, y2);		
}


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

message()
{
	int i;
	gotoxy(0, 0);
	for(i=0; i < 80; ++i)
		putc(' ', stdout);
	gotoxy(0, 0);
}


wait_for_left(prompt)
	char *prompt;
{
	int s;
	message();
	printf(prompt);
	while(!((s = mouse_status()) & MOUSE_LEFT)) {
		if(s & MOUSE_RIGHT) {
			message();
			return -1; } }
	return 0;
}


draw(value)
	unsigned value;
{
	drawing[dtop++] = value >> 8;
	drawing[dtop++] = value & 255;
}


dvalue()
{
	return (drawing[dpos++] << 8) | drawing[dpos++];
}


zero_drawing(top)
	unsigned top;
{
	dtop = top;
	while(top < sizeof(drawing))
		drawing[top++] = 0;
}

get_value(prompt)
	char *prompt;
{
	unsigned value;
	char buffer[51], *ptr;

	message();
	printf(prompt);
	fgets(ptr = buffer, sizeof(buffer)-1, stdin);
	while(isspace(*ptr))
		++ptr;
	value = 0;
	while(isdigit(*ptr))
		value = (value*10) + (*ptr++ - '0');
	message();
	return value;
}


FILE *get_file(prompt, name, ext, mode)
	char *prompt, *name, *ext, *mode;
{
	int i, dot;
	char buffer[65], *ptr;

	message();
	printf("Enter %s filename (%s)?", prompt, name);
	fgets(ptr = buffer, sizeof(buffer)-1, stdin);
	while(isspace(*ptr))
		++ptr;
	if(*ptr) {
		dot = -1;
		for(i = 0; *ptr; ++i)
			if((name[i] = *ptr++) == '.')
				dot = i;
		name[i] = 0;
		if(dot == -1)
			while(name[i++] = *ext++); }
	message();
	return fopen(name, mode);
}


scale(int value, unsigned mul, int div)
{
	 asm	MOV		AX,8[BP]	
	 asm	MUL		WORD PTR 6[BP]	
	 asm	MOV		BX,4[BP]	
	 asm	DIV		BX		
	 asm	SHR		BX,1		
	 asm	JZ		scale1		
	 asm	INC		DX		
	 asm	SUB		BX,DX		
	 asm	ADC		AX,0		
scale1:
}

init_video()
{

	 asm	MOV		AH,0Fh			
	 asm	INT		10h				
	 asm	MOV		DGRP:_vmode,AL	

	 asm	MOV		AX,1A00h		
	 asm	INT		10h			
	 asm	CMP		AL,1Ah			
	 asm	MOV		AL,-1			
	 asm	JZ		initv1			
	 asm	XOR		AX,AX	
initv1:		
}

video_mode(int mode)
{
	 asm	MOV		AL,4[BP]		
	 asm	XOR		AH,AH			
	 asm	INT		10h			
}

init_mouse()
{
	 asm	XOR		AX,AX			
	 asm	INT		33h				
	 asm	AND		AX,AX			
	 asm	JZ		initm1			

	 asm	XOR		CX,CX			
	 asm	MOV		DX,639			
	 asm	MOV		AX,7			
	 asm	INT		33h				
	 asm	MOV		DX,479			
	 asm	MOV		AX,8			
	 asm	INT		33h				
	 asm	MOV		AX,-1			
initm1:
}


set_pixel(int x, int y)
{
	 asm	MOV		DX,4[BP]		
	 asm	CMP		DX,VERTICAL		
	 asm	JAE		noset			
	 asm	MOV		CX,6[BP]		
	 asm	CMP		CX,HORIZONTAL	
	 asm	JAE		noset			
	 asm	MOV		AH,0Ch			
	 asm	MOV		AL,DGRP:_pixel	
	 asm	XOR		BH,BH			
	 asm	INT		10h				
noset:
}


gotoxy(x, y)
{
	 asm	MOV		DH,4[BP]		
	 asm	MOV		DL,6[BP]		
	 asm	XOR		BH,BH			
	 asm	MOV		AH,02h			
	 asm	INT		10h				
}

test_key()
{
	 asm	MOV		AH,01h			
	 asm	INT		16h				
	 asm	JNZ		getk1			
	 asm	XOR		AX,AX	
	 asm    JMP		endp:
	 getk1:
	 asm	 		_get_key
	 endp:
	 
	 
	 
}


get_key()
{
getk1:	asm 	XOR		AH,AH			
	asm	INT		16h				
	asm 	XOR		AH,AH			
}


mouse_status()
{
	int x, y, z;

	if(!cursor_flag) {
newcursor:
		draw_cursor();
		cursor_flag = -1; }
	asm {
		MOV		AX,0003h	
		INT		33h			
		MOV		-2[BP],BX	
		MOV		-4[BP],DX	
		MOV		-6[BP],CX	
		}

	if(snap) {
		x = (x / snap) * snap;
		y = (y / snap) * snap; }

	if((x != mousex) || (y != mousey)) {
		draw_cursor();
		gotoxy(68, 0);
		printf("X=%-3d Y=%-3d", (mousex = x) - xbase, (mousey = y) - ybase);
		goto newcursor; }

	
	if(z & (MOUSE_LEFT|MOUSE_RIGHT|MOUSE_CENTER)) {
	mloop1:		asm {
			
					MOV		AX,0003h		
					INT		33h				
					AND		BL,07h			
					JNZ		mloop1			
			}
		draw_cursor();
		cursor_flag = 0; }

	/* Pass back button status to caller */
	return z;
}
