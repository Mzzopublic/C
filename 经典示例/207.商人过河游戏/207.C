#include <stdio.h>
#include <conio.h>
#include <string.h>
void dwframe()
{
	window(1,1,25,80);
	textbackground(LIGHTBLUE);
	textcolor(YELLOW);
	clrscr();
	gotoxy(2,2);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(2,3);
	printf("┃");
	gotoxy(76,3);
	printf("┃");
	gotoxy(2,4);
	printf("┃");
	gotoxy(76,4);
	printf("┃");
	gotoxy(2,5);
	printf("┃");
	gotoxy(76,5);
	printf("┃");
	gotoxy(2,6);
	printf("┃");
	gotoxy(76,6);
	printf("┃");
	gotoxy(2,7);
	printf("┃");
	gotoxy(76,7);
	printf("┃");
	gotoxy(2,8);
	printf("┃");
	gotoxy(76,8);
	printf("┃");
	gotoxy(2,9);
	printf("┃");
	gotoxy(76,9);
	printf("┃");
	gotoxy(2,10);
	printf("┃");
	gotoxy(76,10);
	printf("┃");
	gotoxy(2,11);
	printf("┃");
	gotoxy(76,11);
	printf("┃");
	gotoxy(2,11);
	printf("┃");
	gotoxy(76,11);
	printf("┃");
	gotoxy(2,12);
	printf("┃");
	gotoxy(76,12);
	printf("┃");
	gotoxy(2,14);
	printf("┃");
	gotoxy(76,14);
	printf("┃");
	gotoxy(2,15);
	printf("┃");
	gotoxy(76,15);
	printf("┃");
	gotoxy(2,16);
	printf("┃");
	gotoxy(76,16);
	printf("┃");
	gotoxy(2,17);
	printf("┃");
	gotoxy(76,17);
	printf("┃");
	gotoxy(2,18);
	printf("┃");
	gotoxy(76,18);
	printf("┃");
	gotoxy(2,19);
	printf("┃");
	gotoxy(76,19);
	printf("┃");
	gotoxy(2,20);
	printf("┃");
	gotoxy(76,20);
	printf("┃");
	gotoxy(2,21);
	printf("┃");
	gotoxy(76,21);
	printf("┃");
	gotoxy(2,22);
	printf("┃");
	gotoxy(76,22);
	printf("┃");
	gotoxy(2,23);
	printf("┃");
	gotoxy(76,23);
	printf("┃");
	gotoxy(2,24);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}
void helpf() 
{
	dwframe();
	gotoxy(25,4);
	printf("^_^欢迎使用商人过河游戏！^_^");
	gotoxy(4,6);
	printf("游戏说明：");
	gotoxy(5,7);
	printf("三个商人带着三个随从和货物过河，船每次最多只能载两个人，");
	gotoxy(5,8);
	printf("要求在过河期间，任一岸上商人的人数要大于或等于随从的人数，");
	gotoxy(5,9);
	printf("否则随从会杀掉商人抢走货物。");
	gotoxy(5,11);
	printf("游戏操作：（1）依次输入过河的商人和随从人数；");
	gotoxy(5,12);
	printf("          （2）非法输入按键将重新开始。");
	gotoxy(2,13);
	printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫");
}
char a[6];
char b[6];
void printcase(char a[],char b[])
{
	int i,j,xa,xb,x0,ya,yb,y0;
 	xa=xb=x0=ya=yb=y0=0;
 	gotoxy(11,14);
 	printf("此岸                彼岸\n");
  	for(i=0;i<6;i++) 
  	{
	   	if(a[i]=='A')
	    		xa++;
	   	else if (a[i]=='B')
	    		xb++;
	   	else if(a[i]=='0')
	    		x0++;
  	}
  	gotoxy(11,15);
  	for(i=1;i<=xa;i++) 
  		printf("商人  ");
 	gotoxy(11,16);
  	for(i=1;i<=xb;i++)
  		printf("随从  ");
 	gotoxy(11,17);
 	for(i=1;i<=x0;i++)
  		printf("   ");
	
 	for(j=0;j<6;j++)
 	{
	  	if(b[j]=='A')
	   		ya++;
	  	else if(b[j]=='B')
	   		yb++;
	 	else if(b[j]=='0')
	   		y0++;
 	}
	gotoxy(31,15);
	for(j=1;j<=ya;j++)
	  	printf("商人 ");
	gotoxy(31,16);
	for(j=1;j<=yb;j++)
	  	printf("随从 ");
	gotoxy(31,17);
	for(j=1;j<=y0;j++)
	   	printf("   ");
	if(xa==0 && xb==0 && ya==3 && yb==3)
	{
	  	gotoxy(15,21);
	  	printf("恭喜你！你已成功帮助商人安全过河！按任意键退出...");
	  	getch();
	  	window(1,1,25,80);
		textbackground(BLACK);
		textcolor(LIGHTGRAY);
		clrscr();
	  	exit(0);
	}
}

