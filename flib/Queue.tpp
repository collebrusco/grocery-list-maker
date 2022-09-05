#pragma once
#include <string>
#include <assert.h>
#include <iostream>


template <typename T>
class Queue {
private:
	int maxSize;
	int count;
	int front;
	int tail;
	T* q;
public:
	Queue(int);
	~Queue();

	bool isEmpty();
	bool isFull();

	void enqueue(T);
	T dequeue();

	int size();

	void display();
};

template <typename T>
Queue<T>::Queue(int ms){
	maxSize = ms;
	q = new T[maxSize];
	front = 0;
	tail = -1;
	count = 0;
}

template <typename T>
Queue<T>::~Queue(){
	delete [] q;
}

template <typename T>
bool Queue<T>::isEmpty(){
	return (count == 0);
}

template <typename T>
bool Queue<T>::isFull(){
	return (count == maxSize);
}

template <typename T>
void Queue<T>::enqueue(T itm){
	assert(!isFull());
	tail = (tail + 1) % maxSize;
	q[tail] = itm;
	count++;
}

template <typename T>
T Queue<T>::dequeue(){
	assert(!isEmpty());
	count--;
	T result = q[front];
	front = (front + 1) % maxSize;
	return result;
}

template <typename T>
void Queue<T>::display(){
	int idx = front;
	for (int i = 0; i < count; i++){
		std::cout << q[idx] << " ";
		idx = (idx + 1) % maxSize;
	}
	std::cout << "\n";
}

template <typename T>
int Queue<T>::size(){
	return count;
}