#include <stdio.h>
#define N 200
#define SCORES 5
#define NUMLEN 10
struct std_type{
	char no[NUMLEN];/*学号*/
	char *name;/*名字符串指针*/
	int scores[SCORES];/*五门功课的成绩*/
};
struct std_type students[N];
int order[N];
int total[N];

/*[函数]输入一个学生信息函数*/
int readastu(struct std_type *spt)
{
	int len,j;
	char buf[120];/*输入字符串的缓冲区*/

	printf("\nNumber   :   ");/*输入学号*/
	if(scanf("%s",buf)==1)
		strncpy(spt->no,buf,NUMLEN-1);
	else
		return 0;/*Ctrl+Z结束输入*/
	printf("Name   :   ");/*输入姓名*/
	if(scanf("%s",buf)==1)
	{
		len=strlen(buf);
		spt->name=(char *)malloc(len+1);/*申请存贮姓名的空间*/
		strcpy(spt->name,buf);
	}
	else return 0;/*Ctrl+Z结束输入*/
	printf("Scores   :   ");/*输入成绩*/
	for(j=0;j<SCORES;j++)
		if(scanf("%d",spt->scores+j)!=1)
			break;
			if(j==0)/*一个成绩也未输入*/
			{
				free(spt->name);/*释放存贮姓名的空间*/
				return 0;
			}
			for(;j<SCORES;j++)/*少数未输入的成绩用0分代之*/
				spt->scores[j]=0;
			return 1;
	}

/*[函数]输出一个学生信息的函数*/
int writeastu(struct std_type *spt)
{
	int i;

	printf("Number   :   %s\n",spt->no);/*输出学号*/
	printf("Name     :   %s\n",spt->name);/*输出姓名*/
	printf("Scores   :   ");/*输出成绩*/
	for(i=0;i<SCORES;i++)
		printf("%4d",spt->scores[i]);
	printf("\n\n");
}

main()
{
	int n,i,j,t;

	clrscr();
	for(n=0;readastu(students+n);n++);
	/*采用冒泡法对学生信息数组排序*/
	for(i=0;i<n;i++)
	{
		order[i]=i;/*预置第i个输入的学生*/
		for(t=0,j=0;j<SCORES;j++)/*求第i个学生的总分*/
			t+=students[i].scores[j];
		total[i]=t;
	}
	/*冒泡排序*/
	for(i=0;i<n-1;i++)/*共扫视n-1遍*/
		for(j=0;j<n-1-i;j++)
			if(total[order[j]]<total[order[j+1]])
			{/*交换名次*/
			 t=order[j];
			 order[j]=order[j+1];
			 order[j+1]=t;
			}
	for(j=0;j<n;j++)/*输出*/
		writeastu(students+order[j]);
	printf("\n Press any key to quit...\n");
	getch();
}
