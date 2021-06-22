//
// Created by ZenBook on 13.12.2019.
//


#include <random>

#include "gtest.h"
#include "binary_search_tree.h"

namespace {

std::vector<int> GenerateVector(int k, int l, int r) {
    std::vector<int> vec(k);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dis(l, r);
    for (int &item : vec) {
        item = dis(mt);
    }
    return vec;
}

}  // namespace

TEST(BinarySearchTree, Stupakevich_Sample) {
    BinarySearchTree<int> tree;
    ASSERT_EQ(tree.size(), 0);
    ASSERT_FALSE(tree.contains(1));

    tree.insert(1);
    ASSERT_EQ(tree.size(), 1);
    ASSERT_TRUE(tree.contains(1));

    tree.insert(1);
    ASSERT_EQ(tree.size(), 2);
    ASSERT_TRUE(tree.contains(1));

    tree.insert(2);
    ASSERT_EQ(tree.size(), 3);
    ASSERT_TRUE(tree.contains(2));

    tree.erase(2);
    ASSERT_EQ(tree.size(), 2);
    ASSERT_FALSE(tree.contains(2));

    tree.erase(1);
    tree.erase(1);
    ASSERT_TRUE(tree.empty());

    std::multiset<int> s;
    std::vector<int> vec = GenerateVector(1e5, 1, 1e9);
    std::vector<int> what = GenerateVector(1e5, 1, 2);
    for (int i = 0; i < 1e3; i++) {
        if (what[i] == 1) {
            s.insert(vec[i]);
            tree.insert(vec[i]);
        } else {
            if (tree.contains(vec[i])) {
                s.erase(s.find(vec[i]));
                tree.erase(vec[i]);
            }
        }
        ASSERT_EQ(s.size(), tree.size());
    }
}

TEST(Iterators, Stupakevich_Sample) {
    std::vector<int> k_vec = {1, 5, 10, 100, 1'000, 10'000, 100'000};
    std::vector<int> l_vec = {1, 1, 1, 1, 1, 1, 1};
    std::vector<int> r_vec = {9, 9, 100,
                              1'000'000'000, 1'000'000'000,
                              1'000'000'000, 1'000'000'000};
    for (int i = 0; i < 7; i++) {
        BinarySearchTree<int> tree;
        std::vector<int> items = GenerateVector(k_vec[i], l_vec[i], r_vec[i]);
        for (int &item : items) {
            tree.insert(item);
        }
        std::sort(items.begin(), items.end());
        std::vector<int> res_items;
        BinarySearchTree<int>::ConstIterator it = tree.begin();
        for (int j = 0; j < k_vec[i]; j++) {
            res_items.push_back(*it);
            ++it;
        }
        ASSERT_EQ(items, res_items);
    }
    for (int i = 0; i < 7; i++) {
        BinarySearchTree<int> tree;
        std::vector<int> items = GenerateVector(k_vec[i], l_vec[i], r_vec[i]);
        for (int &item : items) {
            tree.insert(item);
        }
        std::sort(items.begin(), items.end());
        std::reverse(items.begin(), items.end());
        std::vector<int> res_items;
        BinarySearchTree<int>::ConstIterator it = tree.end();
        for (int j = 0; j < k_vec[i]; j++) {
            --it;
            res_items.push_back(*it);
        }
        ASSERT_EQ(items, res_items);
    }
    {
        BinarySearchTree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(1);
        tree.insert(3);

        BinarySearchTree<int>::ConstIterator it1 = tree.begin();
        BinarySearchTree<int>::ConstIterator it2 = tree.begin();

        ASSERT_EQ(it1, it2);
        it1++;
        ASSERT_EQ(*it1, 1);
        ASSERT_EQ(*it2, 1);
        ASSERT_NE(it1, it2);
        it1++;
        ASSERT_EQ(*it1, 2);
        it1++;
        ASSERT_EQ(*it1, 3);
        it1++;
        ASSERT_EQ(it1, tree.end());
    }
    {
        BinarySearchTree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(1);
        tree.insert(3);
        tree.insert(3);

        BinarySearchTree<int>::ConstIterator it1 = tree.find(3);
        tree.erase(it1);
        BinarySearchTree<int>::ConstIterator it3 = tree.find(2);
        tree.erase(it3);

        ASSERT_EQ(*it1, 3);
        ASSERT_EQ(*it3, 2);

        std::vector<int> items;
        for (int item : tree) {
            items.push_back(item);
        }
        ASSERT_THAT(items, testing::ElementsAre(1, 1, 3));
    }
    // Ещё нужно сделать тест, где проверятеся удаление
    // значения именно по итератору
    // а именно у нас в вершине есть [{1, 2}, {1, 3}],
    // нужно удалить именно {1, 3} пару.
}
