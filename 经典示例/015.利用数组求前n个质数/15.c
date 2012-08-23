#define N 50
main()
{
	int primes[N];
	int pc,m,k;

	clrscr();
	printf("\n The first %d prime numbers are:\n",N);
	primes[0]=2;/*2是第一个质数*/
	pc             =1;/*已有第一个质数*/
	m               =3;/*被测试的数从3开始*/
	while(pc<N)
	{
	 /*调整m使它为下一个质数*/
	k=0;
	while(primes[k]*primes[k]<=m)
		if(m%primes[k]==0)
		{/*m是合数*/
		    m+=2;/*让m取下一个奇数*/
		    k=1;/*不必用primes[0]=2去测试m，所以k从一开始*/
		}
		else
		k++;/*继续用下一个质数去测试*/
	primes[pc++]=m;
	m+=2;/*除2外，其余质数均是奇数*/
	}
	/*输出primes[0]至primes[pc-1]*/
	for(k=0;k<pc;k++)
		printf("%4d",primes[k]);
	printf("\n\n Press any key to quit...\n ");
	getch();
}