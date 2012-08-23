#include<stdio.h>
#include<mem.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>
#define MAX 100
typedef struct{
char dm[5]; /*产品代码*/
char mc[11]; /*产品名称*/
int dj; /*单价*/
int sl; /*数量*/
long je; /*金额*/
}PRO;
PRO sell[MAX];
void ReadDat();
void WriteDat();
void SortDat()
{
	int I,j;
	PRO xy;
	for(I=0;I<99;I++)
	for(j=I+1;j<100;j++)
	if(strcmp(sell[I].dm,sell[j].dm)<0)
	{xy=sell[I];sell[I]=sell[j];sell[j]=xy;}
	else if(strcmp(sell[I].dm,sell[j].dm)==0)
	if(sell[I].je<sell[j].je)
	{xy=sell[I]; sell[I]=sell[j]; sell[j]=xy;}
}
void main()
{
	memset(sell,0,sizeof(sell));
	ReadDat();
	SortDat();
	WriteDat();
}
void ReadDat()
{
	FILE *fp;
	char str[80],ch[11];
	int i;
	fp=fopen("IN175.DAT","r");
	for(i=0;i<100;i++){
	fgets(str,80,fp);
	memcpy(sell[i].dm,str,4);
	memcpy(sell[i].mc,str+4,10);
	memcpy(ch,str+14,4);ch[4]='\0';
	sell[i].dj=atoi(ch);
	memcpy(ch,str+18,5);ch[5]='\0';
	sell[i].sl=atoi(ch);
	sell[i].je=(long)sell[i].dj*sell[i].sl;}
	fclose(fp);
}
void WriteDat(void)
{
	FILE *fp;
	int i;
	fp=fopen("OUT175.DAT","w");
	for(i=0;i<100;i++){
	fprintf(fp,"%s %s %4d %5d %10ld\n", sell[i].dm,sell[i].mc,sell[i].dj,sell[i].sl,sell[i].je);}
	fclose(fp);
}
