/* double_list.c -- I originally implement double list  */
/*                  referring to the text book */


#include<stdio.h>
#include<stdlib.h>

struct double_list{
  int data;
  struct double_list *next_ptr;
  struct double_list *previous_ptr;
};

struct double_list *global_head_ptr = NULL;


void double_enter(struct double_list *head_ptr, int item){
  struct double_list *insert_ptr;
  struct double_list *new_item_ptr;

  insert_ptr = head_ptr;
  /* identify the appropriate place to insert */
  while(1){
    if(insert_ptr->next_ptr == NULL)
      break;
    if(item <= insert_ptr->data)
      break;
    if(insert_ptr->data <= item && item <= insert_ptr->next_ptr->data)
      break;
    insert_ptr = insert_ptr->next_ptr;
  }

  /* create a new item */
  new_item_ptr = malloc(sizeof(struct double_list));
  if(new_item_ptr == NULL){
    fprintf(stderr, "Error:unable to allocate memory\n");
    exit(8);
  }
  new_item_ptr->data = item;

  /* at this point, we cannot know what kind of "break" in while(1) */
  /* was executed. And since the place of new_item differs by the kind */
  /* of "break", we need to change how to connect new_item to existing link */
  /* according to what kind "break" was executed. */
  if(item <= insert_ptr->data){
    /* If this condition is true, that means break of (isert_ptr->next_ptr == NULL) or (item <= insert_ptr->data) in while(1) is executed */
    /* If break of (isert_ptr->next_ptr == NULL) in while(1) was executed, */
    /* being true to (item <= insert_ptr->data) means the place of new_item */
    /* will be either the second last or */
    /* the head (in this case the number of items in list is only one). */
    /* If the place of new_item is second last, step1~2 and step4~5 would be */
    /* executed. If the place is the head of linked list, step1~3 and step5 */
    /* would be executed. */
    /* If break of (item <= insert_ptr->data) in while(1) was executed, */
    /* that means the place of new_item will be neither the second last */
    /* nor the head. In this case, step1~2 and step4~5 would be executed. */

    /* connect new_item and existing double_list */
    new_item_ptr->next_ptr = insert_ptr; /* step1 */
    new_item_ptr->previous_ptr = insert_ptr->previous_ptr; /* step2 */
    /* changing pointers of existing items */
    if(insert_ptr->previous_ptr == NULL){ /* this means, insert_ptr->data had been the smallest value */
      /* in other words, item is now the smallest value and placed to the head of the double linked list */
      global_head_ptr = new_item_ptr; /* step3 */
    }
    if(insert_ptr->previous_ptr != NULL) /* if insert_ptr->previous_ptr is NULL, the next operation cannot be done */
      /* in other words, if (insert_ptr->previous_ptr != NULL) is not true, that means new item is placed to the head of the linked list, so we must not reconnect previous item to new item, since in that case there is no item previous to insert_ptr */
      insert_ptr->previous_ptr->next_ptr = new_item_ptr; /* step4 */
    insert_ptr->previous_ptr = new_item_ptr;		 /* step5 */
  }
  else{				/* (insert_ptr->data < item) is true */
    /* if this condition is true, that means break of (isert_ptr->next_ptr == NULL) or (insert_ptr->data <= item && item <= insert_ptr->next_ptr->data) in while(1) is executed */
    /* If break of (isert_ptr->next_ptr == NULL) in while(1) was executed, */
    /* being true to (insert_ptr->data < item) means the place of new_item */
    /* will be tail (the last item). If the place of new_item is the last, */
    /*step6~7 and step9 would be executed. */
    /* If break of (insert_ptr->data <= item && item <= insert_ptr->next_ptr->data) in while(1) was executed, */
    /* that means the place of new_item will be between insert_ptr and */
    /* insert_ptr->next_ptr, and it is ensured that insert_ptr->next_ptr */
    /* is not NULL, so that insert_ptr->next_ptr->previous_ptr can be */
    /* referenced. In this case, all of step6~9 would be executed. */
    
    /* connect new_item and existing double_list */
    new_item_ptr->next_ptr = insert_ptr->next_ptr; /* step6 */
    new_item_ptr->previous_ptr = insert_ptr;	   /* step7 */
    /* changing pointers of existing items */
    if(insert_ptr->next_ptr != NULL) /* if insert_ptr->next_ptr is NULL, the next operation cannot be done */
      /* in other words, if (insert_ptr->next_ptr != NULL) is not true, that means new item is placed to the tail of the linked list (new item is the greatest value among it and existing linked list), so we must not reconnect next item to new item, since in that case there is no item next to insert_ptr */
      insert_ptr->next_ptr->previous_ptr = new_item_ptr; /* step8 */
    insert_ptr->next_ptr = new_item_ptr;		 /* step9 */
  }
}


void show_struct_double_list(struct double_list *head_ptr){
  struct double_list *current_double_list_ptr;

  if(head_ptr == NULL){		/* when there is no items in the list */
    printf("no items in the double linked list\n");
    return;
  }
  
  current_double_list_ptr = head_ptr;

  while(current_double_list_ptr != NULL){
    printf("%d\n", current_double_list_ptr->data);
    current_double_list_ptr = current_double_list_ptr->next_ptr;
  }
}


