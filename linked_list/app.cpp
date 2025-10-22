#include <iostream>
#include "LIB_LinkedList.h"

int main(void) {
    LinkedList list;
    linkedListInit(&list);

    Data* n1 = createNode(10);
    Data* n2 = createNode(20);
    Data* n3 = createNode(30);
    Data* n4 = createNode(40);
    Data* n5 = createNode(50);

    std::cout << "=== Circular Doubly Linked List Demo ===\n\n";

    std::cout << "InsertFirst(10):\n";
    insertFirst(&list, (Node*)n1);
    printList(&list);
    printListReverse(&list);

    std::cout << "\nInsertLast(20):\n";
    insertLast(&list, (Node*)n2);
    printList(&list);
    printListReverse(&list);

    std::cout << "\nInsertLast(30):\n";
    insertLast(&list, (Node*)n3);
    printList(&list);
    printListReverse(&list);

    std::cout << "\nInsertAfter(40 after 20):\n";
    insertAfter(&list, (Node*)n4, (Node*)n2);
    printList(&list);
    printListReverse(&list);

    std::cout << "\nInsertBefore(50 before 10):\n";
    insertBefore(&list, (Node*)n5, (Node*)n1);
    printList(&list);
    printListReverse(&list);

    std::cout << "\nDeleting Node:\n";
    // removeNode(&list, (Node*)n5);
    deleteNode(&list, (Node*)n1);
    printList(&list);
    printListReverse(&list);

    // LinkedList list;
    // linkedListInit(&list);

    // Data* n1 = createNode(10);
    // Data* n2 = createNode(20);
    // Data* n3 = createNode(30);
    // Data* n4 = createNode(40);
    // Data* n5 = createNode(50);

    // insertFirst(&list, (Node*)n1);
    // insertLast(&list, (Node*)n2);
    // insertLast(&list, (Node*)&n3);
    // insertAfter(&list, (Node*)&n4, (Node*)&n2);
    // insertBefore(&list, (Node*)&n5, (Node*)&n1);

    // printList(&list);
    // printListReverse(&list);

    return 0;
}
