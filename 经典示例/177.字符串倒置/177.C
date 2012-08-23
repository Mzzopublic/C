#include "stdio.h"
#include "string.h"
#include "conio.h"
char xx[50][80];
int maxline=0;
int ReadDat(void);
void WriteDat(void);
void StrOR(void) 
{
	int I,j,k,index,strl;
	char ch;
	for(I=0;I<maxline;I++)
	{
		strl=strlen(xx[I]);
		index=strl;
		for(j=0;j<strl;j++)
			if(xx[I][j]=='o')
			{
				for(k=j;k<strl-1;k++)
					xx[I][k]=xx[I][k+1]; 
				xx[I][strl-1]= ' ';
				index=j;
			}
		for(j=strl-1;j>=index;j--)
		{
			ch=xx[I][strl-1];
			for(k=strl-1;k>0;k--)
				xx[I][k]=xx[I][k-1];
			xx[I][0]=ch;
		}
	}
}
void main()
{
	clrscr();
	if(ReadDat())
	{
		printf("Can't open the file!\n");
		return;
		}
	StrOR();
	WriteDat();
	system("pause");
}
int ReadDat(void)
{
	FILE *fp;int i=0;char *p;
	if((fp=fopen("in173.dat","r"))==NULL) return 1;
	while(fgets(xx[i],80,fp)!=NULL)
	{
		p=strchr(xx[i],'\n');
		if(p)
			*p=0;
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
	fp=fopen("out173.dat","w");
	for(i=0;i<maxline;i++)
	{
		printf("%s\n",xx[i]);
		fprintf(fp,"%s\n",xx[i]);
	}
	fclose(fp);
}
