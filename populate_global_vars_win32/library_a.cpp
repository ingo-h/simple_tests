// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-11-29
//--------------------------------------------------------------------------

#include <iostream>

__declspec(dllexport) int global_var_lib_a{11111};

__declspec(dllexport) void function_lib_a() {
    std::cout << "function from library A called.\n";
}
