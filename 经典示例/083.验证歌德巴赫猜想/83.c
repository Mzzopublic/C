#include<stdio.h>
#include<math.h>
int fflag(int n);
void main()
{
    int i,j,n;
    long max;
    clrscr();
    puts("============================================================");
    puts("||       This program will verify the Goldbach Guess.     ||");
    puts("|| That is any positive even number can be broken up into ||");
    puts("||               the sum of two prime numbers.            ||");
    puts("||       e.g., 4=2+2, 6=3+3, 8=3+5, 10=3+7, 12=5+7,...    ||");
    puts("============================================================");
    printf("\n >> Please input the scale n you want to verify : ");
    scanf("%ld",&max);
    printf("\n >> Now the program starts to verify the even number\n");
    printf(" >> less than %ld equals to sum of two prime numbers.\n\n",max);
    for(i=4,j=0;i<=max;i+=2)
    {
        for(n=2;n<i;n++)         /*将偶数i分解为两个整数*/
            if(fflag(n))         /*分别判断两个整数是否均为素数*/
                if(fflag(i-n))
                {
		    printf("%4d=%2d+%2d ",i,n,i-n);        /*若均是素数则输出*/
		    j++;
		    if(j==5)
		    {
			printf("\n");
			j=0;
		    }
                    break;
                }
            if(n==i)  printf("error %d\n",i);
    }
    puts("\n >> Press any key to quit...");
    getch();
}

int fflag(int i)           /*判断是否为素数*/
{
    int j;
    if(i<=1)return 0;
    if(i==2)return 1;
    if(!(i%2))return 0;     /*if no,return 0*/
    for(j=3;j<=(int)(sqrt((double)i)+1);j+=2)
        if(!(i%j))return 0;
    return 1;              /*if yes,return 1*/
}
