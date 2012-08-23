#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define n 4
#define m 3
#define w 2

struct achievement  //定义表示成绩的结构体
{int schoolnumber;//学校编号
char name[20];	//姓名
int mark;	//分数
int result;};

struct pro     //表示项目的结构体
{int tag;//项目编号
struct achievement ach[5];
int number;};

struct Node         
{struct pro date;
struct Node *next;};


void ListInitiate(struct Node * *head)    //初始化单链表
{
    if((*head=(struct Node*)malloc(sizeof(struct Node)))==NULL) exit(1);
    (*head)->next=NULL;
}


void main()
{
    int i,j,t;
    int x[n]={0};int y[n]={0};
    struct Node *head;
    struct Node *p;
    struct Node *q;
    ListInitiate(&head);
    p=head;
	for(i=0;i<m+w;i++)      //输入成绩
    {
        j=i+1;
        printf("请输入第%d个项目的信息\n",j);
        p->date.number=j;
        printf("所取的名次数为:");
        scanf("%d",&p->date.tag);
        while(p->date.tag!=3&&p->date.tag!=5)
        {printf("输入有误，请重新输入!");
		printf("所取的名次数为:");
		scanf("%d",&p->date.tag);}
        t=1;
        while(t<=p->date.tag)
        {
            printf("第%d名的名字:",t);
            scanf("%s",&p->date.ach[t-1].name);
            printf("第%d名的学校:",t);
            scanf("%d",&p->date.ach[t-1].schoolnumber);
            printf("第%d名的分数:",t);
            scanf("%d",&p->date.ach[t-1].mark);
            p->date.ach[t-1].result=t;
            t++;
        }
        q=(struct Node*)malloc(sizeof(struct Node));  //生成新结点
        p->next=q;
        p=q;
        p->next=NULL;
    }
	for(i=0;i<n;i++)     //产生成绩单
	{
		j=i+1;
		printf("学校%d成绩单\n",j);
		//t=1;
		p=head;
		do
		{
			t=1;
			while(t<=p->date.tag)
			{ 
				if(p->date.ach[t-1].schoolnumber==j)
				{            
					printf("获奖项目:%d   ",p->date.number);
					printf("名次:%d   \n",p->date.ach[t-1].result);
					printf("获奖人姓名:%s   ",p->date.ach[t-1].name);
					printf("所得分数:%d   ",p->date.ach[t-1].mark);
					if(p->date.number<=m)
						x[i]=x[i]+p->date.ach[t-1].mark;
					else
						y[i]=y[i]+p->date.ach[t-1].mark;
				}    
				t++;
			}
			p=p->next;
		}while(p!=NULL);
		printf("\n男子团体总分:%d   ",x[i]);
		printf("女子团体总分:%d   \n",y[i]);
		printf("\n团体总分:%d\n",x[i]+y[i]);
	}
}
