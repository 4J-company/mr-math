#include "gtest/gtest.h"
#include "mr-math/math.hpp"

class Vector3DTest : public ::testing::Test {
protected:
  mr::Vec3f v1{1.0, 2.0, 3.0};
  mr::Vec3f v2{4.0, 5.0, 6.0};
};

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
    mr::Matr4f::RowT{1, 2, 3, 4},
    mr::Matr4f::RowT{5, 6, 7, 8},
    mr::Matr4f::RowT{9, 10, 11, 12},
    mr::Matr4f::RowT{13, 14, 15, 16}
  };
  mr::Matr4f m2 {
    mr::Matr4f::RowT{17, 18, 19, 20},
    mr::Matr4f::RowT{21, 22, 23, 24},
    mr::Matr4f::RowT{25, 26, 27, 28},
    mr::Matr4f::RowT{29, 30, 31, 32}
  };
};
// Test Matrix addition
TEST_F(MatrixTest, Addition) {
    mr::Matr4f result = m1 + m2;
    mr::Matr4f expected {
      mr::Matr4f::RowT{18, 20, 22, 24},
      mr::Matr4f::RowT{26, 28, 30, 32},
      mr::Matr4f::RowT{34, 36, 38, 40},
      mr::Matr4f::RowT{42, 44, 46, 48}
    };

    EXPECT_EQ(result[0][0], expected[0][0]);
    EXPECT_EQ(result[0][1], expected[0][1]);
    EXPECT_EQ(result[0][2], expected[0][2]);
    EXPECT_EQ(result[0][3], expected[0][3]);

    EXPECT_EQ(result[1][0], expected[1][0]);
    EXPECT_EQ(result[1][1], expected[1][1]);
    EXPECT_EQ(result[1][2], expected[1][2]);
    EXPECT_EQ(result[1][3], expected[1][3]);

    EXPECT_EQ(result[2][0], expected[2][0]);
    EXPECT_EQ(result[2][1], expected[2][1]);
    EXPECT_EQ(result[2][2], expected[2][2]);
    EXPECT_EQ(result[2][3], expected[2][3]);

    EXPECT_EQ(result[3][0], expected[3][0]);
    EXPECT_EQ(result[3][1], expected[3][1]);
    EXPECT_EQ(result[3][2], expected[3][2]);
    EXPECT_EQ(result[3][3], expected[3][3]);
}

// Test Matrix multiplication
TEST_F(MatrixTest, Multiplication) {
    mr::Matr4f result = m1 * m2;
    mr::Matr4f expected {
      mr::Matr4f::RowT{250, 260, 270, 280},
      mr::Matr4f::RowT{618, 644, 670, 696},
      mr::Matr4f::RowT{986, 1028, 1070, 1112},
      mr::Matr4f::RowT{1354, 1412, 1470, 1528}
    };

    EXPECT_EQ(result[0][0], expected[0][0]);
    EXPECT_EQ(result[0][1], expected[0][1]);
    EXPECT_EQ(result[0][2], expected[0][2]);
    EXPECT_EQ(result[0][3], expected[0][3]);

    EXPECT_EQ(result[1][0], expected[1][0]);
    EXPECT_EQ(result[1][1], expected[1][1]);
    EXPECT_EQ(result[1][2], expected[1][2]);
    EXPECT_EQ(result[1][3], expected[1][3]);

    EXPECT_EQ(result[2][0], expected[2][0]);
    EXPECT_EQ(result[2][1], expected[2][1]);
    EXPECT_EQ(result[2][2], expected[2][2]);
    EXPECT_EQ(result[2][3], expected[2][3]);

    EXPECT_EQ(result[3][0], expected[3][0]);
    EXPECT_EQ(result[3][1], expected[3][1]);
    EXPECT_EQ(result[3][2], expected[3][2]);
    EXPECT_EQ(result[3][3], expected[3][3]);
}
