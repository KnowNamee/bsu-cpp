#ifndef CONTAINERS_H_
#define CONTAINERS_H_

/* Проблема в описанном интерфейсе BDList'а заключается в том, что
 * во всех методах, куда передаются аргументы типа Node *element,
 * может случиться так, что Node *element принадлежит совершенно друому объекту
 *
 * Чтобы это предотвратить, можно хранить в Node поле типа BDList*, чтобы указать,
 * какому объекту принадлежит Node, и в вызове таких функкций делать соотв. assert
 */

#include <utility>
#include <vector>
#include <cassert>
#include <iostream>

namespace containers {

// --------------------------------------------------------------------------

// Тут следует расположить код решения задачи 1 (BiDirectionalList).

template<typename T>
class BiDirectionalList {
 public:
    struct Node {
     public:
        T value;

     private:
        friend class BiDirectionalList;

        Node *next = nullptr;
        Node *prev = nullptr;

        Node(Node *next, Node *prev, const T& val);
    };

    BiDirectionalList() = default;
    BiDirectionalList(const std::initializer_list<T>&);
    BiDirectionalList(const BiDirectionalList<T>&);
    BiDirectionalList(BiDirectionalList<T>&&);
    ~BiDirectionalList();

    int Size() const;
    bool IsEmpty() const;

    void PushFront(const T& value);
    void PushBack(const T& value);

    void PopFront();
    void PopBack();

    void InsertBefore(BiDirectionalList<T>::Node *element, const T& value);
    void InsertAfter(BiDirectionalList<T>::Node *element, const T& value);
    void Erase(BiDirectionalList<T>::Node *element);

    int Find(const T& value) const;
    std::vector<int> FindAll(const T& value) const;

    std::vector<T> ToVector() const;

    Node* Front();
    Node* Back();
    const Node* Front() const;
    const Node* Back()  const;

    Node* operator[](int i);
    const Node* operator[](int i) const;

    bool operator==(const BiDirectionalList<T> &other) const;
    bool operator!=(const BiDirectionalList<T> &other) const;

    BiDirectionalList& operator=(const BiDirectionalList<T> &other);
    BiDirectionalList& operator=(BiDirectionalList<T> &&other);

 private:
    Node *head_  = nullptr;
    Node *tail_  = nullptr;
    int size_ = 0;

