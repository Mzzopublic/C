#include <stdio.h>
#include <stdlib.h>
struct node {
	int				value;
	struct node* 	left;
	struct node* 	right;
};
typedef struct node  NODE;
typedef struct node* PNODE;
void new_node (PNODE* n, int value) {

	*n = (PNODE)malloc (sizeof(NODE));
	if (*n != NULL) {
	    (*n)->value	= value;
		(*n)->left 	= NULL;
		(*n)->right = NULL;
 	}
}
void free_node (PNODE* n) {
	if ((*n) != NULL) {
		free (*n);
		*n = NULL;
 	}
}
void free_tree (PNODE* n) {
    if (*n == NULL) return;
	if ((*n)->left != NULL) {
	    free_tree (&((*n)->left));
 	}
	if ((*n)->right != NULL) {
	    free_tree (&((*n)->right));
	}
	free_node (n);
}
//查找结点
PNODE find_node (PNODE n, int value) {
	if (n == NULL) {
		return NULL;
	} else if (n->value == value) {
	    return n;
 	} else if (value <= n->value) {
 	    return find_node (n->left, value);
  	} else {
  	    return find_node (n->right, value);
	}
}
//插入结点
void insert_node (PNODE* n, int value) {
	if (*n == NULL) {
		new_node (n, value);
    } else if (value == (*n)->value) {
        return;
	} else if (value < (*n)->value) {
        insert_node (&((*n)->left), value);
	} else {
		insert_node (&((*n)->right), value);
	}
}
//最长路径
int get_max_depth (PNODE n) {
  int left = 0;
  int right = 0;
    	if (n == NULL) {
  	    return 0;
	}
		if (n->left != NULL) {
		left = 1 + get_max_depth (n->left);
	}
	if (n->right != NULL) {
		right = 1 + get_max_depth (n->right);
	}
 	return (left > right ? left : right );
}
//最短路径
int get_min_depth (PNODE n) {
  int left = 0;
  int right = 0;
  	if (n == NULL) {
  	    return 0;
	}
	if (n->left != NULL) {
		left = 1 + get_min_depth (n->left);
	}
	if (n->right != NULL) {
		right = 1 + get_min_depth (n->right);
	}
	return (left < right ? left : right );
}
int get_num_nodes (PNODE n) {
  int left = 0;
  int right = 0;
  	if (n == NULL) {
  	    return 0;
	}
	if (n->left != NULL) {
		left = get_num_nodes (n->left);
	}
	if (n->right != NULL) {
		right = get_num_nodes (n->right);
	}
	return (left + 1 + right);
}
//最短路径长度
int get_min_value (PNODE n) {
    if (n == NULL) return 0;
	if (n->left == NULL) {
	    return n->value;
 	} else {
 	    return get_min_value(n->left);
  	}
}
//最长路径长度
int get_max_value (PNODE n) {
	if (n == NULL) return 0;
	if (n->right == NULL) {
	    return n->value;
 	} else {
 	    return get_max_value(n->right);
  	}
}
//删除结点
void deletenode (PNODE *n) {
	PNODE tmp = NULL;
	if (n == NULL) return;
	if ((*n)->right == NULL) {
	    tmp = *n;
	    *n = (*n)->left;
	    free_node (n);
	} else if ((*n)->left == NULL) {
	    tmp = *n;
	    *n = (*n)->right;
	    free_node (n);
	} else {
        for (tmp = (*n)->right; tmp->left != NULL; tmp = tmp->left);
        tmp->left = (*n)->left;
        tmp = (*n);
        *n = (*n)->right;
        free_node (&tmp);
	}
}
void delete_node (PNODE *n, int value) {
	PNODE node;
    if (n == NULL) return;
    node = find_node (*n, value);
	if ((*n)->value == value) {
		deletenode (n);
    } else if (value < (*n)->value) {
		delete_node (&((*n)->left), value);
    } else {
		delete_node(&((*n)->right), value);
	}
}
void pre_order_traversal(PNODE n)
{
    if (n != NULL) {
		printf ("%i ", n->value);
        pre_order_traversal (n->left);
        pre_order_traversal( n->right);
    }
}
void in_order_traversal (PNODE n)
{
    if (n != NULL) {
        in_order_traversal (n->left);
        printf ("%i ", n->value);
        in_order_traversal ( n->right);
    }
}
void post_order_traversal (PNODE n)
{
    if (n != NULL) {
        post_order_traversal (n->left);
        post_order_traversal (n->right);
        printf ("%i ", n->value);
    }
}
int main() {
	char buf[50];
	int  option;
	PNODE tree = NULL;
    PNODE node = NULL;
    	while (1) {
		printf ("--------------------------\n");
		printf ("Options are:\n\n");
		printf (" 0  Exit\n");
		printf (" 1  Insert node\n");
		printf (" 2  Delete node\n");
		printf (" 3  Find node\n");
		printf (" 4  Pre order traversal\n");
		printf (" 5  In order traversal\n");
		printf (" 6  Post order traversal\n");
		printf (" 7  Max depth\n");
		printf (" 8  Min depth\n");
		printf (" 9  Max value\n");
		printf (" 10 Min value\n");
		printf (" 11 Node Count\n\n");
		printf ("--------------------------\n");
		printf ("Select an option: ");
		fgets (buf, sizeof(buf), stdin);
		sscanf (buf, "%i", &option);
		printf ("--------------------------\n");
		if (option < 0 || option > 11) {
		    fprintf (stderr, "Invalid option");
		    continue;
		}
			switch (option) {
		    case 0:
		        exit (0);
		    case 1:
		        printf ("Enter number to insert: ");
				fgets (buf, sizeof(buf), stdin);
				sscanf (buf, "%i", &option);
				printf ("\n\n");
				insert_node (&tree, option);
				break;
		    case 2:
            	printf ("Enter number to delete: ");
				fgets (buf, sizeof(buf), stdin);
				sscanf (buf, "%i", &option);
				printf ("\n\n");
				delete_node (&tree, option);
				break;
		    case 3:
            	printf ("Enter number to find: ");
				fgets (buf, sizeof(buf), stdin);
				sscanf (buf, "%i", &option);
				printf ("\n\n");
				node = find_node (tree, option);
				if (node != NULL) {
				    printf ("Found node\n\n");
				} else {
				    printf ("Couldn't find node\n\n");
				}
				break;
		    case 4:
				printf ("Pre order traversal: ");
				pre_order_traversal (tree);
				printf ("\n\n");
				break;
		    case 5:
		        printf ("In order traversal: ");
		    	in_order_traversal (tree);
		    	printf ("\n\n");
		    	break;
		    case 6:
		        printf ("Post order traversal: ");
		    	post_order_traversal (tree);
       			printf ("\n\n");
		    	break;
		    case 7:
		        printf ("Max depth is %i\n\n", get_max_depth (tree));
		        break;
		    case 8:
		        printf ("Min depth is %i\n\n", get_min_depth (tree));
		        break;
		    case 9:
		        printf ("Max value is %i\n\n", get_max_value (tree));
		        break;
		    case 10:
		        printf ("Min value is %i\n\n", get_min_value (tree));
		        break;
      		case 11:
		        printf ("Node Count is %i\n\n", get_num_nodes (tree));
		        break;
		}
 	}
	return 0;
}
