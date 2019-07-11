/***********************************************************************
 * GritVM.cpp (PP3)
 * Author: Matthew Sumpter
 * Description: Implementation file for GritVM class that interprets
 *              the GritVM programming language. It can read in a file of code
 *              written in GritVM, run the instructions, and hold the results
 *              in object memory.
 * 
 *              See header file for class architecture
 * *********************************************************************/

#include "GritVM.hpp"
#include "CustomVector.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

// takes Instruction object [instruct] as parameter, evaluates it and alters data members as necessary
void GritVM::evaluateInstruction(const Instruction& instruct)
{
    long arg = instruct.argument;  // capture argument variable

    // each case carries out a potential instruction operation
    switch (instruct.operation)
    {
        case CLEAR:
        {   // set accumulator to 0, advance 1 instruction
            accumulator = 0;
            ++currentInstruct;
            break;
        }
        case AT:
        {   // Sets the accumulator to the value at dataMem[arg], advance 1 instruction
            accumulator = dataMem[arg];
            ++currentInstruct;
            break;
        }
        case SET:
        {   // Sets the dataMem[arg] to accumulator, advance 1 instruction
            dataMem[arg] = accumulator;
            ++currentInstruct;
            break;
        }
        case INSERT:
        {   // inserts in dataMem[arg] the accumulator value, advance 1 instruction
            dataMem.insert(arg, accumulator);
            ++currentInstruct;
            break;
        }
        case ERASE:
        {   // Erases location [arg] from dataMem, advance 1 instruction
            CustomVector<long>::Iterator it = dataMem.begin() + arg;
            dataMem.erase(it);
            ++currentInstruct;
            break;
        }
        case ADDCONST:
        {   // adds [arg] to accumulator, advance 1 instruction
            accumulator += arg;
            ++currentInstruct;
            break;
        }
        case SUBCONST:
        {   // subtracts [arg] to accumulator, advance 1 instruction
            accumulator -= arg;
            ++currentInstruct;
            break;
        }
        case MULCONST:
        {   // multiplies [arg] to accumulator, advance 1 instruction
            accumulator *= arg;
            ++currentInstruct;
            break;
        }
        case DIVCONST:
        {   // divides [arg] to accumulator, advance 1 instruction
            accumulator /= arg;
            ++currentInstruct;
            break;
        }
        case ADDMEM:
        {   // adds dataMem[arg] to accumulator, advance 1 instruction
            accumulator += dataMem[arg];
            ++currentInstruct;
            break;
        }
        case SUBMEM:
        {   // subtracts dataMem[arg] to accumulator, advance 1 instruction
            accumulator -= dataMem[arg];
            ++currentInstruct;
            break;
        }
        case MULMEM:
        {   // multiplies dataMem[arg] to accumulator, advance 1 instruction
            accumulator *= dataMem[arg];
            ++currentInstruct;
            break;
        }
        case DIVMEM:
        {   // divides dataMem[arg] to accumulator, advance 1 instruction
            accumulator /= dataMem[arg];
            ++currentInstruct;
            break;
        }
        case JUMPREL:
        {   // advances the instruction set by [arg] (CAN be negative)
            if (arg == 0)
                throw std::invalid_argument("Invalid Jump Command. Arg cannot equal 0");
            else
            {
                instructMem.advance(currentInstruct, arg);
                break;
            }
        }
        case JUMPZERO:
        {   // if accumulator == 0, advance instruction set by [arg], otherwise advance by 1
            if (arg == 0)
                throw std::invalid_argument("Invalid Jump Command. Arg cannot equal 0");
            else
            {
                if (accumulator == 0)
                {
                    instructMem.advance(currentInstruct, arg);
                    break;
                }
                else
                {
                    ++currentInstruct;
                    break;
                }
            }
        }
        case JUMPNZERO:
        {   // if accumulator != 0, advance instruction set by [arg], otherwise advance by 1
            if (arg == 0)
                throw std::invalid_argument("Invalid Jump Command. Arg cannot equal 0");
            else
            {
                if (accumulator != 0)
                {
                    instructMem.advance(currentInstruct, arg);
                    break;
                }
                else
                {
                    ++currentInstruct;
                    break;
                }
            }   
        }
        case NOOP:
        {   // advance 1 instruction
            ++currentInstruct;
            break;
        }
        case HALT:
        {  // Set status to HALTED, advance 1 instruction
            machineStatus = HALTED;
            ++currentInstruct;
            break;
        }
        case OUTPUT:
        {   // Output accumulator to std::out, advance 1 instruction
            std::cout << accumulator << std::endl;
            ++currentInstruct;
            break;
        }
        case CHECKMEM:
        {   // checks if DM is of size [arg]. If not, status = ERRORED. Advance 1 instruction
            int size = static_cast<int>(dataMem.size());
            ++currentInstruct;
            if (size < arg)
            {
                machineStatus = ERRORED;
                break;
            }
            else
            {
                break;
            }
        }
        default:
            throw std::invalid_argument("Instruction not found");
    }
}

