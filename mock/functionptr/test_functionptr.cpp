// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-06-22

#include <iostream>

int add(int a, int b) { return a + b; }
int multi(int a, int b) { return a * b; }

int (*functionPtr)(int, int);

typedef int (*Operation)(int a, int b);
Operation opt;

//
int main(int argc, char** argv) {
    // Using function pointer direct
    functionPtr = &add;
    std::cout << "Using function pointer direct: result is "
              << std::to_string((*functionPtr)(5, 3)) << "\n";

    functionPtr = multi;
    std::cout << "Using function pointer direct: result is "
              << std::to_string((*functionPtr)(5, 3)) << "\n";

    // Using typedef
    opt = add;
    std::cout << "Using typedef: result is " << std::to_string(opt(6, 3))
              << "\n";

    opt = &multi;
    std::cout << "Using typedef: result is " << std::to_string(opt(6, 3))
              << "\n";

    return 0;
}
