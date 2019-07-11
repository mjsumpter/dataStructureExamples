/***********************************************************************
 * DLinkedList.cpp
 * Author: Matthew Sumpter
 * Description: Implementation file for a doubly-linked list
 * *********************************************************************/

#include "DLinkedList.hpp"
#include <stdexcept>
#include <iostream>

/************************************** Protected Functions **************************************/

// adds a new node containing element [c] to the list before [insertBefore]
void DLinkedList::add(Node* insertBefore, const Elem& c)
{
    Node* newNode = new Node;            // allocate new node
    newNode->elem = c;                   // for Elem c
    newNode->next = insertBefore;        // link new nodes next
    newNode->prev = insertBefore->prev;  // link new nodes previous
    newNode->prev->next = newNode;       // link preceding node;s next to new node
    insertBefore->prev = newNode;        // backwards link next node to new node
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
}

// deconstructor
DLinkedList::~DLinkedList()
{
    // while not empty, remove from back
    while (!empty())
        removeBack();

    delete header;
    delete trailer;
}

// returns true if list is empty
bool DLinkedList::empty() const
{
    return (header->next == trailer); // double ll sentinal design
}

// returns reference to element at front of list
const Elem& DLinkedList::front() const
{
    if( !empty() )
        return header->next->elem;
    else
        throw std::out_of_range("Error: List is empty");
}

// returns reference to element at end of list
const Elem& DLinkedList::back() const
{
    if ( !empty() )
        return trailer->prev->elem;
    else
        throw std::out_of_range("Error: List is empty");
}

// adds [e] to front of list
void DLinkedList::addFront(const Elem& e)
{
    add(header->next, e);
}

// adds [e] to end of list
void DLinkedList::addBack(const Elem& e)
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