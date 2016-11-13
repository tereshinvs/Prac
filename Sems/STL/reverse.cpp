#include <iterator>
#include <algorithm>	

template <class BidirectionalIterator>
void myreverse(BidirectionalIterator first, BidirectionalIterator last) {
    while (first != last && first != --last) {
        std::iter_swap(first, last);
        ++first;
    }
}