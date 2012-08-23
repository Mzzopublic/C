/* 在BC31下编译 或VC6.0*/
/* compile under Borland C++ 3.1 or Visual C++ 6.0*/

/*#include "stdafx.h"*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"

#define TRUE 1
#define FALSE 0
#define STACK_INIT_SIZE 100/*存储空间初始分配量*/
#define STACKINCREMENT 20/*存储空间分配增量*/

typedef struct
{
	int *pBase;/*在构造之前和销毁之后,base的值为NULL*/
	int *pTop;/*栈顶指针*/
	int StackSize;/*当前已分配的存储空间,以元素为单位*/
}Stack;

typedef int BOOLEAN;
 
char Operator[8]="+-*/()#";/*合法的操作符存储在字符串中*/
char Optr;/*操作符*/
int Opnd=-1;/*操作符*/
int Result;/*操作结果*/

/*算符间的优先关系*/
char PriorityTable[7][7]=
{
	{'>','>','<','<','<','>','>'},
	{'>','>','<','<','<','>','>'},
	{'>','>','>','>','<','>','>'},
	{'>','>','>','>','<','>','>'},
	{'<','<','<','<','<','=','o'},
	{'>','>','>','>','o','>','>'},
	{'<','<','<','<','<','o','='},
};

//数据对象的操作方法
//构造一个空栈,如果返回值为0,则表示初始化失败
Stack InitStack()/*这是个效率低的方法*/
{
	Stack S;
	S.pBase=(int*)malloc(STACK_INIT_SIZE*sizeof(int));
	if(!S.pBase)
	{/*内存分配失败*/
		printf("内存分配失败,程序中止运行\n");
		exit(-1);
	}
	else
	{
		S.pTop=S.pBase;
		S.StackSize=STACK_INIT_SIZE;
	}
	return S;
}
//销毁栈S,S不再存在
void DestoryStack(Stack *S)
{
	if(S->pBase)
	{
		free(S->pBase);
		S->pTop=S->pBase=NULL;
        
	}
}
//若栈不空,则用e返回S的栈顶元素
//注:由于应用的特殊,可以不检查栈是否为空
int GetTop(Stack S)
{
	return *(S.pTop-1);
}
//插入元素e为新的栈顶元素,如果成功则返回1,否则返回0
int Push(Stack *S,int e)
{
	if(S->pTop-S->pBase==S->StackSize)
	{//栈满,追加存储空间
		S->pBase=(int*)realloc(S->pBase,S->StackSize+STACKINCREMENT*sizeof(int));
		if(!S->pBase)
			return 0;//存储分配失败
		S->pTop=S->pBase+S->StackSize;
		S->StackSize+=STACKINCREMENT;
	}
	*(S->pTop++)=e;
	return 1;
}

int Pop(Stack *S,int *e)
{//若栈不空,则删除S的栈顶元素,用e 返回其值,并返回1;否则返回0
	if(S->pTop==S->pBase)
		return 0;
	*e=*--(S->pTop);
	return 1;

}
//主函数及其它函数的实现
//比较两个数学符号operator_1,operator_2的计算优先权,在算符优先关系表中查找相应的关系并返回'<','=',或'>'
char CheckPriority(char operator_1,char operator_2)
{
	int i,j;//用来查询算符间优先关系表的下标
	//char *ptr;
	i=strchr(Operator,operator_1)-Operator;//找到传入操作符在字符串Operators中的相对位置
	j=strchr(Operator,operator_2)-Operator;
	//返回算符优先关系表中相应值
	return PriorityTable[i][j];
}

