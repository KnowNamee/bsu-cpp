//
// Created by Computer on 30.12.2019.
//

#ifndef SET_H_
#define SET_H_

#include <iostream>
#include "binary_search_tree.h"



template<typename T>
class Multiset : public BinarySearchTree<T> {
 public:
    using value_type = T;

    Multiset() : BinarySearchTree<T>() {}
    Multiset(const std::initializer_list<T>& init_list) :
                                    BinarySearchTree<T>(init_list) {}

    template<typename IteratorType>
    Multiset(IteratorType begin, IteratorType end);

    bool Includes(const Multiset<T>& other) const;
    Multiset<T> Union(const Multiset<T>& other) const;
    Multiset<T> Difference(const Multiset<T>& other) const;
    Multiset<T> Intersection(const Multiset<T>& other) const;
    Multiset<T> SymmetricDifference(const Multiset<T>& other) const;

    void push_back(const T& value);
};

template<typename T>
template<typename IteratorType>
Multiset<T>::Multiset(IteratorType begin, IteratorType end) {
    while (begin != end) {
        this->insert(*begin);
        begin++;
    }
}

template<typename T>
bool Multiset<T>::Includes(const Multiset<T> &other) const {
    return std::includes(this->begin(), this->end(),
                         other.begin(), other.end());
}

template<typename T>
Multiset<T> Multiset<T>::Intersection(const Multiset<T>& other) const {
    Multiset<T> result;
    std::set_intersection(this->begin(), this->end(),
                          other.begin(), other.end(),
                          std::back_inserter(result));
    return result;
}

template<typename T>
Multiset<T> Multiset<T>::Union(const Multiset<T> &other) const {
    Multiset<T> result;
    std::set_union(this->begin(), this->end(),
                   other.begin(), other.end(),
                   std::back_inserter(result));
    return result;
}

template<typename T>
Multiset<T> Multiset<T>::Difference(const Multiset<T>& other) const {
    Multiset<T> result;
    std::set_difference(this->begin(), this->end(),
                        other.begin(), other.end(),
                        std::back_inserter(result));
    return result;
}

template<typename T>
Multiset<T> Multiset<T>::SymmetricDifference(const Multiset<T>& other) const {
    Multiset<T> result;
    std::set_symmetric_difference(this->begin(), this->end(),
                                  other.begin(), other.end(),
                                  std::back_inserter(result));
    return result;
}

template<typename T>
void Multiset<T>::push_back(const T& value) {
    this->insert(value);
}

class IntegerSet : public Multiset<int> {
 public:
    IntegerSet() = default;
    void insert(const int& value) override;

    friend std::ostream& operator<<(std::ostream& output,
                                            IntegerSet& integer_set);
    friend std::istream& operator>>(std::istream& input,
                                            IntegerSet& integer_set);
 private:
        using Multiset::count;
};

std::ostream &operator<<(std::ostream &output, IntegerSet &integer_set) {
    output << '[';
    if (!integer_set.empty()) {
        auto it = integer_set.begin();
        output << *it;
        while (++it != integer_set.end()) {
            output << ',' << *it;
        }
    }
    output << ']';
    return output;
}

std::istream &operator>>(std::istream &input, IntegerSet &integer_set) {
    char c;
    input >> c;
    while (c != ']') {
        if (input.peek() != ']') {
            int integer;
            input >> integer;
            integer_set.insert(integer);
        }
        input >> c;
    }
    return input;
}

void IntegerSet::insert(const int &value) {
    if (!contains(value)) {
        BinarySearchTree<int>::insert(value);
    }
}

#endif  // SET_H_