void main()
{
	int i,x,y,key,ca,cb,j,aA,aB,bA,bB;
	char ch;
Start:	
	helpf();
	gotoxy(15,16);
	printf("请按任意键开始游戏（按Q键退出）...");
	ch=getch();
	if(ch=='Q'||ch=='q')
	{
		window(1,1,25,80);
		textbackground(BLACK);
		textcolor(LIGHTGRAY);
		clrscr();
		exit(0);
	}
	helpf();
	 
	for(i=0;i<3;i++)
		a[i]='A';
	for(i=3;i<6;i++)
		a[i]='B';
	for(i=0;i<6;i++)
		b[i]='0';
	printcase(a,b);
	while(1)
	{
		gotoxy(20,18);
		printf("输入去彼岸商人的人数： ");
		scanf("%d",&x);
		gotoxy(20,18);
		printf("输入去彼岸随从的人数： ");
		scanf("%d",&y);
		for(ca=0,cb=0,i=0;i<6;i++) 
		{
			if(a[i]=='A')
	    			ca++;
			else if(a[i]=='B')
	    			cb++;
		}
		if(x<0 || x>ca || y<0 || y>cb || x+y<1 ||x+y>2) 
		{
			gotoxy(15,21);
			printf("输入有误！！按任意键重新开始...");
			getch();
			goto Start;
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='A') 
	   			{
	    				a[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(b[j]=='0') 
	   			{
	    				b[j]='A';
	       				break;
	   			}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='B') 
	   			{
	    				a[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(b[j]=='0') 
	   			{
	    				b[j]='B';
	    				break;
				}
		}
		for(aA=0,aB=0,i=0;i<6;i++) 
		{
	  		if(a[i]=='A')
	   			aA++;
	  		else if(a[i]=='B')
	   			aB++;
		}
		for(bA=0,bB=0,i=0;i<6;i++) 
		{
	  		if(b[i]=='A')
	   			bA++;
	  		else if(b[i]=='B')
	   			bB++;
		}   
	 
		if((aA==3) ||(bA==3) || (aA==aB) || (bA==bB))  
		{
	      		helpf();
	      		printcase(a,b);
	         }
		else
		{
			gotoxy(15,21);
			printf("商人被杀，按任意键重新开始...");
			getch();
			goto Start;
		}
		gotoxy(20,18);
		printf("输入回此岸商人的人数： ");
		scanf("%d",&x);
		gotoxy(20,18);
		printf("输入回此岸随从的人数： ");
		scanf("%d",&y);
		for(ca=0,cb=0,i=0;i<6;i++)
		{
	  		if(b[i]=='A')
	   			ca++;
	  		else if(b[i]=='B')
	   			cb++;
		}
		if(x<0 || x>ca || y<0 || y>cb || x+y<1 || x+y>2) 
		{
			gotoxy(15,21);
	   		printf("输入有误！！按任意键重新开始...");
	   		getch();
	  		goto Start;
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(b[j]=='A') 
	   			{
	    				b[j]='0';
	    				break;
	   			}
		}
		for(i=1;i<=x;i++) 
		{
	  		for(j=0;j<6;j++)
	   			if(a[j]=='0') 
	   			{
	    				a[j]='A';
	    				break;
				}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   		if(b[j]=='B') 
	   		{
	    			b[j]='0';
	    			break;
	   		}
		}
		for(i=1;i<=y;i++) 
		{
	  		for(j=0;j<6;j++)
	   		if(a[j]=='0') 
	   		{
	    			a[j]='B';
				break;
	   		}
		}
		for(aA=0,aB=0,i=0;i<6;i++) 
		{
	  		if(a[i]=='A')
	   			aA++;
	  		else if(a[i]=='B')
	   			aB++;
		}
		for(bA=0,bB=0,i=0;i<6;i++) 
		{
	  		if(b[i]=='A')
	   			bA++;
	  		else if(b[i]=='B')
	   			bB++;
		}
	 	if((aA==3) ||(bA==3) || (aA==aB) || (bA==bB))
	 	{
	  		helpf();
	  		printcase(a,b);
		}
		else 
		{
 	  		gotoxy(15,21);
	   		printf("商人被杀，按任意键重新开始...");
	        	getch();
	        	goto Start;
	  	}
	}
}