#include <stdio.h> 
int b[3]; 
jsValue() 
{int a1=1,a2=1,a12,sn,k=2; 
sn=a1+a2; 
while(1) 
{a12=a1+2*a2; 
if(sn<100&&sn+a12>=100) b[0]=k; 
if(sn<1000&&sn+a12>=1000) b[1]=k; 
if(sn<10000&&sn+a12>=10000) {b[2]=k;break;} 
sn=sn+a12; 
a1=a2;a2=a12; 
k++; 
} 
} 

main() 
{ 
	jsValue(); 
	printf("M=100,n=%d\nM=1000,n=%d\nM=10000,n=%d\n",b[0],b[1],b[2]); 
}  
