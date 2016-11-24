// Heap PQueue Header File
// by Cristobal Sciutto
// Section: Shirin Salehi

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue(); // dequeue() = O(log(n))
    void enqueue(string value, int priority); // enqueue() = O(log(n))
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    // Pointer to heap's dynamic array
    PQEntry *arr;
    // Number of elements in heap
    int count;
    // Max number of elements in current array
    int capacity;

    // Private Functions
    // Dynamically increases size of array
    void resize();
    // Swaps elements in an array
    void swap(int i, int j);
    // Bubble Up algorithm
    void bubbleUp(int i);
    // Bubble Down algorithm
    void bubbleDown(int i);
};

#endif
