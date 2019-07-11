/* CalcList.hpp
 * Author: Matthew Sumpter
 * Description: CalcList stores a series of calculations (CalcEntry objects) as a doubly linked-list.
 * */

#ifndef CALCLIST_HPP
#define CALCLIST_HPP


// Enum that contains all the arithmetic functions needed for newOperation
typedef enum
{
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION
} FUNCTIONS;

class CalcEntry {
    private:
        double value;           // operand
        FUNCTIONS operation;    // operation
        double prevTotal;       // total before entry

        CalcEntry* prev;
        CalcEntry* next;

        friend class CalcList;
};

class CalcList {
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

        double total() const { return currentTotal; };
        void newOperation(const FUNCTIONS func, const double operand);
        void removeLastOperation();
        std::string toString(unsigned short precision) const;        
};

#endif //CALCLIST_HPP