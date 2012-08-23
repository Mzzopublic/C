#include <stdio.h>
void main()
{
	enum fruit {apple, orange, banana, pineapple, pear};/* 定义枚举结构*/
	enum fruit x,y,z,pri;/*定义枚举变量*/
	int n,loop;
	n=0;
	for(x=apple;x<=pear;x++)
		for(y=apple;y<=pear;y++)
			if(x!=y)
			{
				for(z=apple;z<=pear;z++)
					if((z!=x)&&(z!=y))
					{
						n=n+1;
						printf("\n %-4d",n);
						for(loop=1;loop<=3;loop++)
						{
							switch(loop)
							{
								case 1:pri=x;break;
								case 2:pri=y;break;
								case 3:pri=z;break;
								default: break;
							}
							switch(pri)
							{
								case apple: printf(" %-9s","apple");break;
								case orange: printf(" %-9s","orange");break;
								case banana: printf(" %-9s","banana");break;
								case pineapple: printf(" %-9s","pineapple");break;
								case pear: printf(" %-9s","pear");break;
								default: break;
							}
						}
					}
				}
	printf("\n\n There are %d kinds of fruit plates.\n",n);
	puts(" Press any key to quit...");
	getch();
	return;
}
