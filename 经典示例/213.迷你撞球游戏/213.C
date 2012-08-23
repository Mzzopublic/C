/*
打砖块程序
*/
#include<dos.h>
#include<stdio.h>
#include<graphics.h>
#include<math.h>
union REGS regs;
unsigned int zhuan[5];

void quitgame();
void wingame();
int Msinit();
void Setmouse(int left,int right,int top,int buttom);
int Msread(int *xp,int *yp,int *bup);
void Draw(int x,int y,int sizex,int sizey);
void mycircle(int x, int y, int r, int color);

main()
{
	int mode=VGAHI,driver=VGA;
	char ch;
 	unsigned int l;
 	int i,gi,gj,j,flag=1;/*i,j是循环变量,flag是标记变量,-1:向x负半轴移动,+1:向x正半轴移动*/
 	double qx,qy,k,b=0.0,speech=0.4,x,y;
 	double r=2.0,bx=60.0,byy=270.0;
 	double pianx=100.0,piany=100.0,tx=20.0,ty=10.0,jx=2.0,jy=2.0;
 	int mx=0,my=0,mb,sum;/*sum纪录砖块的数目*/
 	FILE * p;
  	if((p = fopen("record.dat", "r")) == NULL)
    	{
    		p=fopen("record.dat","w");
    		fprintf(p,"0 0 0 0 0\n");
    		rewind(p);
     	}
     	fclose(p);

	initgraph(&driver,&mode,"C:\\tc");
	setbkcolor(BLUE);
 	Msinit();
 	Setmouse((int)(pianx+1+bx/2),(int)((tx+jx)*8+pianx-1-bx/2),(int)byy+piany,(int)byy+piany);

star:	cleardevice();/*程序重载的介入点*/
	setcolor(RED);
	outtextxy(30,20,"^_^ Welcome to Play Hitting Brick Game! ^_^");

	sum=0;
 	qx=100.0+pianx-10;qy=180.0+pianx-10;k=0.33;
 	setcolor(7);
 	rectangle((int)(pianx-2),(int)(piany-2),(int)((tx+jx)*8+2+pianx),302+piany);
 	setcolor(1);
 	rectangle((int)(pianx-1),(int)(piany-1),(int)((tx+jx)*8+1+pianx),301+piany);
 	/*读取盘面情况*/
 	p=fopen("record.dat", "r");
 	for(i=0;i<5;i++)
 		fscanf(p,"%x ",&zhuan[i]);
 	fclose(p);
 	/*画砖块*/
 	for(i=0;i<5;i++)
 	{
 		l=1;
  		for(j=0;j<16;j++)
  		{
  			if((zhuan[i]&l)==0)
   			{
   				Draw((int)((jx+tx)*((16*i+j)%8)+pianx+jx),(int)((jy+ty)*((int)((16*i+j)/8))+piany+jy),(int)tx,(int)ty);
    				sum+=1;
   			}
   			l=l*2;
  		}
 	}
	gotoxy(5,4);
	printf("Press any key to start game...Q key to quit...");
	ch=getch();
	if(ch=='q'||ch=='Q')
		quitgame();
	else
	{
		gotoxy(5,4);
		printf("                                              ");
	}

 	for(;;)
	{
		setfillstyle(1, 0);
  		bar(mx-bx/2,my,mx+bx/2,my+5);
  		Msread(&mx, &my, &mb);

  		Draw(mx-bx/2,my,bx,5);
  		setcolor(0);
		mycircle(qx,qy,r,0);
  		/*判断求是否反弹*/
  		if(qx-r<=pianx+1 || qx+r>=(tx+jx)*8+pianx-1)
  		{
  			flag=-flag;
  			k=-k;
  		}
  		if(qy-r<=piany+1)
  			k=-k;

  		for(gi=0;gi<5;gi++)
  		{
  			l=1;
  			for(gj=0;gj<16;gj++)
  			{
  				if((zhuan[gi]&l)==0)
    				{
    					j=(16*gi+gj)/8;
     					i=(16*gi+gj)%8;
     					x=(jx+tx)*i+jx+tx/2+pianx;
     					y=(jy+ty)*j+jy+ty/2+piany;
     					/*边判断1*/
     					if(qy>=y-ty/2 && qy<=y+ty/2 &&(pow(qx+r-x+tx/2,2)<1 || pow(qx-r-x-tx/2,2)<1))
      					{
      						flag=-flag;
      						k=-k;
       						zhuan[gi]=zhuan[gi]|l;
       						sum-=1;
       						if(sum==0)
       							wingame();
       						setfillstyle(1, 0);
       						bar((jx+tx)*i+pianx+jx,(jy+ty)*j+piany+jy,(jx+tx)*i+pianx+jx+tx,(jy+ty)*j+piany+jy+ty);
      					}
      					else
      					/*边判断2*/
     					if(qx>=x-tx/2 && qx<=x+tx/2 &&(pow(qy+r-y+ty/2,2)<1 || pow(qy-r-y-ty/2,2)<1))
      					{
      						k=-k;
       						zhuan[gi]=zhuan[gi]|l;
       						sum-=1;
       						if(sum==0)
       							wingame();
       						setfillstyle(1, 0);
       						bar((jx+tx)*i+pianx+jx,(jy+ty)*j+piany+jy,(jx+tx)*i+pianx+jx+tx,(jy+ty)*j+piany+jy+ty);
      					}
      					else
      					/*角判断*/
     					if(pow(qx-x+tx/2,2)+pow(qy-y+ty/2,2)<=r*r || pow(qx-x-tx/2,2)+pow(qy-y+ty/2,2)<=r*r || pow(qx-x+tx/2,2)+pow(qy-y-ty/2,2)<=r*r || pow(qx-x-tx/2,2)+pow(qy-y-ty/2,2)<=r*r)
      					{
      						flag=-flag;
       						zhuan[gi]=zhuan[gi]|l;
       						sum-=1;
       						if(sum==0)
       							wingame();
       						setfillstyle(1, 0);
       						bar((jx+tx)*i+pianx+jx,(jy+ty)*j+piany+jy,(jx+tx)*i+pianx+jx+tx,(jy+ty)*j+piany+jy+ty);
      					}
     				}
     				l=l*2;
     			}
     		}
  		/*棍棒的反弹*/
  		if(qx<=mx+bx/2 && qx>=mx-bx/2 && pow(qy+r-my,2)<1)
  		{
  			k=-(k/pow(k*k,0.5))*(0.3*bx/(pow(pow(qx-mx,2),0.5)+0.000001));
  		}
  		if((int)(qy+r)>my+0.5)
  		{
			gotoxy(5,5);
  			printf("You have lost the game!Press any key to restart...");
  			getch();
  			goto star;
  		}
  		b=qy-qx*k;
  		if(flag==1)
  			qx=qx+speech/pow(1.0+k*k,0.5);
  		if(flag==-1)
  			qx=qx-speech/pow(1.0+k*k,0.5);
  		qy=qx*k+b;/*计算球心坐标*/
  		setcolor(14);
		mycircle((int)qx,(int)qy,r,14);
  		delay(1);
  		if(mb==1)
  		{
			gotoxy(5,5);
  			printf("You have lost the game!Press any key to restart...");
  			getch();
  			goto star;
  		}
 	}
}

