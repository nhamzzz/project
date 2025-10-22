#include <iostream>
#include "LIB_LinkedList.h"

// Initialize the list
void linkedListInit(LinkedList* list) {
    list->head = nullptr;
}

// Create node
Data* createNode(int value) {
    Data* newNode = new Data;
    newNode->data = value;
    newNode->node.next = nullptr;
    newNode->node.prev = nullptr;
    return newNode;
}

// Insert at the beginning
void insertFirst(LinkedList* list, Node* node_to_insert) {
    if (list == nullptr || node_to_insert == nullptr)
        return;

    if (list->head == nullptr) {
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

    node_ref->next->prev = node_to_insert;
    node_ref->next = node_to_insert;

    // // if we inserted after the tail (head->prev), update tail's next
    // if (node_ref == list->head->prev)
    //     list->head->prev = node_to_insert;
}

// Insert before a specific node
void insertBefore(LinkedList* list, Node* node_to_insert, Node* node_ref) {
    if (list == nullptr || node_to_insert == nullptr || node_ref == nullptr)
        return;

    node_to_insert->next = node_ref;
    node_to_insert->prev = node_ref->prev;

    node_ref->prev->next = node_to_insert;
    node_ref->prev = node_to_insert;

    // If we inserted before the head, update head pointer
    if (node_ref == list->head)
        list->head = node_to_insert;
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

    node_to_remove->prev->next = node_to_remove->next;
    node_to_remove->next->prev = node_to_remove->prev;

    if (list->head == node_to_remove)
        list->head = node_to_remove->next;

    node_to_remove->next = node_to_remove->prev = nullptr;
}

// Detaches and deallocates (frees) memory
void deleteNode(LinkedList* list, Node* node_to_delete) {
    if (list == nullptr || node_to_delete == nullptr)
        return;

    removeNode(list, node_to_delete); // unlink from the list

    // Now safely delete the node memory
    Data* dataNode = (Data*)node_to_delete;
    delete dataNode;
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
        Data* dataNode = (Data*)current;
        std::cout << dataNode->data;
        if (current->next != list->head)
            std::cout << " -> ";
        else
            std::cout << " -> (back to head)";
        current = current->next;
    } while (current != list->head);
    std::cout << std::endl;
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
        Data* dataNode = (Data*)current;
        std::cout << dataNode->data;
        if (current->prev != tail)
            std::cout << " <- ";
        else
            std::cout << " <- (back to tail)";
        current = current->prev;
    } while (current != tail);
    std::cout << std::endl;
}
