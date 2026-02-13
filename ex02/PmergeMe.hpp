#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <utility>
#include <cstddef>
#include <iostream>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void sort(int argc, char **argv);
    void printbuildJacobsthalOrder(size_t n) const;

private:

    typedef std::pair<int, int> Pair;

    void parseInput(int argc, char **argv);
    void validateNumber(const std::string &s) const;

    std::vector<size_t> buildJacobsthalOrder(size_t n) const;

    void fordJohnsonVector();
    void fordJohnsonDeque();

    void print() const;

    template <typename Container>
    typename Container::iterator findInsertPosition(Container& c, int value, typename Container::iterator limit);

    template <typename Container>
    void fordJohnson(Container& c);

    template <typename Container>
    bool isSorted(const Container &c) const;

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
        typename Container::iterator mid =
            left + (std::distance(left, right) / 2);

        if (value < *mid)
            right = mid;
        else
            left = mid + 1;
    }

    return left;
}

template <typename Container>
void PmergeMe::fordJohnson(Container& c)
{
    if (c.size() <= 1)
        return;

    std::vector<Pair> pairs;
    bool has_straggler = (c.size() % 2 != 0);
    int straggler = 0;

    for (size_t i = 0; i + 1 < c.size(); i += 2)
    {
        ++_compare_count;
        if (c[i] > c[i + 1])
            pairs.push_back(Pair(c[i], c[i+1]));
        else
            pairs.push_back(Pair(c[i+1], c[i]));
    }

    if (has_straggler)
        straggler = c.back();

    Container main;
    for (size_t i = 0; i < pairs.size(); ++i)
        main.push_back(pairs[i].first);

    fordJohnson(main);

    std::vector<Pair> sortedPairs;
    sortedPairs.reserve(pairs.size());

    std::vector<bool> used(pairs.size(), false);

    for (size_t i = 0; i < main.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (!used[j] && pairs[j].first == main[i])
            {
                sortedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    pairs = sortedPairs;

    std::vector<size_t> a_pos(pairs.size());

    for (size_t i = 0; i < pairs.size(); ++i)
        a_pos[i] = i;

    size_t pending_count = pairs.size() + (has_straggler ? 1 : 0);

    std::vector<size_t> order = buildJacobsthalOrder(pending_count);

    for (size_t idx : order)
    {
        if (idx == 0)
        {
            main.insert(main.begin(), pairs[idx].second);
            for (size_t j = 0; j < a_pos.size(); ++j)
                    ++a_pos[j];
            continue;
        }
        typename Container::iterator limit;
        int value;

        if (idx < pairs.size())
        {
            limit = main.begin() + a_pos[idx];
            value = pairs[idx].second;
        }
        else
        {
            limit = main.end();
            value = straggler;
        }

        typename Container::iterator pos = findInsertPosition(main, value, limit);
        size_t ins_idx = std::distance(main.begin(), pos);

        main.insert(pos, value);

        for (size_t j = 0; j < a_pos.size(); ++j)
        {
            if (a_pos[j] >= ins_idx)
                ++a_pos[j];
        }
    }

    c = main;
}

template <typename Container>
bool PmergeMe::isSorted(const Container &c) const
{
    if (c.size() < 2)
        return true;

    typename Container::const_iterator it = c.begin();
    typename Container::const_iterator next = it;
    ++next;

    for (; next != c.end(); ++it, ++next)
    {
        if (*next < *it)
            return false;
    }
    return true;
}

#endif