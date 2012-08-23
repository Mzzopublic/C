#include <conio.h>
#include <math.h>
#include <stdio.h>
float countvalue()
{
	float x0,x1=0.0;
	while(1)
	{
		x0=x1;
		x1=cos(x0);
		if(fabs(x0-x1)<1e-6) 
			break;
	}
	return x1;
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
main()
{
	clrscr();
	puts(" This program is to find the real root of");
	puts("          function cos(x)-x=0.");
	printf(" >> The real root is %f.\n",countvalue());
	printf(" >> The result of cos(%f)-%f is %f.\n",countvalue(),countvalue(),cos(countvalue())-countvalue());
	writeDat();
	PressKeyToQuit();
}
writeDat()
{
	FILE *wf;
	wf=fopen("OUT179.DAT","w");
	fprintf(wf,"%f\n",countvalue());
	fclose(wf);
}
