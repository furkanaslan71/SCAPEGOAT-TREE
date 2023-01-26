#include <iostream>

#include "TreeMap.h"

int main() {

    TreeMap<std::string, int> treeMap;

    treeMap.print();

    treeMap.update("tokyo", 37468000);
    treeMap.update("delhi", 28514000);
    treeMap.update("shangai", 25582000);
    treeMap.update("sao paulo", 21650000);
    treeMap.update("mexico city", 21581000);
    treeMap.update("cairo", 20076000);
    treeMap.update("mumbai", 19980000);
    treeMap.update("beijing", 19618000);
    treeMap.update("dhaka", 19578000);
    treeMap.update("osaka", 19281000);

    treeMap.print();

    return 0;
}
