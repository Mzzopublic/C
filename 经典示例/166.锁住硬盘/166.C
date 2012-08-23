#include<bios.h>
#include<stdio.h>
#include<conio.h>
int main(void)
{
	int result;
        char a='N';
	char buffer[512];
	clrscr();
	printf(" This is a hard disk lock program.\n");
        printf(" Do you want to lock your hard disk? (Y/N): ");
        scanf("%c",&a);
        if (a == 'Y'||a=='y')
        {
	  result = biosdisk(2,0x80,0,0,1,1,buffer);
          if(result)
	  {
		buffer[510] = 0x0;
		buffer[511] = 0x0;
		printf(" Fail to read main boot sector!\n");
          }
	  if(!result) result = biosdisk(3,0x80,0,0,1,1,buffer);
	  (!result)?(printf(" Writing main boot sector successfully!\n")):(printf(" Fail to write main boot sector!\n"));
	}
	printf(" Press any key to quit...");
   	getch();
   	return 0;
}