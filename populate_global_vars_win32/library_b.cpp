// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-11-29
//--------------------------------------------------------------------------

#include "library_a.hpp"
#include <iostream>

__declspec(dllexport) int global_var_lib_b{22222};

__declspec(dllexport) void function_lib_b() {
    std::cout << "function from library B called with var from library A = "
              << global_var_lib_a << "\n";
}
