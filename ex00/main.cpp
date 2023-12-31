#include <cstdlib>
#include <iostream>
#include "BitcoinExchange.hpp"

int main(int argc, char *argv[])
{
    BitcoinExchange btc;

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    if (!btc.InitializeDatabase("data.csv"))
    {
        return 1;
    }

    if (!btc.ProcessInputFile(argv[1]))
    {
        return 1;
    }

    return 0;
}
