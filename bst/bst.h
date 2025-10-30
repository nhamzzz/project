#ifndef __BST_H__
#define __BST_H__

typedef struct node {
    struct node* left;
    struct node* right;
} node_t;

typedef struct data {
    node_t node;
    int i;
} data_t;

data_t* createData(int value);
void insert(node_t* root, data_t* data);

void traversePreOrder(node_t* node);
void traverseInOrder(node_t* node);
void traversePostOrder(node_t* node);

#endif

