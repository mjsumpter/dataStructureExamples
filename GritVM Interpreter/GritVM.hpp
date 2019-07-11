/***********************************************************************
 * GritVM.hpp (PP3)
 * Author: Matthew Sumpter
 * Description: Header file for GritVM class that interprets
 *              the GritVM programming language. It can read in a file of code
 *              written in GritVM, run the instructions, and hold the results
 *              in object memory.
 * 
 *              See implementation file for function descriptions
 * *********************************************************************/

#ifndef GRITVM_H
#define GRITVM_H

#include "GritVMBase.hpp"
#include "DLinkedList.hpp"
#include "CustomVector.hpp"

#include <string>
#include <vector>  // required from abstract class

class GritVM : public GritVMInterface
{
    private:
        CustomVector<long> dataMem;                              // Vector ADT that holds the data memory for a program
        DLinkedList instructMem;                                 // Doubley-linked list that holds the list of instructions
        DLinkedList::Iterator currentInstruct;                   // Iterator that tracks the current instruction
        STATUS machineStatus;                                    // Holds the current status of the program
        long accumulator;                                        // Works as the accumulator for the GritVM - stores temp values for calculation 

        void evaluateInstruction(const Instruction& instruct);   // takes Instruction object as parameter, evaluates it and alters data members as necessary

    public:
        GritVM() : currentInstruct( instructMem.begin() ), machineStatus(WAITING), accumulator(0){};

        virtual STATUS load(const std::string filename, const std::vector<long>& initialMemory);
        virtual STATUS run();
        virtual std::vector<long> getDataMem();
        virtual STATUS reset();

        void printVM(bool printData, bool printInstruction);
};

#endif /* GRITVM_H */