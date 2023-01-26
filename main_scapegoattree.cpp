#include <iostream>

#include "ScapegoatTree.h"

int main() {

    ScapegoatTree<int> tree;



    tree.insert(10);
    tree.insert(15);
    tree.insert(20);
    tree.insert(25);
    tree.printPretty();
    tree.insert(30); 
    tree.printPretty();


    return 0;
}
