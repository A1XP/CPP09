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

    size_t jacobsthal(size_t n);
    std::vector<size_t> jacobsthalRanges(size_t n);

    void print() const;

    template <typename Container>
    typename Container::iterator findInsertPosition(Container& c, int value, typename Container::iterator limit);

    template <typename Container>
    void fordJohnson(Container& c);


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

    /* 1. Pairing (a > b) */
    std::vector<Pair> pairs;
    pairs.reserve(c.size() / 2);

    bool has_straggler = (c.size() % 2 != 0);
    int  straggler = 0;

    for (size_t i = 0; i + 1 < c.size(); i += 2)
    {
        ++_compare_count;
        if (c[i] > c[i + 1])
            pairs.push_back(Pair(c[i], c[i + 1])); // a > b
        else
            pairs.push_back(Pair(c[i + 1], c[i]));
    }

    if (has_straggler)
        straggler = c.back();

    /* 2. main-chain = all aᵢ (bigger ones) */
    Container main;
    for (size_t i = 0; i < pairs.size(); ++i)
        main.push_back(pairs[i].first);

    /* 3. Recursively sort main */
    fordJohnson(main);

    /* 4. Create index of aᵢ */
    std::vector<size_t> a_pos(main.size());
    for (size_t i = 0; i < a_pos.size(); ++i)
        a_pos[i] = i;

    /* 5. Insert b₀ */
    main.insert(main.begin(), pairs[0].second);
    for (size_t i = 0; i < a_pos.size(); ++i)
        ++a_pos[i];

    /* 6. Jacobsthal insertion of b₁..bₙ */
    std::vector<size_t> J = jacobsthalRanges(pairs.size());

    size_t prev = 1;
    for (size_t r = 0; r < J.size(); ++r)
    {
        size_t curr = J[r];
        for (size_t i = curr; i > prev; --i)
        {
            size_t idx = i - 1;

            size_t limit_idx = a_pos[idx];
            typename Container::iterator limit =
                main.begin() + limit_idx;

            typename Container::iterator pos =
                findInsertPosition(main, pairs[idx].second, limit);

            size_t inserted_idx = std::distance(main.begin(), pos);
            main.insert(pos, pairs[idx].second);

            /* update a_pos */
            for (size_t j = idx; j < a_pos.size(); ++j)
            {
                if (a_pos[j] >= inserted_idx)
                    ++a_pos[j];
            }
        }
        prev = curr;
    }

    /* 7. Remaining bᵢ in reverse order */
    for (size_t i = pairs.size(); i > prev; --i)
    {
        size_t idx = i - 1;

        size_t limit_idx = a_pos[idx];
        typename Container::iterator limit =
            main.begin() + limit_idx;

        typename Container::iterator pos =
            findInsertPosition(main, pairs[idx].second, limit);

        size_t inserted_idx = std::distance(main.begin(), pos);
        main.insert(pos, pairs[idx].second);

        /* update a_pos */
        for (size_t j = idx; j < a_pos.size(); ++j)
        {
            if (a_pos[j] >= inserted_idx)
                ++a_pos[j];
        }
    }

    /* 8. Insert straggler */
    if (has_straggler)
    {
        typename Container::iterator pos =
            findInsertPosition(main, straggler, main.end());
        main.insert(pos, straggler);
    }

    c = main;
}

#endif

