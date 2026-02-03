#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <utility>   // std::pair
#include <cstddef>   // size_t

class PmergeMe
{
public:
    // Orthodox Canonical Form
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    // Main entry point

    void sort(int argc, char **argv);

private:
    // ===== Types =====

    typedef std::pair<int, int> Pair;

    // ===== Parsing =====
    void parseInput(int argc, char **argv);
    void validateNumber(const std::string &s) const;

    // ===== Ford–Johnson =====

    void fordJohnsonVector();
    void fordJohnsonDeque();
    void fordJohnsonVectorSTD20();

    // Step 1: create (small, big) pairs
    void makePairsVector();
    void makePairsDeque();

    // Step 2: sort "big" elements
    void sortBigVector();
    void sortBigDeque();

    // Step 3: insert "small" using Jacobsthal order
    void insertSmallsVector();
    void insertSmallsDeque();

    // ===== Jacobsthal =====

    std::vector<size_t> buildJacobsthalOrder(size_t n) const;

    // ===== Utilities =====

    size_t findBigPositionVector(int big) const;
    size_t findBigPositionDeque(int big) const;
    void print_input() const;
    void print() const;

private:
    // ===== Data =====

    std::vector<int> _inputVector;
    std::deque<int>  _inputDeque;

    std::vector<Pair> _pairsVector;
    std::deque<Pair>  _pairsDeque;

    std::vector<int> _mainVector;
    std::deque<int>  _mainDeque;

    double _vector_time;
    double _deque_time;
    size_t _elements_amount;
};

#endif