/* free all memories allocated for struct */
void free_struct_double_list(struct double_list *head_ptr){
  struct double_list *current_double_list_ptr;
  struct double_list *next_double_list_ptr;

  if(head_ptr == NULL){ /* when there is no items in the list */
    return;
  }
  
  current_double_list_ptr = head_ptr;
  next_double_list_ptr = head_ptr->next_ptr;

  while(current_double_list_ptr != NULL){
    free(current_double_list_ptr);
    if(current_double_list_ptr->next_ptr == NULL)
      break;
    current_double_list_ptr = next_double_list_ptr;
    next_double_list_ptr = next_double_list_ptr->next_ptr;
  }
}

/* identify pointer to the tail of double linked list */
struct double_list *get_tail_ptr(struct double_list *head_ptr){
  struct double_list *tail_ptr;	/* tail pointer of double linked list */
  struct double_list *current_double_list_ptr;

  current_double_list_ptr = head_ptr;
  while(current_double_list_ptr->next_ptr != NULL)
    current_double_list_ptr = current_double_list_ptr->next_ptr;

  tail_ptr = current_double_list_ptr;
  return tail_ptr;
}

void show_struct_double_list_from_tail(struct double_list *tail_ptr){
  struct double_list *current_double_list_ptr;

  current_double_list_ptr = tail_ptr;

  while(current_double_list_ptr != NULL){
    printf("%d\n", current_double_list_ptr->data);
    current_double_list_ptr = current_double_list_ptr->previous_ptr;
  }
}


/* delete all items in list with input value if it exists in the linked list */
void delete_list_val(struct double_list *first_ptr, int val){
  struct double_list *item_ptr = first_ptr; /* item_ptr: target to be deleted */
  struct double_list *before_ptr = NULL;
  struct double_list *after_ptr = first_ptr->next_ptr;
  
  /* see the values in list from first (head) value one by one */
  /* unlike array, binary search can't be used because middle value can't be found */
  /* although values in list are sorted in increasing order */
  while(item_ptr != NULL){
    /* since list items are sorted in ascending order, if value in current item is greater */
    /* than input val, there is no chance that coming values are equal to input val, so break */
    if(item_ptr->data > val)
      break;
    
    if(item_ptr->data == val){
      /* delete this item */
      /* deal with extreme case 3: when number of items in list is only 1 */
      if(before_ptr == NULL && after_ptr == NULL){
	free(item_ptr);
	global_head_ptr = NULL;
	break;
      }
      
      /* deal with extreme case2: when deleted item is placed in the right edge */
      if(after_ptr == NULL){
	before_ptr->next_ptr = after_ptr;
	free(item_ptr);
	break;			/* no need to search further */
      }
      /* notice: the place of before_ptr itself isn't changed */
      if(before_ptr != NULL)
	/* deal with extreme case 1: when deleted item is in left edge (the smallest item) */
	before_ptr->next_ptr = item_ptr->next_ptr;
      after_ptr->previous_ptr = before_ptr; /* == item_ptr->previous_ptr */
      free(item_ptr);
      item_ptr = after_ptr;
      after_ptr = after_ptr->next_ptr;
      /* global_head_ptr has to be changed if deleted item is in left edge */
      if(before_ptr == NULL)
	global_head_ptr = item_ptr;
      /* since item_ptr is already forwarded by one, no need to forward item pointer again */
      continue;			
    }
    /* forward before_ptr, item_ptr and after_prt by one */
    before_ptr = item_ptr;
    item_ptr = after_ptr;
    after_ptr = after_ptr->next_ptr;
  }

}


int main(){
  char line[100];
  int input_value;
  int i = 0;
  struct double_list *tail_ptr;
  
  while(1){
    printf("enter a value:");
    fgets(line, sizeof(line), stdin);
    if(line[0] == '\n')
      break;
    sscanf(line, "%d", &input_value);
    /* since first item has to be made outside of list() */
    if(i == 0){
      global_head_ptr = malloc(sizeof(struct double_list));
      if(global_head_ptr == NULL){
	fprintf(stderr, "Error:unable to allocate memory\n");
	exit(8);
      }
      global_head_ptr->data = input_value;
      global_head_ptr->next_ptr = NULL;
      global_head_ptr->previous_ptr = NULL;
    }
    else
      double_enter(global_head_ptr, input_value);
    ++i;
  }

  printf("the double linked list from the head\n");
  show_struct_double_list(global_head_ptr);	/* show the value of struct list */
  tail_ptr = get_tail_ptr(global_head_ptr);
  printf("the double linked list from the tail\n");
  show_struct_double_list_from_tail(tail_ptr);

  /* delete entered value if it exists */
  printf("enter value you want to delete: ");
  fgets(line, sizeof(line), stdin);
  sscanf(line, "%d", &input_value);
  delete_list_val(global_head_ptr, input_value);

  show_struct_double_list(global_head_ptr);
  
  free_struct_double_list(global_head_ptr);	/* free all memories allocated for struct */

  return 0;
}
