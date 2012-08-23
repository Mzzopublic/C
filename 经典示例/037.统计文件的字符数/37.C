#include <stdio.h>
main()
{
	char fname[80];/*存贮文件名*/
	FILE *rfp;
	long count;/*文件字符计数器*/

	clrscr();
	printf("Please input the file's name:\n");
	scanf("%s",fname);
	if((rfp=fopen(fname,"r"))==NULL)
	{
		printf("Can't open file %s.\n",fname);
		exit(1);
	}
	count=0;
	while(fgetc(rfp)!=EOF)
		count++;
	fclose(rfp);/*关闭文件*/
	printf("There are %ld characters in file %s.\n",count,fname);
	puts("\n Press any key to quit...");
	getch();
}
