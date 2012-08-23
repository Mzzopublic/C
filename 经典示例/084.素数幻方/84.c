#include<stdio.h>
#include<math.h>
int number[210][5];     /*存放可逆素数及素数分解后的各位数字*/
int select[110];        /*可以放在矩阵第一行和最后一行的素数的下标*/      
int array[4][5];        /*4X4的矩阵，每行0号元素存可逆素数对应的数组下标*/
int count;              /*可逆素数的数目*/
int selecount;          /*可以放在矩阵第一行和最后一行的可逆素数的数目*/
int larray[2][200];     /*存放素数前二、三位数的临时数组所对应的数量计数器*/
int lcount[2];
int num(int number);
int ok(int number);
void process(int i);
void copy_num(int i);
int comp_num(int n);
int find1(int i);
int find2(void);
int find0(int num);
void p_array(void);

FILE *fp;
void main()
{
    int i,k,flag,cc=0,i1,i4;
    clrscr();
    if((fp=fopen("Exa70data.dat","w+"))==NULL)
    {
	printf("\n Can't create file Exa70data.dat !\n");
	exit(0);
    }
    printf("there are magic squares with invertable primes as follw:\n");
    for(i=1001;i<9999;i+=2)                 /*求满足条件的可逆素数*/
    {
        k=i/1000;
        if(k%2!=0&&k!=5&&num(i))     /*若可逆素数的第一位不是偶数或5*/
        {
            number[count][0]=i;      /*存入数组*/
            process(count++);        /*分解素数的各位数字*/
            if(number[count-1][2]%2!=0&&   /*若可逆素数满足放在矩阵第一行*/
               number[count-1][3]%2!=0&&   /*和最后一行的条件，记录可逆素数的*/
               number[count-1][2]!=5&&     /*下标，计数器加1*/
               number[count-1][3]!=5)
                select[selecount++]=count-1;
        }
    }
    larray[0][lcount[0]++]=number[0][0]/100;     /*临时数组的第一行存前二位*/
    larray[1][lcount[1]++]=number[0][0]/10;      /*临时数组的第二行存前三位*/
    for(i=1;i<count;i++)                /*将素数不重复的前二、三位存入临时数组中*/
    {
        if(larray[0][lcount[0]-1]!=number[i][0]/100)
            larray[0][lcount[0]++]=number[i][0]/100;
        if(larray[1][lcount[1]-1]!=number[i][0]/10)
            larray[1][lcount[1]++]=number[i][0]/10;
    }
    for(i1=0;i1<selecount;i1++)                    /*在第一行允许的汇聚围内穷举*/
    {
        array[0][0]=select[i1];                    /*取对应的素数下标*/
        copy_num(0);                               /*复制分解的素数*/
        for(array[1][0]=0;array[1][0]<count;array[1][0]++)    /*穷举第二行*/
        {
            copy_num(1);                /*复制分解的数字*/
            if(!comp_num(2))
                continue;         /*若每列的前两位的组成与素数相矛盾，则试探下一个数*/
            for(array[2][0]=0;array[2][0]<count;array[2][0]++)     /*穷举第三行*/
            {
                copy_num(2);           /*复制分解的数字*/
                if(!comp_num(3))
                    continue;          /*若每列的前三位的组成与素数相矛盾，则试探下一个数*/
                for(i4=0;i4<selecount;i4++)     /*在最后一行允许的范围内穷举*/
                {
                    array[3][0]=select[i4];
                    copy_num(3);                /*复制分解的数字*/
                    for(flag=1,i=1;flag&&i<=4;i++)    /*判断每列是否可逆素数*/
                        if(!find1(i))flag=0;
                    if(flag&&find2())            /*判断对角线是否为可逆素数*/
		    {  	printf("No.%d\n",++cc);
			fprintf(fp,"No.%d\n",cc);
			p_array();
		    }    /*输出幻方矩阵*/
                }
            }
        }
    }
    fclose(fp);
    puts("\n Press any key to quit...");
    getch();
}

int num(int number)              /*判断是否可逆素数*/
{
    int j;
    if(!ok(number)) return 0;
    for(j=0;number>0;number/=10)       /*将素数变为反序数*/
        j=j*10+number%10;
    if(!ok(j)) return 0;           /*判断反序数是否为素数*/
    return 1;
}

int ok(int number)                /*判断是否为素数*/
{
    int i,j;
    if(number%2==0) return 0;
    j=sqrt((double)number)+1;
    for(i=3;i<=j;i+=2)
        if(number%i==0) return 0;
    return 1;
}

void process(int i)                 /*将第i个整数分解为数字并存入数组*/
{
    int j,num;
    num=number[i][0];
    for(j=4;j>=1;j--,num/=10)
        number[i][j]=num%10;
}

void copy_num(int i)        /*将array[i][0]指向的素数的各位数字复制到array[i]中*/
{
    int j;
    for(j=1;j<=4;j++)
	array[i][j]=number[array[i][0]][j];
}

int comp_num(int n)           /*判断array中每列的前n位是否与可逆素数允许的前n位矛盾*/
{
    static int ii;           /*用内部静态变量保存前一次查找到的元素下标*/
    static int jj;    /*ii:前一次查找前二位的下标，jj:前一次查找前三位的下标*/
    int i,num,k,*p;   /*p:指向对应的要使用的前一次下标ii或jj*/
    int *pcount;      /*pcount:指向要使用的临时数组数量的计数器*/
    switch(n){               /*根据n的值选择对应的一组控制变量*/
        case 2:pcount=&lcount[0];p=&ii;break;
        case 3:pcount=&lcount[1];p=&jj;break;
        default:return 0;
    }
    for(i=1;i<=4;i++)          /*对四列分别进行处理*/
    {
        for(num=0,k=0;k<n;k++)       /*计算前n位数字代表的数值*/
            num=num*10+array[k][i];
        if(num<=larray[n-2][*p])     /*与前一次最后查找到的元素进行比较*/
            for(;*p>=0&&num<larray[n-2][*p];(*p)--);/*若前次查找到的元素大，则向前找*/
        else
            for(;p<pcount&&num>larray[n-2][*p];(*p)++);  /*否则向后找*/
        if(*p<0||*p>=*pcount)
        {
            *p=0; return 0;
        }
        if(num!=larray[n-2][*p])
            return 0;            /*前n位不是可逆素数允许的值则返回0*/
    }
    return 1;
}

int find1(int i)               /*判断列方向是否是可逆素数*/
{
    int num,j;
    for(num=0,j=0;j<4;j++)
        num=num*10+array[j][i];
    return find0(num);
}
int find2(void)                /*判断对角线方向是否是可逆素数*/
{
    int num1,num2,i,j;
    for(num1=0,j=0;j<4;j++)
        num1=num1*10+array[j][j+1];
    for(num2=0,j=0,i=4;j<4;j++,i--)
        num2=num2*10+array[j][i];
    if(find0(num1)) return(find0(num2));
    else return 0;
}

int find0(int num)               /*查找是否为满足要求的可逆素数*/
{
    static int j;
    if(num<=number[j][0])for(;j>=0&&num<number[j][0];j--);
    else for(;j<count&&num>number[j][0];j++);
    if(j<0||j>=count){ j=0;return 0; }
    if(num==number[j][0]) return 1;
    else return 0;
}

void p_array(void)                /*输出矩阵*/
{
    int i,j;
    for(i=0;i<4;i++)
    {
	for(j=1;j<=4;j++)
	{
		printf("%d ",array[i][j]);
		fprintf(fp,"%d ",array[i][j]);
	}
	printf("\n");
	fprintf(fp,"\n");
    }
}
