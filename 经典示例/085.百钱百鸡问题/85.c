#include<stdio.h>
void main()
{
    int x,y,z,j=0;
    clrscr();
    puts("************************************************");
    puts("*      This program is to solve Problem of     *");
    puts("*           Hundred Yuan Hundred Fowls.        *");
    puts("*  Which is presented by Zhang Qiujiang,       *");
    puts("* a Chinese ancient mathematician, in his work *");
    puts("* Bible of Calculation: 5 Yuan can buy 1 cock, *");
    puts("* 3 Yuan can buy 1 hen, 1 Yuan buy 3 chickens, *");
    puts("* now one has 100 Yuan to buy 100 fowls, the   *");
    puts("* question is how many cocks, hens, chickens   *");
    puts("* to buy?                                      *");
    puts("************************************************");
    printf("\n The possible plans to buy 100 fowls with 100 Yuan are:\n\n");
    for(x=0;x<=20;x++)               /*外层循环控制鸡翁数*/
        for(y=0;y<=33;y++)           /*内层循环控制鸡母数y在0~33变化*/
        {
            z=100-x-y;             /*内外层循环控制下，鸡雏数z的值受x,y的值的制约*/
            if(z%3==0&&5*x+3*y+z/3==100)
                                   /*验证取z值的合理性及得到一组解的合理性*/
                printf("%2d: cock=%2d hen=%2d chicken=%2d\n",++j,x,y,z);
	}
    puts("\n Press any key to quit...");
    getch();
}
