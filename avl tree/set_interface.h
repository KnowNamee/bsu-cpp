//
// Created by ZenBook on 13.12.2019.
//

#ifndef SET_INTERFACE_H_
#define SET_INTERFACE_H_

#include <vector>

template<typename T>
class SetInterface {
 public:
    virtual  int  size() const = 0;
    virtual bool empty() const = 0;

    virtual bool contains(const T& value) const = 0;

    virtual void insert(const T& value) = 0;
    virtual void erase(const T& value) = 0;

    virtual std::vector<T> ToVector() const = 0;
};

#endif  // SET_INTERFACE_H_
