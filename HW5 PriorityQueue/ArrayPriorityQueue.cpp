// Array PQueue Implementation
// by Cristobal Sciutto
// Section: Shirin Salehi

#include "ArrayPriorityQueue.h"

int const INITIAL_SIZE = 2;

/* ArrayPriorityQueue() contructor:
 * Creates initial dynamic array to store PQueue and
 * sets count and capacity member variables to their
 * intial values */

ArrayPriorityQueue::ArrayPriorityQueue() {
    this->arr = new PQEntry[INITIAL_SIZE];
	this->count = 0;
	this->capacity = INITIAL_SIZE;
}

/* ~ArrayPriorityQueue() deconstructor:
 * Deletes dynamic array */

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete [] arr;
}

/* changePriority():
 * Loops through array until the value is found.
 * If the newPriority is smaller than the current,
 * the priority is updated. Else, an error is thrown.
 * Error is also thrown if the value is not found. */

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
	for (int i = 0; i < count; i++) {
        if (arr[i].value == value) {
            if (arr[i].priority <= newPriority) {
                throw string("You must change to a lower priority!");
            } else {
                arr[i].priority = newPriority;
                return;
            }
        }
	}
    throw string("Value not in queue!");
}

/* clear():
 * Deletes current dynamic array and reinitializes it. */

void ArrayPriorityQueue::clear() {
    delete [] arr;
    arr = new PQEntry[INITIAL_SIZE];
    capacity = INITIAL_SIZE;
	count = 0;
}

/* dequeue():
 * If the array is not empty, loop through the array
 * looking for element of highest priority. Remove that
 * element from the array, substituting it with the last
 * element and returns its value.
 * The count is decremented. */

string ArrayPriorityQueue::dequeue() {
    if (!isEmpty()) {
        int index = 0;
        for (int i = 0; i < count; i++) {
            if (arr[i] < arr[index]) index = i;
        }
        // Save the element of highest priority
        string str = arr[index].value;
        // Substitute that element with the last element of array
        arr[index] = arr[count - 1];
        count --;
        return str;
    } else throw string("Queue is empty!");
}

/* enqueue():
 * Inserts a new element at the end of the dynamic array.
 * In case the array is full, it is dynamically resized
 * using the resize() function.
 * The count is incremented. */

void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry curr(value, priority);
	if (count == capacity) resize();
    arr[count] = curr;
	count++;
}

/* isEmpty():
 * Checks if the count of elements is zero */

bool ArrayPriorityQueue::isEmpty() const {
    return (count == 0);
}

/* peek():
 * If the array is not empty, loop through the array
 * looking for element of highest priority. Return
 * its value. */

string ArrayPriorityQueue::peek() const {
    if (!isEmpty()) {
        int index = 0;
        for (int i = 0; i < count; i++) {
            if (arr[i] < arr[index]) index = i;
        }
        return arr[index].value;
    } else throw string("Queue is empty!");
}

/* peekPriority():
 * If the array is not empty, loop through the array
 * looking for element of highest priority. Return
 * its priority. */

int ArrayPriorityQueue::peekPriority() const {
    if (!isEmpty()) {
        int index = 0;
        for (int i = 0; i < count; i++) {
            if (arr[i] < arr[index]) index = i;
        }
        return arr[index].priority;
    } else throw string("Queue is empty!");
}

/* size():
 * Getter function for count member variable */

int ArrayPriorityQueue::size() const {
	return count;
}

/* operator <<:
 * Loops through array printing out according to the requested format.
 * Only adds a comma if not last element. */

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
	out << "{";
    for (int i = 0; i < queue.count; i++) {
        out << queue.arr[i];
        if (queue.count - i > 1) out << ", ";
	}
	out << "}";
    return out;
}

// Private Function Implementations

/* resize():
 * Dynamically updates the size of the array to double its
 * original. To do this, it first create a new array. Transfers
 * all values, and then deletes the old one. Updates capacity.
 * Used logic from Lecture 17. */

void ArrayPriorityQueue::resize() {
    PQEntry *newarr = new PQEntry[capacity * 2];
	for (int i = 0; i < capacity; i++) {
        newarr[i] = arr[i];
	}
    delete [] arr;
    arr = newarr;
	capacity *= 2;
}
