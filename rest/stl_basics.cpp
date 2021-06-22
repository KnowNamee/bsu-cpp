#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// В данной лабораторной работе можно не писать каждый раз "std::".
using namespace std;

// ---------------------------------------------------------------------------
// Задание 1. Строка из подстрок.

std::string Substring(const std::string& text, int index, int length) {
    std::string answer = "";
    int t_size = text.length();
    int start_index = index % t_size;

    answer += text.substr(start_index);
    while (answer.length() < length) {
        answer += text;
    }
    answer.resize(length);

    return answer;
}

// ---------------------------------------------------------------------------
// Задание 2. K-ая точка.

// В данном задании можно и нужно использовать глобальные переменные.
// YOUR_CODE_HERE

std::vector<std::pair<int, int>> points_inited;

bool PointsComparator(const std::pair<int, int> &a,
                      const std::pair<int, int> &b) {
    int sum_a = a.first + a.second;
    int sum_b = b.first + b.second;

    if (sum_a != sum_b) {
        return sum_a < sum_b;
    } else if (a.second != b.second) {
        return a.second > b.second;
    } else {
        return a.first < b.first;
    }
}

void InitPoints(const std::vector<std::pair<int, int>>& points) {
    points_inited = points;
    std::sort(points_inited.begin(), points_inited.end(), PointsComparator);
}

std::pair<int, int> GetKthPoint(int k) {
    if (0 <= k && k < points_inited.size()) {
        return points_inited[k];
    } else {
        return std::make_pair(0, 0);
    }
}

// ---------------------------------------------------------------------------
// Задание 3. Генерация дробей.

void NextFraction(std::vector<std::pair<int, int>> &fractions,
                  int max_denom, int l_n, int l_d, int r_n, int r_d) {
    int next_n = l_n + r_n;
    int next_d = l_d + r_d;

    if (next_d >= max_denom) {
        return;
    }

    NextFraction(fractions, max_denom, l_n, l_d, next_n, next_d);
    fractions.push_back(std::make_pair(next_n, next_d));
    NextFraction(fractions, max_denom, next_n, next_d, r_n, r_d);
}

std::vector<std::pair<int, int>> GenerateFractions(int max_denom) {
    std::vector<std::pair<int, int>> fractions(1, make_pair(0, 1));
    NextFraction(fractions, max_denom, 0, 1, 1, 1);
    fractions.push_back(std::make_pair(1, 1));

    return fractions;
}

// ---------------------------------------------------------------------------
// Задание 4. Ближайшее слева большее число.

void NearestGreater(const std::vector<int>& input, vector<int>& output) {
    std::stack<int> left;
    output.clear();

    for (int i = 0; i < input.size(); i++) {
        while (!left.empty() && left.top() <= input[i]) {
            left.pop();
        }
        if (!left.empty()) {
            output.push_back(left.top());
        }
        left.push(input[i]);
    }
}

// ---------------------------------------------------------------------------
// Задание 5. Адреса жительства.

vector<pair<string, std::vector<string>>>
AddressHistory(const vector<pair<string,
        pair<string, string>>>& streets_and_persons) {
    vector<pair<pair<string, string>,
    pair<set<string>, vector<string>>>> pstreets;
    map<pair<string, string>, int> index;
    int index_cnt = 1;

    for (auto &x : streets_and_persons) {
        pair<string, string> name = x.second;
        string street = x.first;

        if (index[name] == 0) {
            index[name] = index_cnt++;
            pstreets.push_back(make_pair(name,
                                         make_pair(set<string>(), vector<string>())));
        }

        int cur_index = index[name]-1;

        if (pstreets[cur_index].second.first.find(street) ==
            pstreets[cur_index].second.first.end()) {
            pstreets[cur_index].second.first.insert(street);
            pstreets[cur_index].second.second.push_back(street);
        }
    }

    vector<pair<string, vector<string>>> result;

    for (auto &x : pstreets) {
        result.push_back(make_pair(x.first.first
                                   + " "
                                   + x.first.second, x.second.second));
    }

    return result;
}

// ---------------------------------------------------------------------------
// Задание 6. Суммируем!

// Отправка статистик после выполнения очередной операции.
void SummarizeNumbersSetReportStats(int min_value, int max_value);

int FindMinAndErase(multiset<int> & elements) {
    int min = *elements.begin();
    elements.erase(elements.begin());
    return min;
}

void SummarizeNumbersSet(const vector<int>& values) {
    multiset<int> elements;

    for (int x : values) {
        elements.insert(x);
    }

    for (int i = 0; i + 1 < values.size(); i++) {
        int min1 = FindMinAndErase(elements);
        int min2 = FindMinAndErase(elements);

        elements.insert(min1 + min2);

        auto it = elements.end();
        int max = *(--it);
        int min = *elements.begin();

        SummarizeNumbersSetReportStats(min, max);
    }
}

// ---------------------------------------------------------------------------
// Задание 7. AwesomeMix

std::vector<string> AwesomeMix(
        const std::vector<std::pair<string, string>>& authors_and_songs,
        int authors_limit) {
    map<string, set<string>> authors_songs;

    for (auto &x : authors_and_songs) {
        authors_songs[x.first].insert(x.second);
    }

    vector<pair<string, set<string>>> authors_songs_vec;
    for (auto &x : authors_songs) {
        authors_songs_vec.push_back(x);
    }

    std::sort(authors_songs_vec.begin(), authors_songs_vec.end(),
              [](const pair<string, set<string>> &a,
                 const pair<string, set<string>> &b){
                  if (a.second.size() != b.second.size()) {
                      return a.second.size() > b.second.size();
                  } else {
                      return a.first < b.first;
                  }
              });

    vector<string> result;
    set<string> res_set;
    int k = min(static_cast<int>(authors_songs_vec.size()), authors_limit);
    for (int i = 0; i < k; i++) {
        for (auto &x : authors_songs_vec[i].second) {
            if (res_set.find(x) == res_set.end()) {
                result.push_back(x);
                res_set.insert(x);
            }
        }
    }
    sort(result.begin(), result.end());

    return result;
}

// ---------------------------------------------------------------------------
// Задание 8. Массивы без циклов

int Count(const std::vector<int>& values, int x, int m) {
    if (find(values.begin(), values.end(), x) == values.end()) {
        return std::count_if(values.begin(), values.end(), [&](int x){
            return __gcd(m, x) == 1;
        });
    } else {
        return std::count_if(values.begin(), values.end(), [&](int x){
            return x % m == 0;
        });
    }
}

// ---------------------------------------------------------------------------
#ifndef IGNORE_SOLUTION_MAIN

std::vector<std::pair<int, int>> summarize_numbers_set_stats;
void SummarizeNumbersSetReportStats(int min_value, int max_value) {
    summarize_numbers_set_stats.emplace_back(min_value, max_value);
}

int main() {
    return 0;
}
#endif  // IGNORE_SOLUTION_MAIN