BOOLEAN IsOperator(char ch)
{//判断一个字符是否为打操作符
	if(strchr(Operator,ch))
		return TRUE;
	else 
		return FALSE;

}
//从键盘获得输入
void GetInput(void)
{
	char Buffer[20];//键盘输入缓冲区,用来处理输入多位数的情况
	char ch;//存放键盘输入
	int index;//存放Buffer的下标
	index=0;
	ch=getch();//从键盘读入一个字符
	while(ch!=13&&!IsOperator(ch))
	{//如果输入的字符是回车符或是操作符,循环结束
		if(ch>='0'&&ch<='9')
		{//将字符回显到屏幕
			printf("%c",ch);
			Buffer[index]=ch;
			index++;

		}
		ch=getch();
	}
	if(ch==13)
		Optr='#';//输入的表达式以回车符结束
	else
	{
		Optr=ch;
		printf("%c",ch);

	}
	if(index>0)
	{
		Buffer[index]='\0';
		Opnd=atoi((Buffer));
	}
	else
		Opnd=-1;//程序不支持输入负数,当Opnd为负数时,表示输入的字符为操作符
}
//计算形如a+b之类的表达式,theta为操作符,a,b为操作数
int Calc(int a,char theta,int b)
{
	switch(theta)
	{
	case '+':
		return a+b;
	case '-':
		return a-b;
	case '*':
		return a*b;
	default:
		if(b==0)//除数为零的情况
		{
			printf("除数不能为");
			return 0;//返回0用以显示
		}
		else
			return a/b;
	}
}
/*表达式求值*/
BOOLEAN EvaluateExpression()
{
	int temp;//临时变量
	char theta;//存放操作符的变量
	int itheta;//存放出栈的操作符的变量add by me
	int a,b;//存放表达式运算时的中间值
	int topOpnd;//栈顶操作数
	char topOptr;//栈顶操作符
	
	Stack OPTR=InitStack();//操作符栈
	Stack OPND=InitStack();//操作数栈

	if(!Push(&OPTR,'#'))//操作符栈中的第一个为#字符
		return FALSE;

	GetInput();//从键盘获得输入

	while(Optr!='#'||GetTop(OPTR)!='#')
	{//如果Optr>=0,表示有操作数输入
		if(Opnd>=0)Push(&OPND,Opnd);
		switch(CheckPriority(GetTop(OPTR),Optr))
		{
		case '<'://栈顶元素优先权低
			if(!Push(&OPTR,Optr))return FALSE;
				GetInput();
			break;
		case '='://脱括号并接收键盘输入
			Pop(&OPTR,&temp);GetInput();
			break;
		case '>'://退栈并将运算结果入栈
			//先用itheta得到操作符在赋给theta
			Pop(&OPTR,&itheta);
			Pop(&OPND,&b);
			Pop(&OPND,&a);
			theta = (char)( itheta );
			Push(&OPND,Calc(a,itheta,b));
			Opnd=-1;
			break;

		}
	}
	//本算法中,当输入只有一个操作数然后就输入回车符时,
	//OPND.pTop==OPND.pBase
	//如果OPND.pTop==OPND.pBase并且Opnd<0,则说明用户
	//未输入任何操作和操作符而直接输入[回车],程序直接
	//退出运行
	if(OPND.pTop==OPND.pBase&&Opnd<0)
	{
		printf("\n\n感谢使用!\n");
		exit(1);

	}
	else if(OPND.pTop==OPND.pBase)
		Result=Opnd;
	else
	{
		Result=GetTop(OPND);
		DestoryStack(&OPND);
		DestoryStack(&OPTR);
	}
	return TRUE;

}

void Message(void)
{
	printf("\n四则运算表达式求值演示\n");
	printf("-------------------------------\n");
	printf("使用方法:请从键盘上直接输入表达式,以回车键结束.如45*(12-2)[回车]\n");
	printf("注0:不输入任何数而直接按[回车]键,将退出程序.\n");
	printf("注1:本程序暂时不接受除数字键及四则运算符之外的任何其它键盘输入.\n");
	printf("注2:本程序暂时只能处理正确的表达式,不支持输入负数.\n");
	printf("-------------------------------\n\n");
}
void main(void)
{
	int i;//用来一些说明性信息
	Message();
	for(i=1;;i++)
	{
		printf("表达式%d:",i);
		if(EvaluateExpression())
			printf("=%d\n",Result);
		else
			printf("计算中遇到错误\n");
		
	}
}
