#include <stdio.h>
int jsvalue(int bb[])
{
	int I,j,k=0;
	int hun,ten,data;
	for(I=100;I<=999;I++)
	{
		j=10;
		while(j*j<=I)
		{
			if (I==j*j)
			{
				hun=I/100;data=I-hun*100;
				ten=data/10;data=data-ten*10;
				if(hun==ten||hun==data||ten==data)
				{
					bb[k]=I;
					k++;
				}
			}
			j++;
		} 
	}
	return k;
}
void PressKeyToQuit()
{
	printf("\n Press any key to quit...");
	getch();
	return;
}
main()
{
	int b[20],num;
	clrscr();
	puts(" This program is to find the Perfect Square Numbers.");
	puts(" which have 3 digits, and 2 of them are the same.");
	puts(" These numbers are as follows:");
	num=jsvalue(b);
	writeDat(num,b);
	PressKeyToQuit();
}
writeDat(int num,int b[])
{
	FILE *out;
	int i;
	out=fopen("out180.dat","w");
	fprintf(out,"%d\n",num);
	for(i=0;i<num;i++)
	{
		fprintf(out,"%d\n",b[i]);
		printf(" %d",b[i]);
	}
	fclose(out);
}
