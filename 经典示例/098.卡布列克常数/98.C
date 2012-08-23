#include<stdio.h>
void vr6174(int);
void parse_sort(int num,int *each);
void max_min(int *each,int *max,int *min);
void parse_sort(int num,int *each);
int count=0;
void main()
{

    int n=1;
    clrscr();
    puts("**************************************************************");
    puts("*         This program is to verify Comgrich Content.        *");
    puts("* That is any 4-digit number whose digitals are not the same *");
    puts("* has the rule: (1) range the 4 digits to get the maximum    *");
    puts("* 4-digit number, (2) range the 4 digits to get the minimum  *");
    puts("* 4-digit number, (3) get the difference of these two numbers*");
    puts("* that is a new 4-digit number. Continute to calculate with  *");
    puts("* (1)-(3),the result in the end is 6174,the Comgrich Content.*");
    puts("**************************************************************");
    while(n!=0)
    {
	    printf(" >> Please input a 4-digit number to verify(0 to quit): ");
	    scanf("%d",&n);      /*输入任一整数*/
	    if(n==0)
		break;
	    printf(" >> ------ Results of verification: ------------\n");
	    count=0;
	    vr6174(n);           /*调用函数进行验证*/
	    printf(" >> ---------------------------------------------\n");
    }
    puts("\n Press any key to quit...");
    getch();
}
void vr6174(int num)
{
    int each[4],max,min;
    if(num!=6174&&num)    /*若不等于74且不等于0则进行卡布列克运算*/
    {
        parse_sort(num,each);         /*将整数分解，数字存入each数组中*/
        max_min(each,&max,&min);      /*求数字组成的最大值和最小值*/      
        num=max-min;          /*求最大值和最小值的差*/
        printf(" >> Step No.%d:  %d-%d=%d\n",++count,max,min,num); /*输出该步计算过程*/
        vr6174(num);         /*递归调用自身继续进行卡布列克运算*/
    }
}
void parse_sort(int num,int *each)
{
    int i,*j,*k,temp;
    for(i=0;i<=4;i++)         /*将NUM分解为数字*/
    {
        j=each+3-i;
        *j=num%10;
        num/=10;
    }
    for(i=0;i<3;i++)     /*对各保数字从小到大进行排序*/
        for(j=each,k=each+1;j<each+3-i;j++,k++)
            if(*j>*k) { temp=*j;*j=*k;*k=temp;}
    return;
}
void max_min(int *each,int *max,int *min)    /*将分解的数字还原为最大整数和最小整数*/
{
    int *i;
    *min=0;
    for(i=each;i<each+4;i++)     /*还原为最小的整数*/
        *min=*min*10+*i;
    *max=0;
    for(i=each+3;i>=each;i--)    /*还原为最大的整数*/
        *max=*max*10+*i;
    return;
}
