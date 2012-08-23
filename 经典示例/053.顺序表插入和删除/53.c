
#define ListSize 100/* 假定表空间大小为100 */
#include <stdio.h>
#include <stdlib.h>
void Error(char * message)
{
printf("错误:%s\n",message);
exit(1);
}/* 从0开始计， 表空间大小应为101了 */
struct Seqlist{
int  data[ListSize];/* 向量data用于存放表结点 */
int length; /*  当前的表长度 */
};
/* 以上为定义表结构 */

/* ------------以下为两个主要算法---------- */
void InsertList(struct Seqlist *L, int x, int i)
{
/* 将新结点x插入L所指的顺序表的第i个结点ai的位置上 */
int j;
if ( i < 0 || i > L -> length )
Error("position error");/* 非法位置，退出 */
if ( L->length>=ListSize )
Error("overflow");
 for ( j=L->length-1 ; j >= i ; j --)
L->data[j+1]=L->data [j];
L->data[i]=x ;
L->length++ ;
}

void DeleteList ( struct Seqlist *L, int i )
{/* 从L所指的顺序表中删除第i个结点ai */
int j;
 if ( i< 0 || i > L-> length-1)
Error( " position error" ) ;
 for ( j = i+1 ; j < L-> length ; j++ )
    L->data [ j-1 ]=L->data [ j]; /* 结点前移 */
L-> length-- ; /* 表长减小 */
}
/* ===========以下为验证算法而加======= */
void Initlist(struct Seqlist *L)
{
	L->length=0;
}
void main()
{
	 struct Seqlist *SEQA;
	 int i;
	 SEQA = (struct Seqlist *)malloc(sizeof(struct Seqlist));
	 Initlist(SEQA);

	 for (i=0;i<ListSize;i++)
	 {	
		 InsertList (SEQA,i,i);
		 printf("%d\n",SEQA->data[i]);
	 }
		 DeleteList (SEQA,99);
	 for (i=0;i<ListSize-1;i++)
	 {
	printf("%d\n",SEQA->data[i]);
	}
}
