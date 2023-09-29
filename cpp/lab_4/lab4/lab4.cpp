#include <iostream>
#include <vector>
#include <string>
#include "Wizard.h"
#include "Archer.h"
#include "Swordsman.h"
#include "Mercenary.h"
#include "Guild.h"
#include "Weapons.h"

enum class eUnit {
    ArcherType, SwordsmanType, WizardType
};

int randomInt(int min, int max);
void generateUnits(Guild* guild, std::string distance[], size_t N);
void checkGuild(Guild guild);
void createArmy(std::vector<Mercenary> from, std::vector<Mercenary>* current, size_t which);
void checkArmy(std::vector<Mercenary> army);
void generateIdealArmy(std::vector<std::string>* army, std::string distance[], int* wallet, int* count_bougth_units);
void checkIdealArmy(std::vector<std::string> army);
void printRules();

int main()
{
    srand(time(NULL));

    std::string distance[3] = {"Near", "Medium", "Long"};
    std::vector<Mercenary> list_of_army;
    std::vector<std::string> my_own_army;
    Guild guild = Guild();

    int N = 0;

    std::cout << "Welcome! Please enter the count of the mercenaries: " << std::endl;
    while (true) {        
        try {
            std::cin >> N;

            if (N < 6) {
                throw std::invalid_argument("Invalid count! Try again...");
            }

            while (true) {
                printRules();

                char mode = 0;
                std::cout << "me: ";
                std::cin >> mode;
                std::cout << std::endl;

                if (mode == 'g') {
                    generateUnits(&guild, distance, N);
                    checkGuild(guild);
                }
                else if (mode == 'a') {
                    if (guild.mercenaries.empty()) {
                        std::cout << "Firstly you need to generate the Guild! Enter the \"g\" command" << std::endl;
                    }
                    else {
                        while (true) {
                            int which = 0;
                            std::cout << "Enter \"0\" to leave\nWhich unit do you want to add to your army? Enter the number of unit: ";
                            std::cin >> which;

                            if (which < 0 || which > N) {
                                std::cout << "Wrong number. Try again..." << std::endl;
                                continue;
                            }
                            else if (!which) {
                                std::cout << "Your army:\n" << std::endl;
                                checkArmy(list_of_army);
                                break;
                            }

                            createArmy(guild.mercenaries, &list_of_army, which - 1);
                            std::cout << "Done!" << std::endl;
                        }
                    }
                }
                else if (mode == 'i') {
                    if (!my_own_army.empty()) {
                        std::cout << "You've been there once!" << std::endl;
                    }
                    else {
                        std::cout << "How many coins do you have? (min. 7500)" << std::endl;

                        int wallet = 0;
                        int count_bougth_units = 0;

                        std::cout << "me: ";
                        std::cin >> wallet;

                        if (wallet < 7500 || wallet < 0 || wallet == 0) {
                            std::cout << "You're broke! Vpahivay na zavode pobol'she" << std::endl;
                        }
                        else {
                            generateIdealArmy(&my_own_army, distance, &wallet, &count_bougth_units);
                            checkIdealArmy(my_own_army);

                            std::cout << "Army was generated successfully!" << std::endl;

                            if (!wallet || wallet < 1500) {
                                std::cout << "The coins ran out :(" << std::endl;
                                std::cout << "We've bougth " << count_bougth_units << " units!" << std::endl;
                            }
                        }
                    }
                }
                else if (mode == 'c') {
                    std::cout << "Please enter the count of the mercenaries: " << std::endl;
                    break;
                }
                else {
                    std::cout << "Invalid command! Try again..." << std::endl;
                }
            }
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}

/*
 * Рандомайзер целых чисел
 *
 * @param min - левая граница диапазона.
 * @param max - правая граница диапазона.
 * @return возвращает случайное целое число из диапазона.
 */
int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*
 * Генерация мага
 *
 * @param guild - указатель на объект класса Guild
 * @param distance[] - массив строк, обозначающих дистанцию боя юнита
 * @param N - количество юнитов
 * @param index - уникальный индекс юнита
 * @param uniq - строковое значение для указания особенных характеристик бойца
 */
void generateWizard(Guild* guild, std::string distance[], size_t N, size_t index, std::string uniq) {
    if (uniq == "medium") {
        Wizard unit(randomInt(1, 100), distance[1], 1000, index + 1);
        Magic magic(randomInt(1, 100));

        Mercenary mercenary(&unit, &magic);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "long") {
        Wizard unit(randomInt(1, 100), distance[2], 1000, index + 1);
        Magic magic(randomInt(1, 100));

        Mercenary mercenary(&unit, &magic);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "any") {
        Wizard unit(randomInt(1, 100), distance[randomInt(0, 2)], 1000, index + 1);
        Magic magic(randomInt(1, 100));

        Mercenary mercenary(&unit, &magic);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "universal") {
        Wizard unit(randomInt(50, 100), distance[randomInt(0, 2)], 1500, index + 1);
        Magic magic(randomInt(50, 100));

        Mercenary mercenary(&unit, &magic);
        guild->mercenaries.push_back(mercenary);
    }
}


/*
 * Генерация лучника
 *
 * @param guild - указатель на объект класса Guild
 * @param distance[] - массив строк, обозначающих дистанцию боя юнита
 * @param N - количество юнитов
 * @param index - уникальный индекс юнита
 * @param uniq - строковое значение для указания особенных характеристик бойца
 */
void generateArcher(Guild* guild, std::string distance[], size_t N, size_t index, std::string uniq) {
    if (uniq == "medium") {
        Archer unit(randomInt(1, 100), distance[1], 1000, index + 1);
        Bow bow(randomInt(1, 100));

        Mercenary mercenary(&unit, &bow);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "long") {
        Archer unit(randomInt(1, 100), distance[2], 1000, index + 1);
        Bow bow(randomInt(1, 100));

        Mercenary mercenary(&unit, &bow);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "any") {
        Archer unit(randomInt(1, 100), distance[randomInt(0, 2)], 1000, index + 1);
        Bow bow(randomInt(1, 100));

        Mercenary mercenary(&unit, &bow);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "universal") {
        Archer unit(randomInt(50, 100), distance[randomInt(0, 2)], 1500, index + 1);
        Bow bow(randomInt(50, 100));

        Mercenary mercenary(&unit, &bow);
        guild->mercenaries.push_back(mercenary);
    }
}

/*
 * Генерация мечника
 *
 * @param guild - указатель на объект класса Guild
 * @param distance[] - массив строк, обозначающих дистанцию боя юнита
 * @param N - количество юнитов
 * @param index - уникальный индекс юнита
 * @param uniq - строковое значение для указания особенных характеристик бойца
 */
void generateSwordsman(Guild* guild, std::string distance[], size_t N, size_t index, std::string uniq) {
    if (uniq == "tank") {
        Swordsman unit(100, distance[0], 2500, index + 1);
        Sword sword(randomInt(1, 100));

        Mercenary mercenary(&unit, &sword);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "hard_damage") {
        Swordsman unit(randomInt(1, 100), distance[0], 2500, index + 1);
        Sword sword(100);

        Mercenary mercenary(&unit, &sword);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "any") {
        Swordsman unit(randomInt(1, 100), distance[0], 1000, index + 1);
        Sword sword(randomInt(1, 100));

        Mercenary mercenary(&unit, &sword);
        guild->mercenaries.push_back(mercenary);
    }
    if (uniq == "universal") {
        Swordsman unit(randomInt(50, 100), distance[0], 1500, index + 1);
        Sword sword(randomInt(50, 100));

        Mercenary mercenary(&unit, &sword);
        guild->mercenaries.push_back(mercenary);
    }
}

/*
 * Генерация гильдии
 *
 * @param guild - указатель на объект класса Guild
 * @param distance[] - массив строк, обозначающих дистанцию боя юнита
 * @param N - количество юнитов
 */
void generateUnits(Guild* guild, std::string distance[], size_t N) {
    int count_uniq_wizards = 0;
    int count_uniq_archers = 0;
    int count_uniq_swordsmans = 0;

    for (size_t i = 0; i < N; i++) {
        int what_unit = randomInt(0, 2);
        if (count_uniq_wizards == 0) {
            generateWizard(guild, distance, N, i, "medium");
            count_uniq_wizards++;
        }
        else if (count_uniq_wizards == 1) {
            generateWizard(guild, distance, N, i, "long");
            count_uniq_wizards++;
        }
        else if (count_uniq_archers == 0) {
            generateArcher(guild, distance, N, i, "medium");
            count_uniq_archers++;
        }
        else if (count_uniq_archers == 1) {
            generateArcher(guild, distance, N, i, "long");
            count_uniq_archers++;
        }
        else if (count_uniq_swordsmans == 0) {
            generateSwordsman(guild, distance, N, i, "tank");
            count_uniq_swordsmans++;
        }
        else if (count_uniq_swordsmans == 1) {
            generateSwordsman(guild, distance, N, i, "hard_damage");
            count_uniq_swordsmans++;
        }
        else {
            if (what_unit == int(eUnit::WizardType)) {
                generateWizard(guild, distance, N, i, "any");
            }
            if (what_unit == int(eUnit::ArcherType)) {
                generateArcher(guild, distance, N, i, "any");
            }
            if (what_unit == int(eUnit::SwordsmanType)) {
                generateSwordsman(guild, distance, N, i, "any");
            }
        }
    }
}

/*
 * Вывод списка гильдии на экран
 *
 * @param guild - объект класса Guild
 */
void checkGuild(Guild guild) {
    guild.printUnits();
}

/*
 * Генерация своей армии на базе юнитов, что есть в гильдии
 *
 * @param from - вектор юнитов гильдии
 * @param current - вектор юнитов собственной армии, куда добавляются выбранные юниты из гильдии
 * @param which - индекс юнита, которого выбирает пользователь
 */
void createArmy(std::vector<Mercenary> from, std::vector<Mercenary>* current, size_t which) {
    current->push_back(from[which]);
}

/*
 * Вывод списка армии на экран
 *
 * @param army - вектор юнитов собственной армии для вывода на экран
 */
void checkArmy(std::vector<Mercenary> army) {
    for (Mercenary element : army) {
        std::cout << element.info_str << "\n\n";
    }
}

/*
 * Генерация идеальной армии с качественными характеристиками
 *
 * @param army - указатель на вектор юнитов идеальной армии, куда будут добавляться купленные юниты 
 * @param distance[] - массив строк, обозначающих дистанцию боя юнита
 * @param wallet - указатель на кошелек пользователя
 * @param count_bought_units - счётчик купленных бойцов для ведения отчёта по количеству купленных юнитов
 */
void generateIdealArmy(std::vector<std::string>* army, std::string distance[], int* wallet, int* count_bougth_units) {
    int const_price = 1500;
    
    while ((*wallet) > 0) {
        if (!(*wallet) || (*wallet) < 1500) {
            std::cout << "The coins ran out :(" << std::endl;
        }

        int what_unit = randomInt(0, 2);
        if (what_unit == int(eUnit::WizardType)) {
            Wizard unit(randomInt(50, 100), distance[randomInt(0, 2)], const_price, 777);
            Magic magic(randomInt(50, 100));

            Mercenary mercenary(&unit, &magic);
            army->push_back(mercenary.info_str);
        }
        if (what_unit == int(eUnit::ArcherType)) {
            Archer unit(randomInt(50, 100), distance[randomInt(0, 2)], const_price, 777);
            Bow bow(randomInt(50, 100));

            Mercenary mercenary(&unit, &bow);
            army->push_back(mercenary.info_str);
        }
        if (what_unit == int(eUnit::SwordsmanType)) {
            Swordsman unit(randomInt(50, 100), distance[0], const_price, 777);
            Sword sword(randomInt(50, 100));

            Mercenary mercenary(&unit, &sword);
            army->push_back(mercenary.info_str);
        }

        (*wallet) -= const_price;
        (*count_bougth_units)++;
    }
}

/*
 * Вывод идеальной армии на экран
 *
 * @param army - вектор юнитов идеальной армии
 */
void checkIdealArmy(std::vector<std::string> army) {
    for (auto element : army) {
        std::cout << element << "\n\n";
    }
}

/*
 * Вывод команд приложения на экран
 */
void printRules() {
    std::cout << "Main commands:\ng - generate the Guild\na - create your army\ni - create ideal army\nc - leave out" << std::endl;
}
