#ifndef _tsp_utils_hpp
#define _tsp_utils_hpp

#include <type_traits>

#define rep(i, s, e) for (int i = (s); i < (e); ++i)
#define req(i, s, e) for (int i = (s); i <= (e); ++i)
#define per(i, s, e) for (int i = (s); i > (e); --i)
#define qer(i, s, e) for (int i = (s); i >= (e); --i)

namespace utils {

template <typename It> inline It next(It it, typename std::iterator_traits<It>::difference_type diff = 1) {
    while (diff--) ++it;
    return it;
}

template <typename It, typename T> inline void iota(It first, It last, T val) {
    for (; first != last; ++first, ++val) *first = val;
}

template <typename It> inline void copy(It first, It last, It dest) {
    for (; first != last; ++first, ++dest) *dest = *first;
}

template <typename It> inline It min_element(It first, It last) {
    It res = first;
    ++first;
    for (; first != last; ++first) {
        if (*first < *res) res = first;
    }
    return res;
}

template <typename It> inline It find(It first, It last, typename std::iterator_traits<It>::value_type val) {
    for (; first != last; ++first)
        if (*first == val) break;
    return first;
}

template <typename T> void swap(T& a, T& b) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template <typename It> It rotate(It first, It n_first, It last) {
    if (first == n_first) return last;
    if (n_first == last) return first;
    It read = n_first, write = first, next_read = first;
    while (read != last) {
        if (write == next_read) next_read = read;
        swap(*write++, *read++);
    }
    rotate(write, next_read, last);
    return write;
}

// only for random access iterator
template <typename It> void reverse(It first, It last) {
    if (first == last) return;
    for (--last; first < last; ++first, --last) swap(*first, *last);
}

template <typename It> bool next_permutation(It first, It last) {
    if (first == last) return false;
    It i = last;
    if (first == --i) return false;
    for (;;) {
        It i1, i2;
        i1 = i;
        if (*--i < *i1) {
            i2 = last;
            while (!(*i < *--i2))
                ;
            swap(*i, *i2);
            reverse(i1, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

}  // namespace utils

#endif