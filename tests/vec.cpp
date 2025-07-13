#include "gtest/gtest.h"
#include "mr-math/math.hpp"

using namespace mr::literals;

class Vector3D : public ::testing::Test {
protected:
  mr::Vec3f v1{ 1.0, 2.0, 3.0 };
  mr::Vec3f v2{ 4.0, 5.0, 6.0 };
};

TEST_F(Vector3D, Constructors) {
  EXPECT_EQ(mr::Vec3f(), mr::Vec3f(0, 0, 0));
  EXPECT_EQ(mr::Vec3f(1), mr::Vec3f(1, 1, 1));
  EXPECT_EQ(mr::Vec3f(mr::Vec2f(1, 2)), mr::Vec3f(1, 2, 0));
  EXPECT_EQ(mr::Vec3f(mr::Vec2f(1, 2), 3), mr::Vec3f(1, 2, 3));
  EXPECT_EQ(mr::Vec3f(mr::Vec4f(1, 2, 3, 4)), mr::Vec3f(1, 2, 3));

  EXPECT_EQ(mr::Vec3f(std::span<const int, 2>{{1, 2}}), mr::Vec3f(1, 2, 0));
  EXPECT_EQ(mr::Vec3f(std::span<const int, 3>{{1, 2, 3}}), mr::Vec3f(1, 2, 3));
  EXPECT_EQ(mr::Vec3f(std::span<const int, 4>{{1, 2, 3, 4}}), mr::Vec3f(1, 2, 3));
  EXPECT_EQ(mr::Vec3f(std::span<const int>{{1, 2, 3}}), mr::Vec3f(1, 2, 3));
}

TEST_F(Vector3D, Getters) {
  EXPECT_EQ(v1.x(), 1.0);
  EXPECT_EQ(v1.y(), 2.0);
  EXPECT_EQ(v1.z(), 3.0);
  EXPECT_EQ(v1[0], 1.0);
  EXPECT_EQ(v1[1], 2.0);
  EXPECT_EQ(v1[2], 3.0);
  EXPECT_EQ(v1.row, mr::Vec3f::RowT(1, 2, 3));
}

TEST_F(Vector3D, Setters) {
  mr::Vec3f v{ 0, 0, 3 };
  v.x(1);
  v.set(1, 2);
  EXPECT_EQ(v, mr::Vec3f(1, 2, 3));
}

TEST_F(Vector3D, Equality) {
  auto copy = v1;
  EXPECT_EQ(v1, copy);
  EXPECT_TRUE(v1.equal(copy));
  EXPECT_TRUE(equal(v1, copy));

  EXPECT_NE(v1, v2);
  EXPECT_FALSE(v1.equal(v2));
  EXPECT_FALSE(equal(v1, v2));
}

TEST_F(Vector3D, Addition) {
  mr::Vec3f result = v1 + v2;
  EXPECT_EQ(result, mr::Vec3f(5, 7, 9));
}

TEST_F(Vector3D, Subtraction) {
  mr::Vec3f result = v1 - v2;
  EXPECT_EQ(result, mr::Vec3f(-3));
}

TEST_F(Vector3D, DotProduct) {
  auto result = v1.dot(v2);
  EXPECT_EQ(result, 32.0);
}

TEST_F(Vector3D, CrossProduct) {
  auto result1 = v1.cross(v2);
  mr::Vec3f expected{ -3, 6, -3 };
  EXPECT_EQ(result1, expected);

  auto result2 = v2.cross(v1);
  EXPECT_EQ(result2, -expected);
}

TEST_F(Vector3D, ComponentsProduct) {
  EXPECT_EQ(v1 * v2, mr::Vec3f(4, 10, 18));
}

TEST_F(Vector3D, Length) {
  EXPECT_NEAR(v1.length(), std::sqrt(14.0f), 0.0001);
  EXPECT_NEAR(v1.length2(), 14.0f, 0.0001);
  EXPECT_NEAR(v1.inversed_length(), 1 / std::sqrt(14.0f), 0.01);
}

TEST_F(Vector3D, Normalize) {
  mr::Vec3f expected{ 0.267261, 0.534522, 0.801784 };
  EXPECT_TRUE(mr::equal(v1.normalized_unchecked(), expected, 0.000001));
  auto copy = v1;
  EXPECT_TRUE(mr::equal(copy.normalize_unchecked(), expected, 0.000001));

  EXPECT_TRUE(mr::equal(v1.normalized_fast_unchecked(), expected, 0.1));
  EXPECT_TRUE(mr::equal(copy.normalize_fast_unchecked(), expected, 0.1));

  auto zero_v = mr::Vec3f{ 0 };
  EXPECT_EQ(zero_v.normalize(), zero_v);
  auto null = zero_v.normalized();
  EXPECT_FALSE(null.has_value());
}

TEST_F(Vector3D, Abs) {
  mr::Vec3f v{ -30, 47, -80 };
  EXPECT_EQ(v.absed(), mr::Vec3f(30, 47, 80));
  EXPECT_EQ(v.abs(), mr::Vec3f(30, 47, 80));
}

TEST_F(Vector3D, Clamp) {
  mr::Vec3f v{ -30, 47, -80 };
  EXPECT_EQ(v.clamped(-47, 0), mr::Vec3f(-30, 0, -47));
  EXPECT_EQ(v.clamp(-47, 0), mr::Vec3f(-30, 0, -47));
}
