// Copyright (C) 2022 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2022-08-05
// -------------------------------------------------------------------------

#include "gtest/gtest.h"

TEST(PipeTestSuite, capture_stderr) {
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
