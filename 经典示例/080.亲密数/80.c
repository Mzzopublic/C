#include<stdio.h>
void main()
{
    int a,i,b,n,m;
    clrscr();
    printf("================================================================\n");
    printf("     This is a program to find friendly numbers pair.\n");
    printf("   Which means the sum of integer A's all factors (except A)\n");
    printf("    equals to the sum of integer B's all factors (except B).\n");
    printf("     < e.g. sum of integer 6's all factors are:1+2+3=6 >\n");
    printf("================================================================\n");
    printf("\n Please input the scale you want to find n: ");
    scanf("%d",&n);
    printf("\n There are following friendly--numbers pair smaller than %d:\n",n);
    for(a=1;a<n;a++)        /*穷举1000以内的全部整数*/
    {
        for(b=0,i=1;i<=a/2;i++)    /*计算数a的各因子，各因子之和存放于b*/
            if(!(a%i))b+=i;        /*计算b的各因子，各因子之和存于m*/
        for(m=0,i=1;i<=b/2;i++)
            if(!(b%i))m+=i;
        if(m==a&&a<b)
            printf("%4d..%4d    ",a,b);     /*若n=a，则a和b是一对亲密数，输出*/
    }
    puts("\n\n Press any key to quit...");
    getch();
}
