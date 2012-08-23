/*/////////////////////////////////////////////////////////////*/
/*                           图的深度优先遍历                  */
/*/////////////////////////////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>

struct node                       /* 图顶点结构定义     */
{
   int vertex;                    /* 顶点数据信息       */
   struct node *nextnode;         /* 指下一顶点的指标   */
};
typedef struct node *graph;       /* 图形的结构新型态   */
struct node head[9];              /* 图形顶点数组       */
int visited[9];                   /* 遍历标记数组       */



/********************根据已有的信息建立邻接表********************/
void creategraph(int node[20][2],int num)/*num指的是图的边数*/
{
   graph newnode;                 /*指向新节点的指针定义*/
   graph ptr;
   int from;                      /* 边的起点          */
   int to;                        /* 边的终点          */
   int i;

   for ( i = 0; i < num; i++ )    /* 读取边线信息，插入邻接表*/
   {
      from = node[i][0];         /*    边线的起点            */
      to = node[i][1];           /*   边线的终点             */
      
	  /* 建立新顶点 */
      newnode = ( graph ) malloc(sizeof(struct node));
      newnode->vertex = to;        /* 建立顶点内容       */
      newnode->nextnode = NULL;    /* 设定指标初值       */
      ptr = &(head[from]);         /* 顶点位置           */
      while ( ptr->nextnode != NULL ) /* 遍历至链表尾   */
         ptr = ptr->nextnode;     /* 下一个顶点         */
      ptr->nextnode = newnode;    /* 插入节点        */
   }
}


/**********************  图的深度优先搜寻法********************/

void dfs(int current)
{
   graph ptr;

   visited[current] = 1;          /* 记录已遍历过       */
   printf("vertex[%d]\n",current);   /* 输出遍历顶点值     */
   ptr = head[current].nextnode;  /* 顶点位置           */
   while ( ptr != NULL )          /* 遍历至链表尾       */
   {
      if ( visited[ptr->vertex] == 0 )  /* 如过没遍历过 */
         dfs(ptr->vertex);              /* 递回遍历呼叫 */
      ptr = ptr->nextnode;              /* 下一个顶点   */
   }
}


/****************************** 主程序******************************/

void main()
{
   graph ptr;
   int node[20][2] = { {1, 2}, {2, 1},  /* 边线数组     */
                       {1, 3}, {3, 1},
                       {1, 4}, {4, 1},
                       {2, 5}, {5, 2},
                       {2, 6}, {6, 2},
                       {3, 7}, {7, 3},
                       {4, 7}, {4, 4},
                       {5, 8}, {8, 5},
                       {6, 7}, {7, 6},
                       {7, 8}, {8, 7} };
   int i;
   clrscr();
   for ( i = 1; i <= 8; i++ )      /*   顶点数组初始化  */
   {
      head[i].vertex = i;         /*    设定顶点值      */
      head[i].nextnode = NULL;    /*       指针为空     */
      visited[i] = 0;             /* 设定遍历初始标志   */
   }

   creategraph(node,20);          /*    建立邻接表      */
   printf("Content of the gragh's ADlist is:\n");
   for ( i = 1; i <= 8; i++ )
   {
      printf("vertex%d ->",head[i].vertex); /* 顶点值    */
      ptr = head[i].nextnode;             /* 顶点位置   */
      while ( ptr != NULL )       /* 遍历至链表尾       */
      {
         printf(" %d ",ptr->vertex);  /* 印出顶点内容   */
         ptr = ptr->nextnode;         /* 下一个顶点     */
      }
      printf("\n");               /*   换行             */
   }
   printf("\nThe end of the dfs are:\n");
   dfs(1);                        /* 打印输出遍历过程   */
   printf("\n");                  /* 换行               */
   puts(" Press any key to quit...");
   getch();
}

