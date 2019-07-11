/***********************************************************************
 * DLinkedList.cpp
 * Author: Matthew Sumpter
 * Description: Implementation file for a doubly-linked list
 * *********************************************************************/

#include "DLinkedList.hpp"
#include <stdexcept>
#include <iostream>

/************************************** DLinkedList::Iterator **************************************/

// NOTE: only functions required for GritVM class are included in DLinkedList::Iterator class

// overloaded * operator, returns Instruction
Instruction& DLinkedList::Iterator::operator*()
{
    return ptr_->elem;
}

// overloaded pre-increment operator
DLinkedList::Iterator& DLinkedList::Iterator::operator++()
{
    ptr_ = ptr_->next;
    return *this;
}

// overloaded pre-decrement operator
DLinkedList::Iterator& DLinkedList::Iterator::operator--()
{
    ptr_ = ptr_->prev;
    return *this;
}

// overloaded inequality operator
bool DLinkedList::Iterator::operator!=(const DLinkedList::Iterator &p) const 
{ 
    return ptr_ != p.ptr_; 
}

/************************************** Protected Functions **************************************/

// adds a new node containing element [c] to the list before [insertBefore]
void DLinkedList::add(Node* insertBefore, Instruction& c)
{
    Node *newNode = new Node;                       // allocate new node
    newNode->elem = c;
    newNode->next = insertBefore;                   // link new nodes next
    newNode->prev = insertBefore->prev;             // link new nodes previous
    newNode->prev->next = newNode;                  // link preceding node;s next to new node
    insertBefore->prev = newNode;                   // backwards link next node to new node
    ++n;
}

// removes node [v] from linked list
void DLinkedList::remove(Node* v)
{
    if ( !empty() )
    {
        // relink nodes, excluding [v]
        v->prev->next = v->next;
        v->next->prev = v->prev;
        delete v;
        --n;
    }
    else
        throw std::out_of_range("Error: List is empty");
}

/**************************************** Public Functions ****************************************/

// constructor
DLinkedList::DLinkedList()
{
    header = new Node;
    trailer = new Node;
    header->next = trailer;
    trailer->prev = header;
    n = 0;
}

// deconstructor
DLinkedList::~DLinkedList()
{
    clear();

    delete header;
    delete trailer;
}

// clears the list of all elements
void DLinkedList::clear()
{
    // while not empty, remove from back
    while (!empty())
        removeBack();
}

// advances [Iterator] by the amount designated by [arg]
void DLinkedList::advance(Iterator& ptr, long arg)
{
    if (arg > 0)
    {
        for (int i = 0; i < arg; ++i)
            ++ptr;
    }
    else if (arg < 0)
    {
        for (int i = 0; i > arg; --i)
            --ptr;
    }
}

// returns true if list is empty
bool DLinkedList::empty() const
{
    return (header->next == trailer); // double ll sentinal design
}

// returns reference to element at front of list
const Instruction& DLinkedList::front() const
{
    if( !empty() )
        return header->next->elem;
    else
        throw std::out_of_range("Error: List is empty");
}

// returns reference to element at end of list
const Instruction& DLinkedList::back() const
{
    if ( !empty() )
        return trailer->prev->elem;
    else
        throw std::out_of_range("Error: List is empty");
}

// adds [e] to front of list
void DLinkedList::addFront(Instruction& e)
{
    add(header->next, e);
}

// adds [e] to end of list
void DLinkedList::push_back(Instruction& e)
{
    add(trailer, e);
}

// removes the element at the front of the list
void DLinkedList::removeFront()
{
    try
    {
        remove(header->next);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// removes the element at the back of the list
void DLinkedList::removeBack()
{
    try
    {
        remove(trailer->prev);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}