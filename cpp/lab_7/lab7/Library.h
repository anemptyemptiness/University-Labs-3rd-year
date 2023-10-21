#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>

/*
* Функция, возвращающая случайное дробное число из диапазона
* 
* @param min - левая граница
* @param max - правая граница
* @return возвращает случайное дробное число из диапазона
*/
float randomFloat(int min, int max) {
	return (float)(rand()) / RAND_MAX * (max - min) + min;
}

/*
* Функция, возвращающая случайное целое число из диапазона
*
* @param min - левая граница
* @param max - правая граница
* @return возвращает случайное целое число из диапазона
*/
int randomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

/*
Класс посетителя
*/
class Person {
private:
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> read_books; // словарь прочтеных книг

	int age = 0;
	std::string name = "";
	std::string prefers_volume = "";
	std::string prefers_genre = "";
	int speed_of_reading = 0; // коэффициент скорости чтения
	int pages_in_day = 30; // дефолтная скорость чтения
	int comlited_books = 0; // количество прочтенных книг
	int timeout = 0; // флаг таймаута
	int is_finished_reading = 0; // закончил ли юзер читать книги за отведенный срок n-дней
	int days_of_completed_reading = 0; // количество дней успешного прочтения книг
	int remaining_days = 0; // количество оставшихся дней симуляции

	/*
	* Функция, печатающая список прочтенных книг
	*
	* @param dict - словарь книг
	*/
	void print(std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> dict) {
		for (const auto& title : dict) {
			std::cout << "Title: " << title.first << std::endl;
			for (const auto& author : title.second) {
				std::cout << "Author: " << author.first << std::endl;
				for (const auto& genre : author.second) {
					std::cout << "Genre: " << genre.first << std::endl;
					for (const auto& volume : genre.second) {
						std::cout << "Volume: " << volume.first << std::endl;
						for (const auto& censore : volume.second) {
							std::cout << "Censore age: " << censore.first << "+" << std::endl;
							std::cout << "Rating: " << censore.second;
							std::cout << std::endl;
						}
					}
				}
			}
			std::cout << "\n" << std::endl;
		}
	}

public:
	/*
	* Конструктор класса Посетителя
	*
	* @param name - имя посетителя
	* @param age - возраст посетителя
	* @param prefers_volume - предпочитаемый объем книги
	* @param prefers_genre - предпочитаемый жанр
	*/
	Person(std::string name, int age, std::string prefers_volume, std::string prefers_genre) :
		name{ name },
		age{ age },
		prefers_volume{ prefers_volume },
		prefers_genre{ prefers_genre }
		{}

	/* 
	* Функция, реализующая логику чтения книги
	* 
	* @param volume - количество страниц в книге
	* @param days - количество дней работы симуляции
	*/
	void read(int volume, int days) {
		int day = 0;
		int koef = randomInt(2, 3);
		this->speed_of_reading = this->pages_in_day * koef;

		while (true) {
			if (day != days && volume == 0) {
				std::cout << this->name + " read the book for " << day << " day(-s)" << std::endl;
				this->comlited_books += 1;
				this->remaining_days = day;
				this->days_of_completed_reading += this->remaining_days;
				break;
			}
			else if (day == days && volume > 0) {
				std::cout << "\033[31m" << this->name + " is still reading the book\033[0m\n" << std::endl;
				this->remaining_days = day;
				this->timeout = 1;
				this->is_finished_reading = 1;
				break;
			}
			else if (day == days && volume == 0) {
				std::cout << this->name + " read the book for " << day << " day(-s)\n" << std::endl;
				this->comlited_books += 1;
				this->remaining_days = day;
				this->days_of_completed_reading += this->remaining_days;
				break;
			}
		
			volume -= this->speed_of_reading > volume ? volume : this->speed_of_reading;
			day++;
		}
	}

	/*
	* Функция, получающая возраст посетителя
	*
	* @return возвращает возраст
	*/
	int getAge() {
		return this->age;
	}

	/*
	* Функция, получающая имя посетителя
	* 
	* @return возвращает имя
	*/
	std::string getName() {
		return this->name;
	}

