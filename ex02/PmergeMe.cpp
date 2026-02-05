#include "PmergeMe.hpp"

#include <iostream>
#include <stdexcept>
#include <climits>
#include <chrono>

PmergeMe::PmergeMe() : _elements_amount(0), _compare_count(0) {}

PmergeMe::PmergeMe(const PmergeMe &other)
    : _inputVector(other._inputVector),
      _inputDeque(other._inputDeque),
      _mainVector(other._mainVector),
      _mainDeque(other._mainDeque),
      _vector_time(other._vector_time),
      _deque_time(other._deque_time),
      _elements_amount(other._elements_amount),
      _compare_count(other._compare_count) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _inputVector = other._inputVector;
        _inputDeque  = other._inputDeque;
        _mainVector  = other._mainVector;
        _mainDeque   = other._mainDeque;
        _vector_time = other._vector_time;
        _deque_time  = other._deque_time;
        _elements_amount = other._elements_amount;
        _compare_count = other._compare_count;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::sort(int argc, char **argv)
{
    parseInput(argc, argv);

    fordJohnsonVector();
    // fordJohnsonDeque();

    print();
}

void PmergeMe::parseInput(int argc, char **argv)
{
    _inputVector.clear();
    _inputDeque.clear();

    for (int i = 1; i < argc; ++i)
    {
        validateNumber(argv[i]);

        long value = std::strtol(argv[i], NULL, 10);
        if (value < 0 || value > INT_MAX)
            throw std::runtime_error("Error");

        _inputVector.push_back(static_cast<int>(value));
        _inputDeque.push_back(static_cast<int>(value));
    }

    _elements_amount = argc - 1;
}

std::vector<size_t> PmergeMe::buildJacobsthalOrder(size_t n) const
{
    std::vector<size_t> jacob;
    std::vector<size_t> order;

    jacob.push_back(0);
    jacob.push_back(1);

    while (jacob.back() < n)
    {
        size_t size = jacob.size();
        jacob.push_back(jacob[size - 1] + 2 * jacob[size - 2]);
    }

    size_t prev = 1;
    for (size_t i = 1; i < jacob.size(); ++i)
    {
        size_t curr = jacob[i];
        if (curr > n)
            curr = n;

        for (size_t j = curr; j > prev; --j)
            order.push_back(j - 1);

        prev = curr;
    }

    return order;
}


void PmergeMe::validateNumber(const std::string &s) const
{
    if (s.empty())
        throw std::runtime_error("Error");

    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!std::isdigit(s[i]))
            throw std::runtime_error("Error");
    }
}

void PmergeMe::fordJohnsonVector()
{
     _compare_count = 0;
    _mainVector = _inputVector; 

    auto start = std::chrono::high_resolution_clock::now();

    fordJohnson(_mainVector);

    auto end = std::chrono::high_resolution_clock::now();
    _vector_time = std::chrono::duration<double, std::micro>(end - start).count();
}

void PmergeMe::fordJohnsonDeque()
{
     _compare_count = 0;
    _mainDeque = _inputDeque;

    auto start = std::chrono::high_resolution_clock::now();

    fordJohnson(_mainDeque);

    auto end = std::chrono::high_resolution_clock::now();
    _deque_time = std::chrono::duration<double, std::micro>(end - start).count();
}

void PmergeMe::print() const
{
    std::cout << "Before:";
    for (size_t i = 0; i < _inputVector.size(); ++i)
        std::cout << " " << _inputVector[i];
    std::cout << std::endl;

    std::cout << "After:";
    for (size_t i = 0; i < _mainVector.size(); ++i)
        std::cout << " " << _mainVector[i];
    std::cout << std::endl;

    std::cout << "Time to process a range of "
              << _elements_amount
              << " elements with std::vector : "
              << _vector_time << " us" << " with "
              << _compare_count << " comparisons. Sorted: " 
              << isSorted(_mainVector)
              << " with size " << _mainVector.size() << std::endl;

    std::cout << "Time to process a range of "
              << _elements_amount
              << " elements with std::deque  : "
              << _deque_time << " us" << " with "
              << _compare_count << " comparisons. Sorted: " 
              << isSorted(_mainDeque)
              << " with size " << _mainDeque.size() << std::endl;
}
