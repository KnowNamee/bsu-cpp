//
// Created by Computer on 30.12.2019.
//

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <iterator>
#include <functional>

template<typename Iterator>
bool IsSorted(Iterator begin, Iterator end) {
    while (++begin != end) {
        if (!(*(--begin) < *(++begin))) {
            return false;
        }
    }
    return true;
}


template<typename Iterator>
Iterator MaxInRange(Iterator begin, Iterator end) {
    auto max = begin;
    while (++begin != end) {
        if (*max < *begin) {
            max = begin;
        }
    }
    return max;
}


template<typename Iterator>
typename std::iterator_traits<Iterator>::value_type
SumOfRange(Iterator begin, Iterator end) {
    using T = typename std::iterator_traits<Iterator>::value_type;
    T sum = T();
    while (begin != end) {
        sum += *(begin++);
    }
    return sum;
}


template<typename Iterator, typename Predicate>
int CountIf(Iterator begin, Iterator end, Predicate pred) {
    int count = 0;
    while (begin != end) {
        count += static_cast<bool>(pred(*(begin++)));
    }
    return count;
}


template<typename Iterator>
int CountIfNot(Iterator begin,
        Iterator end,
     bool(*pred)(const typename std::iterator_traits<Iterator>::value_type&)) {
    int count = 0;
    while (begin != end) {
        count += static_cast<bool>(!pred(*(begin++)));
    }
    return count;
}


template<typename Iterator>
Iterator FindIf(Iterator begin,
        Iterator end,
        std::function<bool(
           const typename std::iterator_traits<Iterator>::value_type&)> pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return begin;
        }
        begin++;
    }
    return end;
}

#endif  // ALGORITHMS_H_
