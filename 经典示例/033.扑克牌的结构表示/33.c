enum suits{CLUBS,DIAMONDS,HEARTS,SPADES};
struct card
{
	enum suits suit;
	char value[3];
};
struct card deck[52];
char cardval[][3]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
char suitsname[][9]={"CLUBS","DIAMONDS","HEARTS","SPADES"};

main()
{
	int i,j;
	enum suits s;
	clrscr();
	for(i=0;i<=12;i++)
		for(s=CLUBS;s<=SPADES;s++)
		{
			j=i*4+s;
			deck[j].suit=s;
			strcpy(deck[j].value,cardval[i]);
		}
	for(j=0;j<52;j++)
		printf("(%s%3s)%c",suitsname[deck[j].suit],deck[j].value,j%4==3?'\n':'\t');
	puts("\nPress any key to quit...");
	getch();
}
