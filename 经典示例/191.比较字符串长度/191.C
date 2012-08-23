#include <stdio.h>

char *fun ( char *s, char *t) 
{
	char *ss=s, *tt=t; 
	while((*ss)&&(*tt)) 
	{ ss++; tt++; } 
	if(*tt) return(t); 
	else return(s); 
} 

main( ) 
{ char a[20],b[10],*p,*q; 
int i; 
printf("Input 1th string:") ; 
gets( a); 
printf("Input 2th string:") ; 
gets( b); 
printf("%s\n",fun (a, b )); 
}
