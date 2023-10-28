#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include "Person.h"

const int CONTAINER_SIZE = 6;

const std::string names[CONTAINER_SIZE] = { "Дмитрий", "Андрей", "Алексей", "Олег", "Семён", "Илья" };
const std::string surnames[CONTAINER_SIZE] = { "Объедков", "Щепелявый", "Сучкин", "Нерусский", "Чакрабоба", "Жюмаперьев"};
const std::string patronymics[CONTAINER_SIZE] = { "Дмитриевич", "Андреевич", "Алексеевич", "Олегович", "Семёнович", "Ильич"};

Person uniquePerson();
std::vector<Person> sortTable(std::vector<Person> persons, std::string name = "", int age = 0);
void findPerson(std::vector<Person> persons, std::string find_name, std::string find_surname, std::string find_patronymic);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");
    size_t size = 0;

    while (true) {
        std::cout << "Сколько человеков хотите?(1-много)\n\x1b[32mввод\033[0m: ";
        std::cin >> size;
        std::cout << std::endl;

        if (size <= 0) {
            std::cout << "Самый умный тут?" << std::endl;
            continue;
        }

        std::vector<Person> persons(size, Person());
        std::generate_n(persons.begin(), size, []() {
            return uniquePerson();
        });

        std::cout << "\x1b[32mПользователи\033[0m:" << std::endl;
        for (auto person : persons)
            std::cout << person << std::endl;

        int count_online = std::count_if(persons.begin(), persons.end(), [](Person person) {
            int count = 0;
            if (person.getActivity() == "online")
                count++;

            return count;
        });

        std::cout << "Количество активных пользователей: \x1b[32m" << count_online << "\033[0m" << std::endl;
        std::cout << "Количество неактивных пользователей: \x1b[32m" << persons.size() - count_online << "\033[0m\n" << std::endl;

        std::map<std::string, int> surnamesakes;
        std::map<std::string, int> namesakes;
        for (auto person : persons) {
            surnamesakes[person.getSurname()] = std::count_if(persons.begin(), persons.end(), [&person](Person another_person) {
                int count = 0;
                if (another_person.getSurname() == person.getSurname()) count++;
                return count;
            });

            namesakes[person.getName()] = std::count_if(persons.begin(), persons.end(), [&person](Person another_person) {
                int count = 0;
                if (another_person.getName() == person.getName()) count++;
                return count;
            });
        }

        for (auto person_surname : surnamesakes)
            std::cout << "Фамилия: " << person_surname.first << "\nКоличество однофамильцев: " << person_surname.second << "\n" << std::endl;

        for (auto person_name : namesakes)
            std::cout << "Имя: " << person_name.first << "\nКоличество тёзок: " << person_name.second << "\n" << std::endl;

        int age_out = 0;
        std::cout << "Введите возраст, чтобы вывести пользователей старше этой цифры\n\x1b[32mввод\033[0m: ";
        std::cin >> age_out;

        std::vector<Person> persons_bigger(persons.size());
        auto iter_end = std::copy_if(persons.begin(), persons.end(), persons_bigger.begin(), [age_out](Person person) {
            return person.getAge() > age_out;
            });
        persons_bigger.erase(iter_end, persons_bigger.end());

        if (!persons_bigger.empty()) {
            std::cout << "\x1b[32mПользователи, старше " << age_out << " лет\033[0m:" << std::endl;
            for (auto person : persons_bigger)
                std::cout << person << std::endl;
        }
        else
            std::cout << "Пользователей старше " << age_out << " \033[31mнет\033[0m в таблице" << std::endl;

        std::string choice = "";

        while (true) {
            std::cout << "Желаете вычислить какого-нибудь пользователя?(да/нет)\n\x1b[32mввод\033[0m: ";
            std::cin >> choice;
            std::cout << std::endl;

            if (choice == "да") {
                std::string find_name = "";
                std::string find_surname = "";
                std::string find_patronymic = "";
                std::cout << "Введите имя человека, которого хотите найти\n\x1b[32mввод\033[0m: ";
                std::cin >> find_name;
                std::cout << "Введите фамилию человека, которого хотите найти\n\x1b[32mввод\033[0m: ";
                std::cin >> find_surname;
                std::cout << "Введите отчество человека, которого хотите найти\n\x1b[32mввод\033[0m: ";
                std::cin >> find_patronymic;
                std::cout << std::endl;
                findPerson(persons, find_name, find_surname, find_patronymic);
            }
            else if (choice == "нет") {
                std::cout << std::endl;
                break;
            }
            else {
                std::cout << "\033[31mТут я решаю, какой ответ правильный\033[0m" << std::endl;
                continue;
            }
        }

        while (true) {
            std::cout << "Желаете отсортировать таблицу?(да/нет)\n\x1b[32mввод\033[0m: ";
            std::cin >> choice;
            std::cout << std::endl;

            if (choice == "да") {
                std::vector<Person> sort_table = persons;
                std::string sort_name = "";
                int sort_age = 0;

                while (true) {
                    std::cout << "По каким категориям хотите сортировать? (введите цифру)" << std::endl;
                    std::cout << "1) имя\n2) возраст\n3) по имени и возрасту\n4) выход\n\x1b[32mввод\033[0m: ";
                    std::cin >> choice;
                    sort_table = persons;

                    if (choice == "1") {
                        std::cout << "Введите имя:\n\x1b[32mввод\033[0m: ";
                        std::cin >> sort_name;
                        sort_table = sortTable(sort_table, sort_name);
                        continue;
                    }
                    else if (choice == "2") {
                        std::cout << "Введите возраст:\n\x1b[32mввод\033[0m: ";
                        std::cin >> sort_age;
                        sort_table = sortTable(sort_table, "", sort_age);
                        continue;
                    }
                    else if (choice == "3") {
                        std::cout << "Введите имя:\n\x1b[32mввод\033[0m: ";
                        std::cin >> sort_name;
                        std::cout << "Введите возраст:\n\x1b[32mввод\033[0m: ";
                        std::cin >> sort_age;
                        sort_table = sortTable(sort_table, sort_name, sort_age);
                        continue;
                    }
                    else if (choice == "4") {
                        std::cout << std::endl;
                        break;
                    }
                    else {
                        std::cout << "Вы ввели \033[31mпарашу\033[0m, давайте заново\n\x1b[32mввод\033[0m: ";
                        std::cout << std::endl;
                        continue;
                    }
                }
            }
            else if (choice == "нет") {
                std::cout << "Ок! До свидания!" << std::endl;
                std::cout << std::endl;
                break;
            }
            else {
                std::cout << "\033[31mТут я решаю, какой ответ правильный\033[0m" << std::endl;
                std::cout << std::endl;
                continue;
            }
        }
    }

    return 0;
}

