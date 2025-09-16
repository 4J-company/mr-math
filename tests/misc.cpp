#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gtest/gtest.h"
#include "mr-math/math.hpp"

using namespace mr::literals;

TEST(Transform, ScaleVector) {
  EXPECT_EQ(mr::Vec3f(1, -1, 0) * mr::scale(mr::Vec3f{30, 47, 80}), mr::Vec3f(30, -47, 0));
}

TEST(Transform, TranslateVector) {
  EXPECT_EQ(mr::Vec3f(0, 0, 22) * mr::translate(mr::Vec3f{30, 47, 80}), mr::Vec3f(30, 47, 102));
}

TEST(Transform, RotateVector) {
  mr::Vec3f v{30, 47, 80};
  mr::Vec3f expected{69.0185819718,23.0733671242,64.908050904};
  EXPECT_TRUE(mr::equal(v * mr::rotate(mr::Norm3f(1, 1, 1), 102_rad), expected, 0.0001));

  EXPECT_TRUE(mr::equal(mr::axis::x * mr::rotate(mr::Yaw(90_deg)), mr::axis::z));
  EXPECT_TRUE(mr::equal(mr::axis::y * mr::rotate(mr::Pitch(90_deg)), -mr::axis::z));
  EXPECT_TRUE(mr::equal(mr::axis::z * mr::rotate(mr::Roll(90_deg)), mr::axis::z));
}

TEST(Transform, TransformVector) {
  mr::Vec3f v = mr::axis::x;
  mr::Vec3f expected{0};
  EXPECT_TRUE(mr::equal(v * mr::scale(mr::Vec3f{2}) * mr::rotate(mr::Yaw(90_deg)) * mr::translate(mr::Vec3f{0, 0, 2}), expected, 0.001));
}

TEST(Camera, DefaultConstructor) {
  mr::Camera<float> cam;
  EXPECT_EQ(cam.position(), mr::Vec3f(0, 0, 0));
  EXPECT_TRUE(mr::equal(cam.direction(), mr::Norm3f(0, 0, -1)));
  EXPECT_TRUE(mr::equal(cam.up(), mr::Norm3f(0, 1, 0)));
  EXPECT_TRUE(mr::equal(cam.right(), mr::Norm3f(1, 0, 0)));
}

TEST(Camera, PositionConstructor) {
  mr::Camera<float> cam(mr::Vec3f(1, 2, 3));
  EXPECT_EQ(cam.position(), mr::Vec3f(1, 2, 3));
  EXPECT_TRUE(mr::equal(cam.direction(), mr::Norm3f(0, 0, -1)));
}

TEST(Camera, FullConstructor) {
  mr::Camera<float> cam(mr::Vec3f(1, 2, 3), mr::Vec3f(0, 0, -1), mr::Vec3f(0, 1, 0));
  EXPECT_EQ(cam.position(), mr::Vec3f(1, 2, 3));
  EXPECT_EQ(cam.direction(), mr::Norm3f(0, 0, -1));
  EXPECT_EQ(cam.up(), mr::Norm3f(0, 1, 0));
  EXPECT_EQ(cam.right(), mr::Norm3f(1, 0, 0));
}

TEST(Camera, CopySemantics) {
  mr::Camera<float> cam1(mr::Vec3f(1, 2, 3));
  mr::Camera<float> cam2(cam1);
  EXPECT_EQ(cam2.position(), mr::Vec3f(1, 2, 3));

  mr::Camera<float> cam3;
  cam3 = cam1;
  EXPECT_EQ(cam3.position(), mr::Vec3f(1, 2, 3));
}

TEST(Camera, MoveSemantics) {
  mr::Camera<float> cam1(mr::Vec3f(1, 2, 3));
  mr::Camera<float> cam2(std::move(cam1));
  EXPECT_EQ(cam2.position(), mr::Vec3f(1, 2, 3));

  mr::Camera<float> cam3;
  cam3 = std::move(cam2);
  EXPECT_EQ(cam3.position(), mr::Vec3f(1, 2, 3));
}

TEST(Camera, PositionMovement) {
  mr::Camera<float> cam;
  cam += mr::Vec3f(1, 2, 3);
  EXPECT_EQ(cam.position(), mr::Vec3f(1, 2, 3));

  cam.position(mr::Vec3f(4, 5, 6));
  EXPECT_EQ(cam.position(), mr::Vec3f(4, 5, 6));
}

