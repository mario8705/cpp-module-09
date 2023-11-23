#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::InitializeDatabase(char const *path)
{
    std::ifstream in;
    std::string line;

    in.open(path);
    if (!in.is_open())
    {
        std::cerr << "Could not open database file " << path << std::endl;
        return false;
    }

    if (!std::getline(in, line))
    {
        std::cerr << "Invalid database header" << std::endl;
        return false;
    }

    while (std::getline(in, line))
    {
        
    }

    return true;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &)
{
    return *this;
}
