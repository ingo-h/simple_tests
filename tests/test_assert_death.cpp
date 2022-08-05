// Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2021-09-03

#include "gtest/gtest.h"

struct myStringStruct {
    int length;
    char message[15];
};

char* return_message(myStringStruct* strPtr) {
    return(strPtr->message);
}

TEST(MyDeathTest, return_string) {
    myStringStruct myString[] = {11, "hello world"};
    myStringStruct* myStrPtr = *&myString;

    std::cout << return_message(myStrPtr) << std::endl;

    EXPECT_DEATH(return_message((myStringStruct*)NULL), "");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
