#include "RPN.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN& other) : st(other.st) {}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
        st = other.st;
    return *this;
}

RPN::~RPN() {}

bool RPN::isNumber(const std::string& token) const
{
    try
    {
        int n = std::stoi(token);
        return n >= 0 && n <= 9;
    }
    catch (...)
    {
        return false;
    }
}

bool RPN::isOperator(const std::string& token) const
{
    return token == "+" || token == "-" || token == "*" || token == "/";
}

void RPN::applyOperator(const std::string& op)
{
    if (st.size() < 2)
        throw std::runtime_error("Error: insufficient values for operation");

    int b = st.top(); st.pop();
    int a = st.top(); st.pop();

    if (op == "+") st.push(a + b);
    else if (op == "-") st.push(a - b);
    else if (op == "*") st.push(a * b);
    else if (op == "/")
    {
        if (b == 0)
            throw std::runtime_error("Error: division by zero");
        st.push(a / b);
    }
}

int RPN::evaluate(const std::string& expression)
{
    std::istringstream ss(expression);
    std::string token;

    while (!st.empty()) st.pop();

    while (ss >> token)
    {
        if (isNumber(token))
            st.push(std::stoi(token));
        else if (isOperator(token))
            applyOperator(token);
        else
            throw std::runtime_error("Error: invalid token: " + token);
    }

    if (st.size() != 1)
        throw std::runtime_error("Error: invalid expression");

    return st.top();
}
