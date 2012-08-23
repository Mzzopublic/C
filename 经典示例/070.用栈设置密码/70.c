#include<stdio.h>
#include<string.h>
#include<conio.h>	
#include<stdlib.h>
#define STACK_INIT_SIZE 10
#define OK 1
#define TRUE 1
#define FALSE  0
#define ERROR 0
char PASSWORD[10]="123456"; /*密码，全局变量*/
typedef char SElemType;
typedef struct STACK /*定义栈类型*/
{
	SElemType *base;
	SElemType *top;
	int stacksize;
	int length;
}SqStack,*Stack;
typedef int Status;
void InitStack(Stack *S) /*初始化栈*/
{
	*S=(SqStack *)malloc(sizeof(SqStack));
	(*S)->base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!(*S)->base)exit(-1);
	(*S)->top=(*S)->base;
	(*S)->stacksize=STACK_INIT_SIZE;
	(*S)->length=0;
}
Status DestroyStack(Stack *S) /* 销毁栈*/
{
	free((*S)->base);
	free((*S));
	return OK;
}
void ClearStack(Stack *S)  /*把栈置为空*/
{
	(*S)->top=(*S)->base;
	(*S)->length=0;
}
Status StackEmpty(SqStack S) /*判断栈空否*/
 {
	if(S.top==S.base) return TRUE;
	else
		return FALSE;
}
void Push(Stack *S,SElemType e)  /*把数据压入栈*/
{
	if((*S)->top - (*S)->base>=(*S)->stacksize)
	{
		(*S)->base=(SElemType *) realloc((*S)->base,
			((*S)->stacksize + 2) * sizeof(SElemType));
		if(!(*S)->base)exit(-1);
		(*S)->top=(*S)->base+(*S)->stacksize;
		(*S)->stacksize += 2;
	}
	*((*S)->top++)=e;
	++(*S)->length;
}
Status Pop(Stack *S) /*删除栈顶元素*/
{
	if((*S)->top==(*S)->base) return ERROR;
	(*S)->top--;
	--(*S)->length;
	return OK;
}
Status GetTop(Stack S,SElemType *e)/*返回栈顶元素*/
{
	if(S->top==S->base) return ERROR;
	*e=*(S->top-1);
	S->top--;
}
void Change(SqStack S,char *a) /*将栈中的元素按反序付给 a */
{ int n=S.length-1 ;
while (!StackEmpty(S))
GetTop(&S,&a[n--]);
}
void Control(Stack *s)
{int i=0,k,j=0;
SElemType ch,*a;
k=strlen(PASSWORD);
printf("input password,you have three chances：\n",k);
for(;;)
{ if(i>=3)
{  i++;
clrscr();
gotoxy(1,1); /*定位黑屏光标位置*/
break;
}
else if(i>0&&i<3)
{  gotoxy(5,2);
for(j=1;j<=(*s)->length;j++)printf(" ");
gotoxy(5,2);ClearStack(s);}
for(;;)  /* 密码输入，可退格 */
{ch=getch();  /* 退格 的ASCII 是8 */
if(ch!=13) /* 判断是否为回车，不是则把它付给下面*/
{if(ch==8) {Pop(s);gotoxy(4+j,2);printf(" ");gotoxy(4+j,2);}
else {printf("*");Push(s,ch);}
j=(*s)->length;}
else break;
}
i++;
if(k!=j) continue; 
else  
{ a=(SElemType *)malloc((*s)->length*sizeof(SElemType));
Change(**s,a);
for(j=1;j<=(*s)->length;)
{if(a[j-1]==PASSWORD[j-1]) j++;
else {j=(*s)->length+2;break;}}
if(j==(*s)->length+2) continue;
else break;}}
if(i==4) printf("\n password wrong!");
else printf("\n password right!\n");
free(a);
}
main()
{Stack s;
clrscr();
InitStack(&s);
Control(&s);
getch();
DestroyStack(&s);
}
