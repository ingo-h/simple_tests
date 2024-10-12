// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-12

#include <iostream>

class Base {
  public:
    int get_var() { return *p_x; }

  protected:
    const int* p_x;
};

class Derived : public Base {
  public:
    Derived() { p_x = &x; }

  private:
    static constexpr int x{3};
} derived;

int main() {
    // Base* p_derived = &derived;
    std::cout << "Derived member variable = " << derived.get_var() << '\n';
    return 0;
}
