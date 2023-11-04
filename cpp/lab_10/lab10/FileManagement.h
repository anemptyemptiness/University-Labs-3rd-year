#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

/*
* Класс Файла
*/
class File {
private:
	friend class Folder;
	friend class FilesTree;

	fs::path file_path = "";
	std::string name = "";
	std::uintmax_t size = 0;

public:
	/*
	* Дефолтный конструктор класса Файла
	*/
	File() {}

	/*
	* Конструктор класса Файла
	* 
	* @param path - путь к файлу
	* @param name - имя файла
	* @param size - размер файла в Кб
	*/
	File(fs::path path, std::string name, std::uintmax_t size) :
		file_path(path),
		name(name),
		size(size)
	{}
};

/*
* Класс Папки
*/
class Folder {
private:
	friend class File;
	friend class FilesTree;

	fs::path folder_path;
	std::map<std::string, std::vector<File>> folder_info;
	std::map<std::string, std::map<std::string, int>> current_files;

	/*
	* Функция, возвращающая имя конкретного файла
	* 
	* @param path - путь к данному файлу
	* 
	* @return возвращает имя данного файла
	*/
	std::string getCurrentFileName(fs::path path) {
		std::string file_path_str = path.generic_string();
		std::string file_name = file_path_str;

		size_t i = file_path_str.size() - 1;
		size_t j = 0;
		while (file_path_str[i] != '/') {
			file_name[j] = file_path_str[i];
			i--;
			j++;
		}
		file_name.erase(j, file_name.length() - j);
		std::reverse(file_name.begin(), file_name.end());

		return file_name;
	}

	std::string getFileNameWithoutExt(std::string name) {
		std::string new_name = name;
		size_t i = 0;
		size_t j = 0;

		while (name[i] != '.') {
			new_name[j] = name[i];
			i++;
			j++;
		}
		new_name.erase(j, new_name.length() - j);

		return new_name;
	}

public:
	/*
	* Конструктор класса Папки
	* 
	* @param path - путь к папке
	*/
	Folder(fs::path path) :
		folder_path(path)
	{}

	/*
	* Функция, печатающая информацию о файле
	*/
	void getInfo() {
		for (const auto info_dict : folder_info) {
			std::cout << "Директория файла: " << info_dict.first << std::endl;
			for (const auto info_vector : info_dict.second) {
				std::cout << "Описание:\n   Путь: " << info_vector.file_path << std::endl;
				std::cout << "   Имя файла: " << info_vector.name << std::endl;
				std::cout << "   Размер файла: " << info_vector.size << std::endl;
			}
		}
	}

	/*
	* Функция, отображающая файлы и папки в конкретной папке
	*/
	void showTree() {
		std::string current_folder_name = getCurrentFileName(folder_path);
		folder_info.clear();
		current_files.clear();

		if (fs::exists(folder_path) && fs::is_directory(folder_path))
			for (const auto entry : fs::directory_iterator{ folder_path }) {
				int count_current_file = 1;

				if (entry.is_directory()) {
					std::cout << "\033[32mпапка\033[0m: " << entry << std::endl;

					std::string filename = getCurrentFileName(entry);
					
					auto iter = current_files.find(filename);
					if (iter == current_files.end())
						current_files[filename]["folder"] = 1;
					else
						current_files[filename]["folder"] += count_current_file;
				}
				else {
					std::cout << "\033[32mфайл\033[0m: " << entry << std::endl;
					
					std::string filename = getCurrentFileName(entry);
					folder_info[current_folder_name].push_back(File(entry, filename, fs::file_size(entry)));

					auto iter = current_files.find(getFileNameWithoutExt(filename));
					if (iter == current_files.end())
						current_files[getFileNameWithoutExt(filename)]["file"] = 1;
					else
						current_files[getFileNameWithoutExt(filename)]["file"] += count_current_file;
				}
			}
		else
			std::cout << "\033[1;31mПапка не существует или не является папкой\033[0m" << std::endl;
	}
};

/*
* Класс Дерева Файлов
*/
class FilesTree {
private:
	Folder root{fs::current_path()}; // корневая папка
	fs::path root_path;

public:
	/*
	* Конструктор класса Дерева Файлов
	*/
	FilesTree() {
		this->root.showTree();
		this->root_path = fs::current_path();
	}

	/*
	* Функция, возвращающая путь к корневой папке
	*
	* @return возвращает путь к корневой папке
	*/
	fs::path getRootPath() {
		return this->root_path;
	}

	/*
	* Функция, реализующая команду create_dir
	*
	* @param name - имя новой папки
	*/
	void createDirCommand(std::string name) {
		fs::path new_path = name;

		try {
			fs::create_directory(new_path);

			std::cout << "\033[32mПапка успешно создана\033[0m" << std::endl;
		}
		catch (fs::filesystem_error& e) {
			std::cout << "\033[1;31mОшибка создания папки: " << e.what() << "\033[0m" << std::endl;
		}

		root.showTree();
	}

