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
};


template <typename Container>
typename Container::iterator PmergeMe::findInsertPosition(Container& c, int value, typename Container::iterator limit)
{
    typename Container::iterator left = c.begin();
    typename Container::iterator right = limit;

    auto dist = std::distance(left, right);

    if (dist == 0)
        return left;

    if (dist == 1)
    {
        if (value <= *left)
            return left;
        return left + 1;
    }

    if (dist == 2)
    {
        if (value <= *(left + 1))
            return (value <= *left ? left : left + 1);
        return left + 2;
    }

    while (left < right)
    {
        typename Container::iterator mid =
            left + (std::distance(left, right) / 2);

        if (value <= *mid)
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

    /* 1. Пары (a < b) */
    std::vector<Pair> pairs;
    bool has_straggler = (c.size() % 2 != 0);
    int straggler = 0;

    for (size_t i = 0; i + 1 < c.size(); i += 2)
    {
        if (c[i] < c[i + 1])
            pairs.push_back(Pair(c[i], c[i+1])); // a < b
        else
            pairs.push_back(Pair(c[i+1], c[i]));
    }

    if (has_straggler)
        straggler = c.back();

    Container main;
    for (size_t i = 0; i < pairs.size(); ++i)
        main.push_back(pairs[i].second);

    fordJohnson(main);

    for (size_t j = 0; j < pairs.size(); ++j)
    {
        if (main[0] == pairs[j].second)
        {
            main.insert(main.begin(), pairs[j].first);
            pairs.erase(pairs.begin() + j);
            break;
        }
    }

    std::vector<size_t> b_pos(pairs.size());

    for (size_t i = 0; i < main.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (main[i] == pairs[j].second)
            {
                b_pos[j] = i;
                break;
            }
        }
    }

    std::vector<size_t> order = buildJacobsthalOrder(pairs.size());

    for (size_t idx : order)
    {
        typename Container::iterator limit = main.begin() + b_pos[idx];
        typename Container::iterator pos = findInsertPosition(main, pairs[idx].first, limit);
        size_t ins_idx = std::distance(main.begin(), pos);

        main.insert(pos, pairs[idx].first);


        for (size_t j = 0; j < b_pos.size(); ++j)
        {
            if (b_pos[j] >= ins_idx)
                ++b_pos[j];
        }
    }


    if (has_straggler)
    {
        typename Container::iterator pos = findInsertPosition(main, straggler, main.end());
        main.insert(pos, straggler);
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

