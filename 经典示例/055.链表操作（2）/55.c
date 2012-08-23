#include "stdafx.h"
#include "stdio.h"
#ifndef SQLIST_H
#define SQLIST_H
#include <stdlib.h>
#define List_INIT_SIZE  100   //线性表的存储空间初始大小
#define LIST_INCREMENT  10    //分配增量
#define OVERFLOW   -2
#define OK     1
#define ERROR    -1
#define TRUE    1
#define FALSE    0
typedef int  ElemType;
typedef struct{
	ElemType *elem;  //存储空间基址
	int   length;  //当前长度
	int   size;    //当前存储容量(sizeof(ElemType)为单位)
}SqList;
int InitList(SqList &L)
{
	//构建一个线性表L
	L.elem = (ElemType*)malloc(List_INIT_SIZE*sizeof(ElemType));
	if(!L.elem)
	{
		exit(OVERFLOW);
	}
	L.length = 0;
	L.size = List_INIT_SIZE;
	return OK;
}//InitList

void DestoryList(SqList &L)
{
	if(!L.elem)
	{
		delete L.elem;
		L.elem = NULL;
	}
	L.length = 0;
	L.size = 0;
}//DestoryList

void ClearList(SqList &L)
{
	L.elem = NULL;
	L.length = 0;
	L.size = List_INIT_SIZE;
}//ClearList

int ListEmpty(SqList L)
{
	if(L.length > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}//ListEmpty

int ListLength(SqList L)
{
	return L.length;
}//ListLength

int ListInsert(SqList &L, int i, ElemType e)
{
	if(i<1 || i>ListLength(L)+1) //需满足条件1<i<ListLength(L)+1
	{
		return ERROR;
	}
	ElemType * NewBase;  //新的基址
	if(L.length > L.size) //空间已满
	{
		NewBase = (ElemType*)malloc((L.size+LIST_INCREMENT)*sizeof(ElemType));  //申请新的空间
		if(!NewBase)
		{
			exit(OVERFLOW);
		}
		L.elem = NewBase;
		L.size +=LIST_INCREMENT;
		delete NewBase;
		NewBase = NULL;
	}
	ElemType *p;
	ElemType *temp;
	p = &(L.elem[i-1]);  //取得i的位置，即插入位置
	for(temp = &(L.elem[L.length-1]);temp>p;--temp)  //将插入点后的所有元素向后移动一位
	{
		*(temp+1) = *temp;
	}
	*p = e;
	++L.length;
	return OK;
}//ListInsert

int ListDelete(SqList &L, int i, ElemType &e)
{
	if (i<0 || i>ListLength(L)) {
		return ERROR;
	}
	ElemType *p;
	p = &(L.elem[i-1]);
	e = *p;
	ElemType *pos;
	pos =&(L.elem[L.length-1]);
	for(++p; p<=pos; ++p){
		*(p-1) = *p;
	}
	--L.length;
	return OK;
}
#endif //SQLIST_H
int main(){
	SqList list;
	ElemType e;
	InitList(list);
	ListInsert(list, 1, 1);
	ListInsert(list, 2, 2);
	ListInsert(list, 3, 3);
	ListInsert(list, 4, 4);
	ListInsert(list, 5, 5);
	for(int i =0; i<list.length; i++){
		printf("%d\n",list.elem[i]);
	}
	ListDelete(list, 2, e);
	for(i =0; i<list.length; i++){
		printf("%d\n",list.elem[i]);
	}	printf("%d\n", list.length);
	
	return 1;
}
