//
// Created by ykarroum on 12/26/23.
//

#ifndef CLOX_COMPILER_CHUNK_H
#define CLOX_COMPILER_CHUNK_H

#include <cstdint>
#include <vector>
#include <cstdlib>

#include "common.h"
#include "value.h"

enum OpCode : uint8_t
{
    OP_CONSTANT,
    OP_RETURN
};

struct Chunk
{
    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<Value> constants;

    void write(uint8_t byte, int line);
    size_t addConstant(Value constant);
};

#endif //CLOX_COMPILER_CHUNK_H
