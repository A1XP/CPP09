#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
private:
    std::map<std::string, double> data;

    void fillData(const std::string& dataFile);
    void fillLine(const std::string& line);
    bool checkDate(const std::string& date) const;
    bool checkRate(const std::string& rate, bool is_input) const;

public:
    BitcoinExchange(const std::string& dataFile = "data.csv");
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();
    
    void processFile(const std::string& inputFile) const;

};

#endif