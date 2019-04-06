
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>		/* for use of isalpha() */


struct node{
  struct node *right;		/* after current node in dictionary */
  struct node *left;		/* before current node in dictionary */
  char *word;
};

struct node *root;		/* pointer to root node (global) */

void memory_error(){
  fprintf(stderr, "Error! malloc fails.\n");
  exit(1);
}


char *save_string(char *string){
  char *saved_string_ptr;

  saved_string_ptr = malloc(sizeof(string)/sizeof(char) + 1);
  /* sizeof(string)/sizeof(char) == number of characters in string +1 (+1 is for '\0') */
  if(saved_string_ptr == NULL)
    memory_error();
  
  strcpy(saved_string_ptr, string);
  return saved_string_ptr;
  
}

/* put word to binary tree */
void enter(struct node **node_ptr_ptr, char *tmp_word){
  int result;
  
  /* if node is NULL, create new node */
  if((*node_ptr_ptr) == NULL){
    (*node_ptr_ptr) = malloc(sizeof(struct node));
    if((*node_ptr_ptr) == NULL){
      memory_error();
    }
    
    if(root == NULL)
      root = (*node_ptr_ptr);
    
    /* since content of tmp_word changes, current content has to be saved */
    /* in other memory place (in malloc, heap memory) */
    (*node_ptr_ptr)->word = save_string(tmp_word);
    /* since currently this node is a leaf, left and right are NULL */
    (*node_ptr_ptr)->left = NULL;
    (*node_ptr_ptr)->right = NULL;
    return;
  }

  /* if strcmp(str_ptr1, str_ptr2) > 0, that means str1 is after str2 in dictionary */
  /* if strcmp(str_ptr1, str_ptr2) = 0, that means str1 and str2 are equal */
  result = strcmp((*node_ptr_ptr)->word, tmp_word);
  /* therefore, if result > 0, it means newly inserted word (tmp_word) is before word of */
  /* current node, thus tmp_word should be inserted to left node to curent node */

  if(result == 0)
    return;

  if(result > 0){		/* then, tmp_word should be inserted to left node */
    enter(&((*node_ptr_ptr)->left), tmp_word);
  }
  else{
    enter(&((*node_ptr_ptr)->right), tmp_word);
  }

}

void scan(FILE *input_file){
  char ch;
  char tmp[100];		/* temporary storage of a word */
  int i;
  
  while(1){			/* don't stop until it reaches EOF */
    
    /* skip spaces and all other characters other than alphabet */
    while(1){
      ch = fgetc(input_file);
      if(isalpha(ch) || ch == EOF)
	break;
    }
    
    if(ch == EOF)
      break;

    tmp[0] = ch;
    for(i=1; i<99; i++){	/* read input_file until current word ends */
      ch = fgetc(input_file);
      if(isalpha(ch) == 0)
	break;
      tmp[i] = ch;
    }
    tmp[i] = '\0';

    /* put gained word to binary tree */
    enter(&root, tmp);
  }
}


void show_binary_tree(struct node *node_ptr){
  if(node_ptr == NULL)
    return;
  
  show_binary_tree(node_ptr->left);
  printf("%s\n", node_ptr->word);
  show_binary_tree(node_ptr->right);
}


void free_binary_tree(struct node *node_ptr){
  if(node_ptr == NULL)
    return;

  /* free memory from leaf node (root node is the last to be freed) */
  free_binary_tree(node_ptr->left);
  free_binary_tree(node_ptr->right);
  free(node_ptr);
}
  
