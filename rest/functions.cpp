#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>

// В данной лабораторной работе можно не писать каждый раз "std::".
using namespace std;

// ---------------------------------------------------------------------------
// Задание 1. Сумма элементов массива.

double GetSum(const vector<double> &vec) {
    double sum = 0.;

    for (double x : vec) {
        sum += x;
    }

    return sum;
}

// ---------------------------------------------------------------------------
// Задание 2. Магическая сумма матрицы.

uint32_t GetMagicXor(const vector<uint32_t> &vec) {
    uint32_t ans = 0x00000000;

    for (uint32_t x : vec) {
        if (__builtin_popcount(x) <= 4) {
            ans ^= x;
        }
    }

    return ~ans;
}

vector<uint32_t> GetMagicXor(const vector<vector<uint32_t>> &matr) {
    int size_i = matr.size();
    vector<uint32_t> ans(size_i);

    for (int i = 0; i < size_i; i++) {
        ans[i] = GetMagicXor(matr[i]);
    }

    return ans;
}

// ---------------------------------------------------------------------------
// Задание 3. Точка на плоскости

// Функция возвращает номер Вашего варианта.
int GetVariant() {
    return 4;
}
// Функция определяет, лежит ли точнка в указанной области.
bool IsPointInArea(double x, double y) {
    long double eps = 1e-9;
    if (x <= eps && y + eps >= 0) {
        // quarter 2
        return (x * x + y * y <= 25 + eps) &&
               (x <= y * y - 1.5 * y - 2.5 + eps) &&
               (eps + y >= -2 * x - 7) &&
               (x + eps> -3.5 || y <= eps + tan(x / 2.));
    } else if (x + eps >= 0 && y < eps) {
        // quarter 4
        return (x * x + y * y + eps>= 25) &&
               (x < 3.2 + eps || y + eps >= tan(x / 2.)) &&
               (x < 4. + eps);
    }

    return false;
}

// ---------------------------------------------------------------------------
// Задание 4. Сортировки

void SelectionSort(vector<string> &vec) {
    int sz = vec.size();

    for (int i = 0; i + 1 < sz; i++) {
        int min_element_index = i + 1;
        for (int j = i + 2; j < sz; j++) {
            if (vec[min_element_index] > vec[j]) {
                min_element_index = j;
            }
        }
        if (vec[i] > vec[min_element_index]) {
            swap(vec[i], vec[min_element_index]);
        }
    }
}

void ExcahngeSort(vector<string> &vec) {
    int sz = vec.size();

    for (int i = 0; i + 1 < sz; i++) {
        for (int j = i + 1; j < sz; j++) {
            if (vec[i] > vec[j]) {
                swap(vec[i], vec[j]);
            }
        }
    }
}

void BubbleSort(vector<string> &vec) {
    int sz = vec.size();

    for (int i = 0; i < sz - 1; i++) {
        for (int j = 0; j < sz - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void ShakerSort(vector<string> &vec) {
    int li = 0;
    int ri = static_cast<int>(vec.size()) - 1;

    while (li < ri) {
        for (int i = li; i < ri; i++) {
            if (vec[i] > vec[i + 1]) {
                swap(vec[i], vec[i + 1]);
            }
        }
        ri--;
        for (int i = ri; i > li; i--) {
            if (vec[i - 1] > vec[i]) {
                swap(vec[i - 1], vec[i]);
            }
        }
        li++;
    }
}

// ---------------------------------------------------------------------------
// Задание 5. Бинарный поиск

int FindRecursively(const vector<int> &vec, int l, int r, int value) {
    if (l > r) {
        return -1;
    }

    int mid = l + (r - l) / 2;

    if (value == vec[mid]) {
        return mid;
    } else if (value > vec[mid]) {
        return FindRecursively(vec, mid + 1, r, value);
    } else {
        return FindRecursively(vec, l, mid - 1, value);
    }
}

int FindRecursively(const vector<int> &vec, int value) {
    return FindRecursively(vec, 0, static_cast<int>(vec.size()) - 1, value);
}

int FindIteratively(const vector<int> &vec, int value) {
    int l = 0;
    int r = static_cast<int>(vec.size()) - 1;

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (vec[mid] == value) {
            return mid;
        } else if (value > vec[mid]) {
            l = mid + 1;
        } else if (value < vec[mid]) {
            r = mid - 1;
        }
    }

    return -1;
}

// ---------------------------------------------------------------------------

#ifndef IGNORE_SOLUTION_MAIN

int main() {
    return 0;
}

#endif  // IGNORE_SOLUTION_MAIN
