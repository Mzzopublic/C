#include <stdio.h>
#define MAX 50
/* 函数rep实现对s中出现的s1中的字符替换为s2中相应的字符 */
rep(char *s,char *s1,char *s2)
{
	char *p;

	for(;*s;s++)/*顺序访问字符串s中的每个字符*/
	{
		for(p=s1;*p&&*p!=*s;p++);/*检查当前字符是否在字符串s1中出现*/
		if(*p)*s=*(p-s1+s2);/*当前字符在字符串s1中出现，用字符串s2中的对应字符代替s中的字符*/
	}
}
main( )/*示意程序*/
{
	char s[MAX];/*="ABCABC";*/
	char s1[MAX],s2[MAX];
	clrscr();
	puts("Please input the string for s:");
	scanf("%s",s);
	puts("Please input the string for s1:");
	scanf("%s",s1);
	puts("Please input the string for s2:");
	scanf("%s",s2);

	rep(s,s1,s2);
	puts("The string of s after displace is:");
	printf("%s\n",s);
	puts("\n Press any key to quit...");
	getch();
}
