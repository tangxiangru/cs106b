// LinkedList PQueue Implementation
// by Cristobal Sciutto
// Section: Shirin Salehi

#include "LinkedPriorityQueue.h"

/* LinkedPriorityQueue() contructor:
 * Instantiates the "front" pointer to NULL. */

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

/* ~LinkedPriorityQueue() deconstructor:
 * Calls the clear function to free memory and
 * deletes the "front" pointer. */

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
    delete front;
}

/* changePriority():
 * Loops through linked list until the value is found.
 * If the newPriority is smaller than the current,
 * the prioirty is updated. Else, an error is thrown.
 * Error is also thrown if the value is not found. */

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode *iter = front;
    while (iter->value != value) {
        if (iter->next != NULL) iter = iter->next;
        else throw string("Value not in queue!");
    }
    if (iter->priority <= newPriority) {
        throw string("You must change to a lower priority!");
    } else iter->priority = newPriority;
}

/* clear():
 * Creates a temp variable that holds the current front pointer,
 * moves front forward, and uses that temp variable to delete
 * allocated heap memory.
 */

void LinkedPriorityQueue::clear() {
    while (front != NULL) {
        ListNode *temp = front;
        front = front->next;
        delete temp;
    }
}

/* dequeue():
 * If the list is not empty, return the element of
 * highest priority (first element). To make sure memory
 * is not lost, a temp variable is used to store the
 * location of the dequeued element which is then deleted. */

string LinkedPriorityQueue::dequeue() {
    if (!isEmpty()) {
        ListNode *temp = front;
        string str = front->value;
        front = front->next;
        delete temp;
        return str;
    } else throw string("Queue is empty!");
}

/* enqueue():
 * Using two pointer, prev and curr, loop through array until curr
 * is higher priority than the node to be inserted. In that case,
 * perform the connections to insert it into that place. One edge
 * case is if the queue is empty, in which case prev == NULL, and
 * front becomes the node that is inserted.
 */

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode *node = new ListNode(value, priority);
    ListNode *prev = NULL;
    ListNode *curr = front;

    // Loop through until insert spot
    while (curr != NULL) {
        if (*curr > *node) break;
        prev = curr;
        curr = curr->next;
    }

    node->next = curr;
    if (prev == NULL) front = node;
    else prev->next = node;
}

/* isEmpty():
 * Checks if the front pointer is null */

bool LinkedPriorityQueue::isEmpty() const {
    return (front == NULL);
}

/* peek():
 * If not empty, returns value of highest priority, which
 * is the first element of the linked list.
 * Else, throws error. */

string LinkedPriorityQueue::peek() const {
    if (!isEmpty()) {
        return front->value;
    } else throw string("Queue is empty!");
}

/* peek():
 * If not empty, returns the highest priority, which
 * is the priority of the first element of the linked list.
 * Else, throws error. */

int LinkedPriorityQueue::peekPriority() const {
    if (!isEmpty()) {
        return front->priority;
    } else throw string("Queue is empty!");;
}

/* size():
 * Loops through linked list incrementing a counter function.
 * Returns the counter. */

int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode *iter = front;
    while (iter != NULL) {
        count += 1;
        iter = iter->next;
    }
    return count;
}

/* operator <<:
 * Loops through linked list printing out according to the requested format.
 * Only adds a comma if not last element. */

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode *iter = queue.front;
    out << "{";
    while (iter != NULL) {
        out << *iter;
        if (iter->next != NULL) out << ", ";
        iter = iter->next;
    }
    out << "}";
    return out;
}
