#include <stdio.h>
#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#define INTR 0x1c

void interrupt ( *oldhandler) ();
int fre[]={523,659,587,659,587,494,587,523,440,440,440,330,392,440,494,494,330,415,494,523,523,523};

void interrupt handler(){
	unsigned i;
	static int flag;
	static int k=0;
	union{
		long divisor;
		unsigned char c[2];
	}music;
	unsigned char port;
	flag++;
	if (flag>=4){
		flag=0;
		music.divisor=1190000/fre[k];
		outportb(67,182);
		outportb(66,music.c[0]);
		outportb(66,music.c[1]);
		port=inportb(97);
		outport(97,port|3);
		k++;
		if(k>=22) k=1;
	}
	oldhandler();
}

int main(void)
{
	int gdriver=9,gmode=2,errorcode;
	int midx,midy,i;
	oldhandler=getvect(INTR);
	setvect(INTR,handler);
	/*registerbgidriver( EGAVGA_driver );*/
	initgraph(&gdriver,&gmode,"c:\\tc");
	errorcode=graphresult();
	if(errorcode!=grOk)
	{
		printf("graphic error:%s\n",grapherrormsg(errorcode));
		printf("press any key to halt");
		getch();
		exit(1);
	}
	midx=getmaxx()/2;
	midy=getmaxy()/2;
	for(i=SOLID_FILL;i<USER_FILL;i++)
	{
		setfillstyle(i,getmaxcolor());
		bar(midx-50,midy-50,midx+50,midy+50);
		delay(2000);
		getch();
	}
	getch();
	closegraph();
	setvect(INTR,oldhandler);
	nosound();
	return(0);
}
