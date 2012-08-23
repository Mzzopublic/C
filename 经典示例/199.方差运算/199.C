#include <stdio.h> 
#define MAX 4 
int xx[MAX],odd=0,even=0; 
double ave1=0.0,ave2=0.0,totfc=0.0; 

void Compute(void) 
{ int i,yy[MAX]; 
for(i=0;i<MAX;i++) 
yy[i]=0; 
for(i=0;i<MAX;i++) 
if(xx[i]%2) { yy[odd++]=xx[i];  ave1+=xx[i];} 
else { even++;   ave2+=xx[i];} 
if(odd==0) ave1=0; 
else ave1/=odd; 
if(even==0) ave2=0; 
else ave2/=even; 
for(i=0;i<odd;i++) 
totfc+=(yy[i]-ave1)*(yy[i]-ave1)/odd; 
} 

void main() 
{ 
	int i; 
	Compute(); 
	printf("输入要运算的十进制整数：\n");
	scanf("%d",&i);
	printf("ODD=%d\nAVE1=%f\nAVE2=%f\nTOTFC=%f\n",odd,ave1,ave2,totfc); 
} 
