#pragma once
#include <iostream>

/*
* ����� ����
*/
template <typename T>
class Node {
public:
	T data;
	Node<T>* next;
	Node<T>* previous;

	/*
	* ����������� ������ ����
	* 
	* @param data - ��������, ���������� � ����
	*/
	Node(T data) {
		this->data = data;
		this->next = nullptr;
		this->previous = nullptr;
	}
};

/*
* ����� ����������� �������
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
	* ����� ���������
	*/
	class Iterator {
	private:
		Node<T>* current_node;
	public:
		/*
		* ����������� ������ ���������
		*
		* @param node - ����
		*/
		Iterator(Node<T>* node) : current_node{ node } {}

		/*
		* �������, ������������� �������� ++
		*
		* @return ���������� ������ �� ��������, ����������� �� ���������� ����
		*/
		Iterator& operator++() {
			if (current_node) {
				current_node = current_node->next;
			}
			return *this;
		}

		/*
		* �������, ������������� �������� *
		*
		* @return ���������� ������ �� ��������, ���������� � ����
		*/
		T& operator*() {
			return current_node->data;
		}

		/*
		* �������, ������������� �������� !=
		*
		* @return ���������� ������� ��������, �����������, ����� �� ����
		* ��������� ��������� ���� �������������� ���������
		*/
		bool operator!=(const Iterator& other) {
			return current_node != other.current_node;
		}
	};

	/*
	* ����������� ������ ����������� ������
	*/
	DoubleList() {
		this->head = this->tail = NULL;
	}

	/*
	* ���������� ������ ����������� ������
	*/
	~DoubleList() {
		while (head != NULL)
			popFront();
	}

	/*
	* �������, ����������� ����� ���� � ������
	* 
	* @param data - ��������, ������� ����� ��������� � ����
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
	* �������, ����������� ����� ���� � �����
	*
	* @param data - ��������, ������� ����� ��������� � ����
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
	* �������, ���������� ������ ����
	*
	* @return ��������, ������� ����� ��������� � ����
	*/
	T getFront() {
		if (head == NULL)
			return NULL;

		return (*head).data;
	}

	/*
	* �������, ���������� ��������� ����
	*
	* @return ��������, ������� ����� ��������� � ����
	*/
	T getBack() {
		if (tail == NULL)
			return NULL;

		return (*tail).data;
	}

	/*
	* �������, ��������� ������ ����
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
	* �������, ��������� ��������� ����
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
	* �������, �����������, ������ �� ������
	* 
	* @return ���������� ������� ��������
	*/
	bool isEmpty() {
		return size > 0 ? false : true;
	}

	/*
	* �������, ������������ ������ ������
	* 
	* @return ���������� ������ ������
	*/
	size_t getSize() {
		return this->size;
	}

	/*
	* �������, ������������ �������� ������� ����
	* 
	* @return ���������� �������� ������� ����
	*/
	Iterator begin() {
		return Iterator(head);
	}

	/*
	* �������, ������������ �������� ���������� ����
	*
	* @return ���������� �������� ���������� ����
	*/
	Iterator end() {
		return Iterator(nullptr);
	}
};
