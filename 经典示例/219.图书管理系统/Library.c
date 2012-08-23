# include<stdio.h>
# include<conio.h>
# include<dos.h>
# include<string.h>
# include<math.h>

# define TRUE 1
# define FALSE 0

struct member
{
   int mid;
   char mname[20],madd[30];
   struct msince
   {
    int day,mon,year;
   } ms;
   struct mexpir
   {
     int day,mon,year;
   } me;

} M;


struct book
{
   struct bkid
   {
      int gno,bno,no;
   } b;
   char bname[20],author[15],pub[20];
   int price;
} B;


struct transaction
{
   int mid,tid;
   struct bookid
   {
      int gno,bno,no;
   } b;
   struct issued
   {
    int day,mon,year;
   } i;

   struct returned
   {
    int day,mon,year;
   } r;

   float fine;
   char reason;
} T;

struct date d;

FILE *fm,*fb,*ft;

char *mainmenu[]={
		  "Member",
		  "Book",
		  "Reports",
		  "Help",
		  "Exit"
		};

char *memmenu[]={
		 "Add New Member",
		 "Renew Existing Member",
		 "Issue Duplicate I-Card",
		 "Back"
		};

char *bookmenu[]={
		  "Add New Books",
		  "Issue Book",
		  "Return Book",
		  "Back"
		 };


char *rptmenu[]={
		  "Members Details",
		  "Books Details",
		  "Transactions Details",
		  "Back"
		 };

void mainscr();
void showmenu(char **menu,int count,int sr,int sc);
int getchoice(char **menu,char *hotkeys,int r,int c,int count);

void addmember();
void renewmem();
void memdupid();
int memcheck(int id);

void addbook();
void issbook();
void retbook();
int  chkbook(int bno,int gno,int no);

void transac1(int mid,char ch);
void transac2(int mid,int bno,int gno,int no,char c);

void allmem();
void allbook();
void alltransac();

void showhelp();


/**************************** MAIN ******************************************/

void main()
{
   int ch,ch1,ch2,ch3;
   fm=fopen("member.dat","a+");
   ft=fopen("trans.dat","a+");
   fb=fopen("book.dat","a+");

   mainscr();

   while(1)
   {
    bk:
    showmenu(mainmenu,5,10,35);
    ch=getchoice(mainmenu,"MBRHE",10,35,5);
   switch(ch)
   {
    case 1:bk1:
	   showmenu(memmenu,4,10,35);
	   ch1=getchoice(memmenu,"ARIB",10,35,4);
	   switch(ch1)
	   {
	     case 1: addmember(); goto bk1;
	     case 2: renewmem();  goto bk1;
	     case 3: memdupid();  goto bk1;
	     case 4: goto bk;
	   }
	   break;
    case 2:bk2:
	   showmenu(bookmenu,4,10,35);
	   ch2=getchoice(bookmenu,"AIRB",10,35,4);
	   switch(ch2)
	   {
	     case 1: addbook(); goto bk2;
	     case 2: issbook(); goto bk2;
	     case 3: retbook(); goto bk2;
	     case 4: goto bk;
	   }
	   break;
    case 3:bk3:
	   showmenu(rptmenu,4,10,35);
	   ch3=getchoice(rptmenu,"MBTB",10,35,4);
	   switch(ch3)
	   {
	     case 1: allmem(); goto bk3;
	     case 2: allbook();goto bk3;
	     case 3: alltransac();goto bk3;
	     case 4: goto bk;
	   }
	   break;
    case 4:
	   showhelp(); break;
    case 5:fclose(ft); fclose(fm); fclose(fb);
	   exit();
   }
  }
}
/******************** Function Do Display The Main Screen *******************/

