#include <stdio.h>
#include <malloc.h>
#define  N 7                                         //定义N=7，表示有7个链表单元
#define  OVERFLOW  0
#define  OK  1
typedef struct LNode{                              //定义链表结构
	int password;
	int order;
	struct LNode *next;
}LNode,*LinkList;

int  PassW[N]={3,1,7,2,4,8,4};
void  Joseph(LinkList p,int m,int x);          //声明函数
int main()
{    
	int  i,m;
	LinkList  Lhead,p,q;     //定义三个指向链表结构的指针
	Lhead=(LinkList)malloc(sizeof(LNode));  //初始化头节点
	if(!Lhead)return OVERFLOW;                   //分配空间失败返回
	Lhead->password=PassW[0];
	Lhead->order=1;
	Lhead->next=NULL;
	p=Lhead;
	for(i=1;i<7;i++){
		if(!(q=(LinkList)malloc(sizeof(LNode))))return OVERFLOW;
		q->password=PassW[i];                      //初始化循环列表中的密码值
		q->order=i+1;
		p->next=q;p=q;                 //新创建一个指针节点并使p->next指向它,再使p=q
	}
	p->next=Lhead;                        //使p->next指向头节点,从而形成循环链表
	printf("请输入第一次计数值m:  \n");
	scanf("%d",&m);                                      //用户输入第一次计数值m
	printf("第一次计数值m= %d \n",m);
	Joseph(p,m,N);
    return OK;
}
void  Joseph(LinkList p,int m,int x){
	LinkList q;
	int i;
	if(x==0)return;                          //链表中没有节点的话,立即返回上一层函数
	q=p;
	m%=x;                                  //m对x求余,从而求出链表中的第几个单元是所求节点
	if(m==0)m=x;               //若m刚好可以整除x,则令m=x,因为如果m=0,则不进行下一个
	//for循环,那样就无法使q指向要删除节点,p指向他的的前一节点,那样则无法进行删除操作  
	for(i=1;i<=m;i++){
		p=q;
		q=p->next;                    //使q指向要删除的节点,p指向q的前一个节点
	}
	p->next=q->next;                              //从循环链表中删除q指向的节点
	i=q->password;
	printf("%d  ",q->order);                     
	free(q);                                        //释放q指向的空间
	Joseph(p,i,x-1);
}