	/*
	* Функция, устанавливающая предпочитаемый объем страниц
	* 
	* @param volume - тип страниц
	*/
	void setPreferVolume(std::string volume) {
		this->prefers_volume = volume;
	}

	/*
	* Функция, получающая предпочитаемый объем страниц
	*
	* @return возвращает тип страниц
	*/
	std::string getPreferVolume() {
		return this->prefers_volume;
	}

	/*
	* Функция, устанавливающая предпочитаемый жанр
	*
	* @param genre - жанр
	*/
	void setPreferGenre(std::string genre) {
		this->prefers_genre = genre;
	}

	/*
	* Функция, получающая предпочитаемый жанр
	*
	* @return возвращает жанр
	*/
	std::string getPreferGenre() {
		return this->prefers_genre;
	}

	/*
	* Функция, проверяющая, закончились ли дни симуляции
	*
	* @return возвращает флаг таймаута
	*/
	int isTimeout() {
		return this->timeout;
	}

	/*
	* Функция, получающая количество прочтенных книг
	*
	* @return возвращает количество прочтенных книг
	*/
	int getComplitedBooks() {
		return this->comlited_books;
	}

	/*
	* Функция, получающая количество дней, потраченных на успешное чтение книг
	*
	* @return возвращает количество дней
	*/
	int getDaysSpentForReading() {
		return this->remaining_days;
	}

	/*
	* Функция, проверяющая, закончил ли посетитель читать книгу за отведенный срок
	*
	* @return возвращает флаг
	*/
	int getFinished() {
		return this->is_finished_reading;
	}

	/*
	* Функция, получающая среднее время прочтения книги
	*
	* @return среднее время
	*/
	float getAverageTimeReading() {
		return float(this->days_of_completed_reading) / float(this->comlited_books);
	}

	/*
	* Функция, добавляющая прочтенную книгу в список прочтенных книг посетителя
	*/
	void readBook(std::string title,
		std::string author,
		std::string genre,
		int volume,
		int censore,
		float mark) {
		read_books[title][author][genre][volume][censore] = mark;
	}

	/*
	* Функция, проверяющая, не читал ли посетитель эту книгу ранее
	*
	* @return возвращает итератор
	*/
	int checkCurrentBook(std::string title) {
		auto iter = read_books.find(title);
		return iter != read_books.end() ? 1 : 0;
	}

	/*
	* Функция, возвращающая список прочтенных книг
	*
	* @return возвращает список прочтенных книг
	*/
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> getBooks() {
		return read_books;
	}

	/*
	* Функция, печатающая список прочтеных книг
	*/
	void printReadBooks() {
		print(read_books);
	}
};

/*
Класс книги
*/
class Book {
private:
	std::string title = "";
	std::string genre = "";
	std::string author = "";
	int volume = 0;
	int censore_age = 0;
	std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>> book_info;

public:
	/*
	* Конструктор класса Книги
	*
	* @param genre - жанр
	* @param volume - объем страниц
	* @param censore_age - возрастное ограничение
	*/
	Book(std::string genre, int volume, int censore_age) :
		genre{ genre },
		volume{ volume },
		censore_age{ censore_age } {}

	/*
	* Функция, устанавливающая автора книги
	*
	* @param author - автор книги
	*/
	void setAuthor(std::string author) {
		this->author = author;
	}

	/*
	* Функция, возвращающая автора книги
	*
	* @return возвращает автора книги
	*/
	std::string getAuthor() {
		return this->author;
	}

	/*
	* Функция, возвращающая информацию о книге
	*
	* @return возвращает книгу
	*/
	std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>> getInfo() {
		book_info[this->author][this->genre][this->volume][this->censore_age] = { 0.0 };
		return book_info;
	}
};

/*
Класс библиотеки
*/
class Library {
private:
	const std::string pages[3] = { "low", "middle", "many" };
	const std::string genres[4] = { "Detective", "Romance novel", "Fantasy", "Fantastic" };
	const int censore_age[2] = { 0, 18 };
	
	int count_of_books = 0;
	int count_persons = 0;
	int working_days = 0;
	int null_size_of_library = 0; // флаг нулевого размера библиотеки

