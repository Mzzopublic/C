#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX 100

char *p[MAX], *qretrieve(void);
int spos = 0;
int rpos = 0;
void enter(void), qstore(char *q), review(void), delete_ap(void);

int main(void)
{
	char s[80];
	register int t;
	
	for(t=0; t < MAX; ++t) p[t] = NULL; /* init array to nulls */
	
	for(;;) {
		printf("Enter, List, Remove, Quit: ");
		gets(s);
		*s = toupper(*s);
		
		switch(*s) {
		case 'E':
			enter();
			break;
		case 'L':
			review();
			break;
		case 'R':
			delete_ap();
			break;
		case 'Q':
			exit(0);
		}
	}
	return 0;
}
/* Enter appointments in queue. */
void enter(void)
{
	char s[256], *p;
	
	do {
		printf("Enter appointment %d: ", spos+1);
		gets(s);
		if(*s==0) break; /* no entry */
		p = (char *) malloc(strlen(s)+1);
		if(!p) {
			printf("Out of memory.\n");
			return;
		}
		strcpy(p, s);
		if(*s) qstore(p);
	} while(*s);
}

/* See what's in the queue. */
void review(void)
{
	register int t;
	
	for(t=rpos; t < spos; ++t)
		printf("%d. %s\n", t+1, p[t]);
}

/* Delete an appointment from the queue. */
void delete_ap(void)
{
	char *p;
	
	if((p=qretrieve())==NULL) return;
	printf("%s\n", p);
}

/* Store an appointment. */
void qstore(char *q)
{
	if(spos==MAX) {
		printf("List Full\n");
		return;
	}
	p[spos] = q;
	spos++;
}

/* Retrieve an appointment. */
char *qretrieve(void)
{
	if(rpos==spos) {
		printf("No more appointments.\n");
		return NULL;
	}
	rpos++;
	return p[rpos-1];
}
