#include <stdio.h>
#define PAGELINE   20
#define PAGESPLINE 2
#define TXTWIDTH   30
#define TXTGAP     10

linecount()/*完成对输出行的计数和一页满后，输出空行*/
{
	static int pline=0;
	int i;

	if(++pline==PAGELINE)
	{
		for(i=0;i<PAGESPLINE;i++)/*输出一页后的空行*/
			printf("\n");
		pline=0;
	}
}

int readline(FILE *fpt)/*完成从指定的文件中读出一行多至30个字符并输出*/
{
	int c,cpos=0;

	while((c=fgetc(fpt))!='\n')
	{
		if(feof(fpt))
			break;/*文件结束推出循环*/
		printf("%c",c);
		cpos++;
		if(cpos>=TXTWIDTH)
			break;
	}
	return cpos;/*返回读入并输出的字符数*/
}

main()
{
	FILE *fpt1,*fpt2;
	char fname[120];/*存贮文件名*/
	int fill1,fill2;/*分别记录两个文件当前行读入并输出的字符数*/

	clrscr();
	printf("Enter file 1 name.\n");
	scanf("%s",fname);
	fpt1=fopen(fname,"r");/*打开文件1*/
	if(fpt1==NULL)
	{	printf("Can't open file %s.\n",fname);
		exit(1);
	}
	printf("Enter file 2 name.\n");
	scanf("%s",fname);
	fpt2=fopen(fname,"r");/*打开文件2*/
	if(fpt2==NULL)
	{	printf("Can't open file %s.\n",fname);
		fclose(fpt1);
		exit(2);
	}

	while(!feof(fpt1)||!feof(fpt2))/*在有文件还未结束时循环*/
	{
		fill1=fill2=0;
		if(!feof(fpt1)) fill1=readline(fpt1);/*在文件未结束时读文件*/
		printf("%*c",TXTWIDTH-fill1+TXTGAP,'');
		if(!feof(fpt2)) fill2=readline(fpt2);/*在文件未结束时读文件*/
		printf("%*c%2d\n",TXTWIDTH-fill2+4,'',fill1+fill2);
		linecount();/*调用行计数函数*/
	}
	puts("\n Press any key to quit...");
	getch();
}
