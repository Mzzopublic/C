#include<stdio.h>
struct date{
    int year;
    int month;
    int day;
};
int days(struct date day);


void main()
{
    struct date today,term;
    int yearday,year,day;
	puts("◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇");
	puts("◇              打鱼还是晒网                        ◇");
	puts("◇    中国有句俗语叫【三天打鱼两天晒网】。          ◇");
	puts("◇某人20岁从1990年1月1日起开始【三天打鱼两天晒网】，◇");
	puts("◇问这个人在以后的某一天中是【打鱼】还是【晒网】？  ◇");
	puts("◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇\n");
	while(1)
	{
		printf(" >> 请输入年/月/日【输入1990 1 1 退出】：");
		scanf("%d%d%d",&today.year,&today.month,&today.day);  /*输入日期*/
		if(today.year<1990)
		{
			if(today.year<1970)
				puts(" >> 对不起，那一年那还没出生呢！按任意键继续...");
			else
				puts(" >> 对不起，那一年他还没开始打鱼呢！按任意键继续...");
			getch();
			continue;
		}
		if(today.year==1990&&today.month==1&&today.day==1)
			break;
		term.month=12;               /*设置变量的初始值：月*/
		term.day=31;                 /*设置变量的初始值：日*/
		for(yearday=0,year=1990;year<today.year;year++)
		{
			term.year=year;
			yearday+=days(term);     /*计算从1990年至指定年的前一年共有多少天*/
		}
		yearday+=days(today);       /*加上指定年中到指定日期的天数*/
		day=yearday%5;               /*求余数*/
		if(day>0&&day<4) printf(" >> %d年%d月%d日，他正在打鱼。\n",today.year,today.month,today.day);   /*打印结果*/
		else printf(" >> %d年%d月%d日，他正在晒网。\n",today.year,today.month,today.day);
		
	}
	puts("\n >>      请按任意键退出...");
	getch();
}

int days(struct date day)
{
    static int day_tab[2][13]=
            {{0,31,28,31,30,31,30,31,31,30,31,30,31,},      /*平均每月的天数*/
             {0,31,29,31,30,31,30,31,31,30,31,30,31,},
    };
    int i,lp;
    lp=day.year%4==0&&day.year%100!=0||day.year%400==0;
      /*判定year为闰年还是平年，lp=0为平年，非0为闰年*/
    for(i=1;i<day.month;i++)            /*计算本年中自1月1日起的天数*/
        day.day+=day_tab[lp][i];
    return day.day;
}
