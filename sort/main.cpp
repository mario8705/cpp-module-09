#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <iterator>
#include <type_traits>
#include <vector>
#include <iostream>

// Cache all the differences between a Jacobsthal number and its
// predecessor that fit in 64 bits, starting with the difference
// between the Jacobsthal numbers 4 and 3 (the previous ones are
// unneeded)
static const std::uint_fast64_t jacobsthal_diff[] = {
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
        uint_fast64_t dist = jacobsthal_diff[k];
        if (dist > static_cast<uint_fast64_t>(std::distance(current_pend, pend.end())))
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

    // If there are pend elements left, insert them into
    // the main chain, the order of insertion does not
    // matter so forward traversal is ok
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

template <typename container>
void merge(container& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp arrays
    container L(n1), R(n2);

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

template <typename container>
void mergeSort(container& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

////////////////////////////////////////////////////////////
// Merge-insertion sort
template<typename container_type>
void merge_insertion_sort_impl(container_type &c)
{
    typedef typename container_type::iterator RandomAccessIterator;

    std::vector<std::pair<int, int> > pairs;

    size_t size = c.size();
    RandomAccessIterator first = c.begin(), last = c.end();

    if (size < 1)
        return ;
    if (size == 2)
    {
        if (first[0] > first[1])
            std::iter_swap(first + 0, first + 1);
        return ;
    }

    // Whether there is a stray element not in a pair
    // at the end of the chain
    bool has_stray = (size % 2 != 0);

    /**
     * Inner sort of the pairs
     */
    RandomAccessIterator end = has_stray ? (last - 1) : last;
    for (RandomAccessIterator it = first ; it != end ; it += 2)
    {
        if (it[1] < it[0])
        {
            pairs.push_back(std::make_pair(it[1], it[0]));
            // std::iter_swap(it, it + 1);
        }
        else
        {
            pairs.push_back(std::make_pair(it[0], it[1]));
        }
    }

    /**
     * Recursively sort the pairs by max
     */
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
    mergeSort(pairs, 0, pairs.size() - 1);

    RandomAccessIterator pairsInsertBack = first;

    for (std::vector<std::pair<int, int> >::iterator it = pairs.begin();
        it != pairs.end();
        ++it)
    {
        *pairsInsertBack++ = it->first;
        *pairsInsertBack++ = it->second;
    }


    ////////////////////////////////////////////////////////////
    // Separate main chain and pend elements

    typedef std::list<RandomAccessIterator> chain_t;

    // The first pend element is always part of the main chain,
    // so we can safely initialize the list with the first two
    // elements of the sequence
    chain_t chain;
    chain.push_back(first);
    chain.push_back(first + 1);

    // Upper bounds for the insertion of pend elements
    std::vector<typename chain_t::iterator> pend;
    // pend.reserve((size + 1) / 2 - 1);

    // Push every lower bound of the pairs into the pend list
    for (RandomAccessIterator it = first + 2; it != end; it += 2)
    {
        pend.push_back(chain.insert(chain.end(), it + 1));
    }

    // Add the last element to pend if it exists; when it
    // exists, it always has to be inserted in the full chain,
    // so giving it chain.end() as end insertion point is ok
    if (has_stray)
    {
        pend.push_back(chain.end());
    }

    ////////////////////////////////////////////////////////////
    // Binary insertion into the main chain
    insert_pend(first, pend, chain);

    ////////////////////////////////////////////////////////////
    // Move values in order to a cache then back to origin

    std::vector<int> cache(size);
    std::transform(chain.begin(), chain.end(), cache.begin(), iter_value<typename std::vector<int>::iterator>());
    std::copy(cache.begin(), cache.end(), first.base());
}

int main()
{
    const int kSeqLen = 4096;

    std::vector<int> v(kSeqLen);
    srand(time(NULL));
    
    bool sorted = true;
    for (std::vector<int>::iterator it = std::begin(v); it != std::end(v); ++it)
    {
        *it = rand() % 10000;
    }

    merge_insertion_sort_impl(v);
    // merge_insertion_sort_impl(v.begin(), v.end());

    // std::cout << v[0];
    for (size_t i = 1; i < v.size(); ++i)
    {
        sorted = sorted && v[i - 1] <= v[i];
        // std::cout << " " << v[i];
        if (!sorted)
            break ;
    }
    // std::cout << std::endl;

    if (!sorted)
    {
        std::cout << "Sequence is not sorted" << std::endl;
    }
    else
    {
        std::cout << "SEQUENCE SORTED" << std::endl;
    }
}