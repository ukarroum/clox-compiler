#include <iostream>
#include <iomanip>

#include "debug.h"
#include "value.h"

void disChunk(const Chunk& chunk, const std::string& name)
{
    std::cout << "== " << name << " ==" << std::endl;

    int offset = 0;

    while(offset < chunk.getCode().size())
    {
        offset = disInstr(chunk, offset);
    }
}

int disInstr(const Chunk& chunk, int offset)
{
    std::cout << std::setfill('0') << std::setw(4) << offset << " ";
    std::cout << chunk.getLine(offset) << " ";

    uint8_t instr = chunk.getCode()[offset];

    switch(instr)
    {
        case OpCode::OP_RETURN:
            return simpleInstr("OP_RETURN", offset);
        case OpCode::OP_CONSTANT:
            return constantInstr("OP_CONSTANT", chunk, offset);
        case OpCode::OP_CONSTANT_LONG:
            return longConstantInst("OP_CONSTANT_LONG", chunk, offset);
        case OpCode::OP_NEGATE:
            return simpleInstr("OP_NEGATE", offset);
        case OpCode::OP_ADD:
            return simpleInstr("OP_ADD", offset);
        case OpCode::OP_SUBSTRACT:
            return simpleInstr("OP_SUBSTRACT", offset);
        case OpCode::OP_MULTIPLY:
            return simpleInstr("OP_MULTIPLY", offset);
        case OpCode::OP_DIVIDE:
            return simpleInstr("OP_DIVIDE", offset);
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
    size_t constant_idx = chunk.getCode()[offset + 1];
    std::cout << name << " " << constant_idx << " '";
    printValue(chunk.getConstants()[constant_idx]);
    std::cout << "'" << std::endl;

    return offset + 2;
}

int longConstantInst(const std::string &name, const Chunk &chunk, int offset)
{
    size_t constant_idx = 0;
    constant_idx += chunk.getCode()[offset + 1];
    constant_idx += chunk.getCode()[offset + 2] << 8;
    constant_idx += chunk.getCode()[offset + 3] << 16;

    std::cout << name << " " << constant_idx << " '";
    printValue(chunk.getConstants()[constant_idx]);
    std::cout << "'" << std::endl;

    return offset + 4;

}
