#include "stdio.h"
long int f(int year,int month)
{/*f(年，月)＝年－1，如月<3;否则，f(年，月)＝年*/
	if(month<3) return year-1;
	else return year;
}

long int g(int month)
{/*g(月)＝月＋13，如月<3;否则，g(月)＝月＋1*/
	if(month<3) return month+13;
	else return month+1;
}

long int n(int year,int month,int day)
{
  /*N=1461*f(年、月)/4+153*g(月)/5+日*/
	return 1461L*f(year,month)/4+153L*g(month)/5+day;
}

int w(int year,int month,int day)
{
  /*w=(N-621049)%7(0<=w<7)*/
	return(int)((n(year,month,day)%7-621049L%7+7)%7);
}

int date[12][6][7];
int day_tbl[ ][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
		    {31,29,31,30,31,30,31,31,30,31,30,31}};
main()
{int sw,leap,i,j,k,wd,day;
 int year;/*年*/
 char title[]="SUN MON TUE WED THU FRI SAT";
clrscr();
printf("Please input the year whose calendar you want to know: ");/*输入年*/
scanf("%d%*c",&year);/*输入年份值和掠过值后的回车*/
sw=w(year,1,1);
leap=year%4==0&&year%100||year%400==0;/*判闰年*/
for(i=0;i<12;i++)
	for(j=0;j<6;j++)
		for(k=0;k<7;k++)
			date[i][j][k]=0;/*日期表置0*/
for(i=0;i<12;i++)/*一年十二个月*/
	for(wd=0,day=1;day<=day_tbl[leap][i];day++)
	{/*将第i＋1月的日期填入日期表*/
	 date[i][wd][sw]=day;
	sw=++sw%7;/*每星期七天，以0至6计数*/
	if(sw==0) wd++;/*日期表每七天一行，星期天开始新的一行*/
	}

	printf("\n|==================The Calendar of Year %d =====================|\n|",year);
for(i=0;i<6;i++)
{/*先测算第i+1月和第i+7月的最大星期数*/
	for(wd=0,k=0;k<7;k++)/*日期表的第六行有日期，则wd!=0*/
		wd+=date[i][5][k]+date[i+6][5][k];
	wd=wd?6:5;
	printf("%2d  %s  %2d  %s |\n|",i+1,title,i+7,title);
	for(j=0;j<wd;j++)
	{
		printf("   ");/*输出四个空白符*/
		/*左栏为第i+1月，右栏为第i+7月*/
		for(k=0;k<7;k++)
			if(date[i][j][k])
				printf("%4d",date[i][j][k]);
			else printf("    ");
		printf("     ");/*输出十个空白符*/
		for(k=0;k<7;k++)
			if(date[i+6][j][k])
				printf("%4d",date[i+6][j][k]);
			else printf("    ");
		printf(" |\n|");
	}
	/*scanf("%*c");/*键入回车输出下一个月的日历*/
 
}
puts("=================================================================|");
puts("\n Press any key to quit...");
getch();
}