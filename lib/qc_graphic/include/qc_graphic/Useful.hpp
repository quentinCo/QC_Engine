#pragma once

#include <iostream>

namespace qc
{

class Useful
{
public:
    template<typename T>
    static void printError(T description)
    {
        std::cerr << "\033[1;38;2;255;57;31mERROR: " << description << "\033[0m" << std::endl;
    }

    template<typename T>
    static void printValid(T description)
    {
        std::cout << "\033[1;38;2;85;255;85m------------- " << description << "\033[0m" << std::endl;
    }

private:
    Useful() {};
};

};  //! namespace qc
