//
// Created by Computer on 30.12.2019.
//

#include <vector>
#include <set>

#include "algorithms.h"
#include "gtest.h"

namespace {

class SixChecker {
 public:
    bool operator()(const int& number) {
        return number == 6;
    }
};

bool IsSix(const int& number) {
    return number == 6;
}

}  // namespace

TEST(Algorithms, Stupakevich_Sample) {
    std::vector<int> vec = {5, 4, 1, 2, 3, 10, 11};
    ASSERT_TRUE(IsSorted(vec.begin() + 2, vec.end()));

    std::set<int> s = {1, 5, 6, 10, 3, 2};
    ASSERT_TRUE(IsSorted(s.begin(), s.end()));

    ASSERT_EQ(*MaxInRange(s.begin(), s.end()), 10);
    ASSERT_EQ(*MaxInRange(vec.begin(), vec.begin() + 2), 5);

    ASSERT_EQ(SumOfRange(s.begin(), s.end()), 27);
    ASSERT_EQ(SumOfRange(vec.begin(), vec.end()), 36);

    ASSERT_EQ(CountIf(vec.begin(), vec.end(), SixChecker()), 0);
    ASSERT_EQ(CountIf(s.begin(), s.end(), SixChecker()), 1);

    ASSERT_EQ(CountIfNot(vec.begin(), vec.end(), IsSix), vec.size());
    ASSERT_EQ(CountIfNot(s.begin(), s.end(), IsSix), s.size() - 1);

    auto it = FindIf(s.begin(), s.end(), [](const int& number) {
        return number == 6;
    });

    s.erase(it);
    ASSERT_EQ(s.find(6), s.end());
}
