#pragma once

#include <stdexcept>
#include <iostream>
#include "List.h"
#include "DNode.h"

using std::cout;
using std::endl;
using std::runtime_error;

template <typename E>
class DLinkedList : public List<E> {
private:
    DNode<E>* head;
    DNode<E>* tail;
    DNode<E>* current;
    int size;

public:
    DLinkedList() {
        current = head = new DNode<E>(nullptr, nullptr);
        head->next = tail = new DNode<E>(nullptr, head);
        size = 0;
    }

    ~DLinkedList() {
        clear();
        delete head;
        delete tail;
    }

    void insert(E element) {
        current->next = new DNode<E>(element, current->next, current);
        current->next->next->previous = current->next;
        size++;
    }

    void append(E element) {
        DNode<E>* newNode = new DNode<E>(element, tail, tail->previous);
        tail->previous->next = newNode;
        tail->previous = newNode;
        size++;
    }

    void setElement(E element) {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        current->next->element = element;
    }

    E remove() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        E result = current->next->element;
        current->next = current->next->next;
        delete current->next->previous;
        current->next->previous = current;
        size--;
        return result;
    }

    void clear() {
        current = head;
        while (current->next != tail) {
            current->next = current->next->next;
            delete current->next->previous;
        }
        tail->previous = head;
        size = 0;
    }

    E getElement() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        return current->next->element;
    }

    void goToStart() {
        current = head;
    }

    void goToEnd() {
        current = tail->previous;
    }

    void goToPos(int pos) {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of Bounds.");
        current = head;
        for (int i = 0; i < pos; i++)
            current = current->next;
    }

    void next() {
        if (current != tail->previous)
            current = current->next;
    }

    void previous() {
        if (current != head) {
            current = current->previous;
        }
    }

    bool atEnd() {
        return current == tail->previous;
    }

    bool atStart() {
        return current == head;
    }

    int getPos() {
        DNode<E>* temp = head;
        int pos = 0;
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
        cout << "[";
        DNode<E>* temp = head->next;
        while (temp != tail) {
            cout << temp->element;
            if (temp != tail->previous)
                cout << ", ";
            temp = temp->next;
        }
        cout << "]" << endl;
    }
};