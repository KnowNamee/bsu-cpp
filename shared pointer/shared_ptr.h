#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include <iostream>
#include <cassert>
#include <utility>

namespace shared_ptr {

template<typename T>
class SharedPtr {
 public:
    explicit SharedPtr(std::nullptr_t);
    explicit SharedPtr(T*);
    SharedPtr();
    SharedPtr(const SharedPtr<T> &);
    SharedPtr(SharedPtr<T> &&);
    ~SharedPtr();

    SharedPtr<T>& operator=(const SharedPtr<T> &ptr);
    SharedPtr<T>& operator=(SharedPtr<T> &&ptr);

    T* Get();
    const T* Get() const;

    T* operator->();
    T& operator *();
    const T* operator->() const;
    const T& operator *() const;

    void Release();

    bool operator== (const SharedPtr<T>& rhs) const;
    bool operator!= (const SharedPtr<T>& rhs) const;
    bool operator== (const T* rhs) const;
    bool operator!= (const T* rhs) const;

    template<typename T1>
    friend bool operator==(const T1* lhs, const SharedPtr<T1>& rhs);

    template<typename T1>
    friend bool operator!=(const T1* lhs, const SharedPtr<T1>& rhs);

    bool operator==(std::nullptr_t rhs) const;
    bool operator!=(std::nullptr_t rhs) const;

    template<typename T1>
    friend bool operator==(std::nullptr_t lhs, const SharedPtr<T1>& rhs);

    template<typename T1>
    friend bool operator!=(std::nullptr_t lhs, const SharedPtr<T1>& rhs);

 private:
    T    *obj_ = nullptr;
    int  *ref_counter_ = nullptr;
    bool *is_valid_ = nullptr;

    void Remove();
    void MakeNull();
    void CopyFrom(const SharedPtr<T>& ptr);
};

/*!
 * @brief Constructor that takes nullptr SharedPtr<T>(nullptr)
 */
template<typename T>
SharedPtr<T>::SharedPtr(std::nullptr_t) {
    MakeNull();
}

/*!
 * @brief Default constructor for SharedPtr
 */
template<typename T>
SharedPtr<T>::SharedPtr() {
    MakeNull();
}

/*!
 * @tparam T - Object type
 * @param obj - Pointer at object which SharedPtr will carry
 */
template<typename T>
SharedPtr<T>::SharedPtr(T *obj) {
    obj_ = obj;
    ref_counter_ = new int(1);
    is_valid_ = new bool(true);
}

/*!
 * @brief Destructor of SharedPtr
 * @brief Calls Remove() inside
 */
template<typename T>
SharedPtr<T>::~SharedPtr() {
    Remove();
}

/*!
 * @tparam T - Any type
 * @return If object detached -> return nullptr
 * @return Otherwise -> return object
 */
template<typename T>
T *SharedPtr<T>::Get() {
    return obj_;
}

/*!
 * @tparam T - Any type
 * @return If object detached -> return nullptr
 * @return Otherwise -> return object
 */
template<typename T>
const T *SharedPtr<T>::Get() const {
    return obj_;
}

/*!
 * @brief Detach obj_ from current SharedPtr.
 * @tparam T - Any type
 */
template<typename T>
void SharedPtr<T>::Release() {
    if (is_valid_ != nullptr) {
        *is_valid_ = false;
    }
    obj_ = nullptr;
}

/*!
 * @brief - Do the same as a destructor of SharedPtr.
 * Destructor calls Remove() inside -> Destructor and Remove
 * are synonyms
 * @tparam T - Any type
 */
template<typename T>
void SharedPtr<T>::Remove() {
    if (ref_counter_ != nullptr) {
        --(*ref_counter_);
        if (*ref_counter_ == 0) {
            if (*is_valid_) {
                delete(obj_);
            }
            delete(ref_counter_);
            delete(is_valid_);
        }
    }
}

/*!
 * @brief Copy constructor for SharedPtr
 * @tparam T - Any type
 * @param ptr - Other SharedPtr
 */
template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &ptr) {
    CopyFrom(ptr);
    if (ref_counter_ != nullptr)
        ++(*ref_counter_);
}

/*!
 * @brief Overload of copy operator= for SharedPtr's
 * @tparam T - Any type
 * @param ptr - Other SharedPtr
 * @return *this
 */
template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &ptr) {
    if (*this == ptr)
        return *this;

    Remove();
    CopyFrom(ptr);
    if (ref_counter_ != nullptr)
        ++(*ref_counter_);

    return *this;
}

/*!
 * @brief Move constructor for SharedPtr
 * @tparam T - Any type
 * @param ptr - Other SharedPtr
 */
template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T> &&ptr) {
    if (ptr.obj_ != nullptr) {
        CopyFrom(ptr);
    } else {
        ptr.Remove();
    }
    ptr.MakeNull();
}

/*!
 * @brief Overload of move operator= for 2 SharedPtr's
 * @tparam T - Any type
 * @param ptr - Other SharedPtr
 * @return *this
 */
template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&ptr) {
    if (*this == ptr)
        return *this;

    Remove();
    CopyFrom(ptr);
    ptr.MakeNull();

    return *this;
}

/*!
 * @brief Inits all the fields of the class with nullptr
 */
template<typename T>
void SharedPtr<T>::MakeNull() {
    obj_ = nullptr;
    is_valid_ = nullptr;
    ref_counter_ = nullptr;
}

template<typename T>
T *SharedPtr<T>::operator->() {
    return Get();
}

template<typename T>
const T *SharedPtr<T>::operator->() const {
    return Get();
}

template<typename T>
T &SharedPtr<T>::operator*() {
    assert(obj_ != nullptr);
    return *obj_;
}

template<typename T>
const T &SharedPtr<T>::operator*() const {
    assert(obj_ != nullptr);
    return *obj_;
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T> &rhs) const {
    return Get() == rhs.Get();
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T> &rhs) const {
    return Get() != rhs.Get();
}

template<typename T>
bool SharedPtr<T>::operator==(const T *rhs) const {
    return Get() == rhs;
}

template<typename T>
bool SharedPtr<T>::operator!=(const T *rhs) const {
    return Get() != rhs;
}

template<typename T>
bool operator==(const T *lhs, const SharedPtr<T> &rhs) {
    return rhs.Get() == lhs;
}

template<typename T>
bool operator!=(const T *lhs, const SharedPtr<T> &rhs) {
    return rhs.Get() != lhs;
}

template<typename T>
bool SharedPtr<T>::operator==(std::nullptr_t rhs) const {
    return Get() == nullptr;
}

template<typename T>
bool SharedPtr<T>::operator!=(std::nullptr_t rhs) const {
    return Get() != nullptr;
}

template<typename T1>
bool operator==(std::nullptr_t lhs, const SharedPtr<T1> &rhs) {
    return rhs.Get() == nullptr;
}

template<typename T1>
bool operator!=(std::nullptr_t lhs, const SharedPtr<T1> &rhs) {
    return rhs.Get() != nullptr;
}

/*!
 * @brief - Copies fields of object ptr to this
 */
template<typename T>
void SharedPtr<T>::CopyFrom(const SharedPtr<T> &ptr) {
    obj_ = ptr.obj_;
    is_valid_ = ptr.is_valid_;
    ref_counter_ = ptr.ref_counter_;
}

}  // namespace shared_ptr

#endif  // SHARED_PTR_H_
