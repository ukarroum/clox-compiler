#include <stdexcept>
#include <format>

#include "chunk.h"

void Chunk::write(uint8_t byte, size_t line)
{
    code.push_back(byte);

    if(lines[lines.size() - 1].first == line)
        lines[lines.size() - 1].second++;
    else
        lines.emplace_back(line, 1);
}

size_t Chunk::addConstant(Value constant)
{
    constants.push_back(constant);

    return constants.size() - 1;
}

uint16_t Chunk::getLine(int offset) const {
    int curr = 0;

    for(auto& e: lines)
    {
        curr += e.second;
        if (curr >= offset)
            return e.first;
    }

    throw std::invalid_argument(std::format("Couldn't find line for offset: {}", offset));
}

void Chunk::writeConstant(Value constant, int line)
{
    size_t idx = addConstant(constant);
    if(constants.size() == 256)
    {
        write(OP_CONSTANT_LONG, line);
        write(idx & 0xFF, line);
        write((idx >> 8) & 0xFF, line);
        write((idx >> 16) & 0xFF, line);
    }
    else
    {
        write(OP_CONSTANT, line);
        write(idx, line);
    }
}
