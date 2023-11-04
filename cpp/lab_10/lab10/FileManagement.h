#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

/*
* ����� �����
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
	* ��������� ����������� ������ �����
	*/
	File() {}

	/*
	* ����������� ������ �����
	* 
	* @param path - ���� � �����
	* @param name - ��� �����
	* @param size - ������ ����� � ��
	*/
	File(fs::path path, std::string name, std::uintmax_t size) :
		file_path(path),
		name(name),
		size(size)
	{}
};

/*
* ����� �����
*/
class Folder {
private:
	friend class File;
	friend class FilesTree;

	fs::path folder_path;
	std::map<std::string, std::vector<File>> folder_info;
	std::map<std::string, std::map<std::string, int>> current_files;

	/*
	* �������, ������������ ��� ����������� �����
	* 
	* @param path - ���� � ������� �����
	* 
	* @return ���������� ��� ������� �����
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
	* ����������� ������ �����
	* 
	* @param path - ���� � �����
	*/
	Folder(fs::path path) :
		folder_path(path)
	{}

	/*
	* �������, ���������� ���������� � �����
	*/
	void getInfo() {
		for (const auto info_dict : folder_info) {
			std::cout << "���������� �����: " << info_dict.first << std::endl;
			for (const auto info_vector : info_dict.second) {
				std::cout << "��������:\n   ����: " << info_vector.file_path << std::endl;
				std::cout << "   ��� �����: " << info_vector.name << std::endl;
				std::cout << "   ������ �����: " << info_vector.size << std::endl;
			}
		}
	}

	/*
	* �������, ������������ ����� � ����� � ���������� �����
	*/
	void showTree() {
		std::string current_folder_name = getCurrentFileName(folder_path);
		folder_info.clear();
		current_files.clear();

		if (fs::exists(folder_path) && fs::is_directory(folder_path))
			for (const auto entry : fs::directory_iterator{ folder_path }) {
				int count_current_file = 1;

				if (entry.is_directory()) {
					std::cout << "\033[32m�����\033[0m: " << entry << std::endl;

					std::string filename = getCurrentFileName(entry);
					
					auto iter = current_files.find(filename);
					if (iter == current_files.end())
						current_files[filename]["folder"] = 1;
					else
						current_files[filename]["folder"] += count_current_file;
				}
				else {
					std::cout << "\033[32m����\033[0m: " << entry << std::endl;
					
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
			std::cout << "\033[1;31m����� �� ���������� ��� �� �������� ������\033[0m" << std::endl;
	}
};

/*
* ����� ������ ������
*/
class FilesTree {
private:
	Folder root{fs::current_path()}; // �������� �����
	fs::path root_path;

public:
	/*
	* ����������� ������ ������ ������
	*/
	FilesTree() {
		this->root.showTree();
		this->root_path = fs::current_path();
	}

	/*
	* �������, ������������ ���� � �������� �����
	*
	* @return ���������� ���� � �������� �����
	*/
	fs::path getRootPath() {
		return this->root_path;
	}

	/*
	* �������, ����������� ������� create_dir
	*
	* @param name - ��� ����� �����
	*/
	void createDirCommand(std::string name) {
		fs::path new_path = name;

		try {
			fs::create_directory(new_path);

			std::cout << "\033[32m����� ������� �������\033[0m" << std::endl;
		}
		catch (fs::filesystem_error& e) {
			std::cout << "\033[1;31m������ �������� �����: " << e.what() << "\033[0m" << std::endl;
		}

		root.showTree();
	}

	/*
	* �������, ����������� ������� create_file
	*
	* @param name - ��� ������ �����
	*/
	void createFileCommand(std::string name) {
		fs::path file_path = name;
		std::ofstream file(file_path);

		if (file.is_open())
			std::cout << "\033[32m���� ������� ������\033[0m" << std::endl;

		file.close();
		root.showTree();
	}

	/*
	* �������, ����������� ������� rename
	*
	* @param name - ����� ���� �� �������� �����
	* @param new_name - ����� ��� ��� �����
	*/
	void renameObjectCommand(std::string name, std::string new_name) {
		if (fs::exists(root.folder_path / name)) {

			fs::path new_path = root.folder_path / new_name;
			fs::path old_path = root.folder_path / name;

			try {
				fs::rename(old_path, new_path);
				std::cout << "\033[32m���� ��� ����� ������� ������������\033[0m" << std::endl;
				root.showTree();
			}
			catch (const fs::filesystem_error& e) {
				std::cout << "\033[1;31m������ ��������������: " << e.what() << "\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31m������� ����� ��� ����� �� ����������\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� delete
	*
	* @param name - ��� �����, �������� ����� �������
	*/
	void deleteObjectCommand(std::string name) {
		if (fs::exists(root.folder_path / name)) {
			try {
				fs::remove(root.folder_path / name);

				std::cout << "\033[32m���� ��� ����� ������� �������\033[0m" << std::endl;
				root.showTree();
			}
			catch (const fs::filesystem_error& e) {
				std::cout << "\033[1;31m������ ��������: " << e.what() << "\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31m������� ����� ��� ����� �� ����������\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� move
	*
	* @param name - ��� �������� �����
	* @param to - ����, ���� ����������� ����
	*/
	void moveObjectCommand(std::string name, fs::path to) {
		if (fs::exists(root.folder_path / name)) {
			if (fs::exists(to)) {
				try {
					fs::rename(root.folder_path / name, to);
					std::cout << "\033[32m���� ��� ����� ������� ����������\033[0m" << std::endl;
				}
				catch (const fs::filesystem_error& e) {
					std::cout << "\033[1;31m������ �����������: " << e.what() << "\033[0m" << std::endl;
				}
			}
			else {
				std::cout << "\033[1;31m����, ���� ����� ����������� ����, �� ����������\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31m�������� ����� ��� ����� �� ����������\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� change_dir
	*
	* @param name - ����� ���� ��������������� ����� ��� �����������
	*/
	void changeDirCommand(std::string name) {
		if (name != "..") {
			if (fs::exists(root.folder_path / name)) {
				fs::path new_path = name;
				root.folder_path /= new_path;
				root.showTree();
			}
			else {
				std::cout << "\033[1;31m������: ������ ���� ���\033[0m" << std::endl;
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
			std::cout << "\033[1;31m������: ������� ����\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� show_tree
	*
	* @param name - ���� ��� ������ �����, ������� ����� ����������
	*/
	void showTreeCommand(fs::path name) {
		if (fs::exists(root.folder_path / name) && fs::is_directory(root.folder_path / name)) {
			root.folder_path /= name;
			root.showTree();
		}
		else {
			std::cout << "\033[1;31m������ �������� ������, � �� �����������\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� info
	*/
	void infoCommand() {
		root.getInfo();
	}

	/*
	* �������, ����������� ������� count
	* 
	* @param name - ��� �������
	* @param type - ��� �������
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
								std::cout << "��� �����: " + info.first << "\n   ���: " + info_deep.first << "\n   ����������: " << info_deep.second << std::endl;
						}
					}
				}
			}
			else {
				std::cout << "\033[1;31m������ ���� ����� ������ ���\033[0m" << std::endl;
			}
		}
		else {
			std::cout << "\033[1;31m������� ����� �� ����������\033[0m" << std::endl;
		}
	}

	/*
	* �������, ����������� ������� find
	*/
	void findCommand() {

	}
};
