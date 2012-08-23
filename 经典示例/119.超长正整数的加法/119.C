#include<stdio.h>
#include<stdlib.h>
#define HUNTHOU 10000
typedef struct node{ int data;
                     struct node *next;
                  }NODE;                 /*定义链表结构*/

NODE *insert_after(NODE *u,int num);     /*在u结点后插入一个新的NODE，其值为num*/
NODE *addint(NODE *p,NODE *q);           /*完成加法操作返回指向*p+*q结果的指针*/
void printint(NODE *s);
NODE *inputint(void);

void main()
{
   NODE *s1,*s2,*s;
   NODE *inputint(), *addint(), *insert_after();
   clrscr();
   puts("*********************************************************");
   puts("*              This program is to calculate             *");
   puts("*       the addition of king sized positive integer.    *");
   puts("*********************************************************");
   printf(" >> Input S1= ");
   s1=inputint();            /*输入被加数*/
   printf(" >> Input S2= ");
   s2=inputint();            /*输入加数*/
   printf(" >> The addition result is as follows.\n\n");
   printf("    S1= "); printint(s1); putchar('\n');     /*显示被加数*/
   printf("    S2= "); printint(s2); putchar('\n');     /*显示加数*/
   s=addint(s1,s2);                                 /*求和*/
   printf(" S1+S2="); printint(s); putchar('\n');    /*输出结果*/
   printf("\n\n Press any key to quit...");
   getch();
}
NODE *insert_after(NODE *u,int num)
{
   NODE *v;
   v=(NODE *)malloc(sizeof(NODE));      /*申请一个NODE*/
   v->data=num;                         /*赋值*/
   u->next=v;                           /*在u结点后插入一个NODE*/
   return v;
}
NODE *addint(NODE *p,NODE *q)         /*完成加法操作返回指向*p+*q结果的指针*/
{
   NODE *pp,*qq,*r,*s,*t;
   int total,number,carry;
   pp=p->next; qq=q->next;
   s=(NODE *)malloc(sizeof(NODE));     /*建立存放和的链表表头*/
   s->data=-1;
   t=s; carry=0;                      /*carry:进位*/
   while(pp->data!=-1&&qq->data!=-1)    /*均不是表头*/
   {
      total=pp->data+qq->data+carry;     /*对应位与前次的进位求和*/
      number=total%HUNTHOU;             /*求出存入链中部分的数值 */
      carry=total/HUNTHOU;              /*算出进位*/
      t=insert_after(t,number);         /*将部分和存入s向的链中*/
      pp=pp->next;                         /*分别取后面的加数*/
      qq=qq->next;
   }
   r=(pp->data!=-1)?pp:qq;          /*取尚未自理完毕的链指针*/
   while(r->data!=-1)              /*处理加数中较大的数*/
   {
      total=r->data+carry;         /*与进位相加*/
      number=total%HUNTHOU;        /*求出存入链中部分的数值*/
      carry=total/HUNTHOU;         /*算出进位*/
      t=insert_after(t,number);     /*将部分和存入s指向的链中*/
      r=r->next;                   /*取后面的值*/
   }
   if(carry) t=insert_after(t,1);     /*处理最后一次进位*/
   t->next=s;                   /*完成和的链表*/
   return s;                     /*返回指向和的结构指针*/
}
NODE *inputint(void)     /*输入超长正整数*/
{
   NODE *s,*ps,*qs;
   struct number {int num;
                  struct number *np;
               }*p,*q;
   int i,j,k;
   long sum;
   char c;
   p=NULL;     /*指向输入的整数，链道为整数的最低的个位，链尾为整数的最高位*/
   while((c=getchar())!='\n')     /*输入整数，按字符接收数字*/
      if(c>='0'&&c<='9')           /*若为数字则存入*/
      {
         q=(struct number *)malloc(sizeof(struct number));    /*申请空间*/
         q->num=c-'0';           /*存入一位整数*/
         q->np=p;                /*建立指针*/
         p=q;
      }
   s=(NODE *)malloc(sizeof(NODE));
   s->data=-1;                  /*建立表求超长正整数的链头*/
   ps=s;
   while(p!=NULL)        /*将接收的临时数据链中的数据转换为所要求的标准形式*/
   {
      sum=0;i=0;k=1;
      while(i<4&&p!=NULL)          /*取出低四位*/
      {
         sum=sum+k*(p->num);   
         i++; p=p->np; k=k*10;
      }
      qs=(NODE *)malloc(sizeof(NODE));          /*申请空间*/
      qs->data=sum;                     /*赋值，建立链表*/
      ps->next=qs;
      ps=qs;
   }
   ps->next=s;
   return s;
}
void printint(NODE *s)
{
   if(s->next->data!=-1)         /*若不是表头，则输出*/
   {
      printint(s->next);             /*递归输出*/
      if(s->next->next->data==-1)
         printf("%d",s->next->data);
      else{
         int i,k=HUNTHOU;
         for(i=1;i<=4;i++,k/=10)
            putchar('0'+s->next->data%(k)/(k/10));
      }
   }
}
