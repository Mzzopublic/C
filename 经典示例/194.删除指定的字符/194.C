#include <stdio.h>
int fun(char s[],int c) 
{
char *q=s; 
for(; *q; q++) 
if(*q != c) *(s++)=*q; 
*s=0; 
} 
main() 
{ 
static char str[]="turbo c and borland c++"; 
char ch; 

printf("原始字符串:%s\n", str); 
printf("输入一个字符:"); 
scanf("%c",&ch); 
fun(str,ch); 
printf("str[]=%s\n",str); 
}
