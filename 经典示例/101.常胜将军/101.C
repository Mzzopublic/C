#include<stdio.h>
void main()
{
   int a=21,i;
   clrscr();
   puts("*****************************************************");
   puts("*            This is a Matchstick Taken Game.       *");
   puts("* There are 21 machsticks, two persons take them in *");
   puts("* turn. Each one each time takes 1 to 4 sticks. The *");
   puts("* one who takes the last stick will lose the game.  *");
   puts("*****************************************************");
   printf(" >> --------------- Game Begin ---------------------\n");
   while(a>0)
   {
      do{
         printf(" >> How many sticks do you wish to take(1~%d)?",a>4?4:a);
         scanf("%d",&i);
      }while(i>4||i<1||i>a);      /*接收正在确的输入*/
      if(a-i>0) printf(" >> %d stick left in the pile.\n",a-i);
      if((a-i)<=0)
      {
         printf(" >> You have taken the last stick.\n");
         printf(" >> ******* You lose! ******* \n");     /*输出取胜标记*/
         printf(" >> --------------- Game Over! ---------------------\n");
         break;
      }
      else
         printf(" >> Compute take %d stick.\n",5-i);    /*输出计算机取的子数*/
      a-=5;
      printf(" >> %d stick left in the pile.\n",a);
   }
   printf("\n Press any key to quit...");
   getch();
}
