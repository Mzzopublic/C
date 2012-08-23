
#include <stdio.h>
#define N 81 
fun ( char *s ) 
{
	strrev(s); 
} 
main( ) 
{ char a[N]; 
printf ( "Enter a string : " ); gets ( a ); 
printf ( "The original string is : " ); puts( a ); 
fun ( a ); 
printf("\n"); 
printf ( "The string after modified : "); 
puts ( a ); 
}
