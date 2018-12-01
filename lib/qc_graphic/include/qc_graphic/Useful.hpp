#pragma once

#include <iostream>

namespace qc
{

class Useful
{
public:
    template<typename T>
    static void PrintError(T description)
    {
        std::cerr << "\033[1;38;2;255;57;31mERROR:\033[0m " << description << std::endl;
    }

    template<typename T>
    static void PrintSucced(T description, unsigned int level = 0)
    {
        std::cout << "\033[1;38;2;85;255;85m";
        for (unsigned int i = 0; i <= level; ++i)
            std::cout << "--";
        
        std::cout << " " << description << "\033[0m" << std::endl;
    }

private:
    Useful() {};
};

};  //! namespace qc
