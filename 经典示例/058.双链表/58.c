#include <stdio.h>
#define N 10
typedef struct node
{
    char name[20];
    struct node *llink,*rlink;
}stud;/*双链表的结构定义*/

/*双链表的创建*/
stud * creat(int n)
{
    stud *p,*h,*s;
    int i;
    if((h=(stud *)malloc(sizeof(stud)))==NULL)
    {
        printf("cannot find space!\n");
        exit(0);
    }
    h->name[0]='\0';
    h->llink=NULL;
    h->rlink=NULL;
    p=h;
    for(i=0;i<n;i++)
    {
        if((s= (stud *) malloc(sizeof(stud)))==NULL)
        {
            printf("cannot find space!\n");
            exit(0);
        }
        p->rlink=s;
	printf("Please input the %d man's name: ",i+1);
        scanf("%s",s->name);
        s->llink=p;
        s->rlink=NULL;
        p=s;
    }
    h->llink=s;
    p->rlink=h;
    return(h);
}

/*查找*/
stud * search(stud *h,char *x)
{
    stud *p;
    char *y;
    p=h->rlink;
    while(p!=h)
    {
        y=p->name;
        if(strcmp(y,x)==0)
            return(p);
        else p=p->rlink;
    }
    printf("cannot find data!\n");
}

/*打印输出*/
void print(stud *h)
{
    stud *p;
    p=h->rlink;
    printf("\nNow the double list is:\n");
    while(p!=h)
    {
        printf("%s ",&*(p->name));
        p=p->rlink;
    }
    printf("\n");
}

/*删除*/
void del(stud *p)
{
    (p->rlink)->llink=p->llink;
    (p->llink)->rlink=p->rlink;
    free (p);
}

/*主函数*/
main()
{
    int number;
    char studname[20];
    stud *head,*searchpoint;
    number=N;
    clrscr();
    puts("Please input the size of the list:");
    scanf("%d",&number);
    head=creat(number);
    print(head);
    printf("\nPlease input the name which you want to find:\n");
    scanf("%s",studname);
    searchpoint=search(head,studname);
    printf("the name you want to find is:%s\n",*&searchpoint->name);
    del(searchpoint);
    print(head);
    puts("\n Press any key to quit...");
    getch();
}
