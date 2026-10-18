#pragma once

template <typename E>
class Stack {
public:
	Stack() {}
	Stack(const Stack<E>&) = delete;
	void operator = (const Stack<E>&) = delete;
	virtual ~Stack() {}
	virtual void push(E element) = 0;
	virtual E pop() = 0;
	virtual E topValue() = 0;
	virtual void clear() = 0;
	virtual bool isEmpty() = 0;
	virtual int getSize() = 0;
	virtual void print() = 0;
};

