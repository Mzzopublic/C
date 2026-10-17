/* */
#include <stdio.h>
int main()//类型呢？
{
	int a=5,b,c,i=10;
	b=a++;
	c=++b;
	printf("a = %d, b = %d, c = %d\n",a,b,c);
	printf("i,i++,i++ = %d,%d,%d\n",i,i++,i++);
	printf("%d\n",++i);
	printf("%d\n",--i);
	printf("%d\n",i++);
	printf("%d\n",i--);
	printf("%d\n",-i++);
	printf("%d\n",-i--);
	getchar();
	return 0;//返回值必须为0，还有你真不用个for写这个吗
}
