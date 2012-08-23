#include <conio.h>
#include <stdio.h>
void countvalue(int *a,int *n)
{
	int I;
	*n=0;
	for(I=1;I<=1000;I++)
		if(I%7==0&&I%11)
		{
			*a=I;
			*n=*n+1;
			a++;
		}
		else if(I%7&&I%11==0)
		{
			*a=I;
			*n=*n+1;
			a++;
		}
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
main()
{ 
	int aa[1000],n,k;
	clrscr();
	puts(" This program is to find numbers\n which can be divided exactly by 7 or 11,\n but can not be divided exactly both by 7 and 11.");
	puts(" These numbers less than 1000 are:");
	countvalue(aa,&n);
	for(k=0;k<n;k++)
		if((k+1)%10==0) 
			printf("\n");
		else 
			printf("%5d",aa[k]);
	writeDAT();
	PressKeyToQuit();
}
writeDAT()
{ 
	int aa[1000],n,k;
	FILE *fp;
	fp=fopen("out185.dat","w");
	countvalue(aa,&n);
	for(k=0;k<n;k++)
		if((k+1)%10==0) 
			fprintf(fp,"\n");
		else
			fprintf(fp,"%5d",aa[k]);
	fprintf(fp,"\n");
	fclose(fp);
}
