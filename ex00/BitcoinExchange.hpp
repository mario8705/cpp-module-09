#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP
# include <map>

class BitcoinExchange
{
public:
    BitcoinExchange();
    ~BitcoinExchange();

    bool InitializeDatabase(char const *path);
    bool ProcessInputFile(char const *path);
    
    void PrintExchangeRate(const std::string &date, double price) const;

private:
    BitcoinExchange(const BitcoinExchange &);
    BitcoinExchange &operator=(const BitcoinExchange &);

    std::map<std::string, double> m_database;
};

#endif // BITCOIN_EXCHANGE_HPP
