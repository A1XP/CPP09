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
    void recursiveFordJohnson(Container& c);

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
void PmergeMe::recursiveFordJohnson(Container& c)
{
    if (c.size() <= 1)
        return;

    typedef typename Container::value_type T;
    typedef std::pair<T, T> PairT;

    std::vector<PairT> pairs;
    pairs.reserve(c.size() / 2);

    bool hasStraggler = (c.size() % 2 != 0);
    T straggler = T();

    size_t i = 0;
    while (i + 1 < c.size())
    {
        T a = c[i];
        T b = c[i + 1];

        if (a < b)
            pairs.push_back(PairT(a, b));
        else
            pairs.push_back(PairT(b, a));

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
        T small = pairs[idx].first;
        T big   = pairs[idx].second;

        typename Container::iterator limit =
            std::lower_bound(main.begin(), main.end(), big);

        typename Container::iterator pos =
            std::lower_bound(main.begin(), limit, small);

        main.insert(pos, small);
    }

    if (hasStraggler)
    {
        main.insert(
            std::lower_bound(main.begin(), main.end(), straggler),
            straggler);
    }

    c = main;
}


#endif

