/*
该程序实现统计一个或多个文件的行数、字数和字符数。
一个行由一个换行符限定，一个字由空格分隔（包括空白符、制表符和换行符），
字符是指文件中的所有字符。要求程序另设三个任选的参数，
让用户指定他所要的统计。它们是：
1 统计文件行数
w 统计文件字数
c 统计文件字符数
若用户未指定任选的参数，则表示三个统计都要。
运行本程序时的参数按一下格式给出：
-l -w -c 文件 文件 ... 文件
其中，前三个任选参数l、w、c的出现与否和出现顺序任意，
或任意组合在一起出现，如：-lwc，-cwl，-lw，-wl，-lc，-cl，－cw等。
*/

#include <stdio.h>
main(int argc, char **argv )
{
	FILE *fp;
	int lflg,wflg,cflg; /* l, w, c三个标志 */
	int inline,inword; /* 行内和字内标志 */
	int ccount,wcount,lcount; /* 字符，字，行 计数器 */
	int c;
	char *s;
	lflg=wflg=cflg=0;
	if(argc<2)
	{
		printf("To run this program, usage: program -l -w -c file1 file2 ... filen \n");
		exit(0);
	}
	while(--argc>=1&&(*++argv)[0]=='-')
	{
		for(s=argv[0]+1;*s!='\0';s++)
		{
			switch(*s)
			{
				case 'l': 
					lflg=1;
					break;
				case 'w':
					wflg=1;
					break;
				case 'c':
					cflg=1;
					break;
				default:
					puts("To run this program, usage: program -l -w -c file1 file2 ... filen");
					exit(0);
			}
		}
	}
	if(lflg==0&&wflg==0&&cflg==0)
		lflg=wflg=cflg=1;
	lcount=wcount=ccount=0;
	while(--argc>=0)
	{
		if((fp=fopen(*argv++,"r"))==NULL)	/* 以只读方式打开文件 */
		{
			fprintf(stderr,"Can't open %s.\n",*argv);
			continue;
		}
		inword=inline=0;
		while((c=fgetc(fp))!=EOF)
		{
			if(cflg)
				ccount++;
			if(wflg)
				if(c=='\n'||c==' '||c=='\t')
					inword=0;
				else if(inword==0)
				{
					wcount++;
					inword=1;
				}
			if(lflg)
				if(c=='\n')
					inline=0;
				else if(inline==0)
				{
					lcount++;
					inline=1;
				}
		}
		fclose(fp);	/* 关闭文件 */
	}
	if(lflg)
		printf(" Lines =         %d\n",lcount);
	if(wflg)
		printf(" Words =         %d\n",wcount);
	if(cflg)
		printf(" Characters =    %d\n",ccount);
}
