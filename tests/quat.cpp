#include "gtest/gtest.h"
#include "mr-math/math.hpp"

using namespace mr::literals; 

class Quaternion : public ::testing::Test {
protected:
  mr::Quat<float> q1{ mr::Degreesf(90), 1, 0, 0 };
};

TEST_F(Quaternion, DefaultConstructor) {
  mr::Quat<float> q;
  EXPECT_EQ((mr::Vec4f)q, mr::Vec4f());
}

TEST_F(Quaternion, ParameterizedConstructor) {
  float w = 0.8775825618903728;
  float x = 0.17805417504364543;
  float y = 0.2670812625654681;
  float z = 0.35610835008729086;

  mr::Quat<float> p(1_rad, 2, 3, 4);
  EXPECT_TRUE(mr::equal(p.w(), w));
  EXPECT_TRUE(mr::equal(p.vec(), mr::Vec3f(x, y, z)));
}

TEST_F(Quaternion, Multiplication) {
  mr::Quat<float> a(1, 2, 3, 4);
  mr::Quat<float> b(5, 6, 7, 8);
  mr::Quat<float> res = a * b;
  mr::Quat<float> expected(-60, 12, 30, 24);
  EXPECT_TRUE(mr::equal(res.vec(), expected.vec()));
  EXPECT_TRUE(mr::equal(res.w(), expected.w()));
}

TEST_F(Quaternion, Addition) {
  mr::Quat<float> c(1, 2, 3, 4);
  mr::Quat<float> d(5, 6, 7, 8);
  mr::Quat<float> res = c + d;
  mr::Quat<float> sum(6, 8, 10, 12);
  EXPECT_TRUE(mr::equal(res.w(), sum.w()));
  EXPECT_TRUE(mr::equal(res.vec(), sum.vec()));
}

TEST_F(Quaternion, Subtraction) {
  mr::Quat<float> e(1, 2, 3, 4);
  mr::Quat<float> f(5, 6, 7, 8);
  mr::Quat<float> diff(-4, -4, -4, -4);
  mr::Quat<float> res = e - f;
  EXPECT_TRUE(mr::equal(res.vec(), diff.vec()));
  EXPECT_TRUE(mr::equal(res.w(), diff.w()));
}

TEST_F(Quaternion, Normalize) {
  mr::Quat<float> g(3, 4, 0, 0);
  g.normalize();

  EXPECT_TRUE(mr::equal(g.w(), 0.6, 0.00001));
  EXPECT_TRUE(mr::equal(g.vec(), mr::Vec3f(0.8, 0, 0)));
}

TEST_F(Quaternion, RotateMatrix) {
  mr::Vec3f v{ 0, 1, 0 };
  mr::Vec3f expected{ 0, 0, 1 };
  EXPECT_TRUE(mr::equal(v * q1, expected));
}

TEST_F(Quaternion, Normalization) {
  // Should create proper unit quaternion
  mr::Quatf q = mr::Quatf(mr::Radiansf(1.0f), mr::Vec3f(0.5f, 0.5f, 0.5f));

  // Check unit length
  float length = q.length();
  EXPECT_TRUE(mr::equal(length, 1.0f));
}

TEST_F(Quaternion, RotationOperation) {
  // 180 degree rotation around X-axis
  mr::Quatf q(mr::pi, mr::Vec3f(1.0f, 0.0f, 0.0f));

  // Should invert Y-component
  mr::Vec3f v(0.0f, 1.0f, 0.0f);
  mr::Vec3f rotated = v * q;

  EXPECT_TRUE(mr::equal(rotated, mr::Vec3f{ 0, -1, 0 }));
}

TEST_F(Quaternion, QuaternionMultiplication) {
  // 90 degree rotation around Z-axis
  mr::Quatf q1(mr::pi / 2, mr::Vec3f(0, 0, 1));
  // 90 degree rotation around X-axis
  mr::Quatf q2(mr::pi / 2, mr::Vec3f(1, 0, 0));

  // Should combine rotations
  mr::Quatf q_combined = q1 * q2;

  // Test against known combination
  mr::Vec3f v(0, 1, 0);
  mr::Vec3f rotated = v * q_combined;

  // Expected result from combined rotations
  EXPECT_TRUE(mr::equal(rotated, mr::Vec3f(0, 0, 1)));
}

TEST_F(Quaternion, IdentityRotation) {
  // Identity quaternion should not change vector
  mr::Quatf q_identity(1.f, mr::Vec3f());
  mr::Vec3f v(1, 2, 3);
  mr::Vec3f rotated = v * q_identity;

  EXPECT_TRUE(mr::equal(rotated, mr::Vec3f(1, 2, 3)));
}
