/* 字符排版函数,将字符行内单字之间的空白字符平均分配插入到单字之间*/
#define N 80
edit(char *s)
{
	int i,sp,w,inw,v,r;
	char buf[N],*str;
	for(inw=sp=w=i=0;s[i];i++)
	{
		if(s[i]==' ')
		{		/* 统计空白个数*/
			sp++;
			inw=0;	/* 置空白符状态*/
		}
		else if(!inw)
		{
			w++;	/* 统计单字个数*/
			inw=1;	/* 置单字状态*/
		}
	}
	if(w<=1)
		return;	/* 单字数不超过1, 不排版 */
	v=sp/(w-1);	/* 每个间隔平均空白符 */
	r=sp%(w-1);	/* 多余的空白符 */
	strcpy(buf,s);
	for(str=buf;;)
	{
		while(*str==' ')str++; /* 掠过空白符 */
		for(;*str&&*str!=' ';) /* 复制单字 */
			*s++=*str++;
		if(--w==0)
			return;		/* 全部单字复制完毕，返回 */
		for(i=0;i<v;i++)
			*s++=' ';	/* 插入间隔空白符 */
		if(r)
		{
			*s++=' ';	/* 插入一个多余空白符 */
			r--;
		}
	}
}
char buff[N];
main()		/* 用于测试edit函数 */
{
	clrscr();
	puts("This is a typeset program!\nPlease input a character line:\n");
	gets(buff);
	edit(buff);
	printf("\nThe character line after typeset is:\n\n%s\n",buff);
	puts("\n Press any key to quit...\n ");
	getch();
}

