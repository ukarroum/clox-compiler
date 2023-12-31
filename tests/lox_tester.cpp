#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>

struct LoxTest
{
    std::string path;
    std::string expected;
};

std::vector<LoxTest> lox_tests = {

};

// https://stackoverflow.com/a/478960
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage: lox_tester <lox compiler> <tests folder>" << std::endl;
        exit(64);
    }

    for(const auto& test: lox_tests)
    {
        auto result = exec((std::string(argv[1]) + " " + argv[2] + "/" + test.path).c_str());
    }
}