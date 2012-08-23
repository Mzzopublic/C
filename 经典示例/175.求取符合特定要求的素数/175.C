#include <conio.h>
#include <stdio.h>
int isP(int m)
{ 
	int i ;
	for(i = 2 ; i < m ; i++)
	if(m % i == 0) return 0 ;
	return 1 ;
}
void num(int m,int k,int xx[])
{
	int i=0;
	for(m=m+1;k>0;m++)
	if(isP(m))
	{ 
		xx[i++]=m;
		k--; 
	}
}
void readwriteDAT()
{
	int m, n, xx[1000], i;
	FILE *rf, *wf ;
	rf = fopen("in171.dat", "r");
	wf = fopen("out171.dat", "w");
	for(i = 0 ; i < 10 ; i++)
	{
		fscanf(rf,"%d%d",&m,&n);
		num(m,n,xx) ;
		for(m=0;m < n ; m++)
			fprintf(wf, "%d ", xx[m]);
		fprintf(wf, "\n");
	}
	fclose(rf);
	fclose(wf);
}
void main()
{
	int m, n, xx[1000] ;
	clrscr() ;
	puts(" This program is to get k prime numbers which are larger than m.");
	printf(" >> Please input two integers to m and k : ") ;
	scanf("%d%d", &m, &n ) ;
	num(m, n, xx) ;
	printf(" >> The %d prime numbers which are larger than %d are:\n ",n,m);
	for(m = 0 ; m < n ; m++)
		printf(" %d ", xx[m]) ;
	readwriteDAT();
	printf("\n Press any key to quit...") ;
	getch();
	return;
}


