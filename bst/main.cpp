#include "bst.h"
#include <stdio.h>
#include <iostream>

int main(void) {
    data_t* d1 = createData(1);
    data_t* d2 = createData(6);
    data_t* d3 = createData(9);
    data_t* d4 = createData(10);

    insert((node_t*)d1, d2);
    insert((node_t*)d1, d3);
    insert((node_t*)d1, d4);

    std::cout << "\nPreOrder: \n";
    traversePreOrder((node_t*)d1);

    std::cout << "\nInOrder: \n";
    traverseInOrder((node_t*)d1);

    std::cout << "\nPostOrder: \n";
    traversePostOrder((node_t*)d1);

    return 0;
}
