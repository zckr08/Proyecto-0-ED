#pragma once
#include <iostream>
#include <stdexcept>
#include "Node.h"
#include "Stack.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class LinkedStack : public Stack<E> {
private:
    Node<E>* top;
    int size;

public:
    LinkedStack() {
        top = nullptr;
        size = 0;
    }

    ~LinkedStack() {
        clear();
    }

    void push(E element) {
        top = new Node<E>(element, top);
        size++;
    }

    E pop() {
        if (size == 0)
            throw runtime_error("Stack is empty");
        E result = top->element;
        Node<E>* temp = top->next;
        delete top;
        top = temp;
        size--;
        return result;
    }

    E topValue() {
        if (size == 0)
            throw runtime_error("Stack is empty.");
        return top->element;
    }

    void clear() {
        Node<E>* temp;
        while (top != nullptr) {
            temp = top->next;
            delete top;
            top = temp;
        }
        size = 0;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    void print() {
        cout << "[";
        for (Node<E>* temp = top; temp != nullptr; temp = temp->next) {
            cout << temp->element;
            if (temp->next != nullptr)
                cout << ", ";
        }
        cout << "]" << endl;
    }
};
