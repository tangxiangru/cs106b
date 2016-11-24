// Heap PQueue Implementation
// by Cristobal Sciutto
// Section: Shirin Salehi

#include "HeapPriorityQueue.h"

int const INITIAL_SIZE = 3;

/* HeapPriorityQueue() contructor:
 * Creates initial dynamic array to store heap and
 * sets count and capacity member variables to their
 * intial values */

HeapPriorityQueue::HeapPriorityQueue() {
    this->arr = new PQEntry[INITIAL_SIZE];
    this->count = 0;
    this->capacity = INITIAL_SIZE;
}

/* ~HeapPriorityQueue() deconstructor:
 * Deletes dynamic array */

HeapPriorityQueue::~HeapPriorityQueue() {
    delete [] arr;
}

/* changePriority():
 * Loops through array until the value is found.
 * If the newPriority is smaller than the current,
 * the priority is updated. Else, an error is thrown.
 * Error is also thrown if the value is not found. */

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 1; i <= count; i++) {
        if (arr[i].value == value) {
            if (arr[i].priority <= newPriority) {
                throw string("You must change to a lower priority!");
            } else {
                arr[i].priority = newPriority;
                bubbleUp(i);
                return;
            }
        }
    }
    throw string("Value not in queue!");
}

/* clear():
 * Deletes current dynamic array and reinitializes it. */

void HeapPriorityQueue::clear() {
    delete [] arr;
    arr = new PQEntry[INITIAL_SIZE];
    capacity = INITIAL_SIZE;
    count = 0;
}

/* dequeue():
 * If the heap is not empty, return the element of
 * highest priority. To maintain the correct ordering,
 * the last element is swapped into the first position
 * and is "bubbled down" using the bubbleDown function.
 * The count is decremented. */

string HeapPriorityQueue::dequeue() {
    if (!isEmpty()) {
        string str = arr[1].value;
        arr[1] = arr[count];
        count--;
        bubbleDown(1);
        return str;
    } else throw string("Queue is empty!");
}

/* enqueue():
 * Inserts a new element at the end of the dynamic array
 * which is "bubbled up" using the bubbleUp function to
 * maintain correct ordering of the heap. In case the array
 * is full, it is dynamically resized using the resize()
 * function. The count is incremented. */

void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry curr(value, priority);
    // The first index is always empty
    if (count == capacity - 1) resize();
    arr[count + 1] = curr;
    bubbleUp(count + 1);
    count ++;
}

/* isEmpty():
 * Checks if the count of elements is zero */

bool HeapPriorityQueue::isEmpty() const {
    return (count == 0);
}

/* peek():
 * If not empty, returns value of highest priority.
 * Else, throws error. */

string HeapPriorityQueue::peek() const {
    if (!isEmpty()) {
        return arr[1].value;
    } else throw string("Queue is empty!");
}

/* peekPriority():
 * If not empty, returns priority of PQEntry of highest priority.
 * Else, throws error. */

int HeapPriorityQueue::peekPriority() const {
    if (!isEmpty()) {
        return arr[1].priority;
    } else throw string("Queue is empty!");
}

/* size():
 * Getter function for count member variable */

int HeapPriorityQueue::size() const {
    return count;
}

/* operator <<:
 * Loops through heap printing out according to the requested format.
 * Only adds a comma if not last element. */

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for (int i = 1; i <= queue.count; i++) {
        out << queue.arr[i];
        if (queue.count != i) out << ", ";
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

void HeapPriorityQueue::resize() {
    PQEntry *newarr = new PQEntry[capacity * 2];
    for (int i = 1; i <= count; i++) {
        newarr[i] = arr[i];
    }
    delete [] arr;
    arr = newarr;
    capacity *= 2;
}

/* swap():
 * Private function that swaps the elements i and j
 * of the binary heap's array. */

void HeapPriorityQueue::swap(int i, int j) {
    PQEntry temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/* bubbleUp():
 * Implementation of the bubbleUp algorithm by recursively comparing
 * the element at the index passed in with its parents, and swapping
 * if the parent is of lower priority. In that case the function is
 * recursively called on the parents.
 * Base Case: the node has no parents, i.e. parent = 0. */

void HeapPriorityQueue::bubbleUp(int i) {
    int parent = i / 2;
    if (parent != 0 && arr[parent] > arr[i]) {
        swap(parent, i);
        bubbleUp(parent);
    }
}

/* bubbleDown():
 * Implementation of the bubbleDown algorithm recursively.
 * Swaps the current element with the child of highest priroity
 * in order to maintain the heap ordering after dequeueing.
 */

void HeapPriorityQueue::bubbleDown(int i) {
    int child1 = 2 * i;
    int child2 = (2 * i) + 1;
    int next = -1;

    // If there are two children, swap with the lowest priority
    if (child2 <= count) next = (arr[child1] <= arr[child2]) ? child1 : child2;
    // Else, swap with the only child
    else if (child1 == count) next = child1;

    // If next has a value not = -1, i.e. has a child and that child is of higher
    // priority, swap and recurse.
    if (next != -1 && arr[i] > arr[next]) {
        swap(i, next);
        bubbleDown(next);
    }
}