/* delete input string */
void delete_tree_node(struct node *node_ptr, char *input_srt){
  int result;
  /* store node pointer's pointer which contains parent's left or right adress */
  struct node **before_ptr_ptr = NULL;
  struct node *tmp_ptr1;
  struct node *tmp_ptr2;
  int find_node_flag = 0;	/* if node with input_str is found, this is flagged */
  
  
  /* first, search node which contains input word */
  while(node_ptr != NULL){
    result = strcmp(node_ptr->word, input_srt);
    if(result > 0){	 /* this means input_str is before current node's word in dictionary */
      /* move left child node */
      before_ptr_ptr = &(node_ptr->left);
      node_ptr = node_ptr->left;
      continue;
    }
    else if(result == 0){	/* find target string */
      find_node_flag = 1;
      /* operations here are complex, you shold refer to a diagram written in the note */
      if(node_ptr == root){	/* dealt with extreme case1: if deleted node is root */
	if(node_ptr->left != NULL)
	  root = node_ptr->left;
	else if(node_ptr != NULL) /* if left child is NULL, and right child is not NULL */
	  root = node_ptr->right;
	else			/* if both children are NULL */
	  root = NULL;
      }
      else			/* otherwise, there is parent node for the deleted node */
	/* both left and right are ok, but here I will connect parent node to left child node */
	/* if node_ptr->left is NULL, (*before_ptr_ptr) is updated in following if sentence */
	(*before_ptr_ptr) = node_ptr->left;

      /* deal with extreme case2: if left or right child node is NULL */
      if(node_ptr->left == NULL && node_ptr->right == NULL){
	if(before_ptr_ptr != NULL) /* before_ptr_ptr is NULL if deleted node is root node */
	  (*before_ptr_ptr) = NULL;
	free(node_ptr);
	return;
      }
      else if(node_ptr->left == NULL){
	/* if only left child is NULL, shift right child node to parent position */
	if(before_ptr_ptr != NULL) /* before_ptr_ptr is NULL if deleted node is root node */
	  (*before_ptr_ptr) = node_ptr->right;
	free(node_ptr);
	return;
      }
      else if(node_ptr->right == NULL){
	/* if only right child is NULL, shift left child node to parent node */
	if(before_ptr_ptr != NULL) /* before_ptr_ptr is NULL if deleted node is root node */
	  (*before_ptr_ptr) = node_ptr->left;
	free(node_ptr);
	return;
      }
      
      /* reaching this point means both left and right child nodes exist */
      /* deal with extreme case3: child node's left and/or right is NULL */
      if(node_ptr->left->right == NULL){
	/* when node_ptr->left->right == NULL, whether or not node_ptr->right->left == NULL, */
	/* procedure is the same */
	node_ptr->left->right = node_ptr->right;
	free(node_ptr);
	return;
      }
      else if(node_ptr->right->left == NULL){ /* in this case, two procedures are required */
	tmp_ptr1  = node_ptr->left->right;
	node_ptr->left->right = node_ptr->right;
	node_ptr->right->left = tmp_ptr1;
	free(node_ptr);
	return;
      }
      else{
	/* "else" means both left child node's right and right child node's left are not NULL */
	/* save adresses to left_child_node->right, right_child_node->left */
	tmp_ptr1 = node_ptr->left->right;
	tmp_ptr2 = node_ptr->right->left;
	/* (*before_ptr_ptr)->right == node_ptr->left->right */
	node_ptr->left->right = node_ptr->right;
	node_ptr->right->left = tmp_ptr1;
	free(node_ptr);
	break;
      }
    }
    else{
      /* move right child node */
      before_ptr_ptr = &(node_ptr->right);
      node_ptr = node_ptr->right;
      continue;
    }
  }

  if(find_node_flag == 0){ /* this means node with input_str does not exist in the first place */
    printf("input string cannot be found in the tree.\n");
    return;
  }

  /* connect tmp_ptr2 to the far right edge of "b" node (for "b" node, refer to note)  */
  node_ptr = tmp_ptr1;		/* start from tmp_ptr1 */
  while(node_ptr != NULL){
    /* for dealing with node_ptr == NULL from start, I split break point as follows */
    if(node_ptr->right == NULL)/* by referrign to right child, I can save adress before NULL */
      break;
    /* go right child until it reaches NULL */
    node_ptr = node_ptr->right;
  }
  node_ptr = tmp_ptr2;

}

int main(int argc, char *argv[]){
  FILE *input_file;
  char input_srt[100];
  char line[100];
  
  if(argc != 2){
    fprintf(stderr, "Error!\n");
    fprintf(stderr, "Usage: %s input_file_name\n", argv[0]);
    return 1;
  }

  input_file = fopen(argv[1], "r");
  if(input_file == NULL){
    fprintf(stderr, "Error! can't open input file\n");
    return 1;
  }

  scan(input_file);		/* scan input file, and create binary tree */
  show_binary_tree(root);	/* show all contents stored in binary tree */

  /* delete entered value if it exists */
  printf("enter value you want to delete: ");
  fgets(line, sizeof(line), stdin);
  sscanf(line, "%s", input_srt);
  delete_tree_node(root, input_srt);
  show_binary_tree(root);
  
  free_binary_tree(root);	/* free allocated memory for created binary tree */
  fclose(input_file);
  
  return 0;
}
