#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

static bool validate_and_parse_number(const std::string &s, double &n)
{
    n = std::atof(s.c_str());
    if (s.length() > 10 || n > 1000)
    {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }
    if (n < 0)
    {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    return true;
}

static bool validate_date(const std::string &s)
{
    int i;
    int month, day, year;
    bool leap;

    if (s.length() != 10)
    {
        return false;
    }
    for (i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7)
        {
            if (s[i] != '-')
                return false;
        }
        else if (!std::isdigit(s[i]))
        {
            return false;
        }
    }
    year = std::atoi(s.substr(0, 4).c_str());
    day = std::atoi(s.substr(8, 2).c_str());
    month = std::atoi(s.substr(5, 2).c_str());
    leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    if (month == 1 || month == 3 || month == 5 || month == 7 ||
        month == 8 || month == 10 || month == 12)
    {
        if (day < 1 || day > 31)
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day < 1 || day > 30)
            return false;
    }
    else if (month == 2)
    {
        if (leap && (day < 1 || day > 29))
            return false;
        else if (!leap && (day < 1 || day > 28))
            return false;
    }
    else
    {
        return false;
    }
    return true;
}

static void trim(std::string &s)
{
    std::string::iterator it = s.begin();
    while (it != s.end() && std::isspace(*it))
    {
        ++it;
    }
    s.erase(s.begin(), it);

    std::string::reverse_iterator rit = s.rbegin();
    while (rit != s.rend() && std::isspace(*rit))
    {
        ++rit;
    }
    s.erase(rit.base(), s.end());
}

bool BitcoinExchange::InitializeDatabase(char const *path)
{
    std::ifstream in;
    std::string line;
    int linenum;

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

    linenum = 1;
    std::string date, price;
    while (std::getline(in, line))
    {
        std::stringstream ss;
        ss << line;

        if (!std::getline(ss, date, ',') || !std::getline(ss, price, ','))
        {
            std::cerr << "Invalid database entry at line " << linenum << ":" << line << std::endl;
            return false;
        }
        m_database.insert(std::make_pair(date, std::atof(price.c_str())));
        ++linenum;
    }
    return true;
}

bool BitcoinExchange::ProcessInputFile(const char *path)
{
    std::ifstream in(path);
    std::string line;

    if (!in.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return false;
    }
    if (!std::getline(in, line))
    {
        std::cerr << "Invalid input file header" << std::endl;
        return false;
    }
    while (std::getline(in, line))
    {
        std::stringstream ss;
        ss << line;

        std::string date, price;
        if (!std::getline(ss, date, '|') || !std::getline(ss, price, '|'))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue ;
        }
        trim(date);
        trim(price);

        if (!validate_date(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue ;
        }

        double pricen;
        if (!validate_and_parse_number(price, pricen))
        {
            continue ;
        }

        PrintExchangeRate(date, pricen);
    }
    return true;
}

void BitcoinExchange::PrintExchangeRate(const std::string &date, double price) const
{
    std::map<std::string, double>::const_iterator it = m_database.lower_bound(date);
    if (it == m_database.end())
    {
        std::cout << "No data available for " << date << std::endl;
        return;
    }
    std::cout << date << " => " << price << " => " << (it->second * price) << std::endl;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &)
{
    return *this;
}
