#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>

BitcoinExchange::BitcoinExchange(const std::string& dataFile)
{
    fillData(dataFile);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : data(other.data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        data = other.data;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::fillData(const std::string& dataFile)
{
    std::ifstream file(dataFile);
    if (!file.is_open())
        throw std::runtime_error("Could not open data file");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line == "date,exchange_rate")
            continue;
        fillLine(line);
    }
}

void BitcoinExchange::processFile(const std::string& inputFile) const
{
    std::ifstream file(inputFile);
    if (!file.is_open())
        throw std::runtime_error("Could not open input file");

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line == "date | value")
            continue;
        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos)
        {
            std::cout << "Invalid delimeter in line: " << line << std::endl;
            continue;   

        }
        std::string date = line.substr(0, pipePos - 1);
        std::string valueStr = line.substr(pipePos + 2);
        if (!checkDate(date))
        {
            std::cout << "Invalid date in input: " << date << std::endl;
            continue;
        }
        if (!checkRate(valueStr, true))
        {
            std::cout << "Invalid value in input: " << valueStr << std::endl;
            continue;
        }
        double value = std::stod(valueStr);
        std::map<std::string, double>::const_iterator it = data.lower_bound(date);
        if (it == data.end() || it->first != date)
        {
            if (it == data.begin())
            {
                std::cout << "No exchange rate available for date: " << date << std::endl;
                continue;
            }
            --it;
        }
        double rate = it->second;
        double result = value * rate;
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
}

void BitcoinExchange::fillLine(const std::string& line)
{
    size_t commaPos = line.find(',');
    if (commaPos == std::string::npos)
        throw std::runtime_error("Invalid data line: " + line);

    std::string date = line.substr(0, commaPos);
    if (!checkDate(date))
    {
        std::cerr << "Invalid date in data: " << date << std::endl;
        return;
    }
    std::string rate = line.substr(commaPos + 1);
    if (!checkRate(rate, false))
    {
        std::cerr << "Invalid rate in data: " << rate << std::endl;
        return;
    }
    data[date] = std::stod(rate);
}

bool BitcoinExchange::checkDate(const std::string& date) const
{
    try
    {
        if (date.length() != 10 || date[4] != '-' || date[7] != '-')
            return false;

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (month < 1 || month > 12 || day < 1 || day > 31)
            return false;

        static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (month == 2)
        {
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (isLeapYear && day > 29)
                return false;
            if (!isLeapYear && day > 28)
                return false;
        }
        else if (day > daysInMonth[month - 1])
        {
            return false;
        }

        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool BitcoinExchange::checkRate(const std::string& rate, bool is_input) const
{
    try
    {
        size_t pos;
        double value = std::stod(rate, &pos);

        if (pos != rate.size())
            return false;
        if (is_input)
            return value >= 0.0 && value <= 1000.0;
        else
            return value >= 0.0;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
