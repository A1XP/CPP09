#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <codecvt>
#include <vector>
#include <deque>
#include <string>
#include <utility>
#include <cstddef>
#include <iostream>
#include <algorithm>

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
    typename Container::iterator findInsertPosition(Container& c, int value, typename Container::iterator limit, bool isStraggler);

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

// static size_t jacobsthal(size_t n)
// {
//     if (n == 0) return 0;
//     if (n == 1) return 1;
//     size_t a = 0, b = 1, c;
//     for (size_t i = 2; i <= n; ++i)
//     {
//         c = b + 2 * a;
//         a = b;
//         b = c;
//     }
//     return b;
// }

// static std::vector<size_t> jacobsthalRanges(size_t n)
// {
//     std::vector<size_t> ranges;
//     for (size_t k = 2;; ++k)
//     {
//         size_t j = jacobsthal(k);
//         if (j >= n)
//             break;
//         ranges.push_back(j);
//     }

//     std::cout << "Jacobsthal ranges for n=" << n << ": ";
//     for (auto r : ranges) std::cout << r << " ";
//     std::cout << std::endl;
//     return ranges;
// }

template <typename Container>
typename Container::iterator PmergeMe::findInsertPosition(Container& c, int value, typename Container::iterator limit, bool isStraggler)
{
    (void)isStraggler;
    typename Container::iterator left = c.begin();
    // if (!isStraggler)
    //     if (left != c.end())
    //         ++left;

    typename Container::iterator right = limit;

    auto dist = std::distance(left, right);

    if (dist == 0)
        return left;

    if (dist == 1)
    {
        ++_compare_count;
        if (value <= *left)
            return left;
        return left + 1;
    }

    if (dist == 2)
    {
        ++_compare_count;
        if (value <= *(left + 1))
            return (value <= *left ? left : left + 1);
        return left + 2;
    }

    int local_compare = 0;

    while (left < right)
    {
        ++_compare_count;
        ++local_compare;
        typename Container::iterator mid =
            left + (std::distance(left, right) / 2);

        if (value <= *mid)
            right = mid;
        else
            left = mid + 1;
    }

    std::cout << "For value: " << value << " we compared " << local_compare << " times, total compares: " << _compare_count << std::endl;

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
        ++_compare_count;
        std::cout << "Comparing " << c[i] << " and " << c[i + 1] << std::endl;
        if (c[i] < c[i + 1])
            pairs.push_back(Pair(c[i], c[i+1])); // a < b
        else
            pairs.push_back(Pair(c[i+1], c[i]));
    }

    if (has_straggler)
        straggler = c.back();

    /* 2. Сортируем большие элементы (bᵢ) рекурсивно */
    Container main;
    for (size_t i = 0; i < pairs.size(); ++i)
        main.push_back(pairs[i].second);

    fordJohnson(main);


    //add a0 element to the beginning of main

    for (size_t j = 0; j < pairs.size(); ++j)
    {
        if (main[0] == pairs[j].second)
        {
            main.insert(main.begin(), pairs[j].first);
            pairs.erase(pairs.begin() + j);
            break;
        }
    }

    /* 3. Строим карту позиций больших элементов bᵢ */

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

    // std::cout << "\nPairs before inserting:";
    // for (size_t i = 0; i < pairs.size(); ++i)
    //     {
    //         std::cout << pairs[i].first << "|" << pairs[i].second << " ";
    //     }
    // std::cout << std::endl;

    // std::cout << "\nMain before inserting:";
    // for (size_t i = 0; i < main.size(); ++i)
    //     {
    //         std::cout << main[i] << " ";
    //     }
    // std::cout << std::endl;
    
    // std::cout << "\nPositions of bᵢ before inserting:";
    // for (size_t i = 0; i < b_pos.size(); ++i)
    //     {
    //         std::cout << b_pos[i] << " ";
    //     }
    // std::cout << std::endl;
    
    /* 4. Определяем порядок вставки маленьких элементов aᵢ через Jacobsthal */
    std::vector<size_t> order = buildJacobsthalOrder(pairs.size());

    // std::cout << "Jacobsthal order for " << pairs.size() << " pairs: ";
    // for (size_t i = 0; i < order.size(); ++i)
    // {
    //     std::cout << order[i] << " ";
    // }
    // std::cout << std::endl;

    /* 5. Вставляем маленькие элементы aᵢ в правильном порядке */
    for (size_t idx : order)
    {
        typename Container::iterator limit = main.begin() + b_pos[idx];
        typename Container::iterator pos = findInsertPosition(main, pairs[idx].first, limit, false);
        size_t ins_idx = std::distance(main.begin(), pos);
        // if (idx == 0)
        //     std::cout << "Inserting a[" << idx << "] = " << pairs[idx].first << " at position " << std::distance(main.begin(), pos) <<  " and ins_index " << ins_idx << std::endl;
        main.insert(pos, pairs[idx].first);

        // обновляем позиции всех больших элементов после вставки
        for (size_t j = 0; j < b_pos.size(); ++j)
        {
            if (b_pos[j] >= ins_idx)
                ++b_pos[j];
        }
        // if (idx >= 0)
        // {
        //     std::cout << "Positions of bᵢ after inserting a[" << idx << "]: " << pairs[idx].first << std::endl;
        //     for (size_t i = 0; i < b_pos.size(); ++i)
        //         {
        //             std::cout << b_pos[i] << " ";
        //         }
        //     std::cout << std::endl;
        // }
    }

    // /* 6. Вставляем оставшиеся aᵢ, которые не попали в Jacobsthal порядок */
    // for (size_t i = 0; i < pairs.size(); ++i)
    // {
    //     if (std::find(order.begin(), order.end(), i) == order.end())
    //     {
    //         typename Container::iterator limit = main.begin() + b_pos[i];
    //         typename Container::iterator pos = findInsertPosition(main, pairs[i].first, limit);
    //         size_t ins_idx = std::distance(main.begin(), pos);
    //         main.insert(pos, pairs[i].first);

    //         for (size_t j = 0; j < b_pos.size(); ++j)
    //         {
    //             if (b_pos[j] >= ins_idx)
    //                 ++b_pos[j];
    //         }
    //     }
    // }

    /* 7. Вставляем оставшийся элемент, если он есть */
    if (has_straggler)
    {
        typename Container::iterator pos = findInsertPosition(main, straggler, main.end(), true);
        main.insert(pos, straggler);
    }

    // std::cout << "\nFinalmain :";
    // for (size_t i = 0; i < main.size(); ++i)
    //     {
    //         std::cout << main[i] << " ";
    //     }
    // std::cout << std::endl;
    

    /* 8. Копируем результат обратно */
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

