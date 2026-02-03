#include "PmergeMe.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <climits>
#include <ranges>
#include <chrono>

/* ========================= */
/* Orthodox Canonical Form   */
/* ========================= */

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    *this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _inputVector = other._inputVector;
        _inputDeque  = other._inputDeque;
        _pairsVector = other._pairsVector;
        _pairsDeque  = other._pairsDeque;
        _mainVector  = other._mainVector;
        _mainDeque   = other._mainDeque;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

/* ========================= */
/* Public interface          */
/* ========================= */

void PmergeMe::sort(int argc, char **argv)
{
    parseInput(argc, argv);
    fordJohnsonVector();
    fordJohnsonDeque();
    print();
}

/* ========================= */
/* Parsing                   */
/* ========================= */

void PmergeMe::parseInput(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        validateNumber(argv[i]);

        std::stringstream ss(argv[i]);
        long value;
        ss >> value;

        if (value < 0 || value > INT_MAX)
            throw std::runtime_error("Error");

        _inputVector.push_back(static_cast<int>(value));
        _inputDeque.push_back(static_cast<int>(value));
    }
    _elements_amount = argc - 1;
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

/* ========================= */
/* Ford–Johnson entry points */
/* ========================= */

void PmergeMe::fordJohnsonVector()
{
    auto start = std::chrono::high_resolution_clock::now();

    makePairsVector();
    sortBigVector();
    insertSmallsVector();

    auto end = std::chrono::high_resolution_clock::now();

    _vector_time = std::chrono::duration<double, std::micro>(end - start).count();
}

void PmergeMe::fordJohnsonDeque()
{
    auto start = std::chrono::high_resolution_clock::now();

    makePairsDeque();
    sortBigDeque();
    insertSmallsDeque();

    auto end = std::chrono::high_resolution_clock::now();

    _deque_time = std::chrono::duration<double, std::micro>(end - start).count();
}

/* ========================= */
/* Pair creation             */
/* ========================= */

void PmergeMe::makePairsVector()
{
    _pairsVector.clear();

    size_t i = 0;
    while (i + 1 < _inputVector.size())
    {
        int a = _inputVector[i];
        int b = _inputVector[i + 1];

        if (a < b)
            _pairsVector.push_back(Pair(a, b));
        else
            _pairsVector.push_back(Pair(b, a));

        i += 2;
    }

    // odd element
    if (i < _inputVector.size())
        _pairsVector.push_back(Pair(_inputVector[i], -1));
}

void PmergeMe::makePairsDeque()
{
    _pairsDeque.clear();

    size_t i = 0;
    while (i + 1 < _inputDeque.size())
    {
        int a = _inputDeque[i];
        int b = _inputDeque[i + 1];

        if (a < b)
            _pairsDeque.push_back(Pair(a, b));
        else
            _pairsDeque.push_back(Pair(b, a));

        i += 2;
    }

    if (i < _inputDeque.size())
        _pairsDeque.push_back(Pair(_inputDeque[i], -1));
}

/* ========================= */
/* Sort big elements         */
/* ========================= */

void PmergeMe::sortBigVector()
{
    _mainVector.clear();

    for (size_t i = 0; i < _pairsVector.size(); ++i)
    {
        if (_pairsVector[i].second != -1)
            _mainVector.push_back(_pairsVector[i].second);
    }

    std::sort(_mainVector.begin(), _mainVector.end());
}

void PmergeMe::sortBigDeque()
{
    _mainDeque.clear();

    for (size_t i = 0; i < _pairsDeque.size(); ++i)
    {
        if (_pairsDeque[i].second != -1)
            _mainDeque.push_back(_pairsDeque[i].second);
    }

    std::sort(_mainDeque.begin(), _mainDeque.end());
}

/* ========================= */
/* Jacobsthal order          */
/* ========================= */

std::vector<size_t> PmergeMe::buildJacobsthalOrder(size_t n) const
{
    std::vector<size_t> order;
    std::vector<size_t> jacob;

    // build Jacobsthal numbers
    jacob.push_back(0);
    jacob.push_back(1);

    while (jacob.back() < n)
    {
        size_t size = jacob.size();
        jacob.push_back(jacob[size - 1] + 2 * jacob[size - 2]);
    }

    size_t prev = 0;
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

/* ========================= */
/* Insert small elements     */
/* ========================= */

size_t PmergeMe::findBigPositionVector(int big) const
{
    return std::lower_bound(_mainVector.begin(), _mainVector.end(), big)
           - _mainVector.begin();
}

void PmergeMe::insertSmallsVector()
{
    std::vector<size_t> order = buildJacobsthalOrder(_pairsVector.size());

    for (size_t k = 0; k < order.size(); ++k)
    {
        size_t i = order[k];
        int small = _pairsVector[i].first;
        int big   = _pairsVector[i].second;

        if (big == -1)
        {
            _mainVector.insert(
                std::lower_bound(_mainVector.begin(), _mainVector.end(), small),
                small);
            continue;
        }

        size_t limit = findBigPositionVector(big);

        _mainVector.insert(
            std::lower_bound(_mainVector.begin(),
                             _mainVector.begin() + limit,
                             small),
            small);
    }
}

/* ========================= */
/* Deque version             */
/* ========================= */

size_t PmergeMe::findBigPositionDeque(int big) const
{
    return std::lower_bound(_mainDeque.begin(), _mainDeque.end(), big)
           - _mainDeque.begin();
}

void PmergeMe::insertSmallsDeque()
{
    std::vector<size_t> order = buildJacobsthalOrder(_pairsDeque.size());

    for (size_t k = 0; k < order.size(); ++k)
    {
        size_t i = order[k];
        int small = _pairsDeque[i].first;
        int big   = _pairsDeque[i].second;

        if (big == -1)
        {
            _mainDeque.insert(
                std::lower_bound(_mainDeque.begin(), _mainDeque.end(), small),
                small);
            continue;
        }

        size_t limit = findBigPositionDeque(big);

        _mainDeque.insert(
            std::lower_bound(_mainDeque.begin(),
                             _mainDeque.begin() + limit,
                             small),
            small);
    }
}

void PmergeMe::print() const
{
    std::cout << "Before:\n";
    for (size_t i = 0; i < _inputVector.size(); ++i)
        std::cout << " " << _inputVector[i];
    std::cout << std::endl;

    std::cout << "After:\n";
    for (size_t i = 0; i < _mainVector.size(); ++i)
        std::cout << " " << _mainVector[i];
    std::cout << std::endl;

	std::cout << "Time to process a range of " << _elements_amount << " elements with std::vector : " << _vector_time << " us" << std::endl;
	std::cout << "Time to process a range of " << _elements_amount << " elements with std::deque  : " << _deque_time << " us" << std::endl;
}
