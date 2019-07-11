/***********************************************************************
 * NotationConverter.cpp
 * Author: Matthew Sumpter
 * Description: Implementation file for the NotationConverter class.
 *  
 * *********************************************************************/

#include "NotationConverter.hpp"
#include "DLinkedDeque.hpp"
#include <stdexcept>
#include <algorithm>

/******************************************** Protected/Helper functions ********************************************/

// returns true if character represents one of the 4 base operations
bool NotationConverter::isoperator(char& testChar) const
{
    return (testChar == '+' || testChar == '-' || testChar == '*' || testChar == '/');
}

// returns true if [oper1] represents a higher precendence operation than [oper2]
bool NotationConverter::higherPrecedence(const char& oper1, const char& oper2) const
{
    bool higher;
    switch (oper2)
    {   // as multiplication and division are highest precedence, [oper1] cannot be higher if
        // [oper2] is * or /
        case '*':
        case '/':
            higher = false;
        case '+':
        case '-':
        {   // if [oper1] is multiplication or division, it takes precedence over [oper2] as addition or subtraction
            if (oper1 == '*' || oper1 == '/')
                higher = true;
            else
                higher = false;
        }
    }

    return higher;
}

// returns [convert] as a string
std::string NotationConverter::charToString(const char& convert) const
{
    std::string temp(1, convert);
    return temp;
}

/******************************************** Public functions ********************************************/

// converts a postfix notation string [inStr] to infix notation, and returns it
std::string NotationConverter::postfixToInfix(std::string inStr)
{
    std::string prefix = postfixToPrefix(inStr);
    std::string infix = prefixToInfix(prefix);
    return infix;
}

// converts a postfix notation string [inStr] to prefix notation, and returns it
std::string NotationConverter::postfixToPrefix(std::string inStr)
{
    DLinkedDeque stack;
    std::string prefixString;

    // for every character in [inStr]
    for (unsigned int i = 0; i < inStr.length(); ++i)
    {
        char input = inStr[i];           // capture single character

        if (input == ' ')                // skip white space
            continue;
        else if ( isalpha(input) )       // if alpha, [input] is operand -> push to stack
        {
            stack.insertFront( charToString(input) );
        }
        else if ( isoperator(input) )   // if operator
        {
            // create compound statement with existing operands
            // on the stack

            std::string operand1 = stack.front();      // get most recent operands from stack
            stack.removeFront();
            std::string operand2 = stack.front();
            stack.removeFront();

            // generate compound expression i.e.: "+ A B"
            std::string compound;
            compound.push_back(input);
            compound.push_back(' ');
            compound += operand2;
            compound.push_back(' ');
            compound += operand1;

            stack.insertFront(compound);              // compound expression gets pushed to stack as compound operand
        }
        else
        {   // if none of the above characters, then input is invalid
            throw std::invalid_argument("Invalid character encountered. Conversion halted");
        }
    }

    return stack.front(); // at the end all that remains in the stack is the final prefix notation
}

// converts an infix notation string [inStr] to postfix notation, and returns it
std::string NotationConverter::infixToPostfix(std::string inStr)
{
    DLinkedDeque operatorStack;

    std::string postfixString;

    // for all characters in [inStr]
    for (unsigned int i = 0; i < inStr.length(); ++i)
    {
        char input = inStr[i];

        if ( input == ' ' )        // skip white space
            continue;
        else if ( input == '(' )
        { // if '(' -> push to stack to track precedence of following operations
            operatorStack.insertFront( charToString(input) );
        }
        else if ( isalpha(input) ) 
        { // if alpha -> push operand directly to final output string. First operand is not preceded by space
            if (postfixString.empty())
                postfixString.push_back(input);
            else
            {
                postfixString.push_back(' ');
                postfixString.push_back(input);
            }
        }
        else if ( isoperator(input) )
        { // if operator -> pop operations from stack and append to output string until open parenthesis reached or current
          //                operator [input] is highest precedence
            char front = operatorStack.front()[0];  // quick conversion from single character string to char data type

            while ( !operatorStack.empty() && front != '(' && higherPrecedence(front, input))
            {
                postfixString += " " + front;
                postfixString.push_back(' ');
                postfixString.push_back(front);
                operatorStack.removeFront();
                front = operatorStack.front()[0];
            }
            operatorStack.insertFront( charToString(input) );
        }
        else if ( input == ')' )
        { // if ')' -> pop all operations on stack and append to output string until '(', then pop '('
            char front = operatorStack.front()[0];
            while ( front != '(' )
            {
                postfixString.push_back(' ');
                postfixString.push_back(front);
                operatorStack.removeFront();
                front = operatorStack.front()[0];
            }
            operatorStack.removeFront();            // remove '(', which was tracking the beginning of an encapsulated precedence of operations
        }
        else
        { // if none of the above characters, then input is invalid
            throw std::invalid_argument("Invalid character encountered. Conversion halted");
        }
    }

    // after running through whole input string [inStr], pop all remaining operations on stack and append to the end of the output string
    while ( !operatorStack.empty() )
    {
        char front = operatorStack.front()[0];
        postfixString.push_back(' ');
        postfixString.push_back(front);
        operatorStack.removeFront();
    }

    return postfixString;
}

// converts an infix notation string [inStr] to prefix notation, and returns it
std::string NotationConverter::infixToPrefix(std::string inStr)
{
    std::string postfix = infixToPostfix(inStr);
    std::string prefix = postfixToPrefix(postfix);
    return prefix;
}

// converts a prefix notation string [inStr] to infix notation, and returns it
std::string NotationConverter::prefixToInfix(std::string inStr)
{
    DLinkedDeque stack;

    std::reverse(inStr.begin(), inStr.end());             // reverse string so operands can be stacked first
    
    // for all characters in [inStr]
    for (unsigned int i = 0; i < inStr.length(); ++i)
    {
        char input = inStr[i]; // capture single character

        if (input == ' ') // skip white space
            continue;
        else if (isalpha(input))
        { // if alpha -> push operand onto stack
            stack.insertFront( charToString(input) );
        }
        else if (isoperator(input))
        { // if operator ->
            // create compound statement with existing operands
            // on the stack

            std::string operand1 = stack.front(); // get most recent operands from stack
            stack.removeFront();
            std::string operand2 = stack.front();
            stack.removeFront();

            // generate compound expression i.e. "(A + B)"
            std::string compound;
            compound.push_back( '(' );
            compound += operand1;
            compound.push_back( ' ' );
            compound.push_back( input );
            compound.push_back( ' ' );
            compound += operand2;
            compound.push_back( ')' );

            stack.insertFront(compound); // push compound expression onto stack as compound operand
        }
        else
        { // if none of the above characters, then input is invalid
            throw std::invalid_argument("Invalid character encountered. Conversion halted");
        }
    }

    return stack.front(); // at the end, 1 string should remain in stack; this is final infix notation
}

// converts a prefix notation string [inStr] to postfix notation, and returns it
std::string NotationConverter::prefixToPostfix(std::string inStr)
{
    std::string infix = prefixToInfix(inStr);
    std::string postfix = infixToPostfix(infix);
    return postfix;
}