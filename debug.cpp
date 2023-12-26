//
// Created by ykarroum on 12/26/23.
//

#include <iostream>
#include <iomanip>

#include "debug.h"
#include "value.h"

void disChunk(const Chunk& chunk, const std::string& name)
{
    std::cout << "== " << name << " ==" << std::endl;

    int offset = 0;

    while(offset < chunk.code.size())
    {
        offset = disInstr(chunk, offset);
    }
}

int disInstr(const Chunk& chunk, int offset)
{
    std::cout << std::setfill('0') << std::setw(4) << offset << " ";
    std::cout << chunk.lines[offset] << " ";

    uint8_t instr = chunk.code[offset];

    switch(instr)
    {
        case OpCode::OP_RETURN:
            return simpleInstr("OP_RETURN", offset);
        case OpCode::OP_CONSTANT:
            return constantInstr("OP_CONSTANT", chunk, offset);
        default:
            std::cerr << "Unknown opcode " << instr << std::endl;
            return offset + 1;
    }
}

int simpleInstr(const std::string& name, int offset)
{
    std::cout << name << std::endl;

    return offset + 1;
}

int constantInstr(const std::string& name, const Chunk& chunk, int offset)
{
    size_t constant_idx = chunk.code[offset + 1];
    std::cout << name << " " << constant_idx << " '";
    printValue(chunk.constants[constant_idx]);
    std::cout << "'" << std::endl;

    return offset + 2;
}

void printValue(Value value)
{
    std::cout << value;
}