	std::vector<int> days_for_persons; // вектор дней симуляции для каждого юзера
	std::vector<Person> persons; // вектор юзеров
	
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> books;
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> pinned_book;
	std::map<std::string, int> taken_books; // словарь взятых книг
	std::map<std::string, float> dict_average; // словарь средних значений книг
	std::map<std::string, int> books_per_author; // словарь авторов книг

	/*
	* Функция, печатающая словарь книг
	*
	* @param dict - словарь книг
	*/
	void print(std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> dict) {
		for (const auto& title : dict) {
			std::cout << "Title: " << title.first << std::endl;
			for (const auto& author : title.second) {
				std::cout << "Author: " << author.first << std::endl;
				for (const auto& genre : author.second) {
					std::cout << "Genre: " << genre.first << std::endl;
					for (const auto& volume : genre.second) {
						std::cout << "Volume: " << volume.first << std::endl;
						for (const auto& censore : volume.second) {
							std::cout << "Censore age: " << censore.first << "+" << std::endl;
							std::cout << "Rating: " << censore.second;
							std::cout << std::endl;
						}
					}
				}
			}
			std::cout << "\n" << std::endl;
		}
	}

	/*
	* Функция, печатающая список взятых книг
	*/
	void printTakenBooks(std::map<std::string, int> dict) {
		for (const auto& taken_book : dict) {
			std::cout << taken_book.first << " - " << taken_book.second << std::endl;
		}
		std::cout << std::endl;
	}

	/*
	* Функция, печатающая список всех книг
	*/
	void printBooks() {
		print(books);
	}

	/*
	* Функция, печатающая выбранные посетителем книги
	*/
	void printPinnedBooks() {
		print(pinned_book);
	}

