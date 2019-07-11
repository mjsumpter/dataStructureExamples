/* CalcList.cpp
 * Author: Matthew Sumpter
 * Description: The implementation file for the CalcList class. See CalcList.hpp for more details
 * */

#include "CalcList.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>

// constructor
CalcList::CalcList() 
{
    // dynamically allocate header and trailer of double linked list
    header = new CalcEntry;
    trailer = new CalcEntry;
    header->next = trailer;
    trailer->prev = header;

    // initialize tracking variables
    currentTotal = 0;
    numOperations = 0;
}

// destructor
CalcList::~CalcList() 
{
    // deletes all CalcEntry objects, beginning at the end of the list
    while(!empty())
        remove(trailer->prev);

    delete header;
    delete trailer;
}

// newOperation adds an operaton to the CalcList and updates the total. Example: newOperation(ADDITION, 10) adds ten to the total
void CalcList::newOperation(const FUNCTIONS func, const double operand)
{
    double prevTotal = currentTotal;   //capture total for storage before altering

    // ensures operation is valid and total is updated before creating new list node
    try
    {   // update total if valid operation
        currentTotal = calculate(currentTotal, operand, func);
    }
    catch(const std::invalid_argument& e)
    {   // invalid operation, rethrow error
        throw e;
    }

    // initialize new CalcEntry
    CalcEntry *newOp = new CalcEntry;
    newOp->operation = func;
    newOp->value = operand;
    newOp->prevTotal = prevTotal;

    // place new CalcEntry at end of CalcList
    (trailer->prev)->next = newOp; // point previous last element at new element
    newOp->prev = trailer->prev;   // point new element at previous last element
    newOp->next = trailer;         // point new element at trailer
    trailer->prev = newOp;         // point trailer at new element

    numOperations++;
}

// removes most recent operation from CalcList and restores previous total
void CalcList::removeLastOperation()
{
    if ( empty() )
        throw std::out_of_range("There are no operations to remove.");
    else
    {   // if list is not empty:
        currentTotal = (trailer->prev)->prevTotal; // restore previous total
        remove(trailer->prev);                     // remove last operation
    }
}

// returns a string of the list of operations completed so far formatted with a fexed point [precision].
std::string CalcList::toString(unsigned short precision) const
{
    std::string output = "";
    int operation = numOperations;

    // cursor passes over every CalcEntry in CalcList in reverse order
    for (CalcEntry *cursor = trailer->prev; cursor != header; cursor = cursor->prev)
    {
        std::stringstream opLine;
        /* example opLine: "3: 30.00*2.00=60.00" */
        opLine << operation-- << ": " << std::fixed << std::setprecision(precision) << cursor->prevTotal << getOpCharacter(cursor->operation)
               << cursor->value << "=" << calculate(cursor->prevTotal, cursor->value, cursor->operation) << '\n';
        output += opLine.str();  // append calculation line to output string
    }

    return output;
}

/***** HELPER FUNCTIONS *****/

// returns true if CalcList object is empty
bool CalcList::empty() const
{
    return ( header->next == trailer );  //double ll sentinal design
}

// removes and deallocates [toRemove] node from CalcList
void CalcList::remove(CalcEntry* toRemove)
{
    CalcEntry* previous = toRemove->prev;  //capture previous node
    CalcEntry* next = toRemove->next;      //capture next node

    previous->next = next;                 //remove toRemove from list
    next->prev = previous;
    
    delete toRemove;
    numOperations--;                       //decrease logged operations
}

// returns the character associated with the operation [func]
char CalcList::getOpCharacter(const FUNCTIONS func) const
{
    char op;
    switch (func)
    {
        case ADDITION:
            op = '+';
            break;
        case SUBTRACTION:
            op = '-';
            break;
        case MULTIPLICATION:
            op = '*';
            break;
        case DIVISION:
            op = '/';
            break;
    }
    return op;
}

// receives values and operation, and returns their result
double CalcList::calculate(const double operand1, const double operand2, const FUNCTIONS func) const
{
    double total;

    switch (func)
    {   // check for type and - if valid - calculate
        case ADDITION:
            total = operand1 + operand2;
            break;
        case SUBTRACTION:
            total = operand1 - operand2;
            break;
        case MULTIPLICATION:
            total = operand1 * operand2;
            break;
        case DIVISION:
        {
            if (operand2 == 0)
                throw std::invalid_argument("Cannot divide by zero.");
            else
            {
                total = operand1 / operand2;
                break;
            }
        }
    }

    return total;
}