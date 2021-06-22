//
// Created by ZenBook on 13.12.2019.
//

#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

#include "set_interface.h"

enum ItType {
    kBegin,
    kEnd,
};

template<typename T>
class BinarySearchTree : public SetInterface<T> {
 private:
    struct TreeNode;

 public:
    class ConstIterator :
            public std::iterator<std::bidirectional_iterator_tag, T> {
     public:
        const T &operator*() const;
        const T *operator->() const;

        ConstIterator &operator++();
        ConstIterator operator++(int);

        ConstIterator &operator--();
        ConstIterator operator--(int);

        bool operator==(const ConstIterator &rhs_it) const;
        bool operator!=(const ConstIterator &rhs_it) const;

     private:
        friend class BinarySearchTree;

        struct IteratorNode {
            IteratorNode() = default;

            IteratorNode(TreeNode *node, TreeNode *next,
                    TreeNode *prev, ItType type = kBegin) {
                this->node = node;
                this->next = next;
                this->prev = prev;
                if (node) {
                    if (type == kBegin)
                        it = node->values.begin();
                    else
                        it = node->values.end();

                    order = {node->left, node->right, node->parent};
                }
            }

            ~IteratorNode() {
                if (node && node->detached) {
                    delete (node);
                }
            }

            TreeNode *node = nullptr;
            TreeNode *next = nullptr;
            TreeNode *prev = nullptr;
            typename std::list<T>::iterator it;

            std::vector<TreeNode *> order;
        };

        ConstIterator() = default;
        ConstIterator(TreeNode *node, TreeNode *next,
                TreeNode *prev, ItType type = kBegin) :
                it_node(node, next, prev, type) {}

        IteratorNode it_node;

        int GetIndex();
    };

    BinarySearchTree() = default;
    BinarySearchTree(const std::initializer_list<T> &list);
    BinarySearchTree(const BinarySearchTree<T> &tree);
    BinarySearchTree(BinarySearchTree<T> &&tree);
    ~BinarySearchTree();

    BinarySearchTree &operator=(const BinarySearchTree<T> &tree);
    BinarySearchTree &operator=(BinarySearchTree<T> &&tree);

    bool operator==(const BinarySearchTree<T> &rhs_tree) const;
    bool operator!=(const BinarySearchTree<T> &rhs_tree) const;

    int count(const T &value) const;
    int size() const override;
    bool empty() const override;

    void insert(const T &value) override;
    void erase(const T &value) override;

    void erase(const ConstIterator &it);
    bool contains(const T &value) const override;

    ConstIterator find(const T &value);
    std::vector<T> ToVector() const override;

    typename BinarySearchTree<T>::ConstIterator begin() const;
    typename BinarySearchTree<T>::ConstIterator end() const;

 private:
    struct TreeNode {
        TreeNode() = default;

        explicit TreeNode(const T &value, TreeNode *p) {
            height = 1;
            detached = false;
            parent = p;
            values.insert(values.begin(), value);
        }

        std::list<T> values;
        int height = 0;
        bool detached = false;
        TreeNode *left = nullptr;
        TreeNode *right = nullptr;
        TreeNode *parent = nullptr;

        bool is_max_value = false;
        bool is_min_value = false;
    };

    TreeNode *root_ = nullptr;
    int size_ = 0;

    TreeNode *min_node = nullptr;
    TreeNode *max_node = nullptr;

    void Clear(TreeNode *node) const;
    void ToVector(TreeNode *node, std::vector<T> *vec) const;

    TreeNode *Insert(TreeNode *node, TreeNode *p, const T &value);
    TreeNode *Erase(TreeNode *node, TreeNode *p, const T &value,
                    bool do_detach = false, TreeNode **real_node = nullptr,
                    typename std::list<T>::iterator *iter = nullptr);
    TreeNode *Balance(TreeNode *node);
    TreeNode *RotateRight(TreeNode *node);
    TreeNode *RotateLeft(TreeNode *node);
    TreeNode *FindMin(TreeNode *node);
    TreeNode *EraseMin(TreeNode *node, TreeNode *p);
    T &GetValue(TreeNode *node) const;
    int GetHeight(TreeNode *node);
    int GetBalanceFactor(TreeNode *node);
    void SetTrueHeight(TreeNode *node);
    void SetParent(TreeNode *node, TreeNode *parent);

