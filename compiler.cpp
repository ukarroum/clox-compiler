//
// Created by ykarroum on 12/27/23.
//

#include <iostream>

#include "compiler.h"
#include "Scanner.h"

void compile(const std::string& code)
{
    Scanner scanner {code};

    while(true)
    {
        Token token = scanner.scanToken();

        std::cout <<
    }
}