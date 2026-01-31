#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
private:
    std::stack<int> st;

    bool isNumber(const std::string& token) const;
    bool isOperator(const std::string& token) const;
    void applyOperator(const std::string& op);

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    int evaluate(const std::string& expression);
};

#endif
