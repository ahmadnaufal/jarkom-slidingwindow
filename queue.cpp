/* 
 * File 		: queue.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Queue implementation
 */ 

#include "queue.h"

using namespace std;

Queue::Queue() { //Ctor
	count = 0;
	front = 0;
	rear = 0;
	maxSize = MaxSizeQueue;
	data = new Frame[maxSize];
}

Queue::Queue(int size) { //Ctor with param
	count = 0;
	front = 0;
	rear = 0;
	maxSize = size;
	data = new Frame[maxSize];
}

Queue::Queue(const Queue &q) { //Cctor
	count = q.count;
	front = q.front;
	rear = q.rear;
	maxSize = q.maxSize;
	data = new Frame[maxSize];
	for (int i=0; i<maxSize; ++i) {
		data[i] = q.data[i];
	}
}

Queue& Queue::operator=(const Queue& q) { //Operator assignment
	count = q.count;
	front = q.front;
	rear = q.rear;
	maxSize = q.maxSize;
	delete[] data;
	data = new Frame[maxSize];
	for (int i=0; i<maxSize; ++i) {
		data[i] = q.data[i];
	}
	return *this;
}

Queue::~Queue() { //Dtor
	delete[] data;
}

bool Queue::isEmpty() { //Return true if the queue is empty
	return (count==0);
}

bool Queue::isFull() { //Return true if the queue is full
	return (count==maxSize);
}

unsigned int Queue::getHead() {
	return front;
}

unsigned int Queue::getTail() {
	return rear;
}

unsigned int Queue::getCount() {
	return count;
}

Frame Queue::getElement(int x) {
	return data[x];
}

void Queue::add(Frame f) { //Add element to the end of queue
	data[rear] = f;
	rear++;
	rear %= maxSize;
	count++;
}

Frame Queue::del() { //Delete element from the front of the queue

	Frame f = data[front];
	front++;
	front %= maxSize;
	count--;
	return f;
}