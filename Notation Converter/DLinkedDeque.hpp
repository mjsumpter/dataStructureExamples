/***********************************************************************
 * DLinkedDeque.hpp
 * Author: Matthew Sumpter
 * Description: Header file for a Deque, implemented using a doubly-linked list.
 *              See implementation file for function descriptions.
 * *********************************************************************/

#ifndef DLINKEDDEQUE_H
#define DLINKEDDEQUE_H

#include "DLinkedList.hpp"

class DLinkedDeque {
    private:
        DLinkedList dlList;                        // double linked list of elements
        int n;                                     // number of elements in deque
    public:
        DLinkedDeque() : n(0) {};
        int size() const { return n; };
        bool empty() const { return n == 0; };
        const Elem& front() const;
        const Elem& back() const;
        void insertFront(const Elem& e);
        void insertBack(const Elem& e);
        void removeFront();
        void removeBack();
};

#endif // DLINKEDDEQUE_H
