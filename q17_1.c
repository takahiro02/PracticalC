/*****************************************************
 * q17_1.c -- show a list of variables and the lines *
 *            of these variables in a C program.     *
 * Usage: q17_1 <C_file_name>                        *
 *****************************************************/
           
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct node{
  struct node *left;		/* tree to the left */
  struct node *right;		/* tree to the right */
  char *var;			/* variable name for this tree */
  int line[100];		/* line numbers in which this variable is */
};

/* the top of the tree */
static struct node *root = NULL;

struct structname_list{		/* struct to store struct names */
  struct structname_list *next;
  char *struct_name;
};

struct structname_list *structname_root = NULL;

/* record line number of input file */
/* I made this variable global since this is also used when initializing node */
int line_num = 1;

/*****************************************************
memory_error -- write error and die
*****************************************************/
void memory_error(){
  fprintf(stderr, "Error:Out of memory\n");
  exit(8);
}

/*****************************************************
save_string -- save a string on the heap
Parameters
     string -- string to save
Returns
     pointer to malloc-ed section of memory with
     the string copied into it.
*****************************************************/
char *save_string(char *string){
  char *new_string;	   /* where we are going to put string */

  new_string = malloc((unsigned)(strlen(string) + 1));

  if(new_string == NULL)
    memory_error();

  strcpy(new_string, string);
  return (new_string);
}


/*****************************************************
 * add_linenum2tree -- add line number of existing variable
 *   to the binary tree
 *****************************************************/
void add_linenum2tree(char *var_ptr, struct node *node_ptr, int line_num){
  int i;
  /* store line number to the last element of line array in the node pointer */
  for(i=0; i<100; i++){
    if(node_ptr->line[i] == 0){
      node_ptr->line[i] = line_num;
      break;
    }
  }
}


/*****************************************************
enter -- enter variable information into the tree

Parameters
     node -- current node we are looking at
     var -- variable name to enter
     line -- line number in which the variable is
*****************************************************/
void enter(struct node **node, char *var){
  int result;			/* result of strcmp */
  char *save_string(char *);	/* save a string on the heap */
  int i;
  
  /*
   *If the current node is null, we have reached the bottom
   *of the tree and must create a new node.
   */
  if((*node) == NULL){

    /* Allocate memory for a new node */
    (*node) = malloc(sizeof(struct node));
    if((*node) == NULL)
      memory_error();

    /* Initialize the new node */
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->var = save_string(var);
    (*node)->line[0] = line_num;
    for(i=1; i<100; i++)
      (*node)->line[i] = 0;
    return;
  }
  /* if strcmp(str_ptr1, str_ptr2) > 0, that means str1 is after str2 in dictionary */
  /* if strcmp(str_ptr1, str_ptr2) = 0, that means str1 and str2 are equal */
  result = strcmp((*node)->var, var);
  /* therefore, if result > 0, it means newly inserted variable name is before variable in */
  /* current node */

  /* The current node already contains the variable, no entry necessary */
  /* but line number should be added. */
  if(result == 0){
    add_linenum2tree((*node)->var, (*node), line_num);
    return;
  }

  /* The variable must be entered in the left or right sub-tree */
  if(result < 0)
    enter(&(*node)->right, var);
  else
    enter(&(*node)->left, var);
}


/*****************************************************
 *check_struct_name -- check whether the input is among     *
 * struct names stored in linked list
 * if so, return 1, otherwise return 0
 *****************************************************/
int check_struct_name(char *var_ptr, struct structname_list *structname_ptr){

  while(structname_ptr != NULL){
    if(strcmp(structname_ptr->struct_name, var_ptr) == 0)
      return 1;
    structname_ptr = structname_ptr->next;
  }
  return 0;

}



/*****************************************************
 *check_type -- check whether the input is among     *
 * variable types(int, char, etc...)
 * if so, return 1, otherwise return 0
 *****************************************************/
int check_type(char *var_ptr){
  char *type_int = "int";
  char *type_char = "char";
  char *type_long = "long";
  char *type_double = "double";
  char *type_void = "void";

  if(strcmp(var_ptr, type_int) == 0)
    return 1;
  else if(strcmp(var_ptr, type_char) == 0)
    return 1;
  else if(strcmp(var_ptr, type_long) == 0)
    return 1;
  else if(strcmp(var_ptr, type_double) == 0)
    return 1;
  else if(strcmp(var_ptr, type_void) == 0)
    return 1;
  else if(check_struct_name(var_ptr, structname_root))
    return 1;
  else
    return 0;
}


/* this is for containg both flag of whether the given variable */
/* name is in the binary tree(if yes, set 1), and the pointer to the node */
/* if exist */
struct flag_ptr{
  int exist_flag;
  struct node *node_ptr;
};

/*****************************************************
 * check_entry -- check whether the input is in the tree
 * if so, return 1, otherwise return 0
 *****************************************************/
struct flag_ptr check_entry(char *var_ptr, struct node *node){
  struct flag_ptr flag_ptr = {0}; /* initialize members of struct flag_ptr */
  struct flag_ptr flag_ptr_left = {0}, flag_ptr_right = {0};
  int result;			/* store result of strcmp */
  
