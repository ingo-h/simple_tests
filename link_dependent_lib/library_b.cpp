// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-10-14
//--------------------------------------------------------------------------

#include "library_b.hpp"

#include "library_a.hpp"
#include <iostream>

UPNPLIB_EXTERN int var_from_lib_a;

void called_from_lib_b() {
    std::cout << "function from library B called with var from library A = "
              << var_from_lib_a << "\n";
}
