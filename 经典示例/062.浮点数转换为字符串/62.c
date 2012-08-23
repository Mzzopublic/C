#include <stdio.h>
#include <string.h>

char c_je[51];  /*大写金额字符变量*/

char* zh( x )                     /*数字金额转换为大写金额子程序*/
double x;                          /*要转换的金额数*/
{
	int i, n, bz;
	char je[14];        /*数字金额的字符变量*/
	char temp[13];
	char f1[10][3] = {"零","壹","贰","叁","肆","伍",
	                  "陆","柒","捌","玖"}; /*数字对应的大写数组变量*/
	char f2[11][3] = {"亿","仟","佰","拾","万",
	                  "仟","佰","拾","元","角","分"}; /*每位数字对应单位数组变量*/
	sprintf( je, "%.01f", 100*x); /*转换成字符*/
	n = strlen( je );
	c_je[0] = '\0';
	bz  = 1;
	for( i = 0; i < n; i++ )
	{
		strcpy( temp, &je[i] );  /*复制到临时数组*/
		if( atoi(temp)==0)    /*判断第i位后是否全为0*/
		{
			bz = 2;
			break;
		}
		if( je[i] != '0' )
		{
			if( bz == 0 )
				strcat( c_je, f1[0] );
			strcat( c_je, f1[je[i] - '0'] ); /*数字串转化字符串*/
			bz = 1;
			strcat( c_je, f2[13-n+i]);
		}
		else
		{
			if( n-i == 7 && (je[i-1]!='0'||je[i-2]!='0' || je[i-3]!='0')) /*判断万位位置*/
				strcat( c_je, "万" );
			if( n-i == 3 ) /*判断个位数的元位置*/
				strcat( c_je, "元");
			bz = 0;
		}
	}
	if( bz == 2 )
	{
		if( n-i >= 7 && n-i < 10 )
			strcat( c_je, "万") ; /*万位数字为0，加‘万’*/
		if( n-i >= 3 )
			strcat( c_je, "元");
		strcat( c_je, "正" ); /*最后不是分位，加“正”*/
	}
	return c_je; /*返回大写金额*/
}

main()
{
	double count;
	//clrscr();
	printf("*********************************************************\n");
	printf("*                                                       *\n");
	printf("*           数字金额转换为大写金额程序 Ver.1.0          *\n");
	printf("*                                                       *\n");
	printf("*                                By RZLIN               *\n");
	printf("*                                                       *\n");
	printf("*               请输入要转换的金额：                    *\n");
	printf("*                     ");
	scanf("%lf", &count );
	printf("*           您输入的金额为：%10.2lf 。               *\n",count);
	printf("*                                                       *\n");
	printf("*转换为大写金额是：%s\n", zh( count ) );
	printf("*                                                       *\n");
	puts("*                请按任意键退出...                      *");
	printf("*********************************************************\n");
	getch();
}