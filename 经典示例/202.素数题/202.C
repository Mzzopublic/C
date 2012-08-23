#include <stdio.h> 
int cnt,sum; 
int isPrime(int number) 
{ 
	int i,tag=1; 
	
	for(i=2;tag && i<=number / 2; i++) 
		if(number % i ==0) tag =0; 
		return tag; 
} 

void countValue() 
{int i; 
for(i=2;i<=90;i++) 
if(isPrime(i)&&isPrime(i+4)&&isPrime(i+10)) 
{cnt++;sum+=i;} 
} 

void main() 
{ 
	cnt=sum=0; 
	countValue(); 
	printf("满足条件的整数的个数＝%d\n",cnt); 
	printf("满足条件的整数的和值＝%d\n",sum); 
}
