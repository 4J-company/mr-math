#include "gtest/gtest.h"
#include "mr-math/math.hpp"

using namespace mr::literals;

TEST(TrigonometricApprox, Sin) {
    EXPECT_NEAR(mr::sin(0.0), 0.0, 1e-6);
    EXPECT_NEAR(mr::sin(0.5235987756), 0.5, 1e-3);
    EXPECT_NEAR(mr::sin(1.0471975512), 0.86602540378, 1e-2);

    double x = 0.7;
    EXPECT_NEAR(mr::sin(-x), -mr::sin(x), 1e-6);
}

TEST(TrigonometricApprox, Cos) {
    EXPECT_NEAR(mr::cos(0.0), 1.0, 1e-6);
    EXPECT_NEAR(mr::cos(1.0471975512), 0.5, 1e-2);
    EXPECT_NEAR(mr::cos(0.5235987756), 0.86602540378, 1e-3);

    double x = 0.9;
    EXPECT_NEAR(mr::cos(-x), mr::cos(x), 1e-6);
}

TEST(TrigonometricApprox, Tan) {
    EXPECT_NEAR(mr::tan(0.0), 0.0, 1e-6);
    EXPECT_NEAR(mr::tan(0.78539816339), 1.0, 2e-2);
    EXPECT_NEAR(mr::tan(0.5235987756), 0.57735026919, 1e-3);

    double x = 0.5;
    EXPECT_NEAR(mr::tan(-x), -mr::tan(x), 1e-6);
}

TEST(TrigonometricApprox, Cot) {
    EXPECT_NEAR(mr::cot(0.78539816339), 1.0, 1e-2);
    EXPECT_NEAR(mr::cot(0.5235987756), 1.73205080757, 1e-2);

    double x = 0.6;
    EXPECT_NEAR(mr::cot(-x), -mr::cot(x), 1e-5);

    double small = 0.1;
    EXPECT_NEAR(mr::cot(small), 1.0 / small, 0.04);
}

TEST(TrigonometricApprox, Consistency) {
    double x = 0.7;
    double s = mr::sin(x);
    double c = mr::cos(x);
    if (c != 0.0) {
        EXPECT_NEAR(mr::tan(x), s / c, 1e-2);
        EXPECT_NEAR(mr::cot(x), c / s, 1e-2);
    }
}
