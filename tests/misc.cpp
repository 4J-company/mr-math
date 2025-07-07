#include <array>

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

// TODO: camera tests

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
