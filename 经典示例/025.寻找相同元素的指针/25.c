/*[函数]在已知两个从小到大有序的数表中寻找都出现的第一个元素的指针*/
#define NULL 0
int *search2(int *pa,int *pb,int an,int bn)
{
	int *ca,*cb;

	ca=pa;cb=pb;/*为ca、cb设定初值*/
	while(ca<pa+an&&cb<pb+bn)/*两个数表都未考察完*/
	{
	/*在两个数表中找下一个相等的元素*/
		if(*ca<*cb)/*数表1的当前元素<数表2的当前元素*/
		    ca++;/*调整数表1的当前元素指针*/
		else if(*ca>*cb)/*数表1的当前元素>数表2的当前元素*/
		    cb++;/*调整数表2的当前元素指针*/
			else	/*数表1的当前元素==数表2的当前元素*/
				/*在前两个数表中找到相等元素*/
			return ca;/*返回在这两个数表中找到相等元素*/
	}
	return NULL;
}

main( )/*只是为了引用函数search2( )*/
{
	int *vp,i;
	int a[ ]={1,3,5,7,9,13,15,27,29,37};
	int b[ ]={2,4,6,8,10,13,14,27,29,37};
	clrscr();
	puts("The elements of array a is:");
	for(i=0;i<sizeof(a)/sizeof(a[0]);i++)
		printf(" %d",a[i]);
	puts("\nThe elements of array b is:");
	for(i=0;i<sizeof(b)/sizeof(b[0]);i++)
		printf(" %d",b[i]);
	vp=search2(a,b,sizeof a/sizeof a[0],sizeof b/sizeof b[0]);
	if(vp) printf("\nThe first same number in both arrays is %d\n",*vp);
	else printf("Not found!\n");
	puts("\n Press any key to quit...\n");
	getch();

}
