#include   <stdio.h> 
#include   <string.h> 
#include   <stdlib.h> 
#include   <conio.h> 
 
/*定义全局变量*/ 
#define   TRUE   1 
#define   FALSE   0 
#define   OK   1 
#define   ERROR   0 
#define   NULL   0 
#define   OVERFLOW   -2 
#define   MAXSIZE   100 
#define   stack_init_size   100 
#define   stackincrement   10 
typedef   char   selemType; 
typedef   char   qelemType; 
typedef   char   elemType; 
typedef   int   status; 
char   e;     
char   demon[MAXSIZE];
     
/* 类型及其基本操作*/   
typedef   struct   
{   
	selemType   *base; 
	selemType   *top; 
	int   stacksize; 
}sqstack;     
status   initstack   (sqstack   *s) 
{     
	s->base=(selemType   *)malloc(stack_init_size*sizeof(selemType));     
	if(!s->base)   exit   (OVERFLOW); 
	s->top=s->base; 
	s->stacksize=stack_init_size; 
	return   OK; 
}/*创建栈*/ 
status   push   (sqstack   *s,selemType   e) 
{ 
	if(s->top-s->base>=s->stacksize) 
	{ 
		s->base=(elemType*)   realloc(s->base,(s->stacksize+stackincrement)*sizeof(elemType))     
		if(!s->base)   exit(OVERFLOW); 
		s->top=s->base+s->stacksize; 
		s->stacksize+=stackincrement; 
	} 
	*(s->top++)=e; 
	return   OK; 
}/*入栈*/ 
status   pop(sqstack   *s,selemType   *e) 
{     
	if(s->top==s->base)   return   ERROR; 
	*e=*(--(s->top)); 
	return   OK; 
}/*出栈*/ 
/*队列类型及其基本操作*/ 
typedef   struct   qnode 
{ 
	qelemType   data; 
	struct   qnode   *next; 
}qnode,*queueptr; 
typedef   struct 
{ 
	queueptr   front; 
	queueptr   rear; 
}linkqueue; 
status   initqueue(linkqueue   *q) 
{ 
	q->front=q->rear=(queueptr)malloc(sizeof(qnode)); 
	if(!q->front)   exit(OVERFLOW); 
	q->front->next=NULL; 
	return   OK; 
}/*创建队列*/ 
status   enqueue(linkqueue   *q,qelemType   e) 
{ 
	queueptr   p; 
	p=(queueptr)malloc(sizeof(qnode)); 
	if(!p)   exit(OVERFLOW); 
	p->data=e; 
	p->next=NULL; 
	q->rear->next=p; 
	q->rear=p; 
	return   OK; 
}/*入队*/ 
status   dequeue(linkqueue   *q,qelemType   *e) 
{ 
	queueptr   p; 
	if(q->front==q->rear)   return   ERROR; 
	p=q->front->next; 
	*e=p->data; 
	q->front->next=p->next; 
	if(q->rear==p) 
	{ 
		q->rear=q->front; 
	} 
	free(p);
	return   OK;
}/*出队*/ 
 /*括号内元素入栈处理函数*/
void   tempstack(sqstack   *temps) 
{ 
	int   i=0; 
	char   t; 
	char   c;
	c=demon[i];
	for(i=0;c!='#';i++)/*遍历数组*/
	{
		c=demon[i];
		if(c=='(')/*遇到开括号*/
		{
			t=demon[i+1];/*取括号中的首字母*/
			push(temps,t);/*入栈*/
			i++;/*指向首字母*/
			do 
			{ 
				i++; 
				c=demon[i]; 
				push(temps,c)/*第一次循环将次字母入栈*/; 
				push(temps,t);/*再将首字母进栈*/ 
			}while(c!=')');/*直到括号中元素全部进栈*/ 
			pop(temps,&t);/*将多余进栈的首字母t出栈*/ 
			pop(temps,&t);   /*将多余进栈的')'出栈*/ 
		} 
	} 
}/*临时栈*/ 

 /*特殊入队函数*/
void   spenqueue(linkqueue   *q,char   key)     
{     
	int   j=0;
	char   a[5]; 
	switch(key)   /*判断大写字母对应的字符串*/ 
	{ 
	case'A':strcpy(a,"ase");break; 
	case'B':strcpy(a,"tAdA");break;
	case'C':strcpy(a,"abc");break; 
	case'D':strcpy(a,"def");break; 
	case'E':strcpy(a,"ghi");break; 
	case'F':strcpy(a,"klm");break; 
	case'H':strcpy(a,"mop");break; 
	default:strcpy(a,"???");   /*不能翻译的魔王语言以"???"输出*/ 
	} 
	while(a[j]!='\0')   /*如果数组还有字母*/ 
	{ 
		enqueue(q,a[j]);/*进队*/ 
		j++; 
	} 
}/*特殊入队*/ 
/*排序入队处理函数*/ 
status   sort(sqstack   *s,linkqueue   *q) 
{     
	qnode   b; 
	int   flag=0;/*大写字母监视哨置零*/ 
	int   i; 
	for(i=0;demon[   i]!='#';i++)/*遍历数组*/ 
	{ 
		b.data=demon[   i]; 
		if(   ('a'<=b.data&&b.data<='z')||b.data=='?')   /*如果是小写字母或者'?'　则直接进栈*/     
		{ 
			enqueue(q,b.data); 
		} 
		else 
		{ 
			if('A'<=b.data&&b.data<='Z')   /*如果是大写字母,则调用特殊进栈函数,*/     
			{ 
				spenqueue(q,b.data); 
				flag=1;   /*发现大写字母监视哨置1*/ 
			} 
			else 
			{ 
				if(b.data=='(')/*如果是括号*/ 
				{
					do 
					{ 
						pop(s,&e); 
						enqueue(q,e); 
					}while(!(s->top==s->base));   /*只要栈不为空,则出栈进队*/     
					while   (b.data!=')')   /*只要还指向括号内元素,就继续往后移,保证原括号内的元素不再进栈*/ 
					{ 
						i++; 
						b.data=demon[i]; 
					} 
				} 
			} 
		} 
	}
	return   flag; 
}/*排序*/
/*主函数*/
status   main() 
{ 
	sqstack   s1; 
	linkqueue   q1; 
	int   k=0; 
	int   flag=1; 
	clrscr(); 
	printf("Please   Input   the   Demon's   Words:\n"); 
	printf("!:   Less   Than   30   Letters:   )\n"); 
	printf("!:   End   with   '#':   )\n\t"); 
	scanf("%s",demon);
	printf("\n***************************************"); 
	initstack(&s1);   /*创建栈*/ 
	initqueue(&q1);   /*创建队*/ 
	tempstack(&s1);   /*调用函数*/ 
	while   (flag==1)   /*如果有大写字母*/ 
	{ 
		k=0; 
		flag=sort(&s1,&q1); 
		while(q1.front!=q1.rear)   /*重写demon[i   ]*/ 
		{ 
			dequeue(&q1,&e); 
			demon[k]=e;
			k++;
		}
		demon[k]='#';
	}
	demon[k]='\0';
	printf("\n***************************************");
	printf("\nThe   Human   Words:\n\t%s",demon);
	printf("\n***************************************");
}
