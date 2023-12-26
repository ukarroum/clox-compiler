//
// Created by ykarroum on 12/26/23.
//

#ifndef CLOX_COMPILER_DEBUG_H
#define CLOX_COMPILER_DEBUG_H

#include <string>

#include "chunk.h"

void disChunk(const Chunk& chunk, const std::string& name);
int disInstr(const Chunk& chunk, int offset);
int simpleInstr(const std::string& name, int offset);
int constantInstr(const std::string& name, const Chunk& chunk, int offset);
void printValue(Value value);

#endif //CLOX_COMPILER_DEBUG_H