    void Clear();
};

template<typename T>
BiDirectionalList<T>::Node::Node(
        BiDirectionalList::Node *next,
        BiDirectionalList::Node *prev,
        const T &val) : value(val) {
    this->next = next;
    this->prev = prev;
}

/*!
 * @brief - Contructor tha takes std::initializer_list<T>
 */
template<typename T>
BiDirectionalList<T>::
        BiDirectionalList(const std::initializer_list<T>& elements) {
    for (auto &e : elements) {
        PushBack(e);
    }
}

/*!
 * @brief - Destructor, calls Clear() inside
 */
template<typename T>
BiDirectionalList<T>::~BiDirectionalList() {
    Clear();
}

/*!
 * @return - Size of the current list
 */
template<typename T>
int BiDirectionalList<T>::Size() const {
    return size_;
}

/*!
 * @return - True if list is empty
 * @return - False otherwise
 */
template<typename T>
bool BiDirectionalList<T>::IsEmpty() const {
    return size_ == 0;
}

/*!
 * @brief - Adding element at Front position of the list
 * @param value - value of a new element
 */
template<typename T>
void BiDirectionalList<T>::PushFront(const T &value) {
    if (!size_) {
        head_ = tail_ = new Node(nullptr, nullptr, value);
    } else {
        head_ = new Node(head_, nullptr, value);
        head_->next->prev = head_;
    }
    size_++;
}

/*!
 * @brief - Adding element at Back position of the list
 * @param value - value of a new element
 */
template<typename T>
void BiDirectionalList<T>::PushBack(const T &value) {
    if (!size_) {
        head_ = tail_ = new Node(nullptr, nullptr, value);
    } else {
        tail_ = new Node(nullptr, tail_, value);
        tail_->prev->next = tail_;
    }
    size_++;
}

/*!
 * @brief - Removing Front element
 */
template<typename T>
void BiDirectionalList<T>::PopFront() {
    assert(size_ > 0);
    Node *to_delete = head_;
    head_ = head_->next;
    delete (to_delete);
    size_--;

    if (!size_) {
        head_ = nullptr;
        tail_ = nullptr;
    } else {
        head_->prev = nullptr;
    }
}

/*!
 * @brief - Removing Back element
 */
template<typename T>
void BiDirectionalList<T>::PopBack() {
    assert(size_ > 0);
    Node *to_delete = tail_;
    tail_ = tail_->prev;
    delete (to_delete);
    size_--;

    if (!size_) {
        head_ = nullptr;
        tail_ = nullptr;
    } else {
        tail_->next = nullptr;
    }
}

/*!
 * @return - First index of element that has value 'value"
 */
template<typename T>
int BiDirectionalList<T>::Find(const T &value) const {
    int i = 0;
    Node *cur = head_;

    while (cur != nullptr) {
        if (cur->value == value) {
            return i;
        } else {
            cur = cur->next;
            i++;
        }
    }

    return -1;
}

/*!
 * @return - vector<int> which contains all indexes of elements that have value 'value'
 */
template<typename T>
std::vector<int> BiDirectionalList<T>::FindAll(const T &value) const {
    int i = 0;
    Node *cur = head_;
    std::vector<int> data;

    while (cur != nullptr) {
        if (cur->value == value) {
            data.push_back(i);
        }
        i++;
        cur = cur->next;
    }

    return data;
}

/*!
 * @return - vector<T> / converted BiDirectionalList<T>
 */
template<typename T>
std::vector<T> BiDirectionalList<T>::ToVector() const {
    std::vector<T> data;
    Node *cur = head_;

    while (cur != nullptr) {
        data.push_back(cur->value);
        cur = cur->next;
    }

    return data;
}

/*!
 * @brief - Inserts element of value 'value' before 'element'
 * @param element - element after inserted
 * @param value - value of inserted element
 */
template<typename T>
void BiDirectionalList<T>::
        InsertBefore(BiDirectionalList<T>::Node *element, const T &value) {
    assert(size_ > 0);
    if (element == head_) {
        PushFront(value);
    } else {
        Node *next = element;
        Node *prev = element->prev;

        prev->next =
                next->prev =
                        new Node(next, prev, value);

        size_++;
    }
}

/*!
 * @brief - Inserts element of value 'value' after 'element'
 * @param element - element before inserted
 * @param value - value of inserted element
 */
template<typename T>
void BiDirectionalList<T>::
        InsertAfter(BiDirectionalList<T>::Node *element, const T &value) {
    assert(size_ > 0);
    if (element == tail_) {
        PushBack(value);
    } else {
        Node *next = element->next;
        Node *prev = element;

        prev->next =
            next->prev =
                new Node(next, prev, value);

        size_++;
    }
}

/*!
 * @brief - Erasing given element from the list
 * @param element - element to erase
 */
template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList<T>::Node *element) {
    assert(size_ > 0);
    if (element == tail_) {
        PopBack();
    } else if (element == head_) {
        PopFront();
    } else {
        Node *prev = element->prev;
        Node *next = element->next;
        prev->next = next;
        next->prev = prev;
        size_--;
        delete (element);
    }
}

template<typename T>
bool BiDirectionalList<T>::operator==(const BiDirectionalList<T> &other) const {
    if (size_ != other.size_)
        return false;

    Node *cur_t = this->head_;
    Node *cur_o = other.head_;

    while (cur_t != nullptr && cur_o != nullptr) {
        if (cur_o->value != cur_t->value) {
            return false;
        }
        cur_o = cur_o->next;
        cur_t = cur_t->next;
    }

    return true;
}

template<typename T>
bool BiDirectionalList<T>::operator!=(const BiDirectionalList<T> &other) const {
    return !(*this == other);
}

/*!
 * @brief - Overload of copy operator= for 2 lists
 * @param other - other list
 * @return *this
 */
template<typename T>
BiDirectionalList<T>&
        BiDirectionalList<T>::operator=(const BiDirectionalList<T> &other) {
    if (&other == this) {
        return *this;
    }

    Clear();

    Node *cur_o = other.head_;
    while (cur_o != nullptr) {
        PushBack(cur_o->value);
        cur_o = cur_o->next;
    }

    return *this;
}

/*!
 * @brief - Copy constructor for list
 * @param other - other list
 */
template<typename T>
BiDirectionalList<T>::BiDirectionalList(const BiDirectionalList<T> &other) {
    *this = other;
}

