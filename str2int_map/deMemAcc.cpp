// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-13

#include <iostream>

class Base {
  public:
    virtual int get_var() = 0;
    void print_var() {
        std::cout << "member variable from Base = " << this->get_var() << '\n';
    }
};

class Derived1 : public Base {
  public:
    int get_var() override { return x; }

  private:
    static constexpr int x{1};
} derived1;

class Derived2 : public Base {
  public:
    int get_var() override { return x; }

  private:
    static constexpr int x{2};
} derived2;


int main() {
    Base* current = &derived1;
    std::cout << "Current member variable = " << current->get_var() << '\n';
    current->print_var();

    current = &derived2;
    std::cout << "Current member variable = " << current->get_var() << '\n';
    current->print_var();

    return 0;
}
