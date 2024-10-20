// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-14

#include <iostream>
#include <array>

class CData1 {
  public:
    static constexpr std::array<int, 2> m_table{1, 2};
};

class CData2 {
  public:
    static constexpr std::array<int, 3> m_table{3, 4, 5};
};

class CCode {
  public:
    void print_size() {
        std::cout << "member table size from CCode = " << m_table.size()
                  << '\n';
    }
};

class CMyTable1 : CData1, CCode {
}


int main() {
    std::cout << "mytable first entry = " << mygtable.m_table[0] << '\n';
    mytable.print_size();

    return 0;
}

