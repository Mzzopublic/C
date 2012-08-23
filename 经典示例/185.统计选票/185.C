#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000
int xx[MAX],odd=0,even=0;
double ave1=0.0,ave2=0.0,totfc=0.0;
void WriteDat(void);
int ReadDat(void)
{ 
	FILE *fp;
	int i,j;
	if((fp=fopen("IN182.DAT","r"))==NULL)
		return 1;
	for(i=0;i<100;i++)
	{
		for(j=0;j<10;j++)
			fscanf(fp, "%d ", &xx[i*10+j]);
		fscanf(fp, "\n");
		if(feof(fp))
			break;
	}
	fclose(fp);
	return 0;
}
void Compute(void)
{
	int I, yy[MAX];
	for(I=0;I<1000;I++)
		if(xx[I]%2)
		{
			odd++;
			ave1+=xx[I];
			yy[odd-1]=xx[I];
		}
		else
		{
			even++;
			ave2+=xx[I];
		}
	ave1/=odd;
	ave2/=even;
	for(I=0;I<odd;I++)
		totfc+=(yy[I]-ave1)*(yy[I]-ave1)/odd;
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
void main()
{
	int i;
	clrscr();
	puts(" This program is to deal with the number from file.");
	puts(" >> The results are:");
	for(i=0;i<MAX;i++)xx[i]=0;
	if(ReadDat())
	{
		printf(" Can't open data file IN182.DAT!\007\n");
		return;
	}
	Compute();
	printf(" There are %d odds.\n There are %d evens.\n The average value of all the odds is %lf.\n The average value of all the evens is %lf.\n The variance of the odds is %lf.\n",odd,even,ave1,ave2,totfc);
	WriteDat();
	PressKeyToQuit();
}
void WriteDat(void)
{
	FILE *fp;
	int i;
	fp=fopen("OUT182.DAT","w");
		fprintf(fp,"%d\n%d\n%lf\n%lf\n%lf\n",odd,even,ave1,ave2,totfc);
	fclose(fp);
}
