#include "gtest/gtest.h"
#include "mr-math/math.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
  // Test the angle-axis constructor with normalized axis
  glm::vec3 axis(2, 3, 4);
  axis = glm::normalize(axis);
  
  mr::Quat<float> p(1_rad, axis.x, axis.y, axis.z);
  
  // Create glm quaternion for comparison using same angle and normalized axis
  glm::quat glm_q = glm::angleAxis(1.0f, axis);
  
  // mr-math stores w as _angle and xyz as _vec
  EXPECT_TRUE(mr::equal(p.w(), glm_q.w));
  EXPECT_TRUE(mr::equal(p.vec(), mr::Vec3f(glm_q.x, glm_q.y, glm_q.z)));
}

TEST_F(Quaternion, Multiplication) {
  mr::Quat<float> a(1, 2, 3, 4);
  mr::Quat<float> b(5, 6, 7, 8);
  mr::Quat<float> res = a * b;
  
  // Compare with glm quaternion multiplication
  glm::quat glm_a(1, 2, 3, 4);
  glm::quat glm_b(5, 6, 7, 8);
  glm::quat glm_res = glm_a * glm_b;
  
  EXPECT_TRUE(mr::equal(res.w(), glm_res.w));
  EXPECT_TRUE(mr::equal(res.vec(), mr::Vec3f(glm_res.x, glm_res.y, glm_res.z)));
}

TEST_F(Quaternion, Addition) {
  mr::Quat<float> c(1, 2, 3, 4);
  mr::Quat<float> d(5, 6, 7, 8);
  mr::Quat<float> res = c + d;
  
  // Compare with glm quaternion addition
  glm::quat glm_c(1, 2, 3, 4);
  glm::quat glm_d(5, 6, 7, 8);
  glm::quat glm_res = glm_c + glm_d;
  
  EXPECT_TRUE(mr::equal(res.w(), glm_res.w));
  EXPECT_TRUE(mr::equal(res.vec(), mr::Vec3f(glm_res.x, glm_res.y, glm_res.z)));
}

TEST_F(Quaternion, Subtraction) {
  mr::Quat<float> e(1, 2, 3, 4);
  mr::Quat<float> f(5, 6, 7, 8);
  mr::Quat<float> res = e - f;
  
  // Compare with glm quaternion subtraction
  glm::quat glm_e(1, 2, 3, 4);
  glm::quat glm_f(5, 6, 7, 8);
  glm::quat glm_res = glm_e - glm_f;
  
  EXPECT_TRUE(mr::equal(res.w(), glm_res.w));
  EXPECT_TRUE(mr::equal(res.vec(), mr::Vec3f(glm_res.x, glm_res.y, glm_res.z)));
}

TEST_F(Quaternion, Normalize) {
  mr::Quat<float> g(3, 4, 0, 0);
  g.normalize();

  // Compare with glm quaternion normalization
  glm::quat glm_g(3, 4, 0, 0);
  glm_g = glm::normalize(glm_g);
  
  EXPECT_TRUE(mr::equal(g.w(), glm_g.w, 0.00001));
  EXPECT_TRUE(mr::equal(g.vec(), mr::Vec3f(glm_g.x, glm_g.y, glm_g.z)));
}

TEST_F(Quaternion, RotateMatrix) {
  mr::Vec3f v{ 0, 1, 0 };
  
  // Compare with glm quaternion rotation
  glm::quat glm_q1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
  glm::vec3 glm_v(0, 1, 0);
  glm::vec3 glm_expected = glm_q1 * glm_v;
  
  mr::Vec3f expected{ glm_expected.x, glm_expected.y, glm_expected.z };
  EXPECT_TRUE(mr::equal(v * q1, expected));
}

TEST_F(Quaternion, Normalization) {
  // Should create proper unit quaternion
  mr::Quatf q = mr::Quatf(mr::Radiansf(1.0f), mr::Vec3f(0.5f, 0.5f, 0.5f));

  // Compare with glm quaternion normalization
  glm::quat glm_q = glm::angleAxis(1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
  glm_q = glm::normalize(glm_q);
  
  // Check unit length
  float length = q.length();
  float glm_length = glm::length(glm_q);
  EXPECT_TRUE(mr::equal(length, glm_length));
  EXPECT_TRUE(mr::equal(length, 1.0f));
}

TEST_F(Quaternion, RotationOperation) {
  // 180 degree rotation around X-axis
  mr::Quatf q(mr::pi, mr::Vec3f(1.0f, 0.0f, 0.0f));

  // Compare with glm quaternion rotation
  glm::quat glm_q = glm::angleAxis(glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 glm_v(0.0f, 1.0f, 0.0f);
  glm::vec3 glm_rotated = glm_q * glm_v;
  
  mr::Vec3f v(0.0f, 1.0f, 0.0f);
  mr::Vec3f rotated = v * q;
  mr::Vec3f expected{ glm_rotated.x, glm_rotated.y, glm_rotated.z };

  EXPECT_TRUE(mr::equal(rotated, expected));
}

TEST_F(Quaternion, QuaternionMultiplication) {
  // 90 degree rotation around Z-axis
  mr::Quatf q1(mr::pi / 2, mr::Vec3f(0, 0, 1));
  // 90 degree rotation around X-axis
  mr::Quatf q2(mr::pi / 2, mr::Vec3f(1, 0, 0));

  // Compare with glm quaternion multiplication
  glm::quat glm_q1 = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0, 0, 1));
  glm::quat glm_q2 = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
  glm::quat glm_combined = glm_q1 * glm_q2;

  // Should combine rotations
  mr::Quatf q_combined = q1 * q2;

  // Test against glm combination
  mr::Vec3f v(0, 1, 0);
  mr::Vec3f rotated = v * q_combined;
  
  glm::vec3 glm_v(0, 1, 0);
  glm::vec3 glm_rotated = glm_combined * glm_v;
  mr::Vec3f expected{ glm_rotated.x, glm_rotated.y, glm_rotated.z };

  EXPECT_TRUE(mr::equal(rotated, expected));
}

TEST_F(Quaternion, IdentityRotation) {
  // Identity quaternion should not change vector
  mr::Quatf q_identity(1.f, mr::Vec3f());
  
  // Compare with glm identity quaternion
  glm::quat glm_identity(1.0f, 0.0f, 0.0f, 0.0f);
  
  mr::Vec3f v(1, 2, 3);
  mr::Vec3f rotated = v * q_identity;
  
  glm::vec3 glm_v(1, 2, 3);
  glm::vec3 glm_rotated = glm_identity * glm_v;
  mr::Vec3f expected{ glm_rotated.x, glm_rotated.y, glm_rotated.z };

  EXPECT_TRUE(mr::equal(rotated, expected));
}
