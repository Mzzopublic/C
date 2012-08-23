#include <stdio.h>
#include <dos.h>
void main(void)
{
	struct country info;
	clrscr();
	puts(" This program is to get the country information:");
	country(0, &info);

	if (info.co_date == 0)
		printf(" >> Date format: mm/dd/yy\n");
	else if (info.co_date == 1)
		printf(" >> Date format: dd/mm/yy\n");
	else if (info.co_date == 2)
		printf(" >> Date format: yy/mm/dd\n");

	printf(" >> Currency symbol %s\n", info.co_curr);
	printf(" >> Decimal separator %s\n", info.co_thsep);
	printf(" >> Date separator %s Time separator %s\n",
		info.co_dtsep, info.co_tmsep);

	if (info.co_currstyle == 0)
		printf(" >> Currency symbol precedes with no leading spaces\n");
	else if (info.co_currstyle == 1)
		printf(" >> Currency symbol follows with no spaces\n");
	else if (info.co_currstyle == 2)
		printf(" >> Currency symbol precedes with leading space\n");
	if (info.co_currstyle == 4)
		printf(" >> Currency symbol follows with space\n");

	printf(" >> Currency significant digits %d\n", info.co_digits);

	if (info.co_time)
		printf(" >> 24 hour time\n");
	else
		printf(" >> 12 hour time\n");

	printf(" >> Data separator %s\n", info.co_dasep);
	printf(" Press any key to quit...");
	getch();
}


