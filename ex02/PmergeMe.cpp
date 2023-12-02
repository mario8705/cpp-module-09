#include "PmergeMe.hpp"
#include <string>
#include <iostream>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <list>
#include <iterator>
#include <vector>

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}

bool PmergeMe::LoadSequence(int count, char *sequence[])
{
    int i;
    std::string s;
    long l;

    for (i = 0; i < count; ++i)
    {
        s = sequence[i];
        if (s.length() > 11)
        {
            std::cout << "Error: too large a number." << std::endl;
            return false;
        }
        l = std::atol(s.c_str());
        if (l < 0)
        {
            std::cout << "Error" << std::endl;
            return false;
        }
        if (l > 2147483647)
        {
            std::cout << "Error: too large a number." << std::endl;
            return false;
        }

        m_vector.push_back((int)l);
        m_queue.push_back((int)l);
    }
    return true;
}

void PmergeMe::PrintSequence(std::ostream &out, const std::string &prefix) const
{
    std::vector<int>::const_iterator it = m_vector.begin();

    out << prefix;
    while (it != m_vector.end())
    {
        out << *it;
        ++it;
        if (it != m_vector.end())
            out << " ";
    }
    out << std::endl;
}

void PmergeMe::Sort(double &vectorTime, double &queueTime)
{
    clock_t start;
    clock_t elapsed;

    start = clock();
    SortVector();
    elapsed = clock() - start;
    vectorTime = static_cast<double>(elapsed * 1000000) / CLOCKS_PER_SEC;

    start = clock();
    SortQueue();
    elapsed = clock() - start;
    queueTime = static_cast<double>(elapsed * 1000000) / CLOCKS_PER_SEC;
}

static const size_t jacobsthal_diff[] = {
    2u, 2u, 6u, 10u, 22u, 42u, 86u, 170u, 342u, 682u, 1366u,
    2730u, 5462u, 10922u, 21846u, 43690u, 87382u, 174762u, 349526u, 699050u,
    1398102u, 2796202u, 5592406u, 11184810u, 22369622u, 44739242u, 89478486u,
    178956970u, 357913942u, 715827882u, 1431655766u, 2863311530u, 5726623062u,
    11453246122u, 22906492246u, 45812984490u, 91625968982u, 183251937962u,
    366503875926u, 733007751850u, 1466015503702u, 2932031007402u, 5864062014806u,
    11728124029610u, 23456248059222u, 46912496118442u, 93824992236886u, 187649984473770u,
    375299968947542u, 750599937895082u, 1501199875790165u, 3002399751580331u,
    6004799503160661u, 12009599006321322u, 24019198012642644u, 48038396025285288u,  
    96076792050570576u, 192153584101141152u, 384307168202282304u, 768614336404564608u,
    1537228672809129216u, 3074457345618258432u, 6148914691236516864u
};

template <typename RandomAccessIterator>
RandomAccessIterator locate_insertion_point(RandomAccessIterator first, RandomAccessIterator last, int value)
{
    size_t len = std::distance(first, last);
    while (len != 0) {
        size_t half_len = len / 2;
        RandomAccessIterator mid = first;
        std::advance(mid, half_len);
        if (value < *(*mid))
            len = half_len;
        else {
            first = ++mid;
            len -= half_len + 1;
        }
    }
    return first;
}

template <typename RandomAccessIterator, typename T>
void insert_pend(RandomAccessIterator first, std::vector<typename T::iterator> &pend, T &chain)
{
    RandomAccessIterator current_it = first + 2;
    typename std::vector<typename T::iterator>::iterator current_pend = pend.begin();

    for (int k = 0; ; ++k)
    {
        // Find next index
        size_t dist = jacobsthal_diff[k];
        if (dist > static_cast<size_t>(std::distance(current_pend, pend.end())))
            break;

        RandomAccessIterator it = current_it + dist * 2;
        typename std::vector<typename T::iterator>::iterator pe = current_pend + dist;

        do
        {
            --pe;
            it -= 2;

            chain.insert(locate_insertion_point(chain.begin(), *pe, *it), it);
        } while (pe != current_pend);

        std::advance(current_it, dist * 2);
        std::advance(current_pend, dist);
    }

    // Insert remaining
    while (current_pend != pend.end())
    {
        chain.insert(locate_insertion_point(chain.begin(), *current_pend, *current_it), current_it);
        current_it += 2;
        ++current_pend;
    }
}

