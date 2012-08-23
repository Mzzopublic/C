#include <stdio.h>
int jsvalue(long n)
{
	int I,strl,half;
	char xy[20];
	ltoa(n,xy,10);
	strl=strlen(xy);
	half=strl/2;
	for(I=0;I<half;I++)
		if(xy[I]!=xy[--strl])
			break;
	if(I>=half) return 1;
	else return 0;
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
main()
{
	long m;
	FILE *out;
	clrscr();
	puts(" This program is to find the Palindrome Numbers.");
	puts(" whose square and cubic are also Palindrome Numbers.");
	puts(" >> These numbers less than 1000 are:");
	out=fopen("out181.dat","w");
	for(m=11;m<1000;m++)
	{
		if(jsvalue(m)&&jsvalue(m*m)&&jsvalue(m*m*m))
		{
			printf(" m=%4ld,m*m=%6ld,m*m*m=%8ld \n",m,m*m,m*m*m);
			fprintf(out,"m=%4ld,m*m=%6ld,m*m*m=%8ld \n",m,m*m,m*m*m);
		}
	}
	fclose(out);
	PressKeyToQuit();
}
