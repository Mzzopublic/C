#include <stdio.h> 
int cnt,sum; 

void countValue() 
{ int s,i,x,n,e,six,nine; 
for(s=1;s<10;s++) 
for(i=0;i<10;i++) 
for(x=0;x<10;x++) 
for(n=1;n<10;n++) 
for(e=0;e<10;e++) 
{ six=s*100+i*10+x; 
nine=n*1000+i*100+n*10+e; 
if(3*six==2*nine) 
{ cnt++;sum+=six+nine;} 
} 
} 
void main() 
{ 
	cnt=sum=0;  
	countValue(); 
	printf("满足条件的个数=%d\n",cnt); 
	printf("满足条件所有的SIX与NINE的和=%d\n",sum); 
}  
