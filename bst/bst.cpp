#include "bst.h"
#include <stdio.h>
#include <iostream>

data_t* createData(int value) {
    // Validate value before allocating memory
    if (value <= 0) {
        printf("Invalid value: %d (must be positive)\n", value);
        return NULL;
    }

    // Allocate memory
    data_t* newData = (data_t*)malloc(sizeof(data_t));
    if (newData == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Initialize fields
    newData->i = value;
    newData->node.left = NULL;
    newData->node.right = NULL;

    printf("Node created with value: %d\n", value);
    return newData;
}

void insert(node_t* root, data_t* data) {
    if (root == NULL) {
        return;
    }

    int root_data = ((data_t*)root)->i;

    node_t* nxt_node = NULL;

    if (root_data > data->i) {
        nxt_node = root->left;
        if (root->left == NULL) {
            root->left = (node_t*) data;
            return;
        }
    }
    else {
        nxt_node = root->right;
        if (root->right == NULL) {
            root->right = (node_t*) data;
            return;
        }
    }

    if (nxt_node != NULL) {
        insert(nxt_node, data);
    }
}

void traversePreOrder(node_t* node) {
    if (node == NULL) {
        return;
    }

    int root = ((data_t*)node)->i;

    std::cout << root << " ";   // Visit root
    std::cout << "[Value: " << root << ", Addr: " << node << "] \n";

    traversePreOrder(node->left);             // Visit left
    traversePreOrder(node->right);            // Visit right
}

// INORDER traversal: Left → Root → Right
void traverseInOrder(node_t* node) {
    if (node == NULL) {
        return;
    }

    int root = ((data_t*)node)->i;

    traverseInOrder(node->left);              // Visit left
    std::cout << root << " ";   // Visit root
    std::cout << "[Value: " << root << ", Addr: " << node << "]  \n";
    traverseInOrder(node->right);             // Visit right (FIXED)
}

// POSTORDER traversal: Left → Right → Root
void traversePostOrder(node_t* node) {
    if (node == NULL) {
        return;
    }

    int root = ((data_t*)node)->i;

    traversePostOrder(node->left);            // Visit left
    traversePostOrder(node->right);           // Visit right
    std::cout << root << " ";   // Visit root
    std::cout << "[Value: " << root << ", Addr: " << node << "]  \n";

}