/*!
 * @brief - Overload of move operator= for 2 lists
 * @param other - other list
 * @return *this
 */
template<typename T>
BiDirectionalList<T>&
    BiDirectionalList<T>::operator=(BiDirectionalList<T> &&other) {
    if (this == &other)
        return *this;

    Clear();

    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;

    other.size_ = 0;
    other.head_ = nullptr;
    other.tail_ = nullptr;

    return *this;
}

/*!
 * @brief - Move constructor for list
 * @param other - other list
 */
template<typename T>
BiDirectionalList<T>::BiDirectionalList(BiDirectionalList<T> &&other) {
    *this = std::move(other);
}

/*!
 * @param i - index of the element
 * @return pointer at element at index i
 */
template<typename T>
typename BiDirectionalList<T>::Node *BiDirectionalList<T>::operator[](int i) {
    assert(0 <= i && i < size_);
    Node *cur = head_;
    while (i--) {
        cur = cur->next;
    }
    return cur;
}

/*!
 * @param i - index of the element
 * @return pointer at element at index i
 */
template<typename T>
const typename
BiDirectionalList<T>::Node *BiDirectionalList<T>::operator[](int i) const {
    assert(0 <= i && i < size_);
    Node *cur = head_;
    while (i--) {
        cur = cur->next;
    }
    return cur;
}

template<typename T>
typename BiDirectionalList<T>::Node *BiDirectionalList<T>::Front() {
    assert(size_ > 0);
    return head_;
}

template<typename T>
typename BiDirectionalList<T>::Node *BiDirectionalList<T>::Back() {
    assert(size_ > 0);
    return  tail_;
}

template<typename T>
const typename BiDirectionalList<T>::Node *BiDirectionalList<T>::Front() const {
    assert(size_ > 0);
    return head_;
}

template<typename T>
const typename BiDirectionalList<T>::Node *BiDirectionalList<T>::Back() const {
    assert(size_ > 0);
    return tail_;
}

template<typename T>
void BiDirectionalList<T>::Clear() {
    while (size_ != 0) {
        PopFront();
    }
}

// --------------------------------------------------------------------------

// Тут следует расположить код решения задачи 2 (Queue / Stack).

template<typename T>
class Queue {
 public:
    Queue() = default;
    Queue(const std::initializer_list<T>& elements);

    int Size() const;
    bool IsEmpty() const;

    void Push(const T& value);
    void Pop();

    const T& Get() const;

    bool operator==(const Queue<T> &other) const;
    bool operator!=(const Queue<T> &other) const;

 private:
    BiDirectionalList<T> queue_;
};

/*!
 * @brief - Constructor that takes initializer_list<T>
 * @param elements - inititalizer_list<T>
 */
template<typename T>
Queue<T>::Queue(const std::initializer_list<T> &elements) {
    queue_ = elements;
}

/*!
 * @brief - Pushing element of value 'value' to the Queue
 * @param value - value of the element
 */
template<typename T>
void Queue<T>::Push(const T &value) {
    queue_.PushBack(value);
}

/*!
 * @brief - Removing element from the Queue
 */
template<typename T>
void Queue<T>::Pop() {
    queue_.PopFront();
}

/*!
 * @return - value of front element of the Queue
 */
template<typename T>
const T& Queue<T>::Get() const {
    return queue_.Front()->value;
}

/*!
 * @return - size of the Queue
 */
template<typename T>
int Queue<T>::Size() const {
    return queue_.Size();
}

/*!
 * @return true - if the queue is empty
 * @return false - otherwise
 */
template<typename T>
bool Queue<T>::IsEmpty() const {
    return queue_.IsEmpty();
}

template<typename T>
bool Queue<T>::operator==(const Queue<T> &other) const {
    return queue_ == other.queue_;
}

template<typename T>
bool Queue<T>::operator!=(const Queue<T> &other) const {
    return queue_ != other.queue_;
}

// --------------------------------------------------------------------------

// Раскоментируйте нужные строки ниже для выбора варианта

enum class Variant {
    kQueue,
    kStack,
};

Variant GetVariant() {
    return Variant::kQueue;
    // return Variant::kStack;
}

template<typename T>
using Container = Queue<T>;

// template<typename T>
// using Container = Stack<T>;

// --------------------------------------------------------------------------

}  // namespace containers

#endif  // CONTAINERS_H_
