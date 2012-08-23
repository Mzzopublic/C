#include <stdio.h>
#define MAX  1000
#define MAXSYMBS 30
#define MAXNODE 59

typedef struct
{
  int weight;
  int flag;
  int parent;
  int lchild;
  int rchild;
}huffnode;

typedef struct
{
  int bits[MAXSYMBS];
  int start;
}huffcode;

main()
{
	huffnode huff_node[MAXNODE];
    huffcode huff_code[MAXSYMBS],cd;
    int i,j,m1,m2,x1,x2,n,c,p;
/*    char symbs[MAXSYMBS],symb; */
    /*数组huff_node初始化*/
    clrscr();
	printf("please input the leaf num of tree:\n");
    scanf("%d",&n);
    for(i=0;i<2*n-1;i++)
	{
		huff_node[i].weight=0;
        huff_node[i].parent=0;
		huff_node[i].flag=0;
		huff_node[i].lchild=-1;
        huff_node[i].rchild=-1;
	}

	printf("please input the weight of every leaf\n");
    for(i=0;i<n;i++)
        scanf("%d",&huff_node[i].weight);
/*构造哈夫曼树*/
    for(i=0;i<n-1;i++)
	{
        m1=m2=MAX;
        x1=x2=0;
        for(j=0;j<n+i;j++)
		{
            if (huff_node[j].weight<m1&&huff_node[j].flag==0)
			{
			    m2=m1;
                x2=x1;
                m1=huff_node[j].weight;
                x1=j;
			}
            else if (huff_node[j].weight<m2&&huff_node[j].flag==0)
			{
			    m2=huff_node[j].weight;
                x2=j;
			}
		}
 
        huff_node[x1].parent=n+i;    /*将找出的两棵子树合并为一棵子树*/
        huff_node[x2].parent=n+i;
        huff_node[x1].flag=1;
        huff_node[x2].flag=1;
        huff_node[n+i].weight=huff_node[x1].weight+huff_node[x2].weight;
        huff_node[n+i].lchild=x1;
        huff_node[n+i].rchild=x2;
	}
/*求字符的哈夫曼编码*/

    for(i=0;i<n;i++)
	{
		cd.start=n;
        c=i;
        p=huff_node[c].parent;
        while(p!=0)
		{
			if(huff_node[p].lchild==c)
            
				cd.bits[cd.start]=0;
            else
				cd.bits[cd.start]=1;
            cd.start=cd.start-1;
            c=p;
            p=huff_node[p].parent;
		}
		cd.start++;
        for(j=cd.start;j<=n;j++)
			huff_code[i].bits[j]=cd.bits[j];
        huff_code[i].start=cd.start;
	}
        /*输出字符的哈夫曼编码*/
    puts("The Hafman code are:");    
    for(i=0;i<n;i++)
	{
		for(j=huff_code[i].start;j<=n;j++)
			printf("%10d",huff_code[i].bits[j]);
        printf("\n");                                                                                                                                                                                                                                                                                                                                                                                                                                      
		
	}
	puts("Press any key to quit...");
	getch();

}