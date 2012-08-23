#include<stdio.h>
void getti(int a,int y,int z);
int i;           /*最后需要分出的重量*/
void main()
{
    int a,y,z;
    clrscr();
    puts("***************************************************************");
    puts("*  This program is to solve Problem of Poisson Beer Division. *");
    puts("* The Problem is as follows: Someone has a bottle of 12 pints *");
    puts("* beer. He wants to get 6 pints, but he does not has a bottle *");
    puts("* of 6 pints. He only has a bottle of 8 pints and a bottle of *");
    puts("* 5 pints. So how can he get 6 pints?                         *");
    puts("***************************************************************");
    printf(" >> Input Full bottle a,Empty b,c, and Get volumes:\n"); /*a 满瓶的容量  y:第一个空瓶的容量  z:第二个空瓶的容量*/
    printf(" >> ");
    scanf("%d%d%d%d",&a,&y,&z,&i);
    getti(a,y,z);           /*按a -> y -> z -> a的操作步骤*/
    /*getti(a,z,y);           /*按a -> z -> y -> a的步骤*/
    printf("\n Press any key to quit...");
    getch();
}
void getti(int a,int y,int z)   /*a:满瓶的容量  y:第一个空瓶的容量  z:第二个空瓶的容量*/
{
   int b=0,c=0,j=0;           /* b:第一瓶实际的重量  c:第二瓶实际的重量 j: 倒的步数*/
   puts(" >> The division steps are as follows.\n");

   printf(" Bottle:    a<%d> b<%d> c<%d>\n",a,y,z);
   printf("-----------------------------\n");
   printf(" Step No.|\n");
   printf("   <%d>   | %4d %4d %4d\n",j++,a,b,c);

   while(a!=i||b!=i&&c!=i)      /*当满瓶!=i或另两瓶都!=i*/
   {
      if(!b)
      {  a-=y; b=y;}    /*如果第一瓶为空，则将满瓶倒入第一瓶中*/
      else if(c==z)
      {  a+=z; c=0;}    /*如果第二瓶满，则将第二瓶倒入满瓶中*/
      else if(b>z-c)    /*如果第一瓶的重量>第二瓶的剩余空间*/
      { b-=(z-c);c=z;}    /*则将装满第二瓶，第一瓶中保留剩余部分*/
      else{ c+=b; b=0;}   /*否则，将第一瓶全部倒入第二瓶中*/
      printf("   <%d>   | %4d %4d %4d\n",j++,a,b,c);
   }
   printf("-----------------------------\n");
}