    void UpdateMinNode(TreeNode *node);
    void UpdateMaxNode(TreeNode *node);
};

template<typename T>
typename
BinarySearchTree<T>::ConstIterator &
BinarySearchTree<T>::ConstIterator::operator++() {
    if (it_node.node && it_node.it != it_node.node->values.end()) {
        if (++it_node.it == it_node.node->values.end()) {
            int index_from = GetIndex() % 3;
            int index_to = index_from;
            for (int i = 1; i <= 3; i++) {
                index_to = (index_to + 1) % 3;
                if (it_node.order[index_to]) {
                    it_node.next = it_node.order[index_to];
                    break;
                }
            }
            if (it_node.node->is_max_value ||
                (it_node.node->is_max_value && it_node.node->is_min_value)) {
                return *this;
            } else if (index_to < 2) {
                // going to left / right
                while (it_node.next) {
                    it_node = {it_node.next,
                               it_node.next->left,
                               it_node.next->parent};
                }
            } else {
                // going to parent
                while (it_node.node != it_node.next->left) {
                    it_node = {it_node.next,
                               it_node.next->parent,
                               it_node.next->right};
                }
                it_node = {it_node.next,
                           it_node.next->right,
                           it_node.next->left};
            }
        }
    }
    return *this;
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator
BinarySearchTree<T>::ConstIterator::operator++(int) {
    ConstIterator it = *this;
    ++(*this);
    return it;
}

template<typename T>
const T &BinarySearchTree<T>::ConstIterator::operator*() const {
    return *it_node.it;
}

template<typename T>
const T *BinarySearchTree<T>::ConstIterator::operator->() const {
    return &(*it_node.it);
}

template<typename T>
int BinarySearchTree<T>::ConstIterator::GetIndex() {
    for (int i = 0; i < 3; i++) {
        if (it_node.prev == it_node.order[i]) {
            return i;
        }
    }
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator
BinarySearchTree<T>::ConstIterator::operator--(int) {
    ConstIterator it = *this;
    --(*this);
    return it;
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator &
BinarySearchTree<T>::ConstIterator::operator--() {
    if (it_node.node) {
        if (it_node.it == it_node.node->values.begin()) {
            // iterator == values.begin(); -> go to next node
            int index_from = GetIndex() % 3;
            int index_to = index_from;
            for (int i = 1; i <= 3; i++) {
                index_to = (index_to - 1 < 0 ? 2 : index_to - 1);
                if (it_node.order[index_to]) {
                    it_node.next = it_node.order[index_to];
                    break;
                }
            }
            if (it_node.node->is_min_value ||
                (it_node.node->is_max_value && it_node.node->is_min_value)) {
                return *this;
            } else if (index_to < 2) {
                // going to left / right
                while (it_node.next) {
                    it_node = {it_node.next,
                               it_node.next->right,
                               it_node.next->parent, kEnd};
                }
            } else {
                // going to parent
                while (it_node.node != it_node.next->right) {
                    it_node = {it_node.next,
                               it_node.next->parent,
                               it_node.next->left, kEnd};
                }
                it_node = {it_node.next,
                           it_node.next->left,
                           it_node.next->right, kEnd};
            }
        }
        it_node.it--;
    }
    return *this;
}

template<typename T>
bool BinarySearchTree<T>::ConstIterator::operator==(
        const BinarySearchTree::ConstIterator &rhs_it) const {
    return it_node.it == rhs_it.it_node.it;
}

template<typename T>
bool BinarySearchTree<T>::ConstIterator::operator!=(
        const BinarySearchTree::ConstIterator &rhs_it) const {
    return it_node.it != rhs_it.it_node.it;
}

template<typename T>
void BinarySearchTree<T>::insert(const T &value) {
    root_ = Insert(root_, nullptr, value);
    size_++;
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::Insert(TreeNode *node, TreeNode *p, const T &value) {
    if (!node) {
        TreeNode *new_node = new TreeNode(value, p);
        UpdateMinNode(new_node);
        UpdateMaxNode(new_node);
        return new_node;
    } else if (GetValue(node) == value) {
        node->values.insert(node->values.begin(), value);
    } else if (GetValue(node) < value) {
        node->right = Insert(node->right, node, value);
    } else {
        node->left = Insert(node->left, node, value);
    }
    return Balance(node);
}

template<typename T>
T &BinarySearchTree<T>::GetValue(BinarySearchTree::TreeNode *node) const {
    assert(node);
    assert(node->values.size() > 0);
    return *node->values.begin();
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *BinarySearchTree<T>::Balance(TreeNode *node) {
    SetTrueHeight(node);
    if (GetBalanceFactor(node) == 2) {
        if (GetBalanceFactor(node->right) < 0) {
            node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
    }
    if (GetBalanceFactor(node) == -2) {
        if (GetBalanceFactor(node->left) > 0) {
            node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
    }
    return node;
}

template<typename T>
int BinarySearchTree<T>::GetHeight(TreeNode *node) {
    return node ? node->height : 0;
}

template<typename T>
int BinarySearchTree<T>::GetBalanceFactor(TreeNode *node) {
    assert(node);
    return GetHeight(node->right) - GetHeight(node->left);
}

template<typename T>
void BinarySearchTree<T>::SetTrueHeight(TreeNode *node) {
    assert(node);
    int hl = GetHeight(node->left);
    int hr = GetHeight(node->right);
    node->height = 1 + std::max(hl, hr);
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::RotateRight(TreeNode *node) {
    TreeNode *left_node = node->left;
    node->left = left_node->right;
    left_node->right = node;

    SetParent(node->left, node);
    SetParent(left_node, node->parent);
    SetParent(node, left_node);

    SetTrueHeight(node);
    SetTrueHeight(left_node);
    return left_node;
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::RotateLeft(TreeNode *node) {
    TreeNode *right_node = node->right;
    node->right = right_node->left;
    right_node->left = node;

    SetParent(right_node, node->parent);
    SetParent(node->right, node);
    SetParent(node, right_node);

    SetTrueHeight(node);
    SetTrueHeight(right_node);
    return right_node;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const std::initializer_list<T> &list) {
    for (const T& item : list) {
        insert(item);
    }
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T> &tree) {
    *this = tree;
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T> &&tree) {
    *this = std::move(tree);
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    Clear(root_);
}

template<typename T>
bool BinarySearchTree<T>::operator==(
        const BinarySearchTree<T> &rhs_tree) const {
    return ToVector() == rhs_tree.ToVector();
}

template<typename T>
bool BinarySearchTree<T>::operator!=(
        const BinarySearchTree<T> &rhs_tree) const {
    return ToVector() != rhs_tree.ToVector();
}

template<typename T>
BinarySearchTree<T> &BinarySearchTree<T>::operator=(
        const BinarySearchTree<T> &tree) {
    if (this == &tree) {
        return *this;
    }

    Clear(root_);
    std::vector<T> tree_values = tree.ToVector();
    for (T &value : tree_values) {
        insert(value);
    }
    return *this;
}

template<typename T>
void BinarySearchTree<T>::Clear(TreeNode *node) const {
    if (node == nullptr) {
        return;
    }
    Clear(node->left);
    Clear(node->right);
    delete (node);
}

template<typename T>
BinarySearchTree<T> &BinarySearchTree<T>::operator=(
        BinarySearchTree<T> &&tree) {
    if (this == &tree) {
        return *this;
    }

    Clear(root_);
    root_ = tree.root_;
    size_ = tree.size_;
    tree.root_ = nullptr;
    tree.size_ = 0;
    return *this;
}

template<typename T>
int BinarySearchTree<T>::count(const T &value) const {
    TreeNode *ptr = root_;
    while (ptr) {
        if (GetValue(ptr) == value) {
            return ptr->values.size();
        } else if (GetValue(ptr) < value) {
            ptr = ptr->right;
        } else {
            ptr = ptr->left;
        }
    }
    return 0;
}

template<typename T>
int BinarySearchTree<T>::size() const {
    return size_;
}

template<typename T>
bool BinarySearchTree<T>::empty() const {
    return !size_;
}

template<typename T>
bool BinarySearchTree<T>::contains(const T &value) const {
    return count(value);
}

template<typename T>
std::vector<T> BinarySearchTree<T>::ToVector() const {
    std::vector<T> ans;
    ToVector(root_, &ans);
    return ans;
}

template<typename T>
void BinarySearchTree<T>::ToVector(TreeNode *node, std::vector<T> *vec) const {
    if (!node) {
        return;
    }
    ToVector(node->left, vec);
    for (T &item : node->values) {
        vec->push_back(item);
    }
    ToVector(node->right, vec);
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::Erase(TreeNode *node, TreeNode *p, const T &value,
                           bool do_detach, TreeNode **real_node,
                           typename std::list<T>::iterator *list_iterator) {
    if (!node) {
        return nullptr;
    } else if (value < GetValue(node)) {
        node->left = Erase(node->left, node, value,
                           do_detach, real_node, list_iterator);
    } else if (value > GetValue(node)) {
        node->right = Erase(node->right, node, value,
                            do_detach, real_node, list_iterator);
    } else {
        if (do_detach) {
            T tree_iterator_value = *(*list_iterator);
            node->values.erase(*list_iterator);
            *real_node = new TreeNode(tree_iterator_value, nullptr);
            (*real_node)->detached = true;
        } else {
            node->values.erase(node->values.begin());
        }
        size_--;
        if (node->values.empty()) {
            TreeNode *left = node->left;
            TreeNode *right = node->right;
            if (node == min_node) {
                min_node = nullptr;
            }
            if (node == max_node) {
                max_node = nullptr;
            }
            if (node->parent) {
                if (node == node->parent->left) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            }
            delete (node);
            if (!right) {
                SetParent(left, p);
                UpdateMinNode(p);
                UpdateMaxNode(p);
                return left;
            }

            TreeNode *min = FindMin(right);
            SetParent(right, min);
            SetParent(min, p);
            SetParent(left, min);

            min->right = EraseMin(right, min);
            min->left = left;
            UpdateMinNode(p);
            UpdateMaxNode(p);
            if (!do_detach) {
                node->parent = nullptr;
                node->left = nullptr;
                node->right = nullptr;
            }
            return Balance(min);
        }
    }
    return Balance(node);
}

template<typename T>
void BinarySearchTree<T>::erase(const T &value) {
    TreeNode *res = Erase(root_, nullptr, value);
    if ((size_ && res) || !size_) {
        root_ = res;
    }
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::FindMin(TreeNode *node) {
    return node->left ? FindMin(node->left) : node;
}

template<typename T>
typename
BinarySearchTree<T>::TreeNode *
BinarySearchTree<T>::EraseMin(TreeNode *node, TreeNode *p) {
    if (!node->left) {
        SetParent(node->right, p);
        return node->right;
    }
    node->left = EraseMin(node->left, node);
    return Balance(node);
}

template<typename T>
void BinarySearchTree<T>::SetParent(TreeNode *node, TreeNode *p) {
    if (node) {
        node->parent = p;
    }
}

template<typename T>
void BinarySearchTree<T>::UpdateMinNode(TreeNode *node) {
    if (!node)
        return;

    if (!min_node || GetValue(min_node) > GetValue(node)) {
        if (min_node)
            min_node->is_min_value = false;

        min_node = node;
        min_node->is_min_value = true;
    }

    UpdateMinNode(node->left);
}

template<typename T>
void BinarySearchTree<T>::UpdateMaxNode(TreeNode *node) {
    if (!node)
        return;

    if (!max_node || GetValue(max_node) < GetValue(node)) {
        if (max_node)
            max_node->is_max_value = false;

        max_node = node;
        max_node->is_max_value = true;
    }

    UpdateMinNode(node->right);
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::begin() const {
    if (min_node)
        return ConstIterator(min_node,
                min_node->left,
                min_node->parent,
                kBegin);
    else
        return ConstIterator();
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::end() const {
    if (max_node)
        return ConstIterator(max_node, max_node->left, max_node->parent, kEnd);
    else
        return ConstIterator();
}

template<typename T>
void BinarySearchTree<T>::erase(const ConstIterator &it) {
    TreeNode *res = Erase(root_, nullptr, *(it.it_node.it), true,
                const_cast<TreeNode **>(&it.it_node.node),
                const_cast<typename std::list<T>::iterator *>(&it.it_node.it));
    if ((size_ && res) || !size_) {
        root_ = res;
    }
}

template<typename T>
typename
BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::find(const T &value) {
    ConstIterator it = begin();
    while (it != end() && *it != value) {
        ++it;
    }
    return it;
}

#endif  // BINARY_SEARCH_TREE_H_
