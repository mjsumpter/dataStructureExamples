/***********************************************************************
 * DLinkedList.hpp
 * Author: Matthew Sumpter
 * Description: (PP2) Header file for a doubly-linked list class that uses
 *              Nodes containing strings. See implementation file for
 *              function descriptions
 * *********************************************************************/
#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <string>

typedef std::string Elem;         // list element is string
class Node {
    private:
        Elem elem;                // node element
        Node* prev;               // pointers to previous and next nodes
        Node* next;
        friend class DLinkedList; // Node for DLinkedList
};

class DLinkedList {
    private:                               // list sentinals
        Node* header;
        Node* trailer;
    protected:                             // local utilities
        void add(Node* insertBefore, const Elem& c);
        void remove(Node* v);
    public:
        DLinkedList();
        ~DLinkedList();
        bool empty() const;
        const Elem& front() const;
        const Elem& back() const;
        void addFront(const Elem& e);
        void addBack(const Elem& e);
        void removeFront();
        void removeBack();
};

#endif // DLINKEDLIST_H