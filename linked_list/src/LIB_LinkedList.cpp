#include <stdio.h>
#include <iostream>
#include "LIB_LinkedList.h"

// Custom struct to store actual data
typedef struct myNode {
    Node node;
    int data;
} MyNode;

// Initialize the list
void linkedListInit(LinkedList* list) {
    list->head = nullptr;
}

// Insert at the beginning
void insertFirst(LinkedList* list, Node* node_to_insert) {
    if (list == nullptr || node_to_insert == nullptr)
        return;

    if (list->head == nullptr) {
        // First node in the list
        list->head = node_to_insert;
        node_to_insert->next = node_to_insert;
        node_to_insert->prev = node_to_insert;
        return;
    }

    Node* tail = list->head->prev;

    node_to_insert->next = list->head;
    node_to_insert->prev = tail;

    tail->next = node_to_insert;
    list->head->prev = node_to_insert;
    list->head = node_to_insert;
}

// Insert at the end
void insertLast(LinkedList* list, Node* node_to_insert) {
    if (list == nullptr || node_to_insert == nullptr)
        return;

    if (list->head == nullptr) {
        list->head = node_to_insert;
        node_to_insert->next = node_to_insert;
        node_to_insert->prev = node_to_insert;
        return;
    }

    Node* tail = list->head->prev;

    tail->next = node_to_insert;
    node_to_insert->prev = tail;
    node_to_insert->next = list->head;
    list->head->prev = node_to_insert;
}


// Insert after a specific node
void insertAfter(LinkedList* list, Node* node_to_insert, Node* node_ref) {
    if (list == nullptr || node_to_insert == nullptr || node_ref == nullptr)
        return;

    node_to_insert->next = node_ref->next;
    node_to_insert->prev = node_ref;

    if (node_ref->next != nullptr)
        node_ref->next->prev = node_to_insert;

    node_ref->next = node_to_insert;
}

// Insert before a specific node
void insertBefore(LinkedList* list, Node* node_to_insert, Node* node_ref) {
    if (list == nullptr || node_to_insert == nullptr || node_ref == nullptr)
        return;

    node_to_insert->next = node_ref;
    node_to_insert->prev = node_ref->prev;

    if (node_ref->prev != nullptr)
        node_ref->prev->next = node_to_insert;
    else
        list->head = node_to_insert; // Inserting before the first node

    node_ref->prev = node_to_insert;
}

// Remove a node from the list
void removeNode(LinkedList* list, Node* node_to_remove) {
    if (list == nullptr || node_to_remove == nullptr || list->head == nullptr)
        return;

    // Single node case
    if (node_to_remove->next == node_to_remove && node_to_remove->prev == node_to_remove) {
        list->head = nullptr;
        node_to_remove->next = node_to_remove->prev = nullptr;
        return;
    }

    // Fix links
    node_to_remove->prev->next = node_to_remove->next;
    node_to_remove->next->prev = node_to_remove->prev;

    // Update head if needed
    if (list->head == node_to_remove)
        list->head = node_to_remove->next;

    node_to_remove->next = node_to_remove->prev = nullptr;
}


// Print the list (head to tail)
void printList(LinkedList* list) {
    if (list->head == nullptr) {
        std::cout << "List is empty." << std::endl;
        return;
    }

    Node* current = list->head;
    std::cout << "List: ";
    do {
        MyNode* my = (MyNode*)current;
        std::cout << my->data << " -> ";
        current = current->next;
    } while (current != list->head);
    std::cout << "(back to head)" << std::endl;
}

// Print the list in reverse (tail to head)
void printListReverse(LinkedList* list) {
    if (list->head == nullptr) {
        std::cout << "List is empty." << std::endl;
        return;
    }

    Node* tail = list->head->prev;
    Node* current = tail;
    std::cout << "List (reverse): ";
    do {
        MyNode* my = (MyNode*)current;
        std::cout << my->data << " -> ";
        current = current->prev;
    } while (current != tail);
    std::cout << "(back to tail)" << std::endl;
}

int main(void) {
    LinkedList list;
    linkedListInit(&list);

    MyNode n1 = { {nullptr, nullptr}, 10 };
    MyNode n2 = { {nullptr, nullptr}, 20 };
    MyNode n3 = { {nullptr, nullptr}, 30 };
    MyNode n4 = { {nullptr, nullptr}, 40 };
    MyNode n5 = { {nullptr, nullptr}, 50 };

    std::cout << "=== Doubly Linked List Demo ===" << std::endl << std::endl;

    std::cout << "InsertFirst(10):" << std::endl;
    insertFirst(&list, (Node*)&n1);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "InsertLast(20):" << std::endl;
    insertLast(&list, (Node*)&n2);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "InsertLast(30):" << std::endl;
    insertLast(&list, (Node*)&n3);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "InsertAfter(40 after 20):" << std::endl;
    insertAfter(&list, (Node*)&n4, (Node*)&n2);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "InsertBefore(50 before 10):" << std::endl;
    insertBefore(&list, (Node*)&n5, (Node*)&n1);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "RemoveNode(20):" << std::endl;
    removeNode(&list, (Node*)&n2);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "RemoveNode(50):" << std::endl;
    removeNode(&list, (Node*)&n5);
    printList(&list);
    printListReverse(&list);

    std::cout << std::endl << "RemoveNode(30):" << std::endl;
    removeNode(&list, (Node*)&n3);
    printList(&list);
    printListReverse(&list);

    return 0;
}
