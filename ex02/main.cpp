#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    PmergeMe pmergeMe;
    double a, b;

    if (argc < 2)
        return 0;
    if (!pmergeMe.LoadSequence(argc - 1, argv + 1))
        return 1;
    pmergeMe.PrintSequence(std::cout, "Before:  ");
    pmergeMe.Sort(a, b);
    pmergeMe.PrintSequence(std::cout, "After:   ");
    std::cout << "Time to process a range of " << pmergeMe.SeqLen() << " element(s) with std::vector : " << (long long)a << " us" << std::endl;
    std::cout << "Time to process a range of " << pmergeMe.SeqLen() << " element(s) with std::dequee : " << (long long)b << " us" << std::endl;
    return 0;
}
