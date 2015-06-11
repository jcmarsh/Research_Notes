#include <stdio.h>
#include <stdlib.h>


// Linked list struct
struct linked_list {
  void* value;
  struct linked_list* next;
};

// Linked list manipulation functions
void add(struct linked_list *ll, void *value) {
  struct linked_list *new_node = malloc(sizeof(struct linked_list));

  new_node->next = ll->next;
  printf("New node next set to: %p\n", new_node->next);
  new_node->value = value;

  ll->next = new_node;
  printf("ll->next set to: %p\n", ll->next);
}

int length(struct linked_list *ll) {
  if (ll == NULL) {
    printf("\tList null\n");
    return 0;
  } else {
    printf("\tList > 1\n");
    printf("\tNext item: %p\n", ll->next);
    return 1 + length(ll->next);
  }
}

void* remove_first(struct linked_list *ll) {
  return ll->value;
}

int contains(struct linked_list *ll, void *value) {
  if (ll == NULL) {
    return 0;
  } else if (ll->value == value) {
    return 1;
  } else {
    return contains(ll->next, value);
  }
}


void main(void) {
  /// linked list test code.
  printf("Sup?\n");

  struct linked_list list;
  struct linked_list item_2;
  struct linked_list item_3;

  item_2.next = &item_3;
  item_2.value = &"yah!";

  item_3.next = NULL;
  item_3.value = &"ahh!";

  list.next = NULL;
  list.value = &"Hello!";

  //  printf("Test: %s\n", list.value);

  printf("length: %d\n", length(&list));
  
  add(&list, &"World");

  //printf("length: %d\n", length(&item_2));

  //list.next = &item_2;

  printf("length: %d\n", length(&list));

  //  add(list, &"It's me, Mario!");

  //  printf("length: %d\n", length(list));
}
