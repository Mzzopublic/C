#include <stdio.h>
#define MAX 200
int a[MAX], b[MAX], cnt = 0;
void jsVal()
{
	int bb[4];
	int I,j,k,flag;
	for (I=0;I<200;I++)
	{
		bb[0]=a[I]/1000; 
		bb[1]=a[I]%1000/100;
		bb[2]=a[I]%100/10; 
		bb[3]=a[I]%10;
		for (j=0;j<4;j++)
		{
			if (bb[j]%2==0)
				flag=1;
			else 
			{
				flag=0;
				break;
			}
		}
		if (flag==1)
		{ 
			b[cnt]=a[I];
			cnt++;
		}
	}
	for(I=0;I<cnt-1;I++)
		for(j=I+1;j<cnt;j++)
			if (b[I]<b[j])
			{
				k=b[I];
				b[I]=b[j];
				b[j]=k;
			}
}
void readDat()
{ 
	int i ;
	FILE *fp ;
	fp = fopen("IN172.DAT", "r") ;
	for(i = 0 ; i < MAX ; i++) 
		fscanf(fp, "%d", &a[i]) ;
	fclose(fp) ;
}
void main()
{
	int i,j;
	clrscr();
	readDat() ;
	jsVal() ;
	printf(" The number of the satisfied integers is %d.\n ", cnt) ;
	for(i = 0,j=0 ; i < cnt ; i++)
	{
		printf("%d ", b[i]) ;
		if(j==9)
		{
			printf("\n ");
			j=0;
		}
		else
			j++;
	}
	printf("\n") ;
	writeDat() ;
	printf(" Press any key to quit...");
	getch();
}
writeDat()
{ 
	FILE *fp ;
	int i ;
	fp = fopen("OUT172.DAT", "w") ;
	fprintf(fp, "%d\n", cnt) ;
	for(i = 0 ; i < cnt ; i++) 
		fprintf(fp, "%d\n", b[i]) ;
	fclose(fp) ;
}
