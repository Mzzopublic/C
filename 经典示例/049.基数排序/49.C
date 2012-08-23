#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#define MAX 5
typedef struct node
{ int k;
  struct node *next;
} *lnode;
lnode my_input(int *d)
{ lnode head,temp,terminal;
 char s[MAX+1];
  printf("Input the records ('0' to end input):\n");
  scanf("%s",s);
  head=NULL;
  *d=0;
  terminal=NULL;
  while(s[0]!='0')
   {
   temp=(lnode)malloc(sizeof(struct node));
    if (strlen(s)>*d)
    *d=strlen(s);
    temp->k=atoi(s);
     if (head==NULL)
      { head=temp;
      terminal=temp;
      }
      else
      {
       terminal->next=temp;
        terminal=temp;
        }
         scanf("%s",s);
         }
	  terminal->next=NULL;

    return head;
}
void my_output(lnode h)
{
	lnode t=h;
	printf("\n");
	while (h!=NULL)
	{
		printf("%d ",h->k);
		h=h->next;
	}
	h=t;
	/* getch(); */
}
lnode Radix_Sort(lnode head,int d)
{
lnode p,q,h,t;
int i,j,x,radix=1;
 h=(lnode)malloc(10*sizeof(struct node));
  t=(lnode)malloc(10*sizeof(struct node));
   for (i=d;i>=1;i--)
   {
   for (j=0;j<=9;j++)
   { 	h[j].next=NULL;
   	t[j].next=NULL;
   }
   p=head;
   while (p!=NULL)
   {
   	x=((p->k)/radix)%10;
   	if (h[x].next==NULL)
   	{
   		h[x].next=p;
   		t[x].next=p;
   	}
   	else
   	{
   		q=t[x].next;
   		q->next=p;
   		t[x].next=p;
   	}
   	p=p->next;
   }

   j=0;
   while (h[j].next==NULL)
    j++;
    head=h[j].next;
    q=t[j].next;
    for (x=j+1;x<=9;x++)
    if (h[x].next!=NULL)
    {
    	q->next=h[x].next;
    	q=t[x].next;
    }
    q->next=NULL;
    radix*=10;
    printf("\n---------------------\n");
    }
    return head;
}
void my_free(lnode h)
{
	lnode temp=h;
	while (temp)
	{
		h=temp->next;
		free(temp);
		temp=h;
	}
}
void main()
{
	lnode h;
	int d;
	clrscr();
	h=my_input(&d);
	puts("The sequence you input is:");
	my_output(h);
	h=Radix_Sort(h,d);
	puts("\nThe sequence after radix_sort is:");
	my_output(h);
	my_free(h);
	puts("\n Press any key to quit...");
	getch();
}

