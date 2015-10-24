/* 
 * File 		: queue.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header of queue
 */ 

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "frame.h"
#define MaxSizeQueue 5

using namespace std;

class Queue {
public:
	Queue(); //Ctor
	Queue(int size); //Ctor with param
	Queue(const Queue& q); //Cctor
	Queue& operator=(const Queue& q); //Operator assignment
	~Queue(); //Dtor

	bool isEmpty(); //Return true if the queue is empty
	bool isFull(); //Return true if the queue is full

	unsigned int getHead();
	unsigned int getTail();
	unsigned int getCount();

	void add(Frame f); //Add element to the end of queue
	Frame del(); //Delete element from the front of the queue

private:
	unsigned int count;
 	unsigned int front;
 	unsigned int rear;
 	unsigned int maxSize;
 	Frame *data;
};

#endif