	/*
	* Функция, реализующая логику подбора книги посетителю
	*
	* @param age - возраст посетителя
	* @param prefers_volume - предпочитаемый объём книги
	* @param prefers_genre - предпочитаемый жанр
	* @to_pin - указатель на словарь, хранящий выбранную книгу
	*/
	int selectBook(int age, std::string prefers_volume, std::string prefers_genre,
		std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>>* to_pin) {
		int selected_book = 0;
		int not_found = 0;
		int taken_book = 1;
		int taken_book_per_author = 1;

		int books_changed = 0;

		std::string find_title = "";

		age = age >= 18 ? 18 : 0;

		while (selected_book == 0) {
			if (books_changed == 1) {
				books = changeDict(find_title, &books);
				selected_book++;
				break;
			}
			if (not_found == 1) {
				break;
			}

			for (const auto& title : books) {
				if (books_changed == 1)
					break;
				for (const auto& author : title.second) {
					for (const auto& genre : author.second) {
						if (genre.first == prefers_genre) {
							for (const auto& volume : genre.second) {
								if (prefers_volume == "low") {
									if (volume.first < 100) {
										for (const auto& censore : volume.second) {
											if (selected_book == 0) {
												find_title = title.first;

												(*to_pin)[find_title][author.first][genre.first][volume.first][censore.first] = censore.second;
												setNumberTakenBooks(find_title, taken_book, &taken_books);
												setNumberTakenBooks(author.first, taken_book_per_author, &books_per_author);

												books_changed = 1;
												break;
											}
										}
									}
								}
								else if (prefers_volume == "middle") {
									if (volume.first >= 100 && volume.first < 500) {
										for (const auto& censore : volume.second) {
											if (selected_book == 0) {
												find_title = title.first;

												(*to_pin)[find_title][author.first][genre.first][volume.first][censore.first] = censore.second;
												setNumberTakenBooks(find_title, taken_book, &taken_books);
												setNumberTakenBooks(author.first, taken_book_per_author, &books_per_author);

												books_changed = 1;
												break;
											}
										}
									}
								}
								else if (prefers_volume == "many") {
									if (volume.first > 500) {
										for (const auto& censore : volume.second) {
											if (selected_book == 0) {
												find_title = title.first;

												(*to_pin)[find_title][author.first][genre.first][volume.first][censore.first] = censore.second;
												setNumberTakenBooks(find_title, taken_book, &taken_books);
												setNumberTakenBooks(author.first, taken_book_per_author, &books_per_author);

												books_changed = 1;
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			if (books_changed != 1)
				not_found++;
		}

		return selected_book ? 1 : 0;
	}

	/*
	* Функция, реализующая логику выбора книги для конкретного посетителя
	*
	* @param selected_book - указатель на флаг выбранной книги
	* @param person - посетитель, объект класса Person
	*/
	void personSelectBook(int* selected_book, Person person) {
		while (!(*selected_book)) {
			if (this->null_size_of_library)
				break;
			if ((*selected_book) == 1)
				break;

			(*selected_book) = selectBook(person.getAge(), person.getPreferVolume(), person.getPreferGenre(), &pinned_book);

			while (!(*selected_book)) {
				if (this->null_size_of_library)
					break;

				std::string new_volume = this->pages[randomInt(0, 2)];
				std::string new_genre = this->genres[randomInt(0, 3)];
				if (books.size() != 0) {
					(*selected_book) = selectBook(person.getAge(), new_volume, new_genre, &pinned_book);
					person.setPreferVolume(new_volume);
					person.setPreferGenre(new_genre);
				}
				else
					null_size_of_library = 1;
			}
		}
	}

	/*
	* Функция, удаляющая книгу из словаря
	*
	* @param title - заголовок книги
	* @param dict - словарь книг
	* 
	* @return возвращает измененный словарь
	*/
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> changeDict(std::string title,
		std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>>* dict) {
		auto search_for_delete = (*dict).find(title);
		(*dict).erase(search_for_delete);


		return (*dict);
	}

	/*
	* Функция, добавляющая книгу в словарь
	*
	* @param title - заголовок книги
	* @param author - автор книги
	* @param genre - жанр книги
	* @param volume - объём страниц книги
	* @param censore - возрастное ограничение книги
	* @param mark - оценка книги
	*/
	void reloadBooks(std::string title,
		std::string author, 
		std::string genre,
		int volume,
		int censore,
		float mark) {
		books[title][author][genre][volume][censore] = mark;
	}

	/*
	* Функция, считающая число взятых книг
	*
	* @param title - заголовок книги
	* @param taken_book - флаг взятой книги
	* @param dict - указатель на словарь взятых книг
	*/
	void setNumberTakenBooks(std::string title, int taken_book, std::map<std::string, int>* dict) {
		(*dict)[title] += taken_book;
	}

	/*
	* Функция, устанавливающая среднюю оценку книги в словарь средних оценок
	*/
	void getAverageRating(std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, std::map<int, float>>>>> person_dict) {
		float average_rating = 0.0;

		for (auto& title : person_dict) {
			for (auto& author : title.second) {
				for (auto& genre : author.second) {
					for (auto& volume : genre.second) {
						for (auto& censore : volume.second) {
							this->dict_average[title.first] += censore.second;
						}
					}
				}
			}
		}
	}

	/*
	* Функция, считающая среднюю арифметическую оценку книг
	*/
	void calculateAverage() {
		std::cout << "\x1b[32mAverage of the book\033[0m:" << std::endl;
		for (auto& title : this->dict_average) {
			if (this->taken_books[title.first] != 0)
				this->dict_average[title.first] /= this->taken_books[title.first];
			std::cout << title.first << ": " << this->dict_average[title.first] << std::endl;
		}
	}

	/*
	* Функция, печатающая статистику
	*/
	void statistics() {
		std::cout << "\x1b[32mStatistics\033[0m:" << std::endl;
		for (int i = 0; i < this->count_persons; i++) {
			std::cout << "\033[35m" + persons[i].getName() + "\033[0m" << std::endl;
			std::cout << "Count of books read: " << persons[i].getComplitedBooks() << std::endl;
			std::cout << "Average time of reading: " << persons[i].getAverageTimeReading() << " day(-s)" << std::endl;
			std::cout << "Books:\n" << std::endl;
			persons[i].printReadBooks();

			getAverageRating(persons[i].getBooks());
		}

		calculateAverage();
		std::cout << "\n\x1b[32mTaken books\033[0m:" << std::endl;
		printTakenBooks(taken_books);
		std::cout << "\x1b[32mTaken books per author\033[0m:" << std::endl;
		printTakenBooks(books_per_author);
	}

public:
	/*
	* Конструктор класса Библиотеки
	*
	* @param count_persons - количество посетителей в 1 день симуляции
	* @param count_of_books - количество книг в библиотеке
	* @param working_days - количество дней симуляции
	*/
	Library(int count_persons, int count_of_books, int working_days) :
		count_of_books{ count_of_books },
		count_persons{ count_persons },
		working_days{ working_days } {
		std::vector<int> temp_vector(count_persons, working_days);
		
		for (const auto& value : temp_vector)
			days_for_persons.push_back(value);
	}

	/*
	* Функция, генерирующая случайным образом книги
	*/
	void generateBooks() {
		for (int i = 0; i < this->count_of_books; i++) {
			std::string title = "Title" + std::to_string(i + 1);
			std::string genre = genres[randomInt(0, 3)];
			int volume = randomInt(20, 800);
			int temp_censore_age = this->censore_age[0];

			if (genre == genres[0] || genre == genres[1])
				temp_censore_age = this->censore_age[1];

			Book book(genre, volume, temp_censore_age);

			if (i >= 0 && i < 10)
				book.setAuthor("Author" + std::to_string(i + 1));
			else if (i >= 10 && i < 20)
				book.setAuthor("Author" + std::to_string(i + 1));
			else
				book.setAuthor("Author" + std::to_string(i + 1));

			books[title] = book.getInfo();
			taken_books[title] = 0;
			dict_average[title] = 0.0;
			books_per_author[book.getAuthor()] = 0;
		}
	}

	/*
	* Функция, реализующая логику работы библиотеки
	*/
	void work() {
		int selected_book = 0;

		for (int i = 0; i < this->count_persons; i++) {
			Person person("person" + std::to_string(i + 1), randomInt(8, 60), this->pages[randomInt(0, 2)], this->genres[randomInt(0, 3)]);
			this->persons.push_back(person);
		}

		int index = 0;
		std::string delete_title = "";
		while (true) { // будем брать новые книги
			if (index == this->count_persons)
				break;

			personSelectBook(&selected_book, persons[index]);

			if (this->null_size_of_library) {
				std::cout << "Library is empty!\n" << std::endl;
				break;
			}

			selected_book = 0;

			int success = 0; // челик дочитал книгу
			int not_read = 0;
			while (index != this->count_persons) { // пока все челы не дочитают текущую книгу до конца
				if (not_read) {
					//this->pinned_book = changeDict(delete_title, &this->pinned_book);
					break;
				}

				if (success > 0) {
					this->pinned_book = changeDict(delete_title, &this->pinned_book);
					break;
				}

				for (auto& title : this->pinned_book) {
					if (not_read) {
						break;
					}
					for (auto& author : title.second) {
						for (auto& genre : author.second) {
							for (auto& volume : genre.second) {
								if (!success) {
									// проверка на то, читал ли уже эту книгу
									if (!this->persons[index].checkCurrentBook(title.first)) {
										this->persons[index].read(volume.first, this->days_for_persons.at(index));
										this->days_for_persons.at(index) -= this->persons[index].getDaysSpentForReading();

										delete_title = title.first;

										float mark = randomFloat(2, 5);
										for (auto& censore : volume.second) {
											censore.second = mark;
											this->persons[index].readBook(title.first, author.first, genre.first, volume.first, censore.first, censore.second);
										}

										success++;
										break;
									}
									else {
										not_read = 1;
										break;
									}
								}
								break;
							}
							break;
						}
						break;
					}
				}
			}
			if (days_for_persons.at(index) == 0) {
				for (auto& title : persons[index].getBooks()) {
					for (auto& author : title.second) {
						for (auto& genre : author.second) {
							for (auto& volume : genre.second) {
								for (auto& censore : volume.second) {
									books[title.first][author.first][genre.first][volume.first][censore.first] = censore.second;
								}
							}
						}
					}
				}
				index++;
			}
		}

		statistics();
	}

	/*
	* Функция, печатающая размер библиотеки
	*/
	void getSize() {
		std::cout << "There are " << books.size() << " books at the library" << std::endl;
	}
};
