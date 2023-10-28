#pragma once
#include <iostream>

/*
* Класс Узла
*/
template <typename T>
class Node {
public:
	T data;
	Node<T>* next;
	Node<T>* previous;

	/*
	* Конструктор класса Узла
	* 
	* @param data - значение, хранящееся в узле
	*/
	Node(T data) {
		this->data = data;
		this->next = nullptr;
		this->previous = nullptr;
	}
};

/*
* Класс Двусвязного списока
*/
template <typename T>	
class DoubleList {
private:
	Node<T>* tail;
	Node<T>* head;
	size_t size = 0;

	friend class Iterator;

public:
	/*
	* Класс Итератора
	*/
	class Iterator {
	private:
		Node<T>* current_node;
	public:
		/*
		* Конструктор класса Итератора
		*
		* @param node - узел
		*/
		Iterator(Node<T>* node) : current_node{ node } {}

		/*
		* Функция, перегружающая оператор ++
		*
		* @return возвращает ссылку на итератор, находящийся на конкретном узле
		*/
		Iterator& operator++() {
			if (current_node) {
				current_node = current_node->next;
			}
			return *this;
		}

		/*
		* Функция, перегружающая оператор *
		*
		* @return возвращает ссылку на значение, хранящееся в узле
		*/
		T& operator*() {
			return current_node->data;
		}

		/*
		* Функция, перегружающая оператор !=
		*
		* @return возвращает булевое значение, проверяющее, равен ли узел
		* основного итератора узлу скопированного итератора
		*/
		bool operator!=(const Iterator& other) {
			return current_node != other.current_node;
		}
	};

	/*
	* Конструктор класса Двусвязного списка
	*/
	DoubleList() {
		this->head = this->tail = NULL;
	}

	/*
	* Деструктор класса Двусвязного списка
	*/
	~DoubleList() {
		while (head != NULL)
			popFront();
	}

	/*
	* Функция, добавляющая новый узел в начало
	* 
	* @param data - значение, которое будет храниться в узле
	*/
	void pushFront(T data) {
		Node<T>* current_node = new Node<T>(data);
		
		current_node->next = head;
		if (head != NULL)
			head->previous = current_node;
		if (tail == NULL)
			tail = current_node;

		head = current_node;
		
		size++;
	}

	/*
	* Функция, добавляющая новый узел в конец
	*
	* @param data - значение, которое будет храниться в узле
	*/
	void pushBack(T data) {
		Node<T>* current_node = new Node<T>(data);

		current_node->previous = tail;
		if (tail != NULL)
			tail->next = current_node;
		if (head == NULL)
			head = current_node;

		tail = current_node;
		
		size++;
	}

	/*
	* Функция, получающая первый узел
	*
	* @return значение, которое будет храниться в узле
	*/
	T getFront() {
		if (head == NULL)
			return NULL;

		return (*head).data;
	}

	/*
	* Функция, получающая последний узел
	*
	* @return значение, которое будет храниться в узле
	*/
	T getBack() {
		if (tail == NULL)
			return NULL;

		return (*tail).data;
	}

	/*
	* Функция, удаляющая первый узел
	*/
	void popFront() {
		if (head == NULL)
			return;

		Node<T>* current_node = head->next;
		
		if (current_node != NULL)
			current_node->previous = NULL;
		else
			tail = NULL;

		delete head;
		head = current_node;
		size--;
	}

	/*
	* Функция, удаляющая последний узел
	*/
	void popBack() {
		if (tail == NULL)
			return;

		Node<T>* current_node = tail->previous;
		if (current_node != NULL)
			current_node->next = NULL;
		else
			head = NULL;

		delete tail;
		tail = current_node;

		size--;
	}

	/*
	* Функция, проверяющая, пустой ли список
	* 
	* @return возвращает булевое значение
	*/
	bool isEmpty() {
		return size > 0 ? false : true;
	}

	/*
	* Функция, возвращающая размер списка
	* 
	* @return возвращает размер списка
	*/
	size_t getSize() {
		return this->size;
	}

	/*
	* Функция, возвращающая итератор первого узла
	* 
	* @return возвращает итератор первого узла
	*/
	Iterator begin() {
		return Iterator(head);
	}

	/*
	* Функция, возвращающая итератор последнего узла
	*
	* @return возвращает итератор последнего узла
	*/
	Iterator end() {
		return Iterator(nullptr);
	}
};
