#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <utility>
#include <cstddef>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void sort(int argc, char **argv);

private:

    typedef std::pair<int, int> Pair;

    void parseInput(int argc, char **argv);
    void validateNumber(const std::string &s) const;

    void fordJohnsonVector();
    void fordJohnsonDeque();

    std::vector<size_t> buildJacobsthalOrder(size_t n) const;

    void print() const;

    template <typename Container>
    typename Container::iterator findInsertPosition(Container& c, int value, typename Container::iterator limit);

    template <typename Container>
    void recursiveFordJohnson(Container& c);

private:
    std::vector<int> _inputVector;
    std::deque<int>  _inputDeque;

    std::vector<int> _mainVector;
    std::deque<int>  _mainDeque;

    double _vector_time;
    double _deque_time;

    size_t _elements_amount;

    size_t _compare_count;
};

template <typename Container>
typename Container::iterator PmergeMe::findInsertPosition(Container& c, int value, typename Container::iterator limit)
{
    typename Container::iterator left = c.begin();
    typename Container::iterator right = limit;

    while (left < right)
    {
        ++_compare_count;
        typename Container::iterator mid = left + (std::distance(left, right) / 2);

        if (value <= *mid)
            right = mid;
        else
            left = mid + 1;
    }

    return left; 
}

template <typename Container>
void PmergeMe::recursiveFordJohnson(Container& c)
{
    if (c.size() <= 1)
        return;

    std::vector<Pair> pairs;
    pairs.reserve(c.size() / 2);

    bool hasStraggler = (c.size() % 2 != 0);
    int straggler = -1;

    size_t i = 0;
    while (i + 1 < c.size())
    {
        int a = c[i];
        int b = c[i + 1];
        
        ++_compare_count;

        if (a < b)
            pairs.push_back(Pair(a, b));
        else
            pairs.push_back(Pair(b, a));

        i += 2;
    }

    if (hasStraggler)
        straggler = c.back();

    Container main;
    for (size_t j = 0; j < pairs.size(); ++j)
        main.push_back(pairs[j].second);

    recursiveFordJohnson(main);

    std::vector<size_t> order = buildJacobsthalOrder(pairs.size());

    for (size_t k = 0; k < order.size(); ++k)
    {
        size_t idx = order[k];
        int small = pairs[idx].first;
        int big   = pairs[idx].second;

        typename Container::iterator limit = findInsertPosition(main, big, main.end());

        typename Container::iterator pos   = findInsertPosition(main, small, limit);

        main.insert(pos, small);
    }

    if (hasStraggler)
    {
        main.insert(findInsertPosition(main, straggler, main.end()), straggler);
    }

    c = main;
}

#endif

