/***********************************************************************
 * DLinkedList.hpp
 * Author: Matthew Sumpter
 * Description: Header file for a doubly-linked list class that uses
 *              Nodes containing strings. See implementation file for
 *              function descriptions
 * *********************************************************************/
#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <string>
#include "GritVMBase.hpp"


class Node 
{
    private:
        Instruction elem = Instruction(UNKNOWN_INSTRUCTION);               // node element
        Node* prev;                                                        // pointers to previous and next nodes
        Node* next;
        friend class DLinkedList;                                          // Node for DLinkedList
};

class DLinkedList
{
    public:
        class Iterator
        {
            private:
                Node* ptr_;

            public:
                Iterator(Node *u) : ptr_(u) {};
                Instruction& operator*();
                Iterator& operator++();
                Iterator& operator--();
                bool operator!=(const DLinkedList::Iterator& p) const;
        };

    private: // list sentinals
        Node *header;
        Node *trailer;
        int n;

    protected: // local utilities
        void add(Node *insertBefore, Instruction &c);
        void remove(Node *v);

    public:
        DLinkedList();
        ~DLinkedList();
        int size() const { return n; };
        void clear();
        Iterator begin() const { return Iterator(header->next); };
        Iterator end() const { return Iterator(trailer); };
        void advance(Iterator& ptr, long arg);
        bool empty() const;
        const Instruction &front() const;
        const Instruction &back() const;
        void addFront(Instruction &e);
        void push_back(Instruction &e);
        void removeFront();
        void removeBack();        
};

#endif // DLINKEDLIST_H