  if(node == NULL)
    return flag_ptr;

  /* check if this node has the same variable name */
  result = strcmp(node->var, var_ptr);
  /* if strcmp(str_ptr1, str_ptr2) > 0, that means str1 is after str2 in dictionary */
  /* if strcmp(str_ptr1, str_ptr2) = 0, that means str1 and str2 are equal */
  if(result == 0){
    flag_ptr.exist_flag = 1;
    flag_ptr.node_ptr = node;
    return flag_ptr;
  }
  
  /* if I wrote following as "if(check~(~,~->left) || check~(~,~->right))", */
  /* even if check~(~,~->left) == 1, it also evaluate check~(~,~->right), which is redundant */
  /* if result > 0, since variable name in current node is after new variable in dictionary, */
  /* new variable is before variable of current node in dictionary. so I don't have to check */
  /* right sub-tree from current node, because of construction criteria. */
  if(result < 0){		/* if result < 0, new variable might be in right sub-tree */
    flag_ptr_right = check_entry(var_ptr, node->right);
    if(flag_ptr_right.exist_flag)
      return flag_ptr_right;
  }
  else{
    flag_ptr_left = check_entry(var_ptr, node->left);
    if(flag_ptr_left.exist_flag)
      return flag_ptr_left;
  }

  /* reaching this step means new variable doesn't exist in current binary tree, */
  /* so return flag_ptr without exist_flag is turned on. */
  return flag_ptr;		/* this flag_ptr isn't changed anything */
}


void check_and_store_structname(char *variable, struct structname_list **structname_ptr_ptr){

  if((*structname_ptr_ptr) == NULL){
    (*structname_ptr_ptr) = malloc(sizeof(struct structname_list));
    if((*structname_ptr_ptr) == NULL)
      memory_error();
    (*structname_ptr_ptr)->next = NULL;
    (*structname_ptr_ptr)->struct_name = save_string(variable);
    return;
  }

  /* reaching this place means structname_ptr_ptr is not NULL */
  /* already the name exists in list */
  if(strcmp((*structname_ptr_ptr)->struct_name, variable) == 0)
    return;
  else{
    check_and_store_structname(variable, &((*structname_ptr_ptr)->next));
  }

}


/* show the value of struct list */
void show_struct_list(struct structname_list *first_ptr){
  struct structname_list *current_item_ptr;

  if(first_ptr == NULL){		/* when there is no items in the list */
    printf("no items remained in the linked list\n");
    return;
  }
  
  current_item_ptr = first_ptr;

  while(current_item_ptr != NULL){
    printf("%s\n", current_item_ptr->struct_name);
    current_item_ptr = current_item_ptr->next;
  }
}


