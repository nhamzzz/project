#ifndef __LIB_LINKED_LIST_H__
#define __LIB_LINKED_LIST_H__   

typedef struct node {
    struct node* next;
    struct node* prev;
} Node;

typedef struct {
    Node* head;
} LinkedList;

typedef struct data {
    Node node;
    int data;
} Data;

void linkedListInit(LinkedList* list);
void insertLast(LinkedList* list, Node* node_to_insert);
void insertFirst(LinkedList* list, Node* node_to_insert);

void insertAfter(LinkedList* list, Node* node_to_insert, Node* node_ref);
void insertBefore(LinkedList* list, Node* node_to_insert, Node* node_ref);
void removeNode(LinkedList* list, Node* node_to_remove);

void printList(LinkedList* list);
void printListReverse(LinkedList* list);
void deleteNode(LinkedList* list, Node* node_to_delete);
Data* createNode(int value);

#endif
