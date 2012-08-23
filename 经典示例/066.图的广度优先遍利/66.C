/*//////////////////////////////////////////*/
/*    图形的广度优先搜寻法                 */
/* ///////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>
#define MAXQUEUE 10               /* 队列的最大容量       */

struct node                       /* 图的顶点结构定义     */
{
   int vertex;
   struct node *nextnode;
};
typedef struct node *graph;       /*  图的结构指针        */
struct node head[9];              /* 图的顶点数组         */
int visited[9];                   /* 遍历标记数组         */

int queue[MAXQUEUE];              /* 定义序列数组         */
int front = -1;                   /* 序列前端            */
int rear = -1;                    /* 序列后端            */


/***********************二维数组向邻接表的转化****************************/
void creategraph(int node[20][2],int num)
{
   graph newnode;                 /*  顶点指针           */
   graph ptr;
   int from;                      /* 边起点             */
   int to;                        /* 边终点             */
   int i;

   for ( i = 0; i < num; i++ )    /* 第i条边的信息处理    */
   {
      from = node[i][0];           /* 边的起点           */
      to = node[i][1];           /* 边的终点             */

	  /*              建立新顶点                         */
      newnode = ( graph ) malloc(sizeof(struct node));
      newnode->vertex = to;       /*    顶点内容         */
      newnode->nextnode = NULL;   /* 设定指针初值         */
      ptr = &(head[from]);        /* 顶点位置             */
      while ( ptr->nextnode != NULL ) /* 遍历至链表尾     */
         ptr = ptr->nextnode;         /* 下一个顶点       */
      ptr->nextnode = newnode;        /* 插入第i个节点的链表尾部 */
   }
}


/************************ 数值入队列************************************/
int enqueue(int value)
{
   if ( rear >= MAXQUEUE )        /* 检查伫列是否全满     */
      return -1;                  /* 无法存入             */
   rear++;                        /* 后端指标往前移       */
   queue[rear] = value;           /* 存入伫列             */
}


/************************* 数值出队列*********************************/
int dequeue()
{
   if ( front  == rear )          /* 队列是否为空         */
      return -1;                  /* 为空，无法取出       */
   front++;                       /* 前端指标往前移       */
   return queue[front];           /* 从队列中取出信息     */
}


/***********************  图形的广度优先遍历************************/
void bfs(int current)
{
   graph ptr;

   /* 处理第一个顶点 */
   enqueue(current);              /* 将顶点存入队列       */
   visited[current] = 1;          /* 已遍历过记录标志置疑1*/
   printf(" Vertex[%d]\n",current);   /* 打印输出遍历顶点值 */
   while ( front != rear )        /* 队列是否为空         */
   {
      current = dequeue();        /* 将顶点从队列列取出   */
      ptr = head[current].nextnode;   /* 顶点位置         */
      while ( ptr != NULL )           /* 遍历至链表尾     */
      {
         if ( visited[ptr->vertex] == 0 ) /*顶点没有遍历过*/
         {
            enqueue(ptr->vertex);     /* 奖定点放入队列   */
            visited[ptr->vertex] = 1; /* 置遍历标记为1    */

	    printf(" Vertex[%d]\n",ptr->vertex);/* 印出遍历顶点值 */
         }
         ptr = ptr->nextnode;     /* 下一个顶点           */
      }
   }
}


/***********************  主程序  ************************************/
/*********************************************************************/
void main()
{
   graph ptr;
   int node[20][2] = { {1, 2}, {2, 1},  /* 边信息数组       */
                       {6, 3}, {3, 6},
                       {2, 4}, {4, 2},
                       {1, 5}, {5, 1},
                       {3, 7}, {7, 3},
                       {1, 7}, {7, 1},
                       {4, 8}, {8, 4},
                       {5, 8}, {8, 5},
                       {2, 8}, {8, 2},
                       {7, 8}, {8, 7} };
   int i;
   clrscr();
   puts("This is an example of Width Preferred Traverse of Gragh.\n");
   for ( i = 1; i <= 8; i++ )        /*顶点结构数组初始化*/
   {
      head[i].vertex = i;
      head[i].nextnode = NULL;
      visited[i] = 0;
   }
   creategraph(node,20);       /* 图信息转换，邻接表的建立 */
   printf("The content of the graph's allist is:\n");
   for ( i = 1; i <= 8; i++ )
   {
      printf(" vertex%d =>",head[i].vertex); /* 顶点值       */
      ptr = head[i].nextnode;             /* 顶点位置     */
      while ( ptr != NULL )       /* 遍历至链表尾         */
      {
         printf(" %d ",ptr->vertex);  /* 打印输出顶点内容     */
         ptr = ptr->nextnode;         /* 下一个顶点       */
      }
      printf("\n");               /* 换行                 */
   }
   printf("The contents of BFS are:\n");
   bfs(1);                        /* 打印输出遍历过程         */
   printf("\n");                  /* 换行                 */
   puts(" Press any key to quit...");
   getch();
}

