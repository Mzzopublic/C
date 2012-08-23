#include<stdio.h>
#include<stdlib.h>
#define NULL 0 /*宏定义*/
typedef struct node /*定义结点类型的数据结构*/
{
	char c; /*数据域，类型为字符型*/
	struct node *next; /*指针域，类型为本结构体类型*/
}Node,*L; /*类型重定义，即Node和*L和struct node等价*/

main()
{
	L l,p,q,r; /*用指针类型定义三个结点类型的指针*/
	char ch;
	l=(L)malloc(sizeof(Node)); /*分配内存空间*/
	l->c='\0'; /*为头结点的数据域赋值，值为空*/
	l->next=NULL; /*指明下一个结点目前不存在*/
	q=l; /*q为游动指针，链表结点的连结要用*/
	printf("Input a character:\n");
	scanf("%c",&ch);
	getchar();
	while(ch!='0') /*输入0表示输入结束*/
	{
		p=(L)malloc(sizeof(Node)); /*为新输入的数据分配内存空间*/
		p->c=ch;
		p->next=NULL; /*新输入的结点在链表的最后，即它的后面没有其它元素*/
		q->next=p; /*q用于将上一个元素链接至当前新元素*/
		q=p; /*q自己移到当前最后一个元素，以备继续链接所用*/
		scanf("%c",&ch);
		getchar();
	}	
	/*以上完成了单链表的创建*/
	q=l->next;
	p=q->next;
	r=p->next;
	q->next=NULL;
	while(r!=NULL)
	{
		p->next=q;
		q=p;
		p=r;
		if(r->next!=NULL) /*r后面还有结点，则逆置继续*/
			r=r->next;
		else
			break;
	}
	r->next=q;
	l->next=r; //头结点指向最后一个结点
	
	q=l; /*输入整个链表前，先将q移到链表头，l一般不动*/
	while(q->next!=NULL) /*若q所指向的元素后面还有其它元素，则将该元素的数据输出*/
	{
		printf("%c-->",q->next->c); /*q->next->c表示q所指向的下一个元素的数据*/
		q=q->next; /*完成该元素的输出后，q移至下一个元素重复输出操作*/
	}
	printf("\n");
}
