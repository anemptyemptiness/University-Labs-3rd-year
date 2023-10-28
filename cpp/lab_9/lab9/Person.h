#pragma once
#include <map>
#include <string>

const std::string activities[2] = { "online", "offline" };

/*
* Функция, генерирующая рандомное целое число из диапазона
* 
* @param min - левая граница диапазона
* @param max - правая граница диапазона
* 
* @return возвращает рандомное целое число
*/
int randomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

/*
* Класс Пользователя
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
		{"А", "A"}, {"М", "M"}, {"Щ", "SHCH"},
		{"Б", "B"}, {"Н", "N"}, {"Ъ", ""},
		{"В", "V"}, {"О", "O"}, {"Ы", "Y"},
		{"Г", "G"}, {"П", "P"}, {"Ь", ""},
		{"Д", "D"}, {"Р", "R"}, {"Э", "E"},
		{"Е", "E"}, {"С", "S"}, {"Ю", "YU"},
		{"Ё", "E"}, {"Т", "T"}, {"Я", "YA"},
		{"Ж", "ZH"}, {"У", "U"}, {"Л", "L"},
		{"З", "Z"}, {"Ф", "F"}, {"Ш", "SH"},
		{"И", "I"}, {"Х", "KH"}, {"Ч", "CH"},
		{"Й", "Y"}, {"Ц", "TS"}, {"К", "K"}
	};

	/*
	* Функция, генерирующая логин
	* 
	* @return возвращает сгенерированный логин
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
	* Функция, генерирующая пароль
	* 
	* @return возвращает сгенерированный пароль
	*/
	std::string generatePassword(int count_symbols) {
		for (size_t i = 0; i < count_symbols; i++)
			this->password += '*';

		return this->password;
	}

	/*
	* Функция, перегружающая оператор >>
	* 
	* @param os - ссылка на поток ввода
	* @param person - ссылка на пользователя
	* 
	* @return возвращает перегруженный поток ввода
	*/
	friend std::ostream& operator<<(std::ostream& os, const Person& person) {
		os << "ФИО: " << person.surname + " " + person.name + " " + person.patronymic << std::endl;
		os << "Возраст: " << person.age << std::endl;
		os << "Логин: " << person.login << std::endl;
		os << "Пароль: " << person.password << std::endl;
		os << "Активность: " << person.activity << std::endl;
		
		return os;
	}

public:
	/*
	* Конструктор класса Пользователя
	*/
	Person() {
	}

	/*
	* Конструктор класса Пользователя
	* 
	* @param name - имя
	* @param surname - фамилия
	* @param patronymic - отчество
	* @param age - возраст
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
			this->login = "А.А.А";

		this->login = generateLogin();
		this->password = generatePassword(randomInt(8, 12));
		this->activity = activities[randomInt(0, 1)];
	}

	/*
	* Функция, возвращающая возраст
	*/
	int getAge() {
		return this->age;
	}

	/*
	* Функция, возвращающая активность в сети
	*/
	std::string getActivity() {
		return this->activity;
	}

	/*
	* Функция, возвращающая фамилию
	*/
	std::string getSurname() {
		return this->surname;
	}

	/*
	* Функция, возвращающая имя
	*/
	std::string getName() {
		return this->name;
	}

	/*
	* Функция, возвращающая отчество
	*/
	std::string getPatronymic() {
		return this->patronymic;
	}
};
