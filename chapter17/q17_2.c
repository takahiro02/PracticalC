#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct item{
  struct item *next_ptr;	/* next item for the list */
  int value;			/* value for the list */
};

struct item *global_first_ptr = NULL;

void list(struct item *first_ptr, const int value){
  /* since value is const, we can't change it */

  struct item *before_ptr;
  struct item *after_ptr;
  struct item *new_item_ptr;
  
  before_ptr = first_ptr;  /* start from the beginning */
  after_ptr = before_ptr->next_ptr;  
  
  /* identify the insertion place */
  while(1){
    if(after_ptr == NULL)
      break;

    if(after_ptr->value >= value)
      break;

    /* advance the pointers */
    after_ptr = after_ptr->next_ptr;
    before_ptr = before_ptr->next_ptr;
  }

  /* create a new item */
  new_item_ptr = malloc(sizeof(struct item));
  new_item_ptr->value = value;

  if(before_ptr->value >= value){ /* in this case, new item is placed before before_item */
    new_item_ptr->next_ptr = before_ptr;
    global_first_ptr = new_item_ptr;
  }
  else{
    before_ptr->next_ptr = new_item_ptr;
    /* if new_item is placed at the edge, since after_ptr is NULL, new_item_ptr->next_ptr */
    /* becomes NULL */
    new_item_ptr->next_ptr = after_ptr;
  }
}

/* show the value of struct list */
void show_struct_list(struct item *first_ptr){
  struct item *current_item_ptr;

  if(first_ptr == NULL){		/* when there is no items in the list */
    printf("no items remained in the linked list\n");
    return;
  }
  
  current_item_ptr = first_ptr;

  while(current_item_ptr != NULL){
    printf("%d\n", current_item_ptr->value);
    current_item_ptr = current_item_ptr->next_ptr;
  }
}

/* free all memories allocated for struct */
void free_struct_list(struct item *first_ptr){
  struct item *current_item_ptr;
  struct item *next_item_ptr;

  if(first_ptr == NULL){ /* when there is no items in the list */
    return;
  }
  
  current_item_ptr = first_ptr;
  next_item_ptr = first_ptr->next_ptr;

  while(current_item_ptr != NULL){
    free(current_item_ptr);
    if(current_item_ptr->next_ptr == NULL)
      break;
    current_item_ptr = next_item_ptr;
    next_item_ptr = next_item_ptr->next_ptr;
  }
}

/* delete all items in list with input value if it exists in the linked list */
/* notice: this function doesn't work if it tries to delete item when number of items is 1 */
void delete_list_val(struct item *first_ptr, int val){
  struct item *item_ptr = first_ptr; /* item_ptr: target to be deleted */
  struct item *before_ptr = NULL;
  
  /* see the values in list from first value one by one */
  /* unlike array, binary search can't be used because middle value can't be found */
  /* although values in list are sorted in increasing order */
  while(item_ptr != NULL){
    /* since list items are sorted in ascending order, if value in current item is greater */
    /* than input val, there is no chance that coming values are equal to input val, so break */
    if(item_ptr->value > val)
      break;
    
    if(item_ptr->value == val){
      /* delete this item */
      /* deal with extreme case2: if deleted item is the only one item remained in the list */
      if(before_ptr == NULL && item_ptr->next_ptr == NULL){
	free(item_ptr);
	global_first_ptr = NULL;
      }
      /* deal with extreme case1: if deleted item is at the left edge(minimum) */
      if(before_ptr == NULL){
	item_ptr = item_ptr->next_ptr;
	free(global_first_ptr);
	global_first_ptr = item_ptr;
	continue;
      }
      /* notice: the place of before_ptr itself isn't changed */
      before_ptr->next_ptr = item_ptr->next_ptr;
      free(item_ptr);
      item_ptr = before_ptr->next_ptr;
      /* since item_ptr is already forwarded by one, no need to forward item pointer again */
      continue;			
    }
    /* forward before_ptr and item_ptr by one */
    before_ptr = item_ptr;
    item_ptr = item_ptr->next_ptr;
  }

}


int main(){
  char line[100];
  int input_value;
  int i = 0;
  
  while(1){
    printf("enter a value:");
    fgets(line, sizeof(line), stdin);
    if(line[0] == '\n')
      break;
    sscanf(line, "%d", &input_value);
    /* since first item has to be made outside of list() */
    if(i == 0){
      global_first_ptr = malloc(sizeof(struct item));
      if(global_first_ptr == NULL){
	fprintf(stderr, "Error:unable to allocate memory\n");
	exit(8);
      }
      global_first_ptr->value = input_value;
      global_first_ptr->next_ptr = NULL;
    }
    else
      list(global_first_ptr, input_value);
    ++i;
  }

  printf("Here is the list of integers you put in\n");
  show_struct_list(global_first_ptr);	/* show the value of struct list */

  /* delete entered value if it exists */
  printf("enter value you want to delete: ");
  fgets(line, sizeof(line), stdin);
  sscanf(line, "%d", &input_value);
  delete_list_val(global_first_ptr, input_value);

  show_struct_list(global_first_ptr);
  free_struct_list(global_first_ptr);	/* free all memories allocated for struct */
  
  return 0;
}
