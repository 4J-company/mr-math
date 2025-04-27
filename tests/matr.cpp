#include "gtest/gtest.h"
#include "mr-math/math.hpp"

using namespace mr::literals;

class Matrix : public ::testing::Test {
protected:
  mr::Matr4f m1{
    1,  2,  3,  4 ,
    5,  6,  7,  8 ,
    9,  10, 11, 12,
    13, 14, 15, 16
  };
  mr::Matr4f m2{
    17, 18, 19, 20,
    21, 22, 23, 24,
    25, 26, 27, 28,
    29, 30, 31, 32
  };
};

TEST_F(Matrix, Getters) {
    EXPECT_EQ(m1[0][0], 1.0);
    EXPECT_EQ(m1[0][1], 2.0);
    EXPECT_EQ(m1[0][2], 3.0);
    EXPECT_EQ(m1[0][3], 4.0);
    EXPECT_EQ(m1[1][0], 5.0);
    EXPECT_EQ(m1[1][1], 6.0);
    EXPECT_EQ(m1[1][2], 7.0);
    EXPECT_EQ(m1[1][3], 8.0);
    EXPECT_EQ(m1[2][0], 9.0);
    EXPECT_EQ(m1[2][1], 10.0);
    EXPECT_EQ(m1[2][2], 11.0);
    EXPECT_EQ(m1[2][3], 12.0);
    EXPECT_EQ(m1[3][0], 13.0);
    EXPECT_EQ(m1[3][1], 14.0);
    EXPECT_EQ(m1[3][2], 15.0);
    EXPECT_EQ(m1[3][3], 16.0);
}

TEST_F(Matrix, Equality) {
    mr::Matr4f copy = m1;
    EXPECT_EQ(m1, copy);
    EXPECT_TRUE(m1.equal(copy));
    EXPECT_TRUE(equal(m1, copy));

    EXPECT_NE(m1, m2);
    EXPECT_FALSE(m1.equal(m2));
    EXPECT_FALSE(equal(m1, m2));
}

TEST_F(Matrix, Addition) {
    mr::Matr4f result = m1 + m2;
    mr::Matr4f expected{
      18, 20, 22, 24,
      26, 28, 30, 32,
      34, 36, 38, 40,
      42, 44, 46, 48
    };

    EXPECT_TRUE(equal(result, expected));
}

TEST_F(Matrix, Multiplication) {
    mr::Matr4f result = m1 * m2;
    mr::Matr4f expected{
      250,  260,  270,  280,
      618,  644,  670,  696,
      986,  1028, 1070, 1112,
      1354, 1412, 1470, 1528
    };

    EXPECT_EQ(result, expected);
}

TEST_F(Matrix, Transposition) {
    mr::Matr4f expected{
      1, 5, 9,  13,
      2, 6, 10, 14,
      3, 7, 11, 15,
      4, 8, 12, 16
    };
    EXPECT_EQ(m1.transposed(), expected);

    mr::Matr4f copy = m1;
    EXPECT_EQ(copy.transpose(), expected);
}

TEST_F(Matrix, Identity) {
    mr::Matr4f expected{
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
    EXPECT_EQ(mr::Matr4f::identity(), expected);
}

TEST_F(Matrix, InverseIdentity) {
    const auto id = mr::Matr4f::identity();
    const auto inv = id.inversed();

    ASSERT_TRUE(inv.has_value());
    EXPECT_TRUE(equal(id, inv.value()));
}

TEST_F(Matrix, Inverse2x2) {
    mr::Matr<float, 2> m = {
      mr::Matr<float, 2>::RowT(1.f, 2.f),
      mr::Matr<float, 2>::RowT(3.f, 4.f),
    };

    const auto inv = m.inversed();
    ASSERT_TRUE(inv.has_value());

    mr::Matr<float, 2> expected = {
      mr::Matr<float, 2>::RowT(-2.f, 1.f),
      mr::Matr<float, 2>::RowT(1.5f, -0.5f),
    };

    const auto res = m * inv.value();
    const auto id = mr::Matr<float, 2>::identity();

    EXPECT_TRUE(equal(inv.value(), expected, 0.000001));
    EXPECT_TRUE(equal(res, id, 0.000001));
}

TEST_F(Matrix, Inverse3x3) {
    mr::Matr3f m = {
      mr::Matr3f::RowT{2.f, 5.f, 7.f},
      mr::Matr3f::RowT{6.f, 3.f, 4.f},
      mr::Matr3f::RowT{5.f, -2.f, -3.f}
    };

    const auto inv = m.inversed();
    ASSERT_TRUE(inv.has_value());

    mr::Matr3f expected = {
      mr::Matr3f::RowT{1.f, -1.f, 1.f},
      mr::Matr3f::RowT{-38.f, 41.f, -34.f},
      mr::Matr3f::RowT{27.f, -29.f, 24.f}
    };

    EXPECT_TRUE(equal(inv.value(), expected, 0.001));
}
