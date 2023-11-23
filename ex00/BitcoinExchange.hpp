#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP
# include <map>

class BitcoinExchange
{
public:
    BitcoinExchange();
    ~BitcoinExchange();

    bool InitializeDatabase(char const *path);

private:
    BitcoinExchange(const BitcoinExchange &);
    BitcoinExchange &operator=(const BitcoinExchange &);

    std::map<time_t, double> m_database;
};

#endif // BITCOIN_EXCHANGE_HPP
