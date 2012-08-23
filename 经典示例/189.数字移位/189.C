#include<stdio.h>
#include<conio.h>
#define MAXNUM 200 
int xx[MAXNUM];
int totnum=0;
int totcnt=0;
double totpjz=0.0;
int readdat(void);
void writedat(void);
void calvalue(void)
{
	int i,data;
	for(i=0;i<MAXNUM;i++)
	{
		if(!xx[i]) break;
		if(xx[i]>0) totnum++;
		data=xx[i]>>1;
		if(data%2==0)
		{	
			totcnt++;
			totpjz+=xx[i];
		}
	}
	totpjz/=totcnt;
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
	puts(" This program is to execute Bit Shift Operation.");
	puts(" >> Now reading datas from file......Succeeded!");
	puts(" >> Calculating......Completed!");
	puts(" >> Results are:");
	for(i=0;i<MAXNUM;i++) xx[i]=0;
	if(readdat())
	{
		printf(" Can't open the data file in188.dat!\007\n");
		return;
	}
	calvalue();
	printf(" >> The total number of data in the file is %d.\n",totnum);
	printf(" >> The even number of data after bit shift is %d.\n",totcnt);
	printf(" >> The variance of these even numbers is %.2lf\n",totpjz);
	writedat();
	PressKeyToQuit();
}
int readdat(void)
{
	FILE *fp;
	int i=0;
	if((fp=fopen("in188.dat","r"))==NULL) return 1;
	while(!feof(fp))
	fscanf(fp,"%d",&xx[i++]);
	fclose(fp);
	return 0;
}
void writedat(void)
{
	FILE *fp;
	fp=fopen("out188.dat","w");
	fprintf(fp,"%d\n%d\n%.2lf\n",totnum,totcnt,totpjz);
	fclose(fp);
}


