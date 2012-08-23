/*
迷宫探险 
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <graphics.h>
#define N 22
#define M 22
int bg[M][N];

void makebg(int,int);
void drawbg(int[][],int,int,int,int,int);
void drawman(int,int,int);
void rect(int,int,int,int);

void main()
{	/* main()开始 */
	int gdriver=VGA,gmode=VGAHI;
	int direc;
	char ch,ch2;
	int step,len,size,x,y,i,j;
	initgraph(&gdriver,&gmode,"c:\\tc");
Startgame:
	step=20;
	len=10;
	size=20;
	x=0;
	y=0;
	i=0;
	j=0;
	makebg(M,N);
	cleardevice();
	setbkcolor(LIGHTBLUE);
	setwritemode(XOR_PUT);
	settextstyle(1,0,3);
	/*setcolor(GREEN);
	outtextxy(100,180,"Press <Q> to quit");*/
	setcolor(RED);
	setfillstyle(LINE_FILL,RED);
	drawbg(bg,M,N,size,0,0);
	setcolor(WHITE);
	x+=len;y+=len;
	drawman(x,y,len);
	gotoxy(60,3);
	printf("M-by Manual");
	gotoxy(60,6);
	printf("C-by Computer");
	gotoxy(60,9);
	printf("Please choice:");
	setcolor(WHITE);
	while(1)
	{
		ch=getch();
		if(ch=='M'||ch=='m')
		{
			/* 人工控制 */
			gotoxy(60,3);
			printf("A-Left     ");
			gotoxy(60,6);
			printf("D-Right      ");
			gotoxy(60,9);
			printf("W-Up           ");
			gotoxy(60,12);
			printf("S-Down");
			gotoxy(60,15);
			printf("Q-Quit");
			while(1)
			{
				ch=getch();
				drawman(x,y,len);
				if(ch=='Q'||ch=='q')
				{
					/*按了Q键，退出游戏*/
					gotoxy(60,18);
					printf("You have press");
					gotoxy(60,19);
					printf("Q key! Do you");
					gotoxy(60,20);
					printf("Want to quit?");
					gotoxy(60,21);
					printf("OK?[Y/N]");
					while(1)
					{
						ch2=getch();
						if(ch2=='Y'||ch2=='y')
						{
							closegraph();
							clrscr();
							exit(0);
						}
						else if(ch2=='N'||ch2=='n')
							goto Startgame;
					}
					
				}
			  	switch(ch)
			  	{
			    		case 'a':
			    		case 'A':
			       			if(j>0&&bg[i][j-1]==0)
			       			{
			            			if(x>step)
			            			{
			            				x-=step;
			            				j--;
			            			}
			       			}
			        		break;
					case 's':
					case 'S':
					        if(i<M-1&&bg[i+1][j]==0)
					        {
					            if(y<479-step)
					            {
					            	y+=step;
					            	i++;
					            }
					        }
					        break;
					case 'd':
					case 'D':
					        if(j<N-1&&bg[i][j+1]==0)
					        {
					            if(x<639-step)
					            {
					            	x+=step;
					            	j++;
					            }
					        }
					        break;
					case 'w':
					case 'W':
					        if(i>0&&bg[i-1][j]==0)
					        {
					            if(y>step)
					            {
					            	y-=step;
					            	i--;
					            }
					        }
					        break;
					default :break;
		  		}
		  		drawman(x,y,len);
		 		delay(800);
		 		if(i>=M-1&&j>=N-1)
		 		{
		    			setcolor(LIGHTGREEN);
					rectangle(450,300,620,380);
		    			setfillstyle(SOLID_FILL,LIGHTGRAY);
					floodfill(480,360,LIGHTGREEN);
		    			settextstyle(0,0,1);
		    			setcolor(MAGENTA);
					outtextxy(455,325,"You won the game!");
					outtextxy(455,355,"Press Q to quit...");
		    			while(1)
		    			{
						ch2=getch();
						if(ch2=='Q'||ch2=='q')
							break;
					}
					closegraph();
					clrscr();
					exit(0);
		    			/*
		    			settextstyle(4,0,3);
		    			setcolor(RED);
		    			outtextxy(150,260,"YOU WIN!");*/
		 		}
			}
			
		}/* 人工控制结束 */
		
		else if(ch=='C'||ch=='c')
		{
			/* 电脑控制 */
			/* direc表示上一步运动方向 */
			/* 并表示下一步运动方向 */
			/* 0~3分别表示 西、北、东、南 */
			gotoxy(60,3);
			printf("Computer now");
			gotoxy(60,6);
			printf("Run the maze");
			gotoxy(60,9);
			printf("Automatically...");
			direc=2;
			i=j=0;
			while(i<M-1||j<N-1)
			{
			    	delay(80000);
			    	drawman(x,y,len);
			   	switch(direc)
			    	{
			    		case 0:
			        		/* 以3,0,1的次序尝试 */ 
			        		if(i<M-1&&bg[i+1][j]==0){
			            			y+=step;i++;
			            			direc=3; 
			        		}    
			        		else if(j>0&&bg[i][j-1]==0){
			            			x-=step;j--;
			            			direc=0;            
			        		}
					        else if(i>0&&bg[i-1][j]==0){
					            y-=step;i--;
					            direc=1;            
					        }
					        else {
					            x+=step;j++;
					            direc=2;            
					        }              
					        break;
			    		case 1:
					        if(j>0&&bg[i][j-1]==0){
					            x-=step;j--;
					            direc=0;            
					        }
					        else if(i>0&&bg[i-1][j]==0){
					            y-=step;i--;
					            direc=1;            
					        }
					        else if(j<N-1&&bg[i][j+1]==0){
					            x+=step;j++;
					            direc=2;            
					        }
					        else{
					            y+=step;i++;
					            direc=3; 
					        }                      
					        break;
					case 2:
					        if(i>0&&bg[i-1][j]==0){
					            y-=step;i--;
					            direc=1;  
					        }    
					        else if(j<N-1&&bg[i][j+1]==0){
					            x+=step;j++;
					            direc=2;            
					        }
					        else if(i<M-1&&bg[i+1][j]==0){
					            y+=step;i++;
					            direc=3; 
					        } 
					        else {
					            x-=step;j--;
					            direc=0;
					        }   
					        break;
					case 3:
					        if(j<N-1&&bg[i][j+1]==0){
					            x+=step;j++;
					            direc=2;            
					        }
					        else if(i<M-1&&bg[i+1][j]==0){
					            y+=step;i++;
					            direc=3; 
					        }
					        else if(j>0&&bg[i][j-1]==0){
					            x-=step;j--;
					            direc=0;
					        } 
					        else {
					            y-=step;i--;
					            direc=1;  
					        }
					        break;
					default :break;
			    	}
			    	drawman(x,y,len);
			}
			gotoxy(60,12);
			printf("Finished!");
			gotoxy(60,15);
			printf("Press any");
			gotoxy(60,18);
			printf("Key to quit...");
			getch();
			closegraph();
			clrscr();
			exit(0);
		}/* 电脑控制结束 */
	}
}/* main()结束 */

