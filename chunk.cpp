//
// Created by ykarroum on 12/26/23.
//

#include "chunk.h"

void Chunk::write(uint8_t byte, size_t line)
{
    code.push_back(byte);
    lines.push_back(line);
}

size_t Chunk::addConstant(Value constant)
{
    constants.push_back(constant);

    return constants.size() - 1;
}