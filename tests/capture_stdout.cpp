// Does not work with googletest because it does not use <sstream> but instead
// <stdio.h> with printf() etc.
// Reference: https://coderedirect.com/questions/247722/how-to-capture-stdout-stderr-with-googletest
// Compile with: g++ -std=c++20 -pedantic-errors -Wall capture_stdout.cpp

#include <iostream>
#include <sstream>

int main() {

    // This can be an ofstream as well or any other ostream
    std::stringstream buffer;

    // Save cout's buffer here
    std::streambuf* sbuf = std::cout.rdbuf();

    // Redirect cout to our stringstream buffer or any other ostream
    std::cout.rdbuf(buffer.rdbuf());

    // Use cout as usual
    std::cout << "Hello World";

    // When done redirect cout to its old self
    std::cout.rdbuf(sbuf);

    std::cout << buffer.str() << std::endl;

    return 0;
}
