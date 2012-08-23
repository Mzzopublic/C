#include<stdio.h>
int a[]={0,1,2,5,8,7,6,3};     /*指针数组.依次存入矩阵中构成环的元素下标*/
int b[9];                      /*表示3X3矩阵,b[4]为空格*/
int c[9];                      /*确定1所在的位置后,对环进行调整的指针数组*/
int count=0;                   /*数字移动步数计数器*/
void main()
{
   int i,j,k,t;
   void print();
   clrscr();
   puts("*****************************************************");
   puts("*         This is a program to Move Numbers.        *");
   puts("*****************************************************");
   printf(" >> Please enter original order of digits 1~8: ");
   for(i=0;i<8;i++)
      scanf("%d",&b[a[i]]);
               /*顺序输入矩阵外边的8个数字,矩阵元素的顺序由指针数组的元素a[i]控制*/
   printf("The sorting process is as felow:\n");
   print();
   for(t=-1,j=0;j<8&&t==-1;j++)          /*确定数字1所在的位置*/
      if(b[a[j]]==1) t=j;                 /*t:记录数字1所在的位置*/
   for(j=0;j<8;j++)     /*调整环的指针数组,将数字1所在的位置定为环的首*/
      c[j]=a[(j+t)%8];
   for(i=2;i<9;i++)         /*从2开始依次调整数字的位置*/
                        /*i:正在处理的数字,i对应在环中应当的正确位置就是i-1*/
      for(j=i-1;j<8;j++)     /*从i应处的正确位置开始顺序查找*/
	 if(b[c[j]]==i&&j!=i-1)      /*若i不在正确的位置*/
         {
            b[4]=i;          /*将i移到中心的空格中*/
	    b[c[j]]=0;print();     /*空出i原来所在的位置,输出*/
            for(k=j;k!=i-1;k--)    /*将空格以前到i的正确位置之间的数字依次向后移动一格*/
            {
	       b[c[k]]=b[c[k-1]];    /*数字向后移动*/
	       b[c[k-1]]=0;
               print();
            }
	    b[c[k]]=i;         /*将中间的数字i移入正确的位置*/
            b[4]=0;            /*空出中间的空格*/
            print();
            break;
         }
	 else if(b[c[j]]==i) break;       /*数字i在正确的位置*/
    printf("\n Press any key to quit...");
    getch();
}
void print(void)        /*按格式要求输出矩阵*/
{
   int c;
   printf(" >> Step No.%2d  ",count++);
   for(c=0;c<9;c++)
      if(c%3==2) printf("%2d  ",b[c]);
      else  printf("%2d",b[c]);
   printf("\n");
}
