#include <stdio.h>  
int findStr(char *str,char *substr) 
{ int  n=0; 
char  *p , *r; 
while ( *str ) 
{p=str; 
r=substr; 
while(*r) 
if(*r==*p) {  r++;  p++;  } 
else  break; 
if(*r=='\0') 
n++; 
str++;    
} 
return  n; 
} 

main() 
{ 
	char str[81], substr[3] ; 
	int n ; 
	printf("ÊäÈëÔ­×Ö·û´®:") ; 
	gets(str) ; 
	printf("ÊäÈë×Ó×Ö·û´®:") ; 
	gets(substr) ; 
	puts(str) ; 
	puts(substr) ; 
	n=findStr(str, substr) ; 
	printf("n=%d\n", n) ; 
}