TEST(Camera, RotationAdjustments) {
  mr::Camera<float> cam(mr::Vec3f(0), mr::Vec3f(0, 0, -1));

  cam += mr::Yaw(90_deg);
  EXPECT_EQ(cam.direction(), mr::Norm3f(1, 0, 0));
  EXPECT_EQ(cam.right(), mr::Norm3f(0, 0, 1));
  EXPECT_EQ(cam.up(), mr::Norm3f(0, 1, 0));

  cam += mr::Pitch(90_deg);
  EXPECT_EQ(cam.direction(), mr::Norm3f(0, 1, 0));
  EXPECT_EQ(cam.right(), mr::Norm3f(0, 0, 1));
  EXPECT_EQ(cam.up(), mr::Norm3f(-1, 0, 0));

  cam += mr::Roll(90_deg);
  EXPECT_EQ(cam.direction(), mr::Norm3f(0, 1, 0));
  EXPECT_EQ(cam.right(), mr::Norm3f(1, 0, 0));
  EXPECT_EQ(cam.up(), mr::Norm3f(0, 0, 1));
}

/* TODO: implement mr::Camera::direction setter
TEST(Camera, SetDirection) {
  mr::Camera<float> cam;
  cam.direction(mr::Norm3f(0, 1, 0));
  EXPECT_EQ(cam.direction(), mr::Norm3f(0, 1, 0));
  EXPECT_EQ(cam.up(), mr::Norm3f(0, 0, -1)); // Orthogonal adjustment
}
*/

TEST(Camera, ProjectionSettings) {
  mr::Camera<float> cam;
  auto& proj = cam.projection();

  proj.distance = 0.1f;
  proj.far = 1000.0f;
  proj.width = 0.32f;
  proj.height = 0.18f;

  EXPECT_EQ(proj.distance, 0.1f);
  EXPECT_EQ(proj.far, 1000.0f);

  proj.resize(1.0f); // Square aspect
  EXPECT_FLOAT_EQ(proj.height, 0.32f);
}

TEST(Camera, PerspectiveMatrix) {
  mr::Camera<float> cam(mr::Vec3f(1, 2, 3));
  mr::Matr4f view = cam.perspective();

  // Should be inverse of camera transform
  mr::Matr4f expected = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    -1, -2, -3, 1
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, OrthographicMatrix) {
  mr::Camera<float> cam;
  auto& proj = cam.projection();
  proj.distance = 0.1f;
  proj.far = 100.0f;
  proj.width = 0.16f;
  proj.height = 0.09f;

  mr::Matr4f ortho = cam.orthographic();

  // Calculated values
  float l = -0.045f;
  float r = 0.045f;
  float b = -0.08f;
  float t = 0.08f;
  float n = 0.1f;
  float f = 100.0f;

  mr::Matr4f expected = {
    2/(r-l),        0,          0,          0,
    0,              2/(t-b),    0,          0,
    0,              0,          2/(n-f),    0,
    (r+l)/(l-r),    (t+b)/(b-t), (f+n)/(n-f), 1
  };

  EXPECT_TRUE(mr::equal(ortho, expected, 0.0001f));
}

TEST(Camera, FrustumMatrix) {
  mr::Camera<float> cam;
  auto& proj = cam.projection();
  proj.distance = 0.1f;
  proj.far = 100.0f;
  proj.width = 0.16f;
  proj.height = 0.09f;

  mr::Matr4f frustum = cam.frustum();

  // Calculated values
  float l = -0.045f;
  float r = 0.045f;
  float b = -0.08f;
  float t = 0.08f;
  float n = 0.1f;
  float f = 100.0f;

  mr::Matr4f expected = {
    2*n/(r-l), 0,           0,          0,
    0,         2*n/(t-b),   0,          0,
    (r+l)/(r-l), (t+b)/(t-b), (f+n)/(n-f), -1,
    0,         0,           2*n*f/(n-f), 0
  };

  EXPECT_TRUE(mr::equal(frustum, expected, 0.0001f));
}

TEST(Camera, FrustumMatrix_DefaultMatchesExpected) {
  mr::Camera<float> cam;
  auto &proj = cam.projection();
  proj.distance = 0.1f;
  proj.far = 100.0f;
  proj.width = 0.16f;
  proj.height = 0.09f;

  mr::Matr4f fr = cam.frustum();

  float l = -proj.height / 2;
  float r =  proj.height / 2;
  float b = -proj.width / 2;
  float t =  proj.width / 2;
  float n = proj.distance;
  float f = proj.far;

  glm::mat4 g = glm::frustum(l, r, b, t, n, f);
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(fr, expected, 0.0001f));
}

