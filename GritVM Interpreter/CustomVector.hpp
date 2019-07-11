/***********************************************************************************
 * CustomVector.hpp (PP3 EC)
 * Author: Matthew Sumpter
 * Description: Template file for Vector ADT class.
 * 
 *              Note: Functionality is specifically restricted to the GritVM class
 * *********************************************************************************/
#ifndef CUSTOMVECTOR_H
#define CUSTOMVECTOR_H

#include <string>
#include <algorithm>

template <typename E>
class CustomVector
{
    public:
        class Iterator
        {
            private:
                E* ptr_;
                Iterator(E* u) { ptr_ = u; };

            public:
                E& operator*()
                {
                    return *ptr_;
                };
                Iterator& operator++()
                {
                    ++ptr_;
                    return *this;
                };
                Iterator& operator--()
                {
                    --ptr_;
                    return *this;
                };
                Iterator operator+(E arg) const
                {
                    return Iterator(this->ptr_ + arg);
                };
                Iterator operator-(E arg) const
                {
                    return Iterator(this->ptr_ - arg);
                };
                bool operator<(const Iterator& second_it)
                {
                    return ptr_ < second_it.ptr_;
                };
                bool operator>=(const Iterator& second_it)
                {
                    return ptr_ >= second_it.ptr_;
                };
                bool operator!=(const Iterator& second_it)
                {
                    return ptr_ != second_it.ptr_;
                };
                void operator=(const E& new_elem)
                {
                    ptr_ = &new_elem;
                };

                friend class CustomVector;
        };

    private:
        int capacity;                // current array size
        int n;                       // number of elements in vector
        E* array;

    public:
        CustomVector() : capacity(0), n(0), array(nullptr) {};            // constructor initializes empty(0) sized array
        ~CustomVector() { delete [] array; };                             // deconstructor
        int size() const { return n; };                                   // returns number of elements stored in array
        bool empty() const { return size() == 0; };                       // returns true if array is empty
        E& operator[](int i) { return array[i]; };                        // overloads [] operator - no error checking
        Iterator begin() const { return Iterator(array); };               // returns an iterator to the first element in array
        Iterator end() const { return Iterator(array + n); };             // returns an iterator to the off-the-end position in the array
        
        // designates a new array of size [N], and reassigns the capacity to [N]
        void reserve(int N)                                        
        {
            if (capacity >= N)  // if function was called although capacity is already sufficient, return
                return;
            E* B = new E[N];    // allocate new array of desired size
            // copy all elements from current array to new array
            for (int j = 0; j < n; j++)
                B[j] = array[j];
            // if current array wasn't empty, delete it
            if (array != nullptr)
                delete [] array;
            // assign new array as member data, update capacity
            array = B;
            capacity = N;
        };
        
        // inserts element [e] at position in array pointed to by [i]
        void insert(Iterator& i, E& e)
        {
            // if more space is needed, increase size of array
            if (n >= capacity)
                reserve(std::max(1, 2 * capacity));
                
            // move all elements above [i] up by one position
            for (auto j = end(); j >= i; --j)
            {
                Iterator m = j + 1;
                Iterator n = j;
                *m = *n;
            }
            *i = e;         // assign new element [e] to position [i]
            n++;            // increase num of elements stored
        };
        
        // inserts element [e] at a position [arg] from the beginning (see above insert function)
        void insert(long& arg, E& e)
        {
            if (n >= capacity)
                reserve(std::max(1, 2 * capacity));

            Iterator i(begin() + arg);
            for (auto j = end(); j >= i; --j)
            {
                Iterator m = j + 1;
                Iterator n = j;
                *m = *n;
            }

            *i = e;
            n++;
        };

        // erase the element at position [i]
        void erase(Iterator i)
        {
            Iterator j = i + 1;
            // move all elements beyond [i] back one position
            for (; j < end(); ++j)
            {
                Iterator m = j - 1;
                Iterator n = j;
                *m = *n;
            }
            n--;   // decrement num of elements stored
        };

        // pushes element [e] to the end of the array
        void push_back(E e)
        {
            // increases capacity if neccessary
            if (n >= capacity)
                reserve(std::max(1, 2 * capacity));

            // if first entry, load element into a[0]
            long first_entry = 0;
            if (n == first_entry)
                insert(first_entry, e);
            else
            {   // if not first entry, load element into end of array
                Iterator end_ = end() - 1;
                insert(end_, e);
            }
        };

        // erases every element in the array, n = 0
        void clear()
        {
            while ( !empty() )
                erase( begin() );
        };
};

#endif // CUSTOMVECTOR_H