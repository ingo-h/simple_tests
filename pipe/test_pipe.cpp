// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-08-05
//------------------------------------------------------------------------------

#include "upnplib_gtest_tools.hpp"
#include "gtest/gtest.h"

namespace upnplib {

void errout() {
    // clang-format off
//  for (int i = 0; i < 63; i++)
    std::cerr << "This is a string of 0128 bytes.                                                                                  end of string.\n"
                 "This is a string of 0256 bytes.                                                                                  end of string.\n"
                 "This is a string of 0384 bytes.                                                                                  end of string.\n"
                 "This is a string of 0512 bytes.                                                                                  end of string.\n"
                 "This is a string of 0640 bytes.                                                                                  end of string.\n"
                 "This is a string of 0768 bytes.                                                                                  end of string.\n"
                 "This is a string of 0896 bytes.                                                                                  end of string.\n"
                 "This is a string of 1024 bytes.                                                                                  end of string.\n";
#if 0
    std::cerr << "This is a string of 0128 bytes.                                                                                  end of string.\n"
                 "This is a string of 0256 bytes.                                                                                  end of string.\n"
                 "This is a string of 0384 bytes.                                                                                  end of string.\n"
                 "This is a string of 0512 bytes.                                                                                  end of string.\n"
                 "This is a string of 0640 bytes.                                                                                  end of string.\n"
                 "This is a string of 0768 bytes.                                                                                  end of string.\n"
                 "This is a string of 0896 bytes.                                                                                  end of string.\n"
                 "This is a string of 1019 bytes.                                                                         end of string.\n";
#endif
    // clang-format on
}

TEST(PipeTestSuite, capture_stderr) {

    for (int i = 0; i < 129; i++) {
        CCaptureStdOutErr captErrObj(STDERR_FILENO);

        captErrObj.start();
        errout();
        std::string captured_err = captErrObj.get();

        std::cout << "CAPTURED: " << i + 1 << " times\n" << captured_err;
        std::cerr << "Check output to stderr\n";
        std::cout << "Check output to stdout\n";
    }
}

} // namespace upnplib

//------------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
// vim: nowrap
