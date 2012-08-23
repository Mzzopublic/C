#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>
unsigned char xx[50][80];
int maxline=0;/*文章的总行数*/
int ReadDat(void);
void WriteDat(void);
void encryptChar()
{
	int I;
	char *pf;
	for(I=0;I<maxline;I++)
	{pf=xx[I];
	while(*pf!=0)
	{if(*pf*11%256>130||*pf*11%256<=32);
	else
	*pf=*pf*11%256;
	pf++; }
	}
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
void main()
{
	clrscr();
	if(ReadDat())
	{
		printf(" Can't open file IN176.DAT!\n");
		return;
	}
	encryptChar();
	WriteDat();
	PressKeyToQuit();
}
int ReadDat(void)
{
	FILE *fp;
	int i=0;
	unsigned char *p;
	if((fp=fopen("in176.dat","r"))==NULL) return 1;
	while(fgets(xx[i],80,fp)!=NULL)
	{
		p=strchr(xx[i],'\n');
		if(p)*p=0;
		i++;
	}
	maxline=i;
	fclose(fp);
	return 0;
}
void WriteDat(void)
{
	FILE *fp;
	int i;
	fp=fopen("out176.dat","w");
	for(i=0;i<maxline;i++)
	{
		printf("%s\n",xx[i]);
		fprintf(fp,"%s\n",xx[i]);
	}
	fclose(fp);
}
