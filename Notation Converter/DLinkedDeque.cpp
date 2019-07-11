/***********************************************************************
 * DLinkedDeque.cpp
 * Author: Matthew Sumpter
 * Description: (PP2) Implementation file for a Deque, implemented using a doubly-linked list.
 * *********************************************************************/

#include "DLinkedDeque.hpp"
#include <stdexcept>

// returns the element at the front of the deque
const Elem& DLinkedDeque::front() const
{
    if(empty())
        throw std::out_of_range("Error: List is empty");
    return dlList.front();
}

// returns the element at the back of the deque
const Elem& DLinkedDeque::back() const
{
    if (empty())
        throw std::out_of_range("Error: List is empty");
    return dlList.back();
}

// inserts [e] at the front of the deque
void DLinkedDeque::insertFront(const Elem& e)
{
    dlList.addFront(e);
    ++n;
}

// inserts [e] at the back of the list
void DLinkedDeque::insertBack(const Elem& e)
{
    dlList.addBack(e);
    ++n;
}

// removes the element located at the front of the deque
void DLinkedDeque::removeFront()
{
    if (empty())
        throw std::out_of_range("Error: List is empty");
    dlList.removeFront();
    --n;
}

// removes the element located at the back of the deque
void DLinkedDeque::removeBack()
{
    if (empty())
        throw std::out_of_range("Error: List is empty");
    dlList.removeBack();
    --n;
}