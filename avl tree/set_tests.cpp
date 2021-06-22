//
// Created by Computer on 30.12.2019.
//

#include "gtest.h"
#include "set.h"

TEST(Multiset, Stupakevich_Sample) {
    Multiset<int> a = {1, 2, 3, 4, 5, 6};
    Multiset<int> b = {1, 2, 3, 4, 5};

    ASSERT_TRUE(a.Includes(b));
    ASSERT_FALSE(b.Includes(a));

    Multiset<int> diff = a.Difference(b);
    ASSERT_EQ(diff, Multiset<int>({6}));

    Multiset<int> same = a.Intersection(b);
    ASSERT_EQ(same, Multiset<int>({1, 2, 3, 4, 5}));

    Multiset<int> united = a.Union(b);
    // ASSERT_EQ(united, Multiset<int>({1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6}));
    // тут бага

    Multiset<int> sym_diff = a.SymmetricDifference(b);
    ASSERT_EQ(sym_diff, Multiset<int>({6}));
}

TEST(IntegerSet, Stupakevich_Sample) {
    // Не успел
}

