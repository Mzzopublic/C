main()
{
	int x,y,z;	/* 定义三个int型变量 */
	int *xp = &x,	/* 定义指针变量xp,并赋值为x的地址，使xp指向x */
	    *yp = &y,	/* 定义指针变量yp,并赋值为y的地址，使yp指向y */
	    *zp = &z;	/* 定义指针变量zp,并赋值为z的地址，使zp指向z */
	int t;
	clrscr();
	printf("\nPlease input x,y,z:\n");
	scanf("%d%d%d",xp,yp,zp);	/* 通过变量的指针，为变量输入值 */
	if(*xp>*yp)	/* 通过指向变量的指针引用变量的值 */
	{
		t=*xp;	/* 通过指向变量的指针引用变量的值 */
		*xp=*yp;/* 通过指向变量x的指针xp,引用变量x的值 */
		*yp=t;	/* 通过指向变量y的指针yp,引用变量y的值 */
	}
	if(*xp>*zp)	/* 通过指向变量的指针,引用变量的值 */
	{
		t=*xp;	/* 通过指向变量x的指针xp,引用变量x的值 */
		*xp=*zp;/* 通过指向变量x的指针xp,引用变量x的值 */
		*zp=t;	/* 通过指向变量z的指针zp,引用变量z的值 */
	}
	if(*yp>*zp)	/* 通过指向变量的指针,引用变量的值 */
	{
		t=*yp;	/* 通过指向变量的指针,引用变量的值 */
		*yp=*zp;/* 通过指向变量y的指针yp,引用变量y的值 */
		*zp=t;/* 通过指向变量z的指针zp,引用变量z的值 */
	}
	printf("x = %d\ty = %d\tz = %d\n",x,y,z);
	printf("\nPress any key to quit...\n");
	getch();
}