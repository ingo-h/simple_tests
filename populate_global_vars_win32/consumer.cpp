// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-11-29
//--------------------------------------------------------------------------

#include "library_a.hpp"
#include "library_b.hpp"
#include <iostream>

int main() {
    function_lib_a();
    function_lib_b();
    std::cout << "Global variable = " << global_var_lib_a
              << " from library A called.\n";
    std::cout << "Global variable = " << global_var_lib_b
              << " from library B called.\n";
    return 0;
}
