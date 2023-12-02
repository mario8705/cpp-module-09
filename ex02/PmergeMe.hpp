#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <vector>
#include <deque>
#include <ostream>

class PmergeMe {
public:
    PmergeMe();
    ~PmergeMe();

    bool LoadSequence(int count, char *sequence[]);
    void PrintSequence(std::ostream &out, const std::string &prefix) const;

    void Sort(double &listTime, double &queueTime);

    inline std::vector<int>::size_type SeqLen() const { return m_vector.size(); }

private:
    void SortVector();
    void SortQueue();

    std::vector<int> m_vector;
    std::deque<int> m_queue;
};

#endif // PMERGEME_HPP
