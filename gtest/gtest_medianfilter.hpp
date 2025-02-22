#include "gtest/gtest.h"
#include "gmock/gmock.h" 

#include <QDebug>

#include "medianfilter.hpp"



using ::testing::AtLeast;

namespace gtest_medianfilter
{
    class GTest_medianfilter : public ::testing::Test
    {
        protected:
            GTest_medianfilter(){}
            ~GTest_medianfilter() override {}
            void SetUp() override{}
            void TearDown() override {}
    };
}  // namespace gtest_medianfilter
