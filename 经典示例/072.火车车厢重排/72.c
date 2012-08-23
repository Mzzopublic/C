#include "stdafx.h"
#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"
#include "malloc.h"
#include "string.h"
#define StackSize 100 //假定预分配的栈空间最多为100个元素 
#define MaxLength  100// 最大的字符串长度 	
typedef int DataType;//假定栈元素的数据类型为整数 	
typedef struct{
	DataType data[StackSize];
	int top;
}SeqStack;   

// 置栈空
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
int  Hold(int c,int *minH, int *minS,SeqStack H[],int k, int n)
{// 在一个缓冲铁轨中放入车厢c
	// 如果没有可用的缓冲铁轨，则返回0
	// 如果空间不足，则引发异常N o M e m
	// 否则返回1
	// 为车厢c寻找最优的缓冲铁轨
	// 初始化
	int BestTrack = 0,i; // 目前最优的铁轨
	int BestTop = n + 1;// 最优铁轨上的头辆车厢
	int x;// 车厢索引
	//扫描缓冲铁轨
	for (i = 1; i <= k; i++)
		if (IsEmpty(&H[i])) 
		{// 铁轨i 不空
			x = Top (&H[i]) ;
			if (c<x && x < BestTop)
			{//铁轨i 顶部的车厢编号最小
				BestTop = x;
				BestTrack = i;
			}
		}
		else // 铁轨i 为空
			if (!BestTrack) 
				BestTrack = i;
			if (!BestTrack) 
				return 0; //没有可用的铁轨
			//把车厢c 送入缓冲铁轨
			Push(&H[BestTrack],c);
			printf("Move car %d  from input to holding track %d\n" ,c, BestTrack);
			//必要时修改minH 和minS
			if (c<*minH) {
				*minH = c; 
				*minS = BestTrack; 
			}
			return 1;
}
void Output(int* minH, int* minS, SeqStack H[ ], int k, int n)
{ //把车厢从缓冲铁轨送至出轨处，同时修改m i n S和m i n H
	int c,i; // 车厢索引
	// 从堆栈m i n S中删除编号最小的车厢minH
	c=Pop(&H[*minS]) ;
	printf("Move car %d from holding track %d to output\n",*minH,*minS);
	// 通过检查所有的栈顶，搜索新的m i n H和m i n S
	*minH=n+2;
	for (i = 1; i <= k; i++)
		if (!IsEmpty(&H[i]) && (c=Top(&H[i])) < *minH) {
			*minH = c;
			*minS = i;
		}
}
int Railroad(int p[], int n, int k)
{// k个缓冲铁轨，车厢初始排序为p [1:n]
	// 如果重排成功，返回1，否则返回0
	// 如果内存不足，则引发异常NoMem 。
	//创建与缓冲铁轨对应的堆栈
	SeqStack *H;
	int NowOut = 1; //下一次要输出的车厢
	int minH =n+1; //缓冲铁轨中编号最小的车厢
	int minS,i; //minH号车厢对应的缓冲铁轨
	H=(SeqStack*)calloc((k+1),sizeof(SeqStack)*(k+1));
	//车厢重排
	for (i = 1; i<= n; i++)
		if (p[i] == NowOut) {// 直接输出t
			printf("移动车厢%d从出口到入口",p[i]);
			NowOut++;
			//从缓冲铁轨中输出
			while (minH == NowOut) {
				Output(&minH, &minS, H, k, n);
				NowOut++;
			}
		}
		else {// 将p[i] 送入某个缓冲铁轨
			if (!Hold(p[i], &minH, &minS, H, k, n))
				return 0;
		}
		return 1;
}
void main(void)
{
	int p[8]={2,4,1,6,5,3,8,7};
	Railroad(p,8,4);
}
