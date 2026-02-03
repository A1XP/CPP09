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

    // template <typename Container>
    // typename Container::iterator findInsertPosition(Container& c, int value, typename Container::iterator limit);

    // template <typename Container>
    // void recursiveFordJohnson(Container& c);
    template <typename Container>
    typename Container::iterator boundedInsert(Container& c, int value, typename Container::iterator limit);

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

// template <typename Container>
// typename Container::iterator PmergeMe::findInsertPosition(Container& c, int value, typename Container::iterator limit)
// {
//     typename Container::iterator left = c.begin();
//     typename Container::iterator right = limit;

//     while (left < right)
//     {
//         ++_compare_count;
//         typename Container::iterator mid = left + (std::distance(left, right) / 2);

//         if (value <= *mid)
//             right = mid;
//         else
//             left = mid + 1;
//     }

//     return left; 
// }

// template <typename Container>
// void PmergeMe::recursiveFordJohnson(Container& c)
// {
//     if (c.size() <= 1)
//         return;

//     std::vector<Pair> pairs;
//     pairs.reserve(c.size() / 2);

//     bool hasStraggler = (c.size() % 2 != 0);
//     int straggler = -1;

//     size_t i = 0;
//     while (i + 1 < c.size())
//     {
//         int a = c[i];
//         int b = c[i + 1];
        
//         ++_compare_count;

//         if (a < b)
//             pairs.push_back(Pair(a, b));
//         else
//             pairs.push_back(Pair(b, a));

//         i += 2;
//     }

//     if (hasStraggler)
//         straggler = c.back();

//     Container main;
//     for (size_t j = 0; j < pairs.size(); ++j)
//         main.push_back(pairs[j].second);

//     recursiveFordJohnson(main);

//     std::vector<size_t> order = buildJacobsthalOrder(pairs.size());

//     for (size_t k = 0; k < order.size(); ++k)
//     {
//         size_t idx = order[k];

//         int small = pairs[idx].first;
//         int big   = pairs[idx].second;

//         typename Container::iterator limit = findInsertPosition(main, big, main.end());

//         typename Container::iterator pos   = findInsertPosition(main, small, limit);

//         main.insert(pos, small);
//     }

//     if (hasStraggler)
//     {
//         main.insert(findInsertPosition(main, straggler, main.end()), straggler);
//     }

//     c = main;
// }

static size_t jacobsthal(size_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    size_t a = 0, b = 1, c;
    for (size_t i = 2; i <= n; ++i)
    {
        c = b + 2 * a;
        a = b;
        b = c;
    }
    return b;
}

static std::vector<size_t> jacobsthalRanges(size_t n)
{
    std::vector<size_t> ranges;
    for (size_t k = 2;; ++k)
    {
        size_t j = jacobsthal(k);
        if (j >= n)
            break;
        ranges.push_back(j);
    }
    return ranges;
}

template <typename Container>
typename Container::iterator PmergeMe::boundedInsert(Container& c, int value, typename Container::iterator limit)
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

    /* 1. Pairing */
    std::vector<Pair> pairs;
    pairs.reserve(c.size() / 2);

    bool has_straggler = (c.size() % 2);
    int straggler = 0;

    for (size_t i = 0; i + 1 < c.size(); i += 2)
    {
        ++_compare_count;
        if (c[i] < c[i+1])
            pairs.push_back(Pair(c[i], c[i+1]));
        else
            pairs.push_back(Pair(c[i+1], c[i]));
    }

    if (has_straggler)
        straggler = c.back();

    /* 2. main = all bᵢ */
    Container main;
    std::vector<typename Container::iterator> b_pos;

    for (size_t i = 0; i < pairs.size(); ++i)
        main.push_back(pairs[i].second);

    /* 3. Recursively sort main */
    fordJohnson(main);

    /* 4. Cache positions of bᵢ */
    for (typename Container::iterator it = main.begin();
            it != main.end(); ++it)
        b_pos.push_back(it);

    /* 5. Insert a₀ */
    typename Container::iterator pos_a0 =
        boundedInsert(main, pairs[0].first, b_pos[0]);
    main.insert(pos_a0, pairs[0].first);

    /* 6. Jacobsthal insertion of a₁..aₙ */
    std::vector<size_t> J = jacobsthalRanges(pairs.size());

    size_t prev = 1;
    for (size_t r = 0; r < J.size(); ++r)
    {
        size_t curr = J[r];
        for (size_t i = curr; i > prev; --i)
        {
            size_t idx = i - 1;
            typename Container::iterator limit = b_pos[idx];
            typename Container::iterator pos =
                boundedInsert(main, pairs[idx].first, limit);
            main.insert(pos, pairs[idx].first);
        }
        prev = curr;
    }

    /* 7. Remaining aᵢ in reverse order */
    for (size_t i = pairs.size(); i > prev; --i)
    {
        size_t idx = i - 1;
        typename Container::iterator limit = b_pos[idx];
        typename Container::iterator pos =
            boundedInsert(main, pairs[idx].first, limit);
        main.insert(pos, pairs[idx].first);
    }

    /* 8. Insert straggler */
    if (has_straggler)
    {
        typename Container::iterator pos =
            boundedInsert(main, straggler, main.end());
        main.insert(pos, straggler);
    }

    c = main;
}

#endif