/* 绘制小人 */
void drawman(int x,int y,int len){
    int r=len/4;
    rect(x-r,y-len,x+r,y-len+2*r);
    line(x,y-len+2*r,x,y);
    line(x-len,y,x+len,y);
    line(x,y,x-len,y+len);
    line(x,y,x+len,y+len);
}
/* 绘制迷宫地图 */
void drawbg(int bg[][N],int a,int b,int size,int x,int y){
    int startx=x;
    int i,j;
    for(i=0;i<a;i++){
        for(j=0;j<b;j++){
            if(bg[i][j]==1)
                rect(x,y,x+size-1,y+size-1);
            x+=size;
        }
        x=startx;
        y+=size;
    }
    rectangle(0,0,size*b,size*a);
    line(0,0,size,0);line(0,0,0,size);
    line(size*b,size*(a-1),size*b,size*a);
    line(size*(b-1),size*a,size*b,size*a);
}
/* 绘制实心矩形 */
void rect(int x0,int y0,int x1,int y1){
    int i,j;
    for(i=x0;i<=x1;i++)
        line(i,y0,i,y1);
}

/* 随机生成代表迷宫地图的数组  */
void makebg(int a,int b){
    int i,j;
    int ran;
    int direc;
/* 初始化迷宫地图  */
    for(i=0;i<a;i++)
        for(j=0;j<b;j++)
            bg[i][j]=1;

/* 随机生成迷宫通路  */
    randomize();
    i=j=0;direc=2;
    while(1){
        bg[i][j]=0;
        if(i>=M-1&&j>=N-1)break;
        ran=(int)rand()*4;
        if(ran<1){
            if(direc!=1&&i<a-1){
                i++;
                direc=3;
            }
        }    
        else if(ran<2){
            if(direc!=2&&j>0){
                j--;
                direc=0;
            }
        }
        else if(ran<3){
            if(direc!=3&&i>0){
                i--;
                direc=1;
            }
        }
        else {
            if(direc!=0&&j<b-1){
                j++;
                direc=2;            
            }
        }
     }
/* 随机生成迷宫其余部分  */
    for(i=0;i<a;i++)
        for(j=0;j<b;j++)
            if(bg[i][j]==1){
                ran=(int)rand()*10;
                if(ran<7)bg[i][j]=0;
            }
}

 
 
