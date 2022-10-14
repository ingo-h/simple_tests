// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-10-14
//--------------------------------------------------------------------------

#include "library_a.hpp"
#include "library_b.hpp"

int main(int argc, char* argv[]) {
    called_from_lib_a();
    called_from_lib_b();
    return 0;
}
