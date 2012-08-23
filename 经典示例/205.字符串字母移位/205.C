#include <stdio.h> 
#define  N 81 

void chg(char *s) 
{while(*s) 
if(*s=='z'||*s=='Z') {*s-=25; s++;} 
else if(*s>='a'&&*s<='y') {*s+=1;s++;} 
else if(*s>='A'&&*s<='Y') {*s+=1;s++;} 
else s++; 
} 

main( ) 
{ 
	char a[N]; 
	
	printf("Enter a string : "); gets(a); 
	printf("The original string is : "); puts(a); 
	chg(a); 
	printf("The string after modified : "); 
	puts (a); 
}  
