#include<stdio.h>
void print(int s[]);
int judge(int c[]);
int j=0;
void main()
{
    static int sweet[10]={10,2,8,22,16,4,10,6,14,20};   /*初始化数组数据*/
    int i,t[10],l;
    clrscr();
    printf("  Child No.    1   2   3   4   5   6   7   8   9   10\n");
    printf("------------------------------------------------------\n");
    printf("  Round No.|\n");
    print(sweet);          /*输出每个人手中糖的块数*/
    while(judge(sweet))      /*若不满足要求则继续进行循环*/
    {
        for(i=0;i<10;i++)    /*将每个人手中的糖分成一半*/
            if(sweet[i]%2==0)     /*若为偶数则直接分出一半*/
                t[i]=sweet[i]=sweet[i]/2;
            else               /*若为奇数则加1后再分出一半*/
                t[i]=sweet[i]=(sweet[i]+1)/2;
        for(l=0;l<9;l++)         /*将分出的一半糖给右(后)边的孩子*/
            sweet[l+1]=sweet[l+1]+t[l];
        sweet[0]+=t[9];
        print(sweet);             /*输出当前每个孩子中手中的糖数*/
    }
    printf("------------------------------------------------------\n");
    printf("\n Press any key to quit...");
    getch();
}
int judge(int c[])
{
    int i;
    for(i=0;i<10;i++)          /*判断每个孩子手中的糖是否相同*/
        if(c[0]!=c[i]) return 1;          /*不相同返回 1*/
    return 0;
}
void print(int s[])      /*输出数组中每个元素的值*/
{
    int k;
    printf("      <%2d> | ",j++);
    for(k=0;k<10;k++)   printf("%4d",s[k]);
    printf("\n");
}
