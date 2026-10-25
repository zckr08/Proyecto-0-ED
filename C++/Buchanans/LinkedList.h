#pragma once
#include <iostream>
#include <stdexcept>
#include"List.h"
#include"Node.h"

using std::runtime_error;
using std::cout;
using std::endl;

template<typename E>
class LinkedList : public List<E> {
private:
	Node<E>* head;
	Node<E>* current;
	Node<E>* tail;
	int size;

public:
	LinkedList() {
		tail = current = head = new Node<E>();
		size = 0;
	}
	~LinkedList() {
		clear();
		delete head;
	}
	void insert(E element) {
		current->next = new Node<E>(element, current->next);

		if (current == tail)
			tail = current->next;

		size++;
	}
	void append(E element) {
		tail = tail->next = new Node<E>(element);
		size++;
	}
	void setElement(E element) {
		if (size == 0)
			throw runtime_error("List is empty.");

		if (current == tail)
			throw runtime_error("No current Element.");

		current->next->element = element;
	}
	E remove() {
		if (size == 0)
			throw runtime_error("List is empty.");
		if (current == tail)
			throw runtime_error("No current Element.");
		E result = current->next->element;
		Node<E>* temp = current->next->next;
		delete current->next;
		current->next = temp;
		if (temp == nullptr)
			tail = current;
		size--;
		return result;
	}
	void clear() {
		while (size > 0) {
			Node<E>* temp = head->next->next;
			delete head->next;
			head->next = temp;
			size--;
		}
		tail = current = head;
		size = 0;
	}
	E getElement() {
		if (size == 0)
			throw runtime_error("Lista vacia");

		if (current == tail)
			throw runtime_error("No current element");

		return current->next->element;
	}
	void goToStart() {
		current = head;
	}
	void goToEnd() {
		current = tail;
	}
	void goToPos(int pos) {
		if (pos < 0 || pos> size)
			throw runtime_error("Index error");
		current = head;
		for (int i = 0; i < pos; i++)
			current = current->next;
	}
	void next() {
		if (current == tail)
			throw runtime_error("Current in tail)");
		current = current->next;
	}
	void previous() {
		if (current == head)
			return;

		Node<E>* temp = head;

		while (temp->next != current)
			temp = temp->next;

		current = temp;
	}
	bool atEnd() {
		return current == tail;
	}
	bool atStart() {
		return current == head;
	}
	bool isEmpty() {
		return size == 0;
	}
	int getPos() {
		int pos = 0;
		Node<E>* temp = head;
		while (temp != current) {
			temp = temp->next;
			pos++;
		}
		return pos;
	}
	int getSize() {
		return size;
	}
	void print() {
		Node<E>* temp = head->next;
		cout << "[";
		while (temp != nullptr) {
			cout << temp->element;
			if (temp != tail)
				cout << ", ";
			temp = temp->next;
		}
		cout << "]" << endl;


	}
};

