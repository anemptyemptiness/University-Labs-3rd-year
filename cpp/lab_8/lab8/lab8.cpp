#include <iostream>
#include "DoubleList.h"

int main()
{
    DoubleList<int> lst;
    lst.pushFront(2);
    lst.pushFront(4);
    lst.pushBack(6);
    std::cout << "Is empty? " << lst.isEmpty() << std::endl;
    std::cout << "Size is: " << lst.getSize() << std::endl;
    std::cout << "Front is: " << lst.getFront() << std::endl;
    std::cout << "Back is: " << lst.getBack() << std::endl;

    for (const auto val : lst) {
        std::cout << val << std::endl;
    }

    return 0;
}
