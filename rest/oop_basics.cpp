#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

// В данной лабораторной работе можно не писать каждый раз "std::".
using namespace std;

// ---------------------------------------------------------------------------
// Задание 1.

// Могут быть объявлены как поля класса для работы с датами.
bool IsLeapYear(int year) {
    return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);
}

struct Date {
    int day   = 0;
    int month = 0;
    int year  = 0;
    bool is_leap = false;

    Date() {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {
        is_leap = IsLeapYear(y);
    }
};

bool operator< (const Date &a, const Date &b) {
    return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

bool operator> (const Date &a, const Date &b) {
    return std::tie(a.year, a.month, a.day) > std::tie(b.year, b.month, b.day);
}

bool operator<= (const Date &a, const Date &b) {
    return std::tie(a.year, a.month, a.day) <= std::tie(b.year, b.month, b.day);
}

bool operator>= (const Date &a, const Date &b) {
    return std::tie(a.year, a.month, a.day) >= std::tie(b.year, b.month, b.day);
}

Date ReadDate(ifstream & fin) {
    int day;
    int month;
    int year;
    fin >> day >> month >> year;
    return Date(day, month, year);
}

void FormatDate(string &date, int len) {
    string add_str(max(0, len - static_cast<int>(date.length())), '0');
    date = add_str + date;
}

string DateToString(const Date& date) {
    string day   = to_string(date.day);
    string month = to_string(date.month);
    string year  = to_string(date.year);

    FormatDate(day, 2);
    FormatDate(month, 2);
    FormatDate(year, 4);

    return day + '.' + month + '.' + year;
}

vector<int> days = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

string GetPrevDate(Date date) {
    date.day--;
    if (date.day == 0) {
        date.month--;
        if (date.month == 0) {
            date = Date(31, 12, date.year - 1);
        } else {
            if (date.month == 2) {
                date = Date(28 + date.is_leap, 2, date.year);
            } else {
                date = Date(days[date.month], date.month, date.year);
            }
        }
    }
    return DateToString(date);
}

string GetNextDate(Date date) {
    date.day++;
    if (date.day > days[date.month] + (date.month == 2 && date.is_leap)) {
        date.month++;
        if (date.month == 13) {
            date = Date(1, 1, date.year + 1);
        } else {
            date = Date(1, date.month, date.year);
        }
    }
    return DateToString(date);
}

int CountDaysBeforeDateInYear(const Date &date) {
    int d = 0;
    for (int i = 1; i < date.month; i++) {
        d += days[i] + (i == 2 && date.is_leap);
    }
    d += date.day;
    return d-1;
}

int ToDays(const Date &date) {
    return   CountDaysBeforeDateInYear(date) + 1 +
             (date.year - 1) * 365 +
             (date.year - 1) / 4 - (date.year - 1) / 100 + (date.year - 1) / 400;
}

// ---------------------------------------------------------------------------
// Задание 2.

void ProcessDates(const string& input_filename, const string& output_filename) {
    std::ifstream fin(input_filename);
    int n_dates;
    fin >> n_dates;
    vector<Date> dates(n_dates);

    for (int i = 0; i < n_dates; i++) {
        dates[i] = ReadDate(fin);
    }
    fin.close();

    std::ofstream fout(output_filename);
    for (int i = 0; i < n_dates; i++) {
        fout << GetPrevDate(dates[i]) << " " << GetNextDate(dates[i]) << endl;
    }

    fout.close();
}

// ---------------------------------------------------------------------------
// Задание 3.

class Calculator {
public:
    Calculator()
            :
            value_(0)
    {};
    explicit Calculator(int value)  // explicit - потому что ЯК так хочет)
            :
            value_(value)
    {};
    int GetResult();
    void Add(int value);
    void Subtract(int value);
    void Multiply(int value);
    void Divide(int value);
    void Modulo(int value);
private:
    int value_;
};

int Calculator::GetResult() {
    return value_;
}

void Calculator::Add(int value) {
    value_ += value;
}

void Calculator::Subtract(int value) {
    value_ -= value;
}

void Calculator::Multiply(int value) {
    value_ *= value;
}

void Calculator::Divide(int value) {
    value_ /= value;
}

void Calculator::Modulo(int value) {
    value_ %= value;
}

// ---------------------------------------------------------------------------
// Задание 4.

class Employee {
public:
    Employee(const string &fname, const string &lname,
             const Date &bdate,   const Date &hdate)
            :
            first_name_(fname),
            last_name_(lname),
            birth_date_(bdate),
            hiring_date_(hdate)
    {};
    string GetFullName() const;
    string ToString() const;
    int GetAgeInYears(const Date& current_date) const;
    int GetTenureInDays(const Date& current_date) const;
private:
    string first_name_;
    string last_name_;
    Date birth_date_;
    Date hiring_date_;
};

string Employee::GetFullName() const {
    return first_name_ + " " + last_name_;
}

string Employee::ToString() const {
    return GetFullName() + " " + DateToString(birth_date_)
           +" -> hired " + DateToString(hiring_date_);
}

int Employee::GetAgeInYears(const Date& current_date) const {
    int ans = current_date.year - birth_date_.year;
    bool was_birthday = (std::tie(birth_date_.month, birth_date_.day) >
                         std::tie(current_date.month, current_date.day));
    return std::max(0, ans - was_birthday);
}

int Employee::GetTenureInDays(const Date &current_date) const {
    if (current_date < hiring_date_) {
        return 0;
    } else {
        return ToDays(current_date) - ToDays(hiring_date_);
    }
}

// ---------------------------------------------------------------------------
#ifndef IGNORE_SOLUTION_MAIN
int main() {
    const Employee employee = Employee("", "",
                                       Date(1, 1, 2000), Date(31, 12, 1599));
    cout << employee.GetTenureInDays(Date(1, 1, 1601));
    return 0;
}
#endif  // IGNORE_SOLUTION_MAIN
