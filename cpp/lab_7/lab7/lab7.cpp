#include <iostream>
#include <string>
#include "Library.h"

int main()
{
    srand(time(NULL));

    while (true) {
        unsigned days = 0;
        unsigned count_of_books = 0;
        unsigned count_of_persons = 0;

        std::cout << "Enter count of simulation's days(10-30): ";
        std::cin >> days;
        
        if (days < 10 || days > 30) {
            std::cout << "Wrong count of days" << std::endl;
            continue;
        }

        std::cout << "Enter count of books at Library(20-30): ";
        std::cin >> count_of_books;

        if (count_of_books < 20 || count_of_books > 30) {
            std::cout << "Wrong count of books" << std::endl;
            continue;
        }

        std::cout << "Enter count of persons(1-5): ";
        std::cin >> count_of_persons;

        if (count_of_persons < 1 || count_of_persons > 5) {
            std::cout << "Wrong count of persons" << std::endl;
            continue;
        }

        if (randomFloat(0, 1) > 0.2) {
            std::cout << "The \x1b[32maliens\033[0m have dropped off another visitor(-s)!" << std::endl;
            count_of_persons += randomInt(1, 3);
        }

        Library library(count_of_persons, count_of_books, days);
        library.generateBooks();

        library.getSize();
        std::cout << "\x1b[32mReading process\033[0m:" << std::endl;
        library.work();
    }

    return 0;
}
