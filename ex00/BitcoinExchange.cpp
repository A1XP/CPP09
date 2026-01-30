#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
    // Constructor implementation (e.g., load data from a file)
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : data(other.data)
{
    // Copy constructor implementation
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        data = other.data;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}