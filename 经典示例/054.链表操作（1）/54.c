#include <stdio.h>
#include <conio.h>
#define N 10

typedef struct node
{
    char name[20];
    struct node *link;
}stud;

stud * creat()
{
    stud *p,*h,*s;
    int i,n;
    puts("\nPlease input the number of linklist:");
    scanf("%d",&n);
    if((h=(stud *)malloc(sizeof(stud)))==NULL)
    {
        printf("cannot find space!");
        exit(0);
    }
    h->name[0]='\0';
    h->link=NULL;
    p=h;
    for(i=0;i<n;i++)
    {
        if((s= (stud *) malloc(sizeof(stud)))==NULL)
        {
            printf("cannot find space!");
            exit(0);
        }
        p->link=s;
    printf("please input %d student's name: ",i+1);
        scanf("%s",s->name);
        s->link=NULL;
        p=s;
    }

    return(h);
}

stud * search(stud *h,char *x)
{
    stud *p;
    char *y;
    p=h->link;
    while(p!=NULL)
    {
        y=p->name;
        if(strcmp(y,x)==0)
        return(p);
        else p=p->link;
    }
    if(p==NULL)
    printf("data not find!");
	return 0;
}

stud * search2(stud *h,char *x)
{
    stud *p,*s;
    char *y;
    p=h->link;
    s=h;
    while(p!=NULL)
    {
        y=p->name;
        if(strcmp(y,x)==0)
            return(s);
        else
        {
            p=p->link;
            s=s->link;
        }
    }
    if(p==NULL)
    printf("data not find!");
	return 0;
}

void insert(stud *p)
{
    char stuname[20];
    stud *s;
    if((s= (stud *) malloc(sizeof(stud)))==NULL)
    {
        printf("cannot find space!");
        exit(0);
    }
printf("\nplease input the student's name: ");
    scanf("%s",stuname);
    strcpy(s->name,stuname);
    s->link=p->link;
    p->link=s;
}

void del(stud *x,stud *y)
{
    stud *s;
    s=y;
x->link=y->link;
    free(s);
}

void print(stud *h)
{
    stud *p;
    p=h->link;
printf("Now the link list is:\n");
    while(p!=NULL)
    {
        printf("%s ",&*(p->name));
        p=p->link;
}
printf("\n");
}

void quit()
{
clrscr();
puts("\n Thank you for your using!\n Press any key to quit...");
getch();
exit(0);
}

void menu(void)
{
    clrscr();
printf("       simple linklise realization of c\n");
printf("    ||=====================================||\n");
printf("    ||                                     ||\n");
printf("    || [1]  create linklist                ||\n");
printf("    || [2]  seach                          ||\n");
printf("    || [3]  insert                         ||\n");
printf("    || [4]  delete                         ||\n");
printf("    || [5]  print                          ||\n");
printf("    || [6]  exit                           ||\n");
printf("    ||                                     ||\n");
printf("    || if no list exist,create first       ||\n");
printf("    ||                                     ||\n");
printf("    ||=====================================||\n");
printf("      Please input your choose(1-6): ");
}

main()
{
    int choose;
    stud *head,*searchpoint,*forepoint;
    char fullname[20];


    while(1)
    {
        menu();
        scanf("%d",&choose);
        switch(choose)
        {
            case 1:
            	clrscr();
            	head=creat();
            	puts("Linklist created successfully! \nPress any key to return...");
            	getch();
            break;
            case 2:
            	clrscr();
                printf("Input the student's name which you want to find:\n");
                scanf("%s",fullname);
                searchpoint=search(head,fullname);
                printf("The stud name you want to find is:%s",*&searchpoint->name);
                printf("\nPress any key to returen...");
	    		getchar();
	    		getchar();
                break;
            case 3:
                clrscr();
                insert(head);
                print(head);
                printf("\nPress any key to returen...");
                getchar();getchar();
                break;
            case 4:
                clrscr();
                print(head);
                printf("\nInput the student's name which you want to delete:\n");
                scanf("%s",fullname);
                searchpoint=search(head,fullname);
                forepoint=search2(head,fullname);
                del(forepoint,searchpoint);
                print(head);
	    puts("\nDelete successfully! Press any key to return...");
	    getchar();
	    getchar();
                break;
            case 5:print(head);
                printf("\nPress any key to return...");
                getchar();getchar();
                break;
            case 6:quit();
                break;
            default:
                clrscr();
                printf("Illegal letter! Press any key to return...");
                menu();
                getchar();
        }
    }
}

