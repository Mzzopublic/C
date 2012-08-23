//////////////////////////////////////////////////////////////////////////
/*                       骑士巡游问题                                   */
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
int f[11][11] ;                                /*定义一个矩阵来模拟棋盘*/
int adjm[121][121];/*标志矩阵，即对于上述棋盘，依次进行编号
		    1--121(行优先）可以从一个棋盘格i跳到棋盘格j时，adjm[i][j]=1*/

void creatadjm(void);                            /*创建标志矩阵函数声明*/
void mark(int,int,int,int);                     /*将标志矩阵相应位置置1*/
void travel(int,int);                                    /*巡游函数声明*/
int n,m;                                 /*定义矩阵大小及标志矩阵的大小*/


/******************************主函数***********************************/
int main()
{
    int i,j,k,l;
    printf("Please input size of the chessboard: ");  /*输入矩阵的大小值*/
	scanf("%d",&n);
    m=n*n;
    creatadjm();                                         /*创建标志矩阵*/
	puts("The sign matrix is:");
    for(i=1;i<=m;i++)                                /*打印输出标志矩阵*/
    {
        for(j=1;j<=m;j++) 
			printf("%2d",adjm[i][j]);
        printf("\n");
    }
    
    printf("Please input the knight's position (i,j): "); /*输入骑士的初始位置*/
    scanf("%d %d",&i,&j);
    l=(i-1)*n+j;                   /*骑士当前位置对应的标志矩阵的横坐标*/
    while ((i>0)||(j>0))                             /*对骑士位置的判断*/
    {
        for(i=1;i<=n;i++)                              /*棋盘矩阵初始化*/
            for(j=1;j<=n;j++)
                f[i][j]=0;
        k=0;                                             /*所跳步数计数*/
        travel(l,k);                                /*从i,j出发开始巡游*/
        puts("The travel steps are:");
        for(i=1;i<=n;i++)                      /*巡游完成后输出巡游过程*/
		{
            for(j=1;j<=n;j++) 
			    printf("%4d",f[i][j]);
            printf("\n");
		}
        
        printf("Please input the knight's position (i,j): ");/*为再次巡游输入起始位置*/
	    scanf("%d %d",&i,&j);
        l=(i-1)*n+j;
    }
	puts("\n Press any key to quit... ");
	getch();
    return 0;
}



/*****************************创建标志矩阵子函数*************************/
void creatadjm()
{
    int i,j;
    for(i=1;i<=n;i++)                                   /*巡游矩阵初始化*/
        for(j=1;j<=n;j++)
            f[i][j]=0;
    for(i=1;i<=m;i++)                                   /*标志矩阵初始化*/
        for(j=1;j<=m;j++)
            adjm[i][j]=0;
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            if(f[i][j]==0)           /*对所有符合条件的标志矩阵种元素置1*/
			{
                f[i][j]=1;
                if((i+2<=n)&&(j+1<=n)) mark(i,j,i+2,j+1);
                if((i+2<=n)&&(j-1>=1)) mark(i,j,i+2,j-1);
                if((i-2>=1)&&(j+1<=n)) mark(i,j,i-2,j+1);
                if((i-2>=1)&&(j-1>=1)) mark(i,j,i-2,j-1);
                if((j+2<=n)&&(i+1<=n)) mark(i,j,i+1,j+2);
                if((j+2<=n)&&(i-1>=1)) mark(i,j,i-1,j+2);
                if((j-2>=1)&&(i+1<=n)) mark(i,j,i+1,j-2);
                if((j-2>=1)&&(i-1>=1)) mark(i,j,i-1,j-2);
			} 
    return;
}


/*********************************巡游子函数*******************************/
void travel(int p,int r)
{
    int i,j,q;
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            if(f[i][j]>r) f[i][j]=0;              /*棋盘矩阵的置〉r时，置0*/
    r=r+1;                                                   /*跳步计数加1*/
    i=((p-1)/n)+1;                                  /*还原棋盘矩阵的横坐标*/
    j=((p-1)%n)+1;                                  /*还原棋盘矩阵的纵坐标*/
    f[i][j]=r;                              /*将f[i][j]做为第r跳步的目的地*/
   


    for(q=1;q<=m;q++)           /*从所有可能的情况出发，开始进行试探式巡游*/
	{
        i=((q-1)/n)+1;
        j=((q-1)%n)+1;
        if((adjm[p][q]==1)&&(f[i][j]==0)) 
			travel(q,r);                                    /*递归调用自身*/
    }
    return;
}

/*************************赋值子函数***************************************/
void mark(int i1,int j1,int i2,int j2)
{
    adjm[(i1-1)*n+j1][(i2-1)*n+j2]=1;
    adjm[(i2-1)*n+j2][(i1-1)*n+j1]=1;
    return;
}
 

