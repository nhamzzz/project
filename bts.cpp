#include <iostream>
using namespace std;

// -------------------- Node Structure --------------------
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// -------------------- Insert Function --------------------
// Inserts a value into the binary search tree (BST)
Node* insert(Node* root, int value) {
    if (root == nullptr)
        return new Node(value);

    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);

    return root;
}

// -------------------- Traversal Functions --------------------

// Inorder (Left → Root → Right)
void inorder(Node* root) {
    if (root == nullptr)
        return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

// Preorder (Root → Left → Right)
void preorder(Node* root) {
    if (root == nullptr)
        return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

// Postorder (Left → Right → Root)
void postorder(Node* root) {
    if (root == nullptr)
        return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

// -------------------- Main Function --------------------
int main() {
    Node* root = nullptr;

    // Insert nodes
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 20);
    root = insert(root, 3);
    root = insert(root, 7);
    root = insert(root, 15);
    root = insert(root, 25);

    cout << "=== Binary Search Tree ===\n\n";

    cout << "Inorder Traversal (Left → Root → Right):\n";
    inorder(root);
    cout << "\n\n";

    cout << "Preorder Traversal (Root → Left → Right):\n";
    preorder(root);
    cout << "\n\n";

    cout << "Postorder Traversal (Left → Right → Root):\n";
    postorder(root);
    cout << "\n";

    return 0;
}