	/*
	* Функция, реализующая команду create_file
	*
	* @param name - имя нового файла
	*/
	void createFileCommand(std::string name) {
		fs::path file_path = name;
		std::ofstream file(file_path);

		if (file.is_open())
			std::cout << "\033[32mФайл успешно создан\033[0m" << std::endl;

		file.close();
		root.showTree();
	}

	/*
	* Функция, реализующая команду rename
	*
	* @param name - часть пути до искомого файла
	* @param new_name - новое имя для файла
	*/
	void renameObjectCommand(std::string name, std::string new_name) {
		if (fs::exists(root.folder_path / name)) {

			fs::path new_path = root.folder_path / new_name;
			fs::path old_path = root.folder_path / name;

			try {
				fs::rename(old_path, new_path);
				std::cout << "\033[32mФайл или папка успешно переименован\033[0m" << std::endl;
				root.showTree();
			}
			catch (const fs::filesystem_error& e) {
				std::cout << "\033[1;31mОшибка переименования: " << e.what() << "\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31mДанного файла или папки не существует\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду delete
	*
	* @param name - имя файла, которого нужно удалить
	*/
	void deleteObjectCommand(std::string name) {
		if (fs::exists(root.folder_path / name)) {
			try {
				fs::remove(root.folder_path / name);

				std::cout << "\033[32mФайл или папка успешно удалены\033[0m" << std::endl;
				root.showTree();
			}
			catch (const fs::filesystem_error& e) {
				std::cout << "\033[1;31mОшибка удаления: " << e.what() << "\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31mДанного файла или папки не существует\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду move
	*
	* @param name - имя искомого файла
	* @param to - путь, куда переместить файл
	*/
	void moveObjectCommand(std::string name, fs::path to) {
		if (fs::exists(root.folder_path / name)) {
			if (fs::exists(to)) {
				try {
					fs::rename(root.folder_path / name, to);
					std::cout << "\033[32mФайл или папка успешно перемещены\033[0m" << std::endl;
				}
				catch (const fs::filesystem_error& e) {
					std::cout << "\033[1;31mОшибка перемещения: " << e.what() << "\033[0m" << std::endl;
				}
			}
			else {
				std::cout << "\033[1;31mПути, куда нужно переместить файл, не существует\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31mИскомого файла или папки не существует\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду change_dir
	*
	* @param name - часть пути рассматриваемой папки для перемещения
	*/
	void changeDirCommand(std::string name) {
		if (name != "..") {
			if (fs::exists(root.folder_path / name)) {
				fs::path new_path = name;
				root.folder_path /= new_path;
				root.showTree();
			}
			else {
				std::cout << "\033[1;31mОшибка: Такого пути нет\033[0m" << std::endl;
				root.folder_path = fs::current_path();
			}
		}
		else if (name == "..") {
			root.folder_path.remove_filename();

			std::string folder_path = root.folder_path.generic_string();
			if (folder_path.length() > 2) {
				folder_path.pop_back();
			}

			root.folder_path = folder_path;
			root.showTree();
		}
		else {
			std::cout << "\033[1;31mОшибка: Задайте путь\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду show_tree
	*
	* @param name - путь для нужной папки, которую нужно отобразить
	*/
	void showTreeCommand(fs::path name) {
		if (fs::exists(root.folder_path / name) && fs::is_directory(root.folder_path / name)) {
			root.folder_path /= name;
			root.showTree();
		}
		else {
			std::cout << "\033[1;31mОбъект является файлом, а не директорией\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду info
	*/
	void infoCommand() {
		root.getInfo();
	}

	/*
	* Функция, реализующая команду count
	* 
	* @param name - имя объекта
	* @param type - тип объекта
	*/
	void countCommand(std::string name, std::string type) {
		auto iter_name = root.current_files.find(name);

		if (iter_name != root.current_files.end()) {
			auto iter_type = root.current_files[name].find(type);

			if (iter_type != root.current_files[name].end()) {
				for (const auto info : root.current_files) {
					if (info.first == name) {
						for (const auto info_deep : info.second) {
							if (info_deep.first == type)
								std::cout << "Имя файла: " + info.first << "\n   Тип: " + info_deep.first << "\n   Количество: " << info_deep.second << std::endl;
						}
					}
				}
			}
			else {
				std::cout << "\033[1;31mДанный файл имеет другой тип\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31mДанного файла не существует\033[0m" << std::endl;
		}
	}

	/*
	* Функция, реализующая команду find
	*/
	void findCommand() {

	}
};