TEST(Camera, FrustumMatrix_AspectResize) {
  mr::Camera<float> cam;
  auto &proj = cam.projection();
  proj.distance = 0.5f;
  proj.far = 250.0f;
  proj.width = 0.3f;
  proj.height = 0.2f;

  // Resize aspect (height = width * aspect)
  proj.resize(2.0f);

  mr::Matr4f fr = cam.frustum();

  float l = -proj.height / 2;
  float r =  proj.height / 2;
  float b = -proj.width / 2;
  float t =  proj.width / 2;
  float n = proj.distance;
  float f = proj.far;

  glm::mat4 g = glm::frustum(l, r, b, t, n, f);
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(fr, expected, 0.0001f));
}

TEST(Camera, FrustumMatrix_TranslationInvariant) {
  // Moving camera should not change projection (frustum) matrix
  mr::Camera<float> cam(mr::Vec3f(1, 2, 3));
  auto &proj = cam.projection();
  proj.distance = 0.2f;
  proj.far = 500.0f;
  proj.width = 0.8f;
  proj.height = 0.4f;

  mr::Matr4f fr1 = cam.frustum();
  cam += mr::Vec3f(10, -5, 2);
  mr::Matr4f fr2 = cam.frustum();

  EXPECT_TRUE(mr::equal(fr1, fr2, 0.0001f));
}

TEST(Camera, View) {
  mr::Camera<float> cam(mr::Vec3f(0, 0, 5));
  mr::Matr4f view = cam.perspective();

  mr::Matr4f expected = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, -5, 1
  };

  EXPECT_EQ(view, expected);
}

