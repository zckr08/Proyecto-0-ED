#pragma once
#define DEFAULT_MAX 1024
#include <iostream>
#include <stdexcept>
#include <string>
#include "List.h"

using std::cout;
using std::cin;
using std::endl;
using std::runtime_error;
using std::string;

template <typename E>
class ArrayList : public List<E> {
private:
	E* elements;
	int max;
	int size;
	int pos;

public:
	ArrayList(int max = DEFAULT_MAX) {
		if (max < 1)
			throw runtime_error("INvalid max size.");
		elements = new E[max];
		this->max = max;
		size = pos = 0;
	}
	~ArrayList() {
		delete[] elements;
	}
	void insert(E element) {
		if (size == max)
			throw runtime_error("List is full.");
		for (int i = size - 1; i >= pos; i--)
			elements[i + 1] = elements[i];
		elements[pos] = element;
		size++;
	}
	void append(E element) {
		if (size == max)
			throw runtime_error("List is full.");
		elements[size] = element;
		size++;
	}
	void setElement(E element) {
		if (size == 0)
			throw runtime_error("Empty list");
		if (pos == size)
			throw runtime_error("No current element.");
		elements[pos] = element;
	}
	E remove() {
		if (size == 0)
			throw runtime_error("Empty list");
		if (pos == size)
			throw runtime_error("No current element.");
		E result = elements[pos];
		for (int i = pos; i < size - 1; i++) {
			elements[i] = elements[i + 1];
		}
		size--;
		return result;
	}
	void clear() {
		pos = size = 0;
	}
	E getElement() {
		if (size == 0)
			throw runtime_error("List is empty");
		if (pos == size)
			throw runtime_error("No current element");
		return elements[pos];
	}
	void goToStart() {
		pos = 0;
	}
	void goToEnd() {
		pos = size;
	}
	void goToPos(int pos) {
		if (pos > size || pos < 0)
			throw runtime_error("Index out of bounds");
		this->pos = pos;
	}
	void next() {
		if (pos < size)
			pos++;
	}
	void previous() {
		if (pos > 0)
			pos--;
	}
	bool atEnd() {
		return pos == size;
	}
	bool atStart() {
		return pos == 0;
	}
	int getPos() {
		return pos;
	}
	int getSize() {
		return size;
	}
	void print() {
		cout << "[";
		for (int i = 0; i < size; i++) {
			cout << elements[i];
			if (i < size - 1)
				cout << ", ";
		}
		cout << "]" << endl;
	}
};

