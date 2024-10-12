// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-14

#include <iostream>
#include <array>

static constexpr size_t m_table_size{2};
class CData {
  public:
    static constexpr std::array<int, m_table_size> m_table{1, 2};
};

class CCode {
  public:
    virtual std::array<int, m_table_size> get_var() = 0;
    void print_var() {
        std::cout << "member table size from CCode = " << this->get_var().size()
                  << '\n';
    }
};

class Derived : public CCode, public CData {
  public:
    std::array<int, m_table_size> get_var() override { return m_table; }
} derived;


int main() {
    CCode* current = &derived;
    std::cout << "Current table first entry = " << current->get_var()[0] << '\n';
    current->print_var();

    return 0;
}