template <typename T>
struct iter_value
{
    typedef typename std::iterator_traits<T>::value_type return_type;

    return_type operator()(T it) const
    {
        return *it;
    }
};

template <typename container_type, typename CIterator>
container_type merge(CIterator left_begin, CIterator left_end, CIterator right_begin, CIterator right_end) {
    container_type result;
    
    CIterator left = left_begin;
    CIterator right = right_begin;

    while (left != left_end && right != right_end) {
        if (left[1] < right[1]) {
            result.push_back(left[0]);
            result.push_back(left[1]);
            std::advance(left, 2);
        } else {
            result.push_back(right[0]);
            result.push_back(right[1]);
            std::advance(right, 2);
        }
    }
    
    while (left != left_end) {
        result.push_back(*left);
        left++;
    }
    
    while (right != right_end) {
        result.push_back(*right);
        right++;
    }
    
    return result;
}

template <typename container_type>
container_type merge_sort(typename container_type::iterator begin, typename container_type::iterator end) {
    typedef typename container_type::iterator Iterator;
    size_t dist = std::distance(begin, end);
    
    if (dist <= 2) {
        return container_type(begin, end);
    }
    
    Iterator middle = begin;
    std::advance(middle, dist / 4 * 2);
    
    container_type left = merge_sort<container_type>(begin, middle);
    container_type right = merge_sort<container_type>(middle, end);
    return merge<container_type, Iterator>(left.begin(), left.end(), right.begin(), right.end());
}

////////////////////////////////////////////////////////////
// Merge-insertion sort
template<typename container_type>
void ford_johnson(container_type &c)
{
    typedef typename container_type::iterator RandomAccessIterator;

    size_t size = c.size();
    int stray;
    RandomAccessIterator first = c.begin(), last = c.end();

    if (size <= 1)
        return ;
    if (size == 2)
    {
        if (first[0] > first[1])
            std::iter_swap(first + 0, first + 1);
        return ;
    }

    bool has_stray = (size % 2 != 0);
    if (has_stray)
        stray = c.back();

    RandomAccessIterator end = has_stray ? (last - 1) : last;
    for (RandomAccessIterator it = first ; it != end ; it += 2)
    {
        if (it[1] < it[0])
            std::iter_swap(it, it + 1);
    }

    container_type ct = merge_sort<container_type>(c.begin(), end);
    if (has_stray)
        ct.push_back(stray);
    c.assign(ct.begin(), ct.end());

    // for (RandomAccessIterator it = first; it != end; std::advance(it, 2))
    // {
    //     for (RandomAccessIterator it2 = it + 2; it2 != end; std::advance(it2, 2))
    //     {
    //         if (it[1] > it2[1])
    //         {
    //             std::swap(it[0], it2[0]);
    //             std::swap(it[1], it2[1]);
    //         }
    //     }
    // }

    typedef std::list<RandomAccessIterator> chain_t;

    chain_t chain;
    chain.push_back(first);
    chain.push_back(first + 1);

    std::vector<typename chain_t::iterator> pend;

    for (RandomAccessIterator it = first + 2; it != end; it += 2)
    {
        pend.push_back(chain.insert(chain.end(), it + 1));
    }

    if (has_stray)
    {
        pend.push_back(chain.end());
    }

    insert_pend(first, pend, chain);

    std::vector<int> cache(size);
    std::transform(chain.begin(), chain.end(), cache.begin(), iter_value<RandomAccessIterator>());
    c.assign(cache.begin(), cache.end());
}

void PmergeMe::SortVector()
{
    ford_johnson(m_vector);
}

void PmergeMe::SortQueue()
{
   ford_johnson(m_queue);
}