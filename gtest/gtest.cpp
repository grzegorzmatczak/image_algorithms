#include "gtest/gtest.h"
#include "gmock/gmock.h" 
#include "gtest_medianfilter.hpp"

using ::testing::AtLeast;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
