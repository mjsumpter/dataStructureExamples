/***********************************************************************
 * NotationConverter.hpp
 * Author: Matthew Sumpter
 * Description: (PP2) Header file for the NotationConverter class.
 *              The class provides the functionality for converting between
 *              prefix, postfix, and infix mathematical notations.
 * 
 *              The only characters allowed in the input string to be converted
 *              are +, -, *, /, (, ), alphabetic characters, and spaces.
 *              All other characters will throw errors.
 *              
 *              Note:
 *                   Instances of a DLinkedDeque are used as stacks to assist
 *                   with tracking and converting operations. See implementation
 *                   file for details on member functions
 * *********************************************************************/
#ifndef NOTATIONCONVERTER_H
#define NOTATIONCONVERTER_H

#include "NotationConverterInterface.hpp"
#include <string>

class NotationConverter : public NotationConverterInterface
{
    protected:
        // helper functions
        bool isoperator(char& testChar) const;
        bool higherPrecedence(const char& oper1, const char& oper2) const;
        std::string charToString(const char& convert) const;

    public:
        virtual std::string postfixToInfix(std::string inStr);
        virtual std::string postfixToPrefix(std::string inStr);

        virtual std::string infixToPostfix(std::string inStr);
        virtual std::string infixToPrefix(std::string inStr);

        virtual std::string prefixToInfix(std::string inStr);
        virtual std::string prefixToPostfix(std::string inStr);
};

#endif /* NOTATIONCONVERTER_H */