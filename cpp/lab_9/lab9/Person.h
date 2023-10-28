#pragma once
#include <map>
#include <string>

const std::string activities[2] = { "online", "offline" };

/*
* �������, ������������ ��������� ����� ����� �� ���������
* 
* @param min - ����� ������� ���������
* @param max - ������ ������� ���������
* 
* @return ���������� ��������� ����� �����
*/
int randomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

/*
* ����� ������������
*/
class Person {
private:
	std::string name = "";
	std::string surname = "";
	std::string patronymic = "";
	short age = 0;
	std::string login = "";
	std::string password = "";
	std::string activity = "";

	std::map<std::string, std::string> transliteration 
	{
		{"�", "A"}, {"�", "M"}, {"�", "SHCH"},
		{"�", "B"}, {"�", "N"}, {"�", ""},
		{"�", "V"}, {"�", "O"}, {"�", "Y"},
		{"�", "G"}, {"�", "P"}, {"�", ""},
		{"�", "D"}, {"�", "R"}, {"�", "E"},
		{"�", "E"}, {"�", "S"}, {"�", "YU"},
		{"�", "E"}, {"�", "T"}, {"�", "YA"},
		{"�", "ZH"}, {"�", "U"}, {"�", "L"},
		{"�", "Z"}, {"�", "F"}, {"�", "SH"},
		{"�", "I"}, {"�", "KH"}, {"�", "CH"},
		{"�", "Y"}, {"�", "TS"}, {"�", "K"}
	};

	/*
	* �������, ������������ �����
	* 
	* @return ���������� ��������������� �����
	*/
	std::string generateLogin() {
		std::string new_login = "";

		for (size_t i = 0; i < this->login.length(); i++) {
			char symbol = this->login[i];

			if (symbol != '.' && symbol != ' ') {
				int upper_symbol = std::toupper(symbol);
				symbol = static_cast<char>(upper_symbol);
				for (auto letter : transliteration) {
					std::string temp = std::string(1, symbol);
					if (temp == letter.first) {
						new_login += letter.second;
					}
				}
			}
			else
				new_login += ".";
		}

		for (size_t i = 5; i < new_login.length(); i++) {
			char symbol = new_login[i];
			int upper_symbol = std::tolower(symbol);
			symbol = static_cast<char>(upper_symbol);
			new_login[i] = symbol;
		}

		return new_login;
	}

	/*
	* �������, ������������ ������
	* 
	* @return ���������� ��������������� ������
	*/
	std::string generatePassword(int count_symbols) {
		for (size_t i = 0; i < count_symbols; i++)
			this->password += '*';

		return this->password;
	}

	/*
	* �������, ������������� �������� >>
	* 
	* @param os - ������ �� ����� �����
	* @param person - ������ �� ������������
	* 
	* @return ���������� ������������� ����� �����
	*/
	friend std::ostream& operator<<(std::ostream& os, const Person& person) {
		os << "���: " << person.surname + " " + person.name + " " + person.patronymic << std::endl;
		os << "�������: " << person.age << std::endl;
		os << "�����: " << person.login << std::endl;
		os << "������: " << person.password << std::endl;
		os << "����������: " << person.activity << std::endl;
		
		return os;
	}

public:
	/*
	* ����������� ������ ������������
	*/
	Person() {
	}

	/*
	* ����������� ������ ������������
	* 
	* @param name - ���
	* @param surname - �������
	* @param patronymic - ��������
	* @param age - �������
	*/
	Person(std::string name, 
		std::string surname, 
		std::string patronymic,
		short age) : 
		name{name},
		surname{surname},
		patronymic{patronymic},
		age{age} {
		this->login = std::string(1, this->name[0])
			+ "."
			+ std::string(1, this->patronymic[0])
			+ "."
			+ this->surname;

		if (!this->age)
			this->login = "�.�.�";

		this->login = generateLogin();
		this->password = generatePassword(randomInt(8, 12));
		this->activity = activities[randomInt(0, 1)];
	}

	/*
	* �������, ������������ �������
	*/
	int getAge() {
		return this->age;
	}

	/*
	* �������, ������������ ���������� � ����
	*/
	std::string getActivity() {
		return this->activity;
	}

	/*
	* �������, ������������ �������
	*/
	std::string getSurname() {
		return this->surname;
	}

	/*
	* �������, ������������ ���
	*/
	std::string getName() {
		return this->name;
	}

	/*
	* �������, ������������ ��������
	*/
	std::string getPatronymic() {
		return this->patronymic;
	}
};