// loads in a GritVM program at [filename], with initial data [initialMemory]
// Loads instruction set into instructMem, loads [initialMemory] into dataMem
// Sets machineStatus based on success when loading GritVM program
STATUS GritVM::load(const std::string filename, const std::vector<long> &initialMemory)
{
    // If machine status is anything other than WAITING, return current status
    if (machineStatus != WAITING)
        return machineStatus;

    // open [filename] file
    std::ifstream program(filename);

    // if file failed to open, throw error
    if (!program)
        throw std::runtime_error(filename + " could not be opened");

    //read each line in .gvm file
    std::string line;
    while (std::getline(program, line))
    {
        // if line empty or first character is #, skip to next line
        char firstChar = line[0];
        if (line.empty() || firstChar == '#')
            continue;

        // parse new instruction from line
        Instruction new_instruction = GVMHelper::parseInstruction(line);

        // if instruction not recognized, change the machine status to ERRORED and return
        if (new_instruction.operation == UNKNOWN_INSTRUCTION)
        {
            machineStatus = ERRORED;
            return machineStatus;
        }

        // add instruction to instructMem list
        instructMem.push_back(new_instruction);
    }
    program.close();

    // if the intructMem size is 0, status = WAITING. Else, status = READY
    machineStatus = instructMem.size() == 0 ? WAITING : READY;

    // copy the vector passed into the load method to the dataMem vector
    for (long elem : initialMemory)
        dataMem.push_back(elem);
    
    // set current instruction to be first in instruction set
    currentInstruct = instructMem.begin();

    //return the current status
    return machineStatus;
}

// Runs the GritVM program currently loaded into object memory until machine status is valid
// Returns machineStatus when program terminates
STATUS GritVM::run()
{
    // if not READY, return current status
    if (machineStatus != READY)
        return machineStatus;

    machineStatus = RUNNING;
    // while not on the last instruction
    while(currentInstruct != instructMem.end())
    {
        // execute current instruction if status is RUNNING
        if (machineStatus == RUNNING)
            evaluateInstruction(*currentInstruct);
        else
            break;
    }

    machineStatus = HALTED;
    return machineStatus;
}

// returns a copy of the current dataMem
std::vector<long> GritVM::getDataMem()
{
    std::vector<long> to_return;

    // copy custom dataMem vector to STL vector for return value
    for (int i = 0; i < dataMem.size(); ++i)
        to_return.push_back(dataMem[i]);

    return to_return;
}

// Sets the accumulator to 0, clears the dataMem and instructMem, sets the machine status to WAITING
STATUS GritVM::reset()
{
    accumulator = 0;
    dataMem.clear();
    instructMem.clear();
    machineStatus = WAITING;

    return machineStatus;
}

// prints the current GritVM object data[if printData == true] and the instruction set stored in memory[if printInstruction == true]
// All information is printed to std::cout
void GritVM::printVM(bool printData, bool printInstruction)
{
    // print current status and accumulator
    std::cout << "****** Output Dump ******" << std::endl;
    std::cout << "Status: " << GVMHelper::statusToString(machineStatus) << std::endl;
    std::cout << "Accumulator: " << accumulator << std::endl;
    if (printData)
    {   // if true, print contents of dataMem
        std::cout << "*** Data Memory ***" << std::endl;
        int index = 0;
        CustomVector<long>::Iterator it = dataMem.begin();
        while (it != dataMem.end())
        {
            long item = (*it);
            std::cout << "Location " << index++ << ": " << item << std::endl;
            ++it;
        }
    }
    if (printInstruction)
    {   // if true, print contents of instructMem
        std::cout << "*** Instruction Memory ***" << std::endl;
        int index = 0;
        DLinkedList::Iterator it = instructMem.begin();
        while (it != instructMem.end())
        {
            Instruction item = (*it);
            std::cout << "Instruction " << index++ << ": " << GVMHelper::instructionToString(item.operation) << " " << item.argument << std::endl;
            ++it;
        }
    }
}