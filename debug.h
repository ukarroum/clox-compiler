#pragma once

#include <string>

#include "chunk.h"

void disChunk(const Chunk& chunk, const std::string& name);
int disInstr(const Chunk& chunk, int offset);
int simpleInstr(const std::string& name, int offset);
int constantInstr(const std::string& name, const Chunk& chunk, int offset);
int longConstantInst(const std::string& name, const Chunk& chunk, int offset);