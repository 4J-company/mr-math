#include "gtest/gtest.h"
#include "mr-math/math.hpp"

class Vector3DTest : public ::testing::Test {
protected:
  mr::Vec3f v1{1.0, 2.0, 3.0};
  mr::Vec3f v2{4.0, 5.0, 6.0};
};

TEST_F(Vector3DTest, Getters) {
  EXPECT_EQ(v1.x(), 1.0);
  EXPECT_EQ(v1.y(), 2.0);
  EXPECT_EQ(v1.z(), 3.0);
  EXPECT_EQ(v1[0], 1.0);
  EXPECT_EQ(v1[1], 2.0);
  EXPECT_EQ(v1[2], 3.0);
}

TEST_F(Vector3DTest, Equality) {
  auto copy = v1;
  EXPECT_EQ(v1, copy);
  EXPECT_TRUE(v1.equal(copy));
  EXPECT_TRUE(equal(v1, copy));

  EXPECT_NE(v1, v2);
  EXPECT_FALSE(v1.equal(v2));
  EXPECT_FALSE(equal(v1, v2));
}

TEST_F(Vector3DTest, Addition) {
  mr::Vec3f result = v1 + v2;
  EXPECT_EQ(result.x(), 5.0);
  EXPECT_EQ(result.y(), 7.0);
  EXPECT_EQ(result.z(), 9.0);
}

TEST_F(Vector3DTest, Subtraction) {
  mr::Vec3f result = v1 - v2;
  EXPECT_EQ(result.x(), -3.0);
  EXPECT_EQ(result.y(), -3.0);
  EXPECT_EQ(result.z(), -3.0);
}

TEST_F(Vector3DTest, DotProduct) {
  auto result = v1.dot(v2);
  EXPECT_EQ(result, 32.0);
}

TEST_F(Vector3DTest, CrossProduct) {
  auto result1 = v1.cross(v2);
  mr::Vec3f expected{-3, 6, -3};
  EXPECT_EQ(result1, expected);

  auto result2 = v2.cross(v1);
  EXPECT_EQ(result2, -expected);
}

TEST_F(Vector3DTest, Length) {
  EXPECT_NEAR(v1.length(), std::sqrt(14.0f), 0.0001);
  EXPECT_NEAR(v1.length2(), 14.0f, 0.0001);
  EXPECT_NEAR(v1.inversed_length(), 1 / std::sqrt(14.0f), 0.01);
}

TEST_F(Vector3DTest, Normalize) {
  auto norm = v1.normalized_unchecked();
  EXPECT_TRUE(norm.equal({0.267261, 0.534522, 0.801784}));

  auto null = mr::Vec3f{0, 0, 0}.normalized();
  EXPECT_FALSE(null.has_value());
}

class MatrixTest : public ::testing::Test {
protected:
  mr::Matr4f m1 {
    1,  2,  3,  4 ,
    5,  6,  7,  8 ,
    9,  10, 11, 12,
    13, 14, 15, 16
  };
  mr::Matr4f m2 {
    17, 18, 19, 20,
    21, 22, 23, 24,
    25, 26, 27, 28,
    29, 30, 31, 32
  };
};

TEST_F(MatrixTest, Getters) {
  EXPECT_EQ(m1[0][0],  1.0);
  EXPECT_EQ(m1[0][1],  2.0);
  EXPECT_EQ(m1[0][2],  3.0);
  EXPECT_EQ(m1[0][3],  4.0);
  EXPECT_EQ(m1[1][0],  5.0);
  EXPECT_EQ(m1[1][1],  6.0);
  EXPECT_EQ(m1[1][2],  7.0);
  EXPECT_EQ(m1[1][3],  8.0);
  EXPECT_EQ(m1[2][0],  9.0);
  EXPECT_EQ(m1[2][1], 10.0);
  EXPECT_EQ(m1[2][2], 11.0);
  EXPECT_EQ(m1[2][3], 12.0);
  EXPECT_EQ(m1[3][0], 13.0);
  EXPECT_EQ(m1[3][1], 14.0);
  EXPECT_EQ(m1[3][2], 15.0);
  EXPECT_EQ(m1[3][3], 16.0);
}

TEST_F(MatrixTest, Equality) {
  mr::Matr4f copy = m1;
  EXPECT_EQ(m1, copy);
  EXPECT_TRUE(m1.equal(copy));
  EXPECT_TRUE(equal(m1, copy));

  EXPECT_NE(m1, m2);
  EXPECT_FALSE(m1.equal(m2));
  EXPECT_FALSE(equal(m1, m2));
}

TEST_F(MatrixTest, Addition) {
  mr::Matr4f result = m1 + m2;
  mr::Matr4f expected {
    18, 20, 22, 24,
    26, 28, 30, 32,
    34, 36, 38, 40,
    42, 44, 46, 48
  };

  EXPECT_TRUE(equal(result, expected));
}

TEST_F(MatrixTest, Multiplication) {
  mr::Matr4f result = m1 * m2;
  mr::Matr4f expected {
    250,  260,  270,  280,
    618,  644,  670,  696,
    986,  1028, 1070, 1112,
    1354, 1412, 1470, 1528
  };

   EXPECT_EQ(result, expected);
}

TEST_F(MatrixTest, Transposition) {
  mr::Matr4f expected {
    1, 5, 9,  13,
    2, 6, 10, 14,
    3, 7, 11, 15,
    4, 8, 12, 16
  };

  EXPECT_EQ(m1.transposed(), expected);
  EXPECT_EQ(m1.transpose(), expected);
}

TEST_F(MatrixTest, Determinant) {
  mr::Matr4f magic_m {
    1, 2, 3, 4,
    2, 2, 0, 0,
    3, 0, 3, 0,
    4, 0, 0, 4
  };

  EXPECT_EQ(magic_m.determinant(), -192);
}

TEST_F(MatrixTest, Inversion) {
  mr::Matr4f magic_m {
    30,  47, 80, 102,
    47,  47, 0,    0,
    80,  0,  80,   0,
    102, 0,  0,  102
  };
  mr::Matr4f expected {
    -1/199.,    1/199.,     1/199.,    1/199.,
     1/199., 152/9353.,    -1/199.,   -1/199.,
     1/199.,   -1/199., 119/15920.,   -1/199.,
     1/199.,   -1/199.,    -1/199., 97/20298.
  };

  EXPECT_EQ(magic_m.inversed(), expected);
  EXPECT_EQ(magic_m.inverse(), expected);
}
