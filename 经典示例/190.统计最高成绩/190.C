#include <stdio.h>
#include <string.h>
#define N 10
void readwritedat();
typedef struct ss{
char num[10];
int s;
}STU;
mmm(STU a[],int n,STU *s)
{
	int i;
	s->s=a[0].s;
	for(i=1;i<n;i++)
		if(a[i].s>s->s)
			*s=a[i];
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
void sort(STU a[],int n)
{
	STU t;
	int i,j;
	for(i=0;i<n-1;i++)
		for(j=i+1;j<n;j++)
			if(a[i].s<a[j].s)
			{
				strcpy(t.num,a[i].num);
				t.s=a[i].s;
				strcpy(a[i].num,a[j].num);
				a[i].s=a[j].s;
				strcpy(a[j].num,t.num);
				a[j].s=t.s;
			}

}
main()
{
	STU a[N]={{"ST01",81},{"ST02",89},{"ST03",66},{"ST04",87},{"ST05",77},
	{"ST06",90},{"ST07",79},{"ST08",61},{"ST09",80},{"ST10",71}},m;
	int i;
	clrscr();
	for(i=0;i<N;i++)
		printf(" No: %s Mark: %2d\n",a[i].num,a[i].s);
	mmm(a,10,&m);
	printf(" The highest is %s,%d.\n",m.num,m.s);
	readwritedat();
	PressKeyToQuit();
}
void readwritedat()
{
	FILE *rf,*wf;
	STU a[100],m;
	int i;
	rf=fopen("in189.dat","r");
	wf=fopen("out189.dat","w");
	for(i=0;i<100;i++)
		fscanf(rf,"%s%d",a[i].num,&a[i].s);
	mmm(a,100,&m);
	fprintf(wf,"the top: %s,%d\n",m.num,m.s);
	sort(a,100);
	for(i=0;i<100;i++)
		fprintf(wf,"%s,%d\n",a[i].num,a[i].s);
	fclose(rf);
	fclose(wf);
}

