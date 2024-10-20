#ifndef TEST_TEMPLATE_CLASS_HPP
#define TEST_TEMPLATE_CLASS_HPP
// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-19

#include <iostream>
#include <array>
#include <cstring>

/// String to integer map entry.
struct str_int_entry {
    const char* name; ///< A value in string form.
    const int id;     ///< Same value in integer form.
};

static constexpr std::array<str_int_entry, 2> arr1{
    {{"GET", 1}, {"POST", 2}}};

static constexpr std::array<str_int_entry, 3> arr2{
    {{"M-SEARCH", 1}, {"NOTIFY", 2}, {"SUBSCRIBE", 5}}};


template <typename T>
class CStrIntMap {
  public:
    CStrIntMap(T& a_table) : m_table(a_table) {}
    size_t get_str_int(const char* name);

  private:
    T& m_table;
};


int main() {
    CStrIntMap table1(arr1);
    size_t idx = table1.get_str_int("POST");
    std::cout << "table1 idx = " << idx << '\n';

    CStrIntMap table2(arr2);
    idx = table2.get_str_int("SUBSCRIBE");
    std::cout << "table2 idx = " << idx << '\n';
    return 0;
}

#endif // TEST_TEMPLATE_CLASS_HPP
