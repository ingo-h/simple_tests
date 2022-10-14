// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-10-14
//--------------------------------------------------------------------------

#include "../visibility.hpp"
#include "library_a.hpp"
#include <iostream>

UPNPLIB_API int var_from_lib_a{43690}; // This is 0xAAAA

void called_from_lib_a() { std::cout << "function from library A called.\n"; }
