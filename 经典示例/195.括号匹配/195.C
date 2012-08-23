#include "stdafx.h"
#include"iostream.h"
#include"string.h"
#include"stdlib.h"
#include"stdio.h"
#define StackSize 100 //假定预分配的栈空间最多为100个元素 
#define MaxLength  100// 最大的字符串长度 	
typedef int DataType;//假定栈元素的数据类型为整数 	
typedef struct{
	DataType data[StackSize];
	int top;
}SeqStack;   
//置栈空
void Initial(SeqStack *S)
{//将顺序栈置空
	S->top=-1;
} 
//判栈空
int IsEmpty(SeqStack *S)
{
	return S->top==-1;
}
//判栈满
int IsFull(SeqStack *S)
{
	return S->top==StackSize-1;
}
//进栈
void Push(SeqStack *S,DataType x)
{
	if (IsFull(S))
	{
		printf("栈上溢"); //上溢,退出运行
		exit(1);
	}
	S->data[++S->top]=x;//栈顶指针加1后将x入栈
}
//出栈
DataType Pop(SeqStack *S)
{
	if(IsEmpty(S))
	{
		printf("栈为空"); //下溢,退出运行
		return -1;
	}
	return S->data[S->top--];//栈顶元素返回后将栈顶指针减1
}	
// 取栈顶元素
DataType Top(SeqStack *S)
{
	if(IsEmpty(S))
	{
		printf("栈为空"); //下溢,退出运行
		exit(1);
	}
	return S->data[S->top];
}
void PrintMatchedPairs(char *expr)
{// 括号匹配
	SeqStack s;
	int j, i,length=strlen(expr);
	// 从表达式expr 中搜索( 和)
	Initial(&s);
	for (i = 1; i<=length; i++){
		if (expr[i-1] =='(') 
			Push(&s,i);
		else if (expr[i-1] ==')')
		{
			j=Pop(&s) ;
			if(j==-1)
				printf("没对应第%d个右括号的左括号",i);
			else
				printf("%d	%d\n",i,j);
		}
	}
	// 堆栈中所剩下的(都是未匹配的
	while(!IsEmpty(&s)) {
		j=Pop(&s) ;
		printf("没对应第%d个左括号的右括号",j);
	}
}
//主函数
void main(void)
{
	char expr[MaxLength];
	printf("请输入符号个数小于%d的表达式\n",MaxLength);
	cin.getline(expr,MaxLength);
	printf("括号对是\n");
	PrintMatchedPairs(expr);
}
