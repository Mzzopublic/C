#include <stdio.h>
int func(char s[],int c)
{
    char *q=s;
    for(; *q; q++)
    if(*q != c) *(s++)=*q;
    *s='\0';
}
void main()
{
    static char str[]="Turbo c and borland c++";
    char ch;
    clrscr() ;	printf(" The string before delete is %s.\n",str);
    printf(" Please input the char to delete : ");	
    scanf("%c",&ch);
    func(str,ch);
    printf(" The string after delete is %s.\n",str);
    printf(" Press any key to quit...");
    getch();
    return;
}
