#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

void main(void)
{
	  struct CMOS
	  {
	     unsigned char current_second;
	     unsigned char alarm_second;
	     unsigned char current_minute;
	     unsigned char alarm_minute;
	     unsigned char current_hour;
	     unsigned char alarm_hour;
	     unsigned char current_day_of_week;
	     unsigned char current_day;
	     unsigned char current_month;
	     unsigned char current_year;
	     unsigned char status_registers[4];
	     unsigned char diagnostic_status;
	     unsigned char shutdown_code;
	     unsigned char drive_types;
	     unsigned char reserved_x;
	     unsigned char disk_1_type;
	     unsigned char reserved;
	     unsigned char equipment;
	     unsigned char lo_mem_base;
	     unsigned char hi_mem_base;
	     unsigned char hi_exp_base;
	     unsigned char lo_exp_base;
	     unsigned char fdisk_0_type;
	     unsigned char fdisk_1_type;
	     unsigned char reserved_2[19];
	     unsigned char hi_check_sum;
	     unsigned char lo_check_sum;
	     unsigned char lo_actual_exp;
	     unsigned char hi_actual_exp;
	     unsigned char century;
	     unsigned char information;
	     unsigned char reserved3[12];
	} cmos;

	char i;
	char *pointer;
	char byte;

	pointer = (char *) &cmos;

	for (i = 0; i < 0x34; i++)
	{
	      outportb(0x70, i);
	      byte = inportb(0x71);
	      *pointer++ = byte;
	}

	clrscr();
	printf(" This program is to get the CMOS infomation.\n");
	printf(" Some of the CMOS information is as follows.\n");
	printf(" >> Current date: %d/%x/%x", cmos.current_month,
	     cmos.current_day, cmos.century);
	if(cmos.current_year<10)
		printf("0%d.\n",cmos.current_year);
	else
		printf("%d.\n",cmos.current_year);
	printf(" >> Current time: %d:%d:%d.\n", cmos.current_hour,
	     cmos.current_minute, cmos.current_second);
	printf(" >> Shutdown type: %d.\n", cmos.shutdown_code);
	printf(" >> Hard disk type %d\n", cmos.fdisk_0_type);
	
	printf(" Press any key to quit...");
	getch();
	return;
  }