void mainscr()
{
  int i,j;
  clrscr();
  textcolor(BLUE);
  for(i=1;i<80;i++)
   for(j=1;j<24;j++)
   {
     gotoxy(i,j); cprintf("?);
   }
  textbackground(BLUE);
  textcolor(LIGHTMAGENTA);
   gotoxy(8,1);
   cprintf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
   gotoxy(8,2);
   cprintf("?                   ษอออออออออออออออออออออออออ?                ?);
   gotoxy(8,3);
   cprintf("ฬออออออออออออออออออออน  Welcome To Obaid Books ฬอออออออออออออออออ?);
   gotoxy(8,4);
   cprintf("?                   ศอออออออออออออออออออออออออ?                ?);
   gotoxy(8,5);
   cprintf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
   textcolor(RED);
   gotoxy(1,10);
   cprintf("Loading Files Please Wait...");
   textcolor(DARKGRAY);
   for(i=1;i<81;i++)
   {
     gotoxy(i,12);
     cprintf("?);
   }
   delay(200);
   textcolor(LIGHTGRAY);
    for(i=1;i<81;i++)
   {
     gotoxy(i,12);
     cprintf("?);
     delay(50);
   }
   textcolor(WHITE);
   for(i=1;i<81;i++)
   {
     gotoxy(i,12);
     cprintf("?);
     delay(20);
   }
  textcolor(WHITE);
}


/************************** The Showmenu function ***************************/
   void showmenu(char **menu,int count,int sr,int sc)
    {
       int i;  clrscr();
	   textcolor(LIGHTMAGENTA);
	   gotoxy(8,1);
	   cprintf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
	   gotoxy(8,2);
	   cprintf("?                   ษอออออออออออออออออออออออออ?                ?);
	   gotoxy(8,3);
	   cprintf("ฬออออออออออออออออออออน  Welcome To Obaid Books ฬอออออออออออออออออ?);
	   gotoxy(8,4);
	   cprintf("?                   ศอออออออออออออออออออออออออ?                ?);
	   gotoxy(8,5);
	   cprintf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

       for(i=0;i<count;i++)
	{
	  gotoxy(sc,sr++);  textcolor(DARKGRAY); cprintf(menu[i]);
	}
    }


/***********************Funtion To Get User's Choice*************************/
  int getchoice(char **menu,char *hotkeys,int r,int c,int count)
  {
   int choice=1,hotkeychoice=1,ascii,scan;
   char *s;
     while(1)
      {
	gotoxy(c,r+choice-1);  textcolor(WHITE+BLINK); cprintf(menu[choice-1]);
	while(!kbhit())
	;
	ascii=getch();
	if(ascii==0)
	     scan=getch();

	gotoxy(c,r+choice-1);  textcolor(DARKGRAY);    cprintf(menu[choice-1]);

	if(ascii==0)
	 {
		if(scan==80)
		    choice++;

		if(scan==72)
		choice--;

		if(choice==0)
		choice=count;

		if(choice>count)
		choice=1;
	  }
	 else
	  {
		if(ascii==13)
		return(choice);

		s=hotkeys;
		hotkeychoice=1;

		while(*s)
		 {
		   if(ascii==*s)
		   return(hotkeychoice);

		   hotkeychoice++;
		   s++;
		 }

	   }
       }
  }


/*********************Function To Add New Members****************************/
  void addmember()
  {
     clrscr();
     rewind(fm);
     while( fread(&M,sizeof(M),1,fm)==1)
     ;
     M.mid+=1;
     fseek(fm,0,SEEK_END);
     printf("\n\t Enter Name::");
     fflush(stdin);gets(M.mname);  M.mname[19]=' ';  M.mname[20]='\0';
     printf("\n\t Enter Add::");
     fflush(stdin);gets(M.madd);   M.madd[29]=' ';   M.madd[30]='\0';

     getdate(&d);
     M.ms.year=d.da_year;     M.ms.day=d.da_day;     M.ms.mon=d.da_mon;
     M.me.day=M.ms.day;       M.me.year=M.ms.year;   M.me.mon=(M.ms.mon+6);
     /*To Set Month After Dec To Jan*/
     if(M.me.mon>12)
      {
	M.me.year+=1;  M.me.mon=(M.me.mon-12);
      }
     /*If Say The Old Date is 31/12/2003 i.e 31 Dec Then   The New Date is 31/06/2004 i.e 31 Jun
     But Jun consists of Only 30 days so we shall add 1 day extra so the correct Date is 01/07/2004 */
     if(M.me.day==31)
     { /*Months Like Apr,Jun,Sep, & Nov have 30 days*/
       if(M.me.mon==4 || M.me.mon==6 || M.me.mon==9 || M.me.mon==11)
       {
	M.me.day=1;
	M.me.mon+=1;
       }

       /*For the Month of Feb there are 28 OR 29 days*/
       if(M.me.mon==2)
	{/*For Years like(1996,2000,2004 etc there are 29 days in Feb */
	 if( (M.me.year%4)==0)
	  {
	    M.me.day=31-29;   M.me.mon+=1;
	  }
	 else
	  {
	    M.me.day=31-28;  M.me.mon+=1;
	  }
	}
     }
     fwrite(&M,sizeof(M),1,fm);
     transac1(M.mid,'A');
  }


/**********************Function To Renew A Member****************************/
  void renewmem()
  {
     FILE *tt;
     int no,ch;
     clrscr();
     tt=fopen("temp1.dat","w");
     printf("\n\t Enter Membership Id::");  scanf("%d",&no);
     ch=memcheck(no);
     if(ch==FALSE)
     {
	printf("\n\t No Such Member....."); getch(); return;
     }
     rewind(fm);
     while( fread(&M,sizeof(M),1,fm)==1 )
      {
	  if(M.mid==no)
	    {
	      M.me.mon=(M.me.mon+6);
		if(M.me.mon>12 && M.me.mon!=12)
		      {
			M.me.year+=1;  M.me.mon=(M.me.mon-12);
		      }
		if(M.me.day==31)
		      {
			 if(M.me.mon==4 || M.me.mon==6 || M.me.mon==9 || M.me.mon==11)
			       {
					M.me.day=1;
					M.me.mon+=1;
			       }
			 if(M.me.mon==2)
			       {
				 if( (M.me.year%4)==0)
				  {
				    M.me.day=31-29;   M.me.mon+=1;
				  }
				 else
				  {
				    M.me.day=31-28;  M.me.mon+=1;
				  }
			}
	    }

	   fwrite(&M,sizeof(M),1,tt);
	}
	else
	  fwrite(&M,sizeof(M),1,tt);
      }
	   fclose(tt); fclose(fm);
	  remove("member.dat");
	  rename("temp1.dat","member.dat");
	  fm=fopen("member.dat","a+");
	  printf("\n\t Member Renewed..");
	  getch();
	  transac1(no,'E');
   }


/********************Function To Issue Duplicate Id Card*********************/
 void memdupid()
 {
     int no,ch;
     clrscr();
     printf("\n\t Enter Membership Id::");  scanf("%d",&no);
     ch=memcheck(no);
     if(ch==FALSE)
     {
	printf("\n\t No Such Member....."); getch(); return;
     }
     printf("\n\t Duplicate ID Issued....");
     getch();
     transac1(no,'D');
 }


/*************************Function To Check A Member*************************/

  int memcheck(int id)
  {
    rewind(fm);
    while(fread(&M,sizeof(M),1,fm)==1)
     {
       if(M.mid==id)
	return TRUE;
     }
     return FALSE;
  }


/*************Function To Record Transaction Made By Members*****************/

   void transac1(int mid,char ch)
   {
     rewind(ft);
     while(fread(&T,sizeof(T),1,ft) ==1)
     ;
     fseek(ft,0,SEEK_END);
     T.tid+=1; T.mid=mid;
     T.reason=ch;
     T.b.bno=0;  T.b.gno=0; T.b.no=0;
     getdate(&d);
     T.i.day=d.da_day;  T.i.mon=d.da_mon;  T.i.year=d.da_year;
     T.r.day=0;         T.r.mon=0;         T.r.year=0;
     if(ch=='A')
      T.fine=500;
     else if(ch=='E')
      T.fine=300;
     else if(ch=='D')
      T.fine=50;
      fwrite(&T,sizeof(T),1,ft);
   }


/*******************Function To Add Books To The Library*********************/

  void addbook()
   {
     int num,i;
     clrscr();
     rewind(fb);
     while( fread(&B,sizeof(B),1,fb)==1)
     ;
     B.b.gno+=1;     getdate(&d);    B.b.bno=d.da_year;
     fseek(fb,0,SEEK_END);
     printf("\n\t Enter No Of Books::");  scanf("%d",&num);
     fflush(stdin);
     printf("\n Enter Name::"); gets(B.bname); B.bname[19]=' '; B.bname[20]='\0';
     fflush(stdin);
     printf("\n Enter Author's Name::"); gets(B.author); B.author[14]=' '; B.author[15]='\0';
     fflush(stdin);
     printf("\n Enter Publisher's Name::"); gets(B.pub); B.pub[19]=' '; B.pub[20]='\0';
     fflush(stdin);
     printf("\n Enter Book Price::");  scanf("%d",&B.price);
     for(i=0;i<num;i++)
     {
       B.b.no=i+1;
       fwrite(&B,sizeof(B),1,fb);
     }
  }


/*******************Function To Issue Book To Members ***********************/

void issbook()
{
   int i,j,k,id;
   int f1,f2;
   clrscr();
   gotoxy(5,1); cprintf("Enter Book id::");
   gotoxy(9,2); cprintf("/");gotoxy(13,2); cprintf("/");

   gotoxy(5,2); scanf("%d",&i);
   gotoxy(10,2); scanf("%d",&j);
   gotoxy(14,2);scanf("%d",&k);
   f1=chkbook(i,j,k);
   if(f1==FALSE)
   {
     printf("\n\t No Such Book.....");   getch();   return;
   }

   printf("\n\t Enter Membership id::");
   scanf("%d",&id);
   f2=memcheck(id);
     if(f2==FALSE)
     {
	printf("\n\t No Such Member....."); getch(); return;
     }
    transac2(id,i,j,k,'I');
    printf("\n\t Book Issued....");
    getch();
}


/*****************Function To Return Book To The Library*********************/

void retbook()
{
   int i,j,k,id;
   int f1,f2,f3;
   f3=FALSE;
   clrscr();
   gotoxy(5,1); cprintf("Enter Book id::");
   gotoxy(9,2); cprintf("/");gotoxy(13,2); cprintf("/");

   gotoxy(5,2); scanf("%d",&i);
   gotoxy(10,2); scanf("%d",&j);
   gotoxy(14,2);scanf("%d",&k);
   f1=chkbook(i,j,k);
   if(f1==FALSE)
   {
     printf("\n\t No Such Book.....");   getch();   return;
   }
   printf("\n\t Enter Membership id::");
   scanf("%d",&id);
   f2=memcheck(id);
     if(f2==FALSE)
     {
	printf("\n\t No Such Member....."); getch(); return;
     }
   rewind(ft);
   while(fread(&T,sizeof(T),1,ft)==1)
   {
       if(T.mid==id && T.b.bno==i && T.b.gno==j && T.b.no==k && T.reason=='I')
	{  f3=TRUE; break;}
   }
   if(f3==TRUE)
   {
     transac2(id,i,j,k,'R');
   }
   else
   printf("\n\t No Such Transaction In The Records......");
}


/*******************Function To Check Bookid ********************************/

int  chkbook(int i,int j,int k)
{
  rewind(fb);
  while(fread(&B,sizeof(B),1,fb)==1)
  {
     if(B.b.bno==i && B.b.gno==j && B.b.no==k)
     {
       return TRUE;
     }
  }
  return FALSE;
}


/*************Function To Record Transaction Made On Books ******************/

void transac2(int mid,int bno,int gno,int no,char c)
{
     FILE *tt1;
     tt1=fopen("tempo","w");
     rewind(ft);

     while(fread(&T,sizeof(T),1,ft) ==1)
     ;


     T.tid+=1; T.mid=mid; T.reason=c;
     T.b.bno=bno;  T.b.gno=gno; T.b.no=no;
      getdate(&d);
      T.i.day=d.da_day;  T.i.mon=d.da_mon;  T.i.year=d.da_year;

     if(c=='I')
     {
       T.r.day=T.i.day;   T.r.mon=T.i.mon+1; T.r.year=T.i.year;
		if(T.r.mon>12 && T.r.mon!=12)
		      {
			T.r.year+=1;  T.r.mon=(T.r.mon-12);
		      }
		if(T.r.day==31)
		      {
			 if(T.r.mon==4 || T.r.mon==6 || T.r.mon==9 || T.r.mon==11)
			       {
					T.r.day=1;
					T.r.mon+=1;
			       }
			 if(T.r.mon==2)
			       {
				 if( (T.r.year%4)==0)
				  {
				    T.r.day=31-29;   T.r.mon+=1;
				  }
				 else
				  {
				    T.r.day=31-28;  T.r.mon+=1;
				  }
			}
	    }
     T.fine=0;
     fwrite(&T,sizeof(T),1,ft);
     }

     else if(c=='R')
     {
       rewind(ft);
       while(fread(&T,sizeof(T),1,ft)==1)
       {
	 if(T.mid==mid && T.b.bno==bno && T.b.gno==gno && T.b.no==no && T.reason=='I')
	 {
	   T.r.day=d.da_day; T.r.mon=d.da_mon; T.r.year=d.da_year;
	   T.reason='R';
	   printf("\n\t Enter Fine::"); scanf("%f",&T.fine);
	   fwrite(&T,sizeof(T),1,tt1);
	 }
	 else
	 {
	   fwrite(&T,sizeof(T),1,tt1);
	 }
       }
       fclose(ft);     fclose(tt1);
       tt1=fopen("tempo","r");
       rewind(tt1);
       ft=fopen("trans.dat","w");
       while(fread(&T,sizeof(T),1,tt1)==1)
       {
	fwrite(&T,sizeof(T),1,ft);
       }
       fclose(ft);
       ft=fopen("trans.dat","a+");
      }
}


/*************Function To Display All Details Of All Members*****************/

void allmem()
{
     clrscr();
     rewind(fm);

 printf("\n\t\t************* List Of All Members ****************   ");
 printf("\nษออหออออออออออออออออออหออออออออออออออออออออออออออหออออออออออออหออออออออออออออป");
 printf("\nบId?      Name       ?       Address           บMember SinceบExpiration    ?);
 printf("\nฬออฮออออออออออออออออออฮออออออออออออออออออออออออออฮออออออออออออฮออออออออออออออน");

 while(fread(&M,sizeof(M),1,fm)==1)
 {
   printf("\n?-2d?-18s?-26s?%-2d/%-2d/%-4d ? %-2d/%-2d/%-4d  ?,M.mid,M.mname,M.madd,M.ms.day,M.ms.mon,M.ms.year,M.me.day,M.me.mon,M.me.year);
 }
 printf("\nศออสออออออออออออออออออสออออออออออออออออออออออออออสออออออออออออสออออออออออออออผ");
   getch();
}


/*************Function To Display All Details Of All Books ******************/

void allbook()
{
     clrscr();
     rewind(fb);

  printf("\n\t\t************* List Of All Books ****************   ");
  printf("\nษออออออออหอออออออออออออออออออหออออออออออออออหอออออออออออออออออออหอออออออ?);
  printf("\n?BookId ?  Book Name       ?Author Name  ? Publisher's Name ?Price ?);
  printf("\nฬออออออออฮอออออออออออออออออออฮออออออออออออออฮอออออออออออออออออออฮอออออออ?);

 while(fread(&B,sizeof(B),1,fb)==1)
 {
  printf("\n?d%d%d  ?,B.b.bno,B.b.gno,B.b.no);
  printf("%-19s?-14s?-19s?d    ?,B.bname,B.author,B.pub,B.price);
  }
  printf("\nศออออออออสอออออออออออออออออออสออออออออออออออสอออออออออออออออออออสอออออออ?);
     getch();
}


/*************Function To Display All Details Of All Transactions ***********/

void alltransac()
{
     clrscr();
     rewind(ft);

   printf("\n\t\t************* List Of All Transactions ****************   ");
   printf("\nษออออออออออหออออออออหอออออออออออหออออออออออออหอออออออออออออหออออออออหออออออออป");
   printf("\n?Trans ID ?Mem Id ? Book Id  ?Issue Date ?Return Date ? Fine  ?Reason ?);
   printf("\nฬออออออออออฮออออออออฮอออออออออออฮออออออออออออฮอออออออออออออฮออออออออฮออออออออน");

 while(fread(&T,sizeof(T),1,ft)==1)
  {
   printf("\n?  %3d    ? %3d   ?9d%d%d?%-2d/%-2d/%-4d ?%-2d/%-2d/%-4d  ?%-6.2f ?  %c    ?,T.tid,T.mid,T.b.bno,T.b.gno,T.b.no,T.i.day,T.i.mon,T.i.year,T.r.day,T.r.mon,T.r.year,T.fine,T.reason);
  }
   printf("\nศออออออออออสออออออออสอออออออออออสออออออออออออสอออออออออออออสออออออออสออออออออผ");

   gotoxy(1,25);

   textcolor(WHITE);
   cprintf("A");
   textcolor(DARKGRAY);
   cprintf("-Member Added ");
   textcolor(WHITE);
   cprintf("E");
   textcolor(DARKGRAY);
   cprintf("-Member Renewed ");
   textcolor(WHITE);
   cprintf("D");
   textcolor(DARKGRAY);
   cprintf("-Duplicate I card ");
   textcolor(WHITE);
   cprintf("I");
   textcolor(DARKGRAY);
   cprintf("-Book Issued ");
   textcolor(WHITE);
   cprintf("R");
   textcolor(DARKGRAY);
   cprintf("-Book Returned");
   getch();
}


/******************* Show Help  ****************************************/

void showhelp()
{
	   textcolor(LIGHTGRAY);
	   clrscr();
	   textcolor(LIGHTMAGENTA);
	   gotoxy(8,1);
	   cprintf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
	   gotoxy(8,2);
	   cprintf("?                   ษอออออออออออออออออออออออออ?                ?);
	   gotoxy(8,3);
	   cprintf("ฬออออออออออออออออออออน  Welcome To Obaid Books ฬอออออออออออออออออ?);
	   gotoxy(8,4);
	   cprintf("?                   ศอออออออออออออออออออออออออ?                ?);
	   gotoxy(8,5);
	   cprintf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
	   textcolor(WHITE);
	   gotoxy(25,8);
	   cprintf("Library Management System");
	   printf("\n\t\t\t\t-By Mohd. Obaid Shaikh");
	   printf("\n\n\tThe System Manages Addition Of New Books And Members");
	   printf("\n\tAnd Also Keep Records Of Transactions Such As");
	   printf("\n\tIssue Or Return Of A Book With Fine");
	   printf("\n\tRenew Existing Member Or Issue Duplicate ID");
	   printf("\n\n\tThe Book Is Issued For 1 Month ");
	   printf("\n\tThe Membership Expires After 6 Months");
	   printf("\n\tThe Book Id Is Of The Format year+groupno+no");
	   gotoxy(9,20);
	   textcolor(LIGHTMAGENTA);
	   cprintf("Send your comments/queries to ");
	   textcolor(LIGHTGREEN+BLINK);
	   cprintf("obaid786@sancharnet.in");
	   getch();
}