/* 函数trans将无符号整数n翻译成d（2<＝d<＝16）进制表示的字符串s */
#define M sizeof(unsigned int)*8
int trans(unsigned n, int d, char s[])
{
	static char digits[] ="0123456789ABCDEF"; /* 十六进制数字的字符 */
	char buf[M+1];
	int j, i = M;
	if(d<2||d>16)
	{
		s[0]='\0';	/* 不合理的进制，置s为空字符串 */
		return 0;	/* 不合理的进制，函数返回0 */
	}
	buf[i]='\0';
	do
	{
		buf[--i]=digits[n%d];	/*译出最低位，对应字符存入对应工作数组中*/
		n/=d;
	}while(n);
	/* 将译出在工作数组中的字符串复制到s */
	for(j=0;(s[j]=buf[i])!='\0';j++,i++);
		/* 其中控制条件可简写成s[j]=buf[i] */
	return j;
}
/* 主函数用于测试函数 trans() */
main()
{
	unsigned int num = 253;
	int scale[]={2,3,10,16,1};
	char str[33];
	int i;
	clrscr();
	for(i=0;i<sizeof(scale)/sizeof(scale[0]);i++)
	{
		if(trans(num,scale[i],str))
			printf("%5d = %s(%d)\n",num,str,scale[i]);
		else
			printf("%5d => (%d) Error! \n",num,scale[i]);
	}
	printf("\n Press any key to quit...\n");
	getch();
}