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
    OP_RETURN,
    OP_NEGATE,
    OP_ADD,
    OP_SUBSTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
};

class Chunk
{
public:
    void write(uint8_t byte, int line);
    size_t addConstant(Value constant);
    const std::vector<uint8_t>& getCode() const { return code; }
    const std::vector<int>& getLines() const { return lines; }
    const std::vector<Value>& getConstants() const { return constants; }

private:
    std::vector<uint8_t> code;
    std::vector<int> lines;
    std::vector<Value> constants;
};

#endif //CLOX_COMPILER_CHUNK_H
