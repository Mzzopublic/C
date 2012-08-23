/* draw Parspl */
#include <graphics.h>
#include <math.h>
#include <stdio.h>

void parspl(p,n,k,e)
int p[][2],n,k,e;
{
	int x,y,i,j,m;
	float t1,t2,t3,t,a,b,c,d;
	if(e==1)
	{
		m=n;
		p[0][0]=p[1][0];
		p[0][1]=p[1][1];
		p[n+1][0]=p[n][0];
		p[n+1][1]=p[n][1];
	}
	else
	{
		m=n+1;
		p[0][0]=p[n][0];
		p[0][1]=p[n][1];
		p[m][0]=p[1][0];
		p[m][1]=p[1][1];
		p[m+1][0]=p[2][0];
		p[m+1][1]=p[2][1];
	}
	t=0.5/k;
	moveto(p[1][0],p[1][1]);
	for(i=0;i<m-1;i++)
	{
		for(j=1;j<k;j++)
		{
			t1=j*t;
			t2=t1*t1;
			t3=t2*t1;
			a=4.0*t2-t1-4.0*t3;
			b=1.0-10.0*t2+12.0*t3;
			c=t1+8.0*t2-12.0*t3;
			d=4.0*t3-2.0*t2;
			x=a*p[i][0]+b*p[i+1][0]+c*p[i+2][0]+d*p[i+3][0];
			y=a*p[i][1]+b*p[i+1][1]+c*p[i+2][1]+d*p[i+3][1];
			lineto(x,y);
		}
		lineto(p[i+2][0],p[i+2][1]);
	}

}

void marking(a,n)
int a[][2],n;
{
	int i;
	setfillstyle(1,WHITE);
	for(i=1;i<=n;i++)
	{
		circle(a[i][0],a[i][1],2);
		floodfill(a[i][0],a[i][1],RED);
	}
	
}

void main()
{
	int i,n,c,p[50][2],px,py;
	char fname[10],ch;
	FILE *fp;
	int gdriver=DETECT,gmode;
	clrscr();
	printf("Please input point numer: ");
	scanf("%d",&n);
	printf("Please input name of the file with the point data:\n >> ");
	scanf("%s",fname);
	if((fp=fopen(fname,"r"))==NULL)
	{
		printf("File %s does not exist! Do you want to create it? Y/N ",fname);
		ch=getch();
		if(ch=='Y'||ch=='y')
		{
			if((fp=fopen(fname,"w+"))==NULL)
			{
				printf("\nError! Can't create file %s!",fname);
				exit(1);
			}
			printf("\nPlease input %d pair coordinates (x,y) of the points:\n",n);
			for(i=0;i<n;i++)
			{
				scanf("%d%d",&px,&py);
				fprintf(fp,"%d %d ",px,py);
			}
		        rewind(fp);
		}
		else
			exit(1);
	}
	rewind(fp);
	for(i=1;i<=n;i++)
		fscanf(fp,"%d%d",&p[i][0],&p[i][1]);
	fclose(fp);
	printf("Please input the figure type: FREE--1, Close--2: ");
	scanf("%d",&c);

	initgraph(&gdriver,&gmode,"c:\\tc");
	cleardevice();
	setbkcolor(9);
	setcolor(4);
	marking(p,n);
	outtextxy(80,460,"Press any key to continue...");
	getch();
	parspl(p,n,10,c);
	outtextxy(80,460,"Press any key to quit...    ");
	getch();
	closegraph();
}