void quitgame()
{
	closegraph();
	window(1,1,80,25);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	clrscr();
	exit(0);
}
void wingame()
{
	gotoxy(5,4);
	printf("You have won the game! Press any key to quit...");
	getch();
	quitgame();
}

/*这个函数用于实现鼠标的初始化*/
int Msinit()
{
	int recored;
 	regs.x.ax=0;
 	int86 (0x33, & regs, & regs);
 	recored=regs.x.ax;
 	if(recored==0)
 	{
 		printf("Mouse not foundd or Mouse driver not installed.\n");
  		return 0;
 	}
 	else
 		return recored;
}

/*下面的函数用于实现设置鼠标的移动范围*/
void Setmouse(int left,int right,int top,int buttom)
{
 	regs.x.ax=7;
 	regs.x.cx=left;
 	regs.x.dx=right;
 	int86(0x33,& regs,& regs);
 	regs.x.ax=8;
 	regs.x.cx=top;
 	regs.x.dx=buttom;
 	int86(0x33,& regs,& regs);
}

/*下面这个函数用于实现鼠标的读取*/
int Msread(int *xp,int *yp,int *bup)
{
	int xnew,ynew,ch;
  	if(kbhit()) return getch();
   	regs.x.ax=3;
   	int86(0x33,& regs,& regs);
   	xnew=regs.x.cx;
   	ynew=regs.x.dx;
   	*bup=regs.x.bx;
 	*xp=xnew;
 	*yp=ynew;
 	return -1;
}

/*下面这个子程序是完成描绘一个按钮*/
void Draw(int x,int y,int sizex,int sizey)/* x, y为左上角坐标sizex,sizey为长和宽*/
{
	int sizx=sizex-1;
 	int sizy=sizey-1;
 	setcolor(15);/*这里最好用白色*/
 	line(x,y,x+sizx-1,y);
 	line(x,y+1,x+sizx-2,y+1);
 	line(x,y,x,y+sizy-1);
 	line(x+1,y,x+1,y+sizy-2);
 	setcolor(8);/*这里最好用深灰色*/
 	line(x+1,y+sizy,x+sizx,y+sizy);
 	line(x+2,y+sizy-1,x+sizx,y+sizy-1);
 	line(x+sizx-1,y+1,x+sizx-1,y+sizy);
 	line(x+sizx,y+2,x+sizx,y+sizy);
 	setcolor(7);/*这里最好用灰色*/
 	putpixel(x,y+sizy,3);
 	putpixel(x+1,y+sizy-1,3);
 	putpixel(x+sizx,y,3);
 	putpixel(x+sizx-1,y+1,3);
 	setfillstyle(1, 7);/*这里最好用灰色,设置填充模式*/
 	bar(x+2,y+2,x+sizx-2,y+sizy-2);
}

void mycircle(int x, int y, int r, int color)
{
	setcolor(color);
	circle(x,y,r);
	setfillstyle(SOLID_FILL,BLUE);
	floodfill(x,y,color);
}


 
