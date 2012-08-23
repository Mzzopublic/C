/*
 * New editor name:  tde, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * Show all available keys the function connected to each key.
 */

struct screen help_screen[] = {
 {1,0,"This utility reads the new help screen from a file.  The file must" },
 {2,0,"exist before running this utility.  The file should contain an image" },
 {3,0,"of the new help screen, which is 80 columns wide and 25 lines long." },
 {4,0,"A sample help file, sample.hlp, was included with this editor." },
 {5,0,"You may modify the sample help file to display your new key definitions"},
 {6,0,"or you may create your own help file.  The file name will be prompted." },
{10,20,"1. Install a new help screen."},
{12,20,"2. Exit"},
{14,15,"Please enter your choice: "},
{0,0,NULL}
};
