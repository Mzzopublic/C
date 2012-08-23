/* 在BC31下编译 */
/* compile under Borland C++ 3.1 */

/*
对四则混合运算所提取的形式化表达式(生成式)
<exp> -> <term> { <addop> <term> }
<addop> -> + | -
<term> -> <factor> { <mulop> <factor> }
<mulop> -> * | /
<factor> -> ( <exp> ) | Number
*/

#include <stdio.h>
#include <stdlib.h>

char token; /*全局标志变量*/

/*递归调用的函数原型*/
int exp( void );
int term( void );
int factor( void );

void error( void ) /*报告出错信息的函数*/
{
	fprintf( stderr, "错误\n");
	exit( 1 );
}

void match( char expectedToken ) /*对当前的标志进行匹配*/
{
	if( token == expectedToken ) token = getchar(); /*匹配成功，获取下一个标志*/
	else error(); /*匹配不成功，报告错误*/
}
void Message(void)
{
	printf("================================================================\n");
	printf("*               递归实现的四则运算表达式求值程序               *\n");
	printf("****************************************************************\n");
	printf("使用方法:请从键盘上直接输入表达式,以回车键结束.如45*(12-2)[回车]\n");
	printf("*****************************************************************\n\n");
}
main()
{
	int result;  /*运算的结果*/
	Message();
	printf(" >> 请输入表达式: ");
	token = getchar(); /*载入第一个符号*/
	
	result = exp(); /*进行计算*/
	if( token == '\n' ) /* 是否一行结束 */
		printf( " >> 表达式的计算结果为 : %d\n", result );
	else error(); /* 出现了例外的字符 */
	puts("\n\n                  请按任意键退出 ...\n");
	getch();
	return 0;
}

int exp( void )
{
	int temp = term(); /*计算比加减运算优先级别高的部分*/
	while(( token == '+' ) || ( token == '-' ))
		switch( token ) {
		case '+': match('+');     /*加法*/
			  temp += term();
			  break;
		case '-': match('-');
			  temp -= term(); /*减法*/
			  break;
		}
	return temp;
}

int term( void )
{
	int div; /*除数*/
	int temp = factor();   /*计算比乘除运算优先级别高的部分*/
	while(( token == '*' ) || ( token == '/' ))
		switch( token ) {
		case '*': match('*');  /*乘法*/
			  temp *= factor();
			  break;
		case '/': match('/');   /*除法*/
			  div = factor();
			  if( div == 0 ) /*需要判断除数是否为0*/
			  {
			  	fprintf( stderr, "除数为0.\n" );
			  	exit(1);
			  }
			  temp /= div; 
			  break;
		}
	return temp;
}

int factor( void )
{
	int temp; 
	if( token == '(' ) /*带有括号的运算*/
	{
		match( '(' );
		temp = exp();
		match(')');
	}
	else if ( isdigit( token )) /*实际的数字*/
	{
		ungetc( token, stdin ); /*将读入的字符退还给输入流*/
		scanf( "%d", &temp ); /*读出数字*/
		token = getchar();  /*读出当前的标志*/
	}
	else error(); /*不是括号也不是数字*/
	return temp;
}