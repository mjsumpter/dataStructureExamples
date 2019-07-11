/* CalcList.hpp
 * Author: Matthew Sumpter
 * Description: (PP1) CalcList stores a series of calculations (CalcEntry objects) as a doubly linked-list.
 * */

#ifndef CALCLIST_HPP
#define CALCLIST_HPP

#include "CalcListInterface.hpp"

class CalcEntry {
    private:
        double value;           // operand
        FUNCTIONS operation;    // operation
        double prevTotal;       // total before entry

        CalcEntry* prev;
        CalcEntry* next;

        friend class CalcList;
};

class CalcList : public CalcListInterface {
    private:
        double currentTotal;
        int numOperations;    // tracks number of CalcEntry objects in list

        // list sentinels
        CalcEntry *header;
        CalcEntry *trailer;

        //helper functions
        bool empty() const;
        void remove(CalcEntry *toRemove);
        char getOpCharacter(const FUNCTIONS func) const;
        double calculate(const double operand1, const double operand2, const FUNCTIONS func) const;

    public:
        CalcList();
        ~CalcList();

        virtual double total() const { return currentTotal; };
        virtual void newOperation(const FUNCTIONS func, const double operand);
        virtual void removeLastOperation();
        virtual std::string toString(unsigned short precision) const;        
};

#endif //CALCLIST_HPP