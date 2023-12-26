#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    Chunk chunk;

    size_t constant = chunk.addConstant(1.2);
    chunk.write(OpCode::OP_CONSTANT, 123);
    chunk.write(constant, 123);
    chunk.write(OpCode::OP_RETURN, 123);

    disChunk(chunk, "test chunk");

    return 0;
}