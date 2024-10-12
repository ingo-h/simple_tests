// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-13

#include <iostream>
#include <array>
#include <cstring>

/// String to integer map entry.
struct str_int_entry {
    const char* name; ///< A value in string form.
    const int id;     ///< Same value in integer form.
};

constexpr std::array<str_int_entry, 2uz> data_tbl1{
    {{"GET", 1}, {"POST", 2}}};

constexpr std::array<str_int_entry, 3uz> data_tbl2{
    {{"M-SEARCH", 1}, {"NOTIFY", 2}, {"SUBSCRIBE", 5}}};


template <typename T>
int str_to_int(const char* name, const T& table) {
    if (name == nullptr || name[0] == '\0')
        return -1;
    for (size_t i{0}; i < table.size(); i++) {
        if (strcasecmp(name, table[i].name) == 0)
            return i;
    }
    return -1;
}


int main() {
    int res = str_to_int("GET", data_tbl1);
    std::cout << "Result1 = " << res << '\n';

    res = str_to_int("SUBSCRIBE", data_tbl2);
    std::cout << "Result2 = " << res << '\n';

    res = str_to_int("GETX", data_tbl1);
    std::cout << "Result1 failure = " << res << '\n';

    res = str_to_int("SUBSCRIB", data_tbl2);
    std::cout << "Result2 failure = " << res << '\n';

    return 0;
}