TEST(Camera, ViewMatrixOrigin) {
  // Camera at origin looking down negative Z
  mr::Camera<float> cam(mr::Vec3f(0, 0, 0), mr::Vec3f(0, 0, -1), mr::Vec3f(0, 1, 0));
  mr::Matr4f view = cam.perspective();

  glm::mat4 g = glm::lookAtRH(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0));
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  if (!mr::equal(view, expected, 0.0001f)) {
    std::cout << "Complex view actual=" << view << "\n";
    std::cout << "Complex view expected=" << expected << "\n";
    std::cout << "cam basis R=" << cam.right() << ", U=" << cam.up() << ", D=" << cam.direction() << "\n";
  }

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, ViewMatrixTranslated) {
  // Camera at (1, 2, 3) looking down negative Z
  mr::Camera<float> cam(mr::Vec3f(1, 2, 3), mr::Vec3f(0, 0, -1), mr::Vec3f(0, 1, 0));
  mr::Matr4f view = cam.perspective();

  glm::mat4 g = glm::lookAtRH(glm::vec3(1,2,3), glm::vec3(1,2,2), glm::vec3(0,1,0));
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, ViewMatrixYaw90) {
  // Camera at origin, yawed 90 degrees (looking down negative X)
  mr::Camera<float> cam(mr::Vec3f(0, 0, 0), mr::Vec3f(-1, 0, 0), mr::Vec3f(0, 1, 0));
  mr::Matr4f view = cam.perspective();

  glm::mat4 g = glm::lookAtRH(glm::vec3(0,0,0), glm::vec3(-1,0,0), glm::vec3(0,1,0));
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, ViewMatrixPitch90) {
  // Camera at origin, pitched 90 degrees (looking down negative Y)
  mr::Camera<float> cam(mr::Vec3f(0, 0, 0), mr::Vec3f(0, -1, 0), mr::Vec3f(0, 0, 1));
  mr::Matr4f view = cam.perspective();

  glm::mat4 g = glm::lookAtRH(glm::vec3(0,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,1));
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, ViewMatrixComplexPose) {
  // Camera at (5, 10, 15) looking at (0, 0, 0) with up vector (0, 1, 0)
  mr::Vec3f pos(5, 10, 15);
  mr::Vec3f target(0, 0, 0);
  mr::Vec3f up(0, 1, 0);
  mr::Norm3f direction = (target - pos).normalized().value();
  
  mr::Camera<float> cam(pos, direction, up);
  mr::Matr4f view = cam.perspective();

  glm::mat4 g = glm::lookAtRH(glm::vec3(pos.x(), pos.y(), pos.z()), glm::vec3(target.x(), target.y(), target.z()), glm::vec3(up.x(), up.y(), up.z()));
  mr::Matr4f expected = {
    g[0][0], g[0][1], g[0][2], g[0][3],
    g[1][0], g[1][1], g[1][2], g[1][3],
    g[2][0], g[2][1], g[2][2], g[2][3],
    g[3][0], g[3][1], g[3][2], g[3][3]
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Camera, ViewMatrixArbitraryRotation) {
  // Camera at origin with arbitrary rotation
  mr::Camera<float> cam(mr::Vec3f(0, 0, 0));
  cam += mr::Yaw(45_deg);
  cam += mr::Pitch(30_deg);
  
  mr::Matr4f view = cam.perspective();

  // Get the camera's basis vectors
  mr::Vec3f right = cam.right();
  mr::Vec3f up = cam.up();
  mr::Vec3f forward = -cam.direction();  // Forward is negative of look direction

  // Expected view matrix using camera's basis vectors
  mr::Matr4f expected = {
    right.x(), up.x(), forward.x(), 0,
    right.y(), up.y(), forward.y(), 0,
    right.z(), up.z(), forward.z(), 0,
    0, 0, 0, 1
  };

  EXPECT_TRUE(mr::equal(view, expected, 0.0001f));
}

TEST(Color, Constructors) {
  EXPECT_EQ(mr::Color(), mr::Color(0, 0, 0, 0));

  const mr::Color expected1{0.3, 0.47, 0.8, 1.0};
  EXPECT_EQ(mr::Color(0.3, 0.47, 0.8), expected1);
  EXPECT_EQ(mr::Color(mr::Vec4f(0.3, 0.47, 0.8, 1)), expected1);

  const mr::Color expected2{0.2980392156862745, 0.4666666666666667, 0.8, 1.0};
  EXPECT_EQ(mr::Color(76, 119, 204, 255), expected2);
  EXPECT_EQ(mr::Color(0x4C'77'CC'FF), expected2);
  EXPECT_EQ(0x4C'77'CC'FF_rgba, expected2);
}

TEST(Color, Formats) {
  const auto color = 0x4C'77'CC'FF_rgba;
  EXPECT_EQ(color.argb(), 0xFF'4C'77'CC_rgba);
  EXPECT_EQ(color.bgra(), 0xCC'77'4c'FF_rgba);
  EXPECT_EQ(color.abgr(), 0xFF'CC'77'4c_rgba);
}

TEST(Color, Getters) {
  const auto color = 0x4C'77'CC'FF_rgba;
  EXPECT_FLOAT_EQ(color.r(), 0.2980392156862745f);
  EXPECT_FLOAT_EQ(color.g(), 0.4666666666666667f);
  EXPECT_FLOAT_EQ(color.b(), 0.8);
  EXPECT_FLOAT_EQ(color.a(), 1.0);

  EXPECT_EQ(color[0], color.r());
  EXPECT_EQ(color[1], color.g());
  EXPECT_EQ(color[2], color.b());
  EXPECT_EQ(color[3], color.a());

  const auto [r, g, b, a] = color;
  EXPECT_EQ(r, color.r());
  EXPECT_EQ(g, color.g());
  EXPECT_EQ(b, color.b());
  EXPECT_EQ(a, color.a());
}

TEST(Color, Setters) {
  auto color = 0x4C'77'CC'FF_rgba;
  color.r(1.0);
  color.set(1, 0.5);
  EXPECT_EQ(color, mr::Color(1.0, 0.5, 0.8, 1.0));
}

TEST(Color, Equality) {
  const auto color1 = 0x4C'77'CC'FF_rgba;
  const auto copy = color1;
  EXPECT_EQ(color1, copy);
  EXPECT_TRUE(color1.equal(copy));
  EXPECT_TRUE(equal(color1, copy));

  const auto color2 = 0x00'00'00'00_rgba;
  EXPECT_NE(color1, color2);
  EXPECT_FALSE(color1.equal(color2));
  EXPECT_FALSE(equal(color1, color2));
}

TEST(Color, Addition) {
  // Values can exeed 1.0 (should they?)
  EXPECT_EQ(mr::Color(1.0, 0.0, 0.5, 1.0) + mr::Color(0.0, 1.0, 0.5, 1.0), mr::Color(1.0, 1.0, 1.0, 2.0));
}

TEST(Utility, Within) {
  EXPECT_FALSE(mr::within(1, 10)(0));
  EXPECT_TRUE(mr::within(1, 10)(1));
  EXPECT_TRUE(mr::within(1, 10)(5));
  EXPECT_TRUE(mr::within(1, 10)(10));
  EXPECT_FALSE(mr::within(1, 10)(11));

  EXPECT_FALSE(mr::within_ex(1, 10)(0));
  EXPECT_FALSE(mr::within_ex(1, 10)(1));
  EXPECT_TRUE(mr::within_ex(1, 10)(5));
  EXPECT_FALSE(mr::within_ex(1, 10)(10));
  EXPECT_FALSE(mr::within_ex(1, 10)(11));

  EXPECT_TRUE(mr::within(1., 10.f)(5));
  EXPECT_TRUE(mr::within_ex(1., 10.f)(5));
}
