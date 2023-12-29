#include <iostream>
#include <fstream>
#include <sstream>

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "VM.h"


void repl(VM& vm)
{
    std::string line;

    while(true)
    {
        std::cout << ">> ";

        getline(std::cin, line);

        if (line.empty())
        {
            std::cout << std::endl;
            break;
        }

        vm.interpret(line);
    }
}

void runFile(const std::string& file, VM& vm)
{
    std::ifstream input(file);
    std::string code;
    std::stringstream ss;

    if(input.is_open())
    {
        ss << input.rdbuf();
        code = ss.str();

        InterpretResult result = vm.interpret(code);

        if(result == InterpretResult::INTERPRET_COMPILE_ERROR)
            exit(65);
        if(result == InterpretResult::INTERPRET_RUNTIME_ERROR)
            exit(70);
    }
    else
    {
        std::cerr << "Couldn't open: " << file << std::endl;
        exit(74);
    }
}
int main(int argc, char *argv[])
{
    VM vm;

    if(argc == 1)
    {
        repl(vm);
    }
    else if(argc == 2)
    {
        runFile(argv[1], vm);
    }
    else
    {
        std::cout << "Usage: clox [path]" << std::endl;
        exit(64);
    }

    return 0;
}