#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        PmergeMe sorter;
        sorter.sort(argc, argv);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error\n";
        return 1;
    }
    return 0;
}