/*****************************************************
scan -- scan the file for variables

Parameters
     name -- name of the file to scan
*****************************************************/
void scan(char *name){
  char variable[100];		/* variable we are working on */
  int index;			/* index into the variable */
  int ch;			/* current character */
  FILE *in_file;		/* input file */
  unsigned short store_flag = 0;	/* flag which turn on when variable type is detected */
  int check_type(char *);	/* return 1 when input matches any variable types */
  void add_linenum2tree(char *, struct node *, int); /* add line number to the existing entry */
  struct flag_ptr check_entry(char *, struct node *);      /* check whether the variable alrady exist in the tree */
  struct flag_ptr flag_ptr;	/* for containing flag of existence of variable and its pointer */
  char *type_struct = "struct";	/* for checking struct type */
  int struct_flag = 0;
  void check_and_store_structname(char *, struct structname_list **);
  char *null_string = "NULL";
  
  in_file = fopen(name, "r");
  if(in_file == NULL){
    fprintf(stderr, "Error:Unable to open %s\n", name);
    exit(8);
  }
  while(1){
    
    /* this while block scans past the whitespace and other non-alphabetical characters */
    /* also skips comment(//~ or /*~) and string space("~") */
    while(1){
      ch = fgetc(in_file);

      /* count line number */
      if(ch == '\n'){
	line_num++;
	continue;
      }

      /* skip string space ("~") */
      if(ch == '"'){
	while(1){
	  ch = fgetc(in_file);
	  if(ch == '\n'){
	    line_num++;
	  }
	  else if(ch == '"')
	    break;
	}
      }

      /* skip comment space (/*~ or //~) */
      if(ch == '/'){
	ch = fgetc(in_file);
	/* deal with type 1 comment. if //, skip until next line comes */
	if(ch == '/'){
	  while(1){
	    ch = fgetc(in_file);
	    if(ch == '\n'){
	      line_num++;
	      break;
	    }
	  }
	}
	/* deal with type 2 comment. until end of comment, skip */
	else if(ch == '*'){
	  while(1){
	    ch = fgetc(in_file);
	    if(ch == '\n')
	      line_num++;
	    else if(ch == '*'){
	      ch = fgetc(in_file);
	      if(ch == '\n')
		line_num++;
	      else if(ch == '/') /* end of comment */
		break;
	    }
	  }
	}
	else if(ch == '\n'){
	  line_num++;
	  continue;
	}
      }

      /* reaching here means it is now in code space */
      
      /* deal with 2 or more variables are defined in one line (char *type_void = "void";) */
      /* initialization of store flag is done here */
      if(store_flag == 1 && ch == ';')
	store_flag = 0;

      /* deal with 2 or more variables are defined in one line (char *type_void = NULL;) */
      if(store_flag == 1 && ch == ';')
	store_flag = 0;

      /* deal with case 2: only type name is listed, and there is no following variable */
      /* -> ~(~, char *) */
      /* this if block doesn't deal with case where ')' comes immediately after type name */
      /* e.g. ~(~, int) this kind of declaration will be dealt in following procedure  */
      if(check_type(variable))	/* this variable is variable stored in previous loop */
	if(ch == ',' || ch == ')')
	  store_flag = 0;
      
      if(isalpha(ch) || (ch == EOF))
	break;
    }

    if(ch == EOF)
      break;

    /* reaching this point means ch holds an alphabet in code space, and */
    /* ch is not in comment or string space */
    /* store variable name in "variable" */
    variable[0] = ch;
    for(index = 1; index < sizeof(variable); ++index){
      ch = fgetc(in_file);
      if(ch == '\n')		/* check '\n' at this point as well */
	line_num++;
      /* if ch is either an alphabet or 0~9 or _ (underbar), this ch is part of variable */
      /* in ascii table, 0 is 48 in decimal, 9 is 57. underbar _ is 95 */
      else if(isalpha(ch) || ('0' <= ch && ch <= '9') || ch == '_')
	variable[index] = ch;
      else
	break;
    }
    /* put a null on the end */
    variable[index] = '\0';	/* this procedure has to be before following procedures */

    /* this if ~ else if block checks mainly the next character after stored string */
    /* check if this is a function name */
    if(ch == '('){		/* ch is already loaded */
      store_flag = 0;
      continue;
    }
    /* store_flag initialization can be done here as well */
    else if(store_flag == 1 && ch == ';'){
      enter(&root, variable);
      store_flag = 0;
      continue;
    }
    /* deal with arguments of function. if it detect ')' and store_flag is on, reset it */
    else if(store_flag == 1 && ch == ')'){
      /* deal with 2 cases: 1-> ~(~, int) as declaration of function, no variable */
      if(check_type(variable)){
	store_flag = 0;
	continue;
      }
      else{      /* case 2-> ~(~, char **argv)  */
	enter(&root, variable);
	store_flag = 0;
	continue;
      }
    }

    /* deal with case where NULL is perceived as variable */
    if(strcmp(variable, null_string) == 0)
      continue;

    /* deal with struct type */
    if(strcmp(type_struct, variable) == 0){
      struct_flag = 1;
      continue;
    }
    if(struct_flag){
      /* if struct_flag is 1, check the next string, and if it is not in linked list, */
      /* store it as a new type name */
      check_and_store_structname(variable, &structname_root);
      struct_flag = 0;
      store_flag = 1;
      /* to deal with the case where struct name and variable name is the same, */
      /* I turn store_flag at this point */
      /* (if it is the same, it happens to satisfy check_type() condition, and */
      /* cannot be stored) */
      continue;
    }

    /* if store flag is 1, store current variable name to binary tree */
    if(store_flag){
      enter(&root, variable);
      /* since it is possible that 2 or more variables follows, */
      /* initialization of store_flag should not be done here. */
      continue;
    }
    
    /* according to the retrieved content, decide whether to store it or not */
    if(check_type(variable)){
      store_flag = 1;
      continue;
    }

    /* since check_entry() costs relatively heavy, this should be placed at the last */
    /* if there already is the variable in the tree, */
    /* add line number to the entry */
    flag_ptr = check_entry(variable, root);
    if(flag_ptr.exist_flag){
      add_linenum2tree(variable, flag_ptr.node_ptr, line_num);
      continue;
    }
    
  }

  fclose(in_file);
}

/*****************************************************
 * print_tree -- print out the variable names and    *
 *               its lines in a tree                 *
 *                                                   *
 *Parameters                                         *
 *      top -- the root of the tree to print         *
*****************************************************/
void print_tree(struct node *top){
  int i;
  
  if(top == NULL)
    return;			/* short tree */

  print_tree(top->left);
  printf("%s\n", top->var);
  for(i=0; top->line[i]!=0; i++)
    printf("%d ", top->line[i]);
  printf("\n");
  print_tree(top->right);
}

int main(int argc, char *argv[]){
  if(argc != 2){
    fprintf(stderr, "Error:Wrong number of parameters\n");
    fprintf(stderr, "      on the command line\n");
    fprintf(stderr, "Usage is:\n");
    fprintf(stderr, "    words 'file'\n");
    exit(8);
  }
  scan(argv[1]);
  print_tree(root);
  /* show_struct_list(structname_root); */
  return (0);
}

