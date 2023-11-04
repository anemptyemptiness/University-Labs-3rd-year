#include <iostream>
#include <filesystem>
#include <Windows.h>
#include "FileManagement.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");

    FilesTree files_tree;
    std::string command = "";

    while (true) {
        std::cout << "я: ";
        std::getline(std::cin, command);
        if (command == "quit") {
            break;
        }
        else if (command.substr(0, 10) == "create_dir") {
            files_tree.createDirCommand(command.substr(11));
        }
        else if (command.substr(0, 11) == "create_file") {
            files_tree.createFileCommand(command.substr(12));
        }
        else if (command.substr(0, 6) == "rename") {
            std::string name = "";
            std::string new_name = "";
            std::cout << "<name> <new_name>\nя: ";
            std::cin >> name >> new_name;
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            files_tree.renameObjectCommand(name, new_name);
        }
        else if (command.substr(0, 6) == "delete") {
            files_tree.deleteObjectCommand(command.substr(7));
        }
        else if (command.substr(0, 4) == "move") {
            std::string name = "";
            std::filesystem::path path_to;
            std::cout << "<name> <to>\nя: ";
            std::cin >> name >> path_to;
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            files_tree.moveObjectCommand(name, path_to);
        }
        else if (command.substr(0, 10) == "change_dir") {
            files_tree.changeDirCommand(command.substr(11));
        }
        else if (command.substr(0, 9) == "show_tree") {
            files_tree.showTreeCommand(command.substr(10));
        }
        else if (command.substr(0, 4) == "info") {
            files_tree.infoCommand();
        }
        else if (command.substr(0, 5) == "count") {
            std::cout << "<name> <type>\nя: ";
            std::string name = "";
            std::string type = "";
            std::cin >> name >> type;

            if (type == "file" || type == "folder") {
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                files_tree.countCommand(name, type);
            }
            else {
                std::cout << "\033[1;31mОшибка: Неверный тип файла. Возможные типы: file / folder\033[0m" << std::endl;
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                continue;
            }
        }
        else {
            std::cout << "\033[1;31mОшибка ввода: Неизвестная команда\033[0m" << std::endl;
        }
    }

    return 0;
}