/*
* Функция, сортирующая таблицу по заданным параметрам
*
* @param persons - вектор сгенерированных пользователей
* @param name - имя, по которому произойдет сортировка
* @param age - возраст, по которому произойдет сортировка
*
* @return возвращает вектор найденных пользователей
*/
std::vector<Person> sortTable(std::vector<Person> persons, std::string name, int age) {
    std::vector<Person> find_persons(persons.size());
    auto iter_end = std::copy_if(persons.begin(), persons.end(), find_persons.begin(), [&name, &age](Person another_person) {
        if (name != "" && age == 0)
            return another_person.getName() == name;
        else if (age != 0 && name == "")
            return another_person.getAge() > age;
        else
            return another_person.getName() == name && another_person.getAge() > age;
    });

    if (find_persons.empty()) {
        std::cout << "\033[31mУвы, таких пользователей не нашлось\033[0m" << std::endl;
    }
    else {
        find_persons.erase(iter_end, find_persons.end());

        for (auto find_person : find_persons) {
            std::cout << find_person << std::endl;
        }
    }

    return find_persons;
}

/*
* Функция, которая производит поиск по ФИО
*
* @param persons - вектор сгенерированных пользователей
* @param find_name - имя, по которому произойдет поиск
* @param find_surname - фамилия, по которой произойдет поиск
* @param find_patronymic - отчество, по которому произойдет поиск
*/
void findPerson(std::vector<Person> persons, std::string find_name, std::string find_surname, std::string find_patronymic) {
    std::vector<Person> find_persons(persons.size());
    auto iter_end = std::copy_if(persons.begin(), persons.end(), find_persons.begin(), [&find_name, &find_surname, &find_patronymic](Person another_person) {
        return another_person.getName() == find_name && another_person.getSurname() == find_surname && another_person.getPatronymic() == find_patronymic;
    });

    if (find_persons.empty()) {
        std::cout << "\033[31mУвы, таких пользователей не нашлось\033[0m" << std::endl;
    }
    else {
        find_persons.erase(iter_end, find_persons.end());

        for (auto find_person : find_persons) {
            std::cout << find_person << std::endl;
        }
    }
}

/*
* Функция, генерирующая пользователя
*
* @return возвращает сгенерированного пользователя
*/
Person uniquePerson() {
    return Person(names[randomInt(0, CONTAINER_SIZE - 1)], surnames[randomInt(0, CONTAINER_SIZE - 1)], patronymics[randomInt(0, CONTAINER_SIZE - 1)], randomInt(18, 65));
}
