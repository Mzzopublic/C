#include <stdio.h>
#include <dos.h>
#include <stdlib.h>

void main(void)
{
	union REGS inregs, outregs;
	struct SREGS segs;
	int i, j;
	int structure_size;
	struct SystemTableEntry
	{
		struct SystemTableEntry far *next;	/*Next SFT entry*/
		unsigned file_count;			/*Files in table*/
		unsigned handle_count;			/*Handles to this file*/
		unsigned open_mode;			/*File open mode*/
		char file_attribute;			/*Attribute byte*/
		unsigned local_remote;			/*Bit 15 set means remote*/
		unsigned far *DPD;			/*Drive parameter block*/
		unsigned starting_cluster;			
		unsigned time_stamp;
		unsigned date_stamp;
		long file_size;
		long current_offset;
		unsigned relative_cluster;
		long directory_sector_number;
		char directory_entry_offset;
		char filename_ext[11];			/*No period, space padded*/
							/*Ignore SHARE fields for example*/
	} far *table_ptr, far *file;
	long far *system_table;
	
	/*Get DOS version*/
	inregs.x.ax = 0x3001;
	intdos (&inregs, &outregs);
	if (outregs.h.al < 3)
	{
		printf("This program requires DOS version 3 or later\n");
		exit (1);
	}
	else if (outregs.h.al == 3)
		structure_size = 0x35;
	else if (outregs.h.al >= 4)
		structure_size = 0x3B;
	/*Get the list of losts pointer*/
	inregs.h.ah = 0x52;
	intdosx (&inregs, &outregs, &segs);
	/*The pointer to the system file table is at offset 4*/
	system_table = MK_FP(segs.es, outregs.x.bx + 4);
	table_ptr = (struct SystemTableEntry far *) *system_table;
	do
	{
		printf("%d entries in table\n", table_ptr->file_count);
		for (i = 0; i < table_ptr->file_count; i++)
		{
			file = MK_FP(FP_SEG(table_ptr), FP_OFF(table_ptr) +
				(i * structure_size));
			if (file->handle_count)
			{
				for (j = 0; j < 8; j++)
					if (file->filename_ext[j] != ' ')
						putchar(file->filename_ext[j]);
					else
						break;
					if (file->filename_ext[8] != ' ')
						putchar('.');
					for (j = 8; j < 11; j++)
						if (file->filename_ext[j] != ' ')
							putchar(file->filename_ext[j]);
						printf (" %ld bytes %x attribute %d references\n",
							file->file_size, file->file_attribute,
							file->handle_count);
			}
		}
		table_ptr = table_ptr->next;
	} while (FP_OFF(table_ptr) != 0xFFFF);
	printf(" Press any key to quit...");
	getch();
	return;
}

