#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct Family *get_person(void);    /* Prototype for input function */
char related(struct Family *pmember1, struct Family *pmember2);
char set_ancestry(struct Family *pmember1, struct Family *pmember2);

struct Date   
{
	int day;
	int month;
	int year;
};

struct Family                      /* Family structure declaration   */
{
	struct Date dob;
	char name[20];
	char father[20];
	char mother[20];
	struct Family *next;            /* Pointer to next structure      */
	struct Family *previous;        /* Pointer to previous structure  */
	struct Family *p_to_pa;         /* Pointer to father structure   */
	struct Family *p_to_ma;         /* Pointer to mother structure   */
};

void main()
{
	struct Family *first = NULL;    /* Pointer to first person        */
	struct Family *current = NULL;  /* Pointer to current person      */
	struct Family *last = NULL;     /* Pointer to previous person     */
	
	char more = '\0';               /* Test value for ending input    */
	
	for( ; ; )
	{
		printf("\nDo you want to enter details of a%s person (Y or N)? ", 
			first != NULL?"nother " : "" );
		scanf(" %c", &more);
		if(tolower(more) == 'n') 
			break;
		
		current = get_person();
		
		if(first == NULL)
		{
			first = current;            /* Set pointer to first Family    */
			last = current;             /* Remember for next iteration    */
		}
		else
		{
			last->next = current;  /* Set next address for previous Family */  
			current->previous = last; /* Set previous address for current */
			last = current;           /* Remember for next iteration */             
		}
	}
	
	current = first;
	
	while(current->next != NULL)  /* Check for relation for each person in    */
	{                       /* the list up to second to last            */
		int parents = 0;      /* Declare parent count local to this block */
		last = current->next; /* Get the pointer to the next              */
		
		while(last != NULL)   /* This loop tests current person           */
		{                     /* against all the remainder in the list    */
			if(related(current, last))         /* Found a parent ?          */
				if(++parents == 2)   /* Yes, update count and check it        */
					break;             /* Exit inner loop if both parents found */
				
				last = last->next;     /* Get the address of the next           */
		} 
		current = current->next;   /* Next in the list to check             */
	}
	
	/* Now tell them what we know */
	
	/* Output Family data in correct order */
	current = first;
	
	while (current != NULL)  /* Output Family data in correct order  */
	{
		printf("\n%s was born %d/%d/%d, and has %s and %s as parents.",
			current->name, current->dob.day, current->dob.month,
			current->dob. year, current->father,  current->mother);
		if(current->p_to_pa != NULL )
			printf("\n\t%s's birth date is %d/%d/%d  ",
			current->father, current->p_to_pa->dob.day,
			current->p_to_pa->dob.month, 
			current->p_to_pa->dob.year);
		if(current->p_to_ma != NULL)
			printf("and %s's birth date is %d/%d/%d.\n  ",
			current->mother, current->p_to_ma->dob.day,
			current->p_to_ma->dob.month, 
			current->p_to_ma->dob.year);
		
		current = current->next;  /* current points to next in list       */
	}
	
	/* Now free the memory */  
	current = first;
	while(current->next != NULL)
	{
		last = current;     /* Save pointer to enable memory to be freed */
		current = current->next; /* current points to next in list       */
		free(last);         /* Free memory for last                      */
	}
}

/*   Function to input data on Family members   */
struct Family *get_person(void)
{
	struct Family *temp;         /* Define temporary structure pointer */
	
	/* Allocate memory for a structure */
	temp = (struct Family*) malloc(sizeof(struct Family));
	printf("\nEnter the name of the person: ");
	scanf("%s", temp -> name );         /* Read the Family's name */
	printf("\nEnter %s's date of birth (day month year); ", temp->name);
	scanf("%d %d %d", &temp->dob.day, &temp->dob.month, &temp->dob.year);
	printf("\nWho is %s's father? ", temp->name );
	scanf("%s", temp->father );        /* Get the father's name */
	printf("\nWho is %s's mother? ", temp -> name );
	scanf("%s", temp -> mother );      /* Get the mother's name */
	temp->next = temp->previous = NULL; /* Set pointers to NULL */
	temp->p_to_pa = temp->p_to_ma = NULL;    /* Set pointers to NULL  */
	return temp;          /* Return address of Family structure */
}

char set_ancestry(struct Family *pmember1, struct Family *pmember2)
{
	if(strcmp(pmember1->father, pmember2->name) == 0)
	{
		pmember1->p_to_pa = pmember2;
		return 1;
	}
	if( strcmp(pmember1->mother, pmember2->name) == 0)
	{
		pmember1->p_to_ma = pmember2;
		return 1;
	}
	else
		return 0;
}
/* Fill in pointers for mother or father relationships */
char related (struct Family *pmember1, struct Family *pmember2)
{
	return set_ancestry(pmember1, pmember2) ||
		set_ancestry(pmember2, pmember1);
}
