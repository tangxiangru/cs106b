// Array PQueue Header File
// by Cristobal Sciutto
// Section: Shirin Salehi

#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

class ArrayPriorityQueue {
public:
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue(); // dequeue() = O(n)
    void enqueue(string value, int priority); // enqueue() = O(1)
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);

private:
    // Pointer to dynamic array
    PQEntry *arr;
    // Number of elements in array
	int count;
    // Max number of elements in current array
	int capacity;

	// Private Functions
    // Dynamically increases size of array
	void resize();
};

#endif
