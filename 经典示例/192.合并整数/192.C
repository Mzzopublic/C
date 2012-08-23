#include <stdio.h>

void fun(int a, int b, long *c) 
{
	*c=a/10*10+a%10*1000+b/10*100+b%10; 
} 
main() 
{ int a,b; long c; 
printf("Input a, b:"); 
scanf("%d%d", &a, &b); 
fun(a, b, &c); 
printf("The result is: %ld\n", c); 
} 
