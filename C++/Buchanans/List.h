#pragma once

template <typename E>
class List {
public:
    List(const List<E>&) = delete;
    void operator = (const List<E>&) = delete;
    List() {}
    virtual void insert(E element) = 0;
    virtual void append(E element) = 0;
    virtual void setElement(E element) = 0;
    virtual E remove() = 0;
    virtual void clear() = 0;
    virtual E getElement() = 0;
    virtual void goToStart() = 0;
    virtual void goToEnd() = 0;
    virtual void goToPos(int pos) = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual bool atEnd() = 0;
    virtual bool atStart() = 0;
    virtual bool isEmpty() = 0;
    virtual int getPos() = 0;
    virtual int getSize() = 0;
    virtual void print() = 0;
};