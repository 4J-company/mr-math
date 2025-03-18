# Usage
### Vectors
Initialization:
- aliases:
```cpp
// mr::Vec3f == mr::Vec3<float> == mr::Vec<float, 3>
mr::Vec2i vi; // T == int
mr::Vec3u vu; // T == unsigned int
mr::Vec4d vd; // T == double
```
- constructors:
```cpp
mr::Vec3f v1;                    // (0, 0, 0)
mr::Vec3f v2 {30};               // (30, 30, 30)
mr::Vec3f v3 {30, 47};           // (30, 47, 0)
mr::Vec3f v4 {30, 47.0, 102.0f}; // (30, 47, 102); convert each argument
mr::Vec3f v5 = mr::axis::x;
```
Operations:
- cross product:
```cpp
mr::Vec3f x = mr::axis::x; // (1, 0, 0) by default
mr::Vec3f y = mr::axis::y; // (0, 1, 0) by default

mr::Vec3f z1 = x.cross(y); // (0, 0, 1)
mr::Vec3f z2 = x % y;      // (0, 0, 1); shortcut
```
- dot product:
```cpp
mr::Vec3f v1 {1, 2, 3};
mr::Vec3f v2 {0, -1, 2};

float d1 = v1.dot(v2); // 4
float d2 = v1 & v2;    // 4; shortcut
```
- arithmetic operations:
```cpp
mr::Vec3f v1 {1, 2, 3};
mr::Vec3f v2 {0, -1, 2};

mr::Vec3f v3 = v1 + v2; // (1, 1, 5)
mr::Vec3f v4 = v1 * v2; // (0, -2, 6); element-wise multiplication
mr::Vec3f v5 = -v1;     // (-1, -2, -3)
mr::Vec3f v6 = 3 * v1;  // (3, 6, 9) - or v1 * 3
```
- comparison
```cpp
mr::Vec3f v1 {1, 2, 3};
mr::Vec3f v2 {1, 2, 3;

bool exact_same = v1 == v2; // uses operator == for T
bool equal = v1.equal(v2, 0.001); // test for equality within given epsilon
```
- get vector's modulus/magnitude/length/norm:
```cpp
mr::Vec3f v {3, 4, 0};

float l1 = v.length();          // 5
float l2 = v.length2();         // 25; faster than v.length()
float l3 = v.inversed_length(); // 1/5; faster but less precise than 1 / v.length()
```
Access components:
```cpp
mr::Vec3f v {30, 47, 102};

float x1 = v.x();      // 30
float x2 = v[1];       // 47
float x3 = v.get<2>(); // 102
auto [x, y, z] = v;    // 30, 47, 80

v.z(80) // set component
```
- normalization:
```cpp
mr::Vec3f v {2, 0, 0};
mr::Norm3f n {2, 0, 0}; // normalize at compile time

std::optional<mr::Norm3f> on = v.normalized(); // does not change v; returns std::nullopt if v.length2() near to zero
mr::Vec3f &rv = v.normalize();                 // change v
```
We provide `*_fast` version when less precision is acceptable and `*_unchecked` ones when you are sure that vector's length greater than 0.

### Matrices
Initialization
```cpp
/// alias for mr::Matr<float, 4>
mr::Matr4f m1 {
    mr::Matr4f::RowT{1, 0, 0, 0},
    mr::Matr4f::RowT{1, 0, 0, 0},
    mr::Matr4f::RowT{1, 0, 0, 0},
    mr::Matr4f::RowT{1, 0, 0, 0},
};
mr::Matr4f m2 {
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4,
};
```
Operations
```cpp
// returns transposed copy
m1.transposed();
// transposes in place
m1.transpose();

mr::Matr4f m4 {
    mr::Matr4f::RowT{2, 0, 0, 0},
    mr::Matr4f::RowT{0, 2, 0, 0},
    mr::Matr4f::RowT{0, 0, 2, 0},
    mr::Matr4f::RowT{0, 0, 0, 2}
    };
// returns inversed copy
mr::Matr4f m5 = m4.inversed();
std::cout << m5 << std::endl;
// output:
// (0.5, 0, 0, 0)
// (0, 0.5, 0, 0)
// (0, 0, 0.5, 0)
// (0, 0, 0, 0.5)

// inverses in place
mr::Matr4f m6 = m4.inverse();
// m4 is equal to m5 at this point

// etc (+ - * [][] ...)
```
### Camera
Initialization
```cpp
mr::Camera cam1 {{0}};
mr::Camera cam2 {{0}, mr::axis::z};
```

Movement and rotation
```cpp
cam1 += mr::Vec3f{1, 2, 3}; // same as moving camera by {1, 2, 3}
cam1.position({1, 2, 3});   // same as placing camera at {1, 2, 3}

cam1 += mr::Yawf{mr::pi};
cam1 += mr::Pitchf{mr::pi};
cam1 += mr::Rollf{mr::pi};
```

Perspective matrices
```cpp
auto perspective = cam1.perspective(); // world -> device
auto frustum = cam1.frustum();         // device -> screen (realistic depth perseption)
auto ortholinear = cam1.orthographic(); // device -> screen (no depth to size corelation)
```
### Quaternions
Initialization
```cpp
mr::Quatf q1 = mr::Quatf(mr::pi / 2, mr::axis::y); // from angle and vector
mr::Quatf q2 = mr::Quatf(0.5_rad, 0.1f, 0.2f, 0.3f); // from components directly
mr::Quatf q3 = mr::Quatf({0.707f, 0.0f, 0.707f, 0.0f}); // from Vec4 (angle first)
```
Operations
```cpp
mr::Quatf q_product = q1 * q2; // chain rotations
mr::Quatf q_sum = q1 + q2; // no meaningful 3d representation

// Compound assignment
q1 += q2;
q_product *= q1;
```
Normalization
```cpp
// same as with Vec4
```
Vector rotation
```cpp
// only supported by Vec3
mr::Vec3f point = mr::Vec3f(1.0f, 0.0f, 0.0f);
mr::Vec3f rotated_point = point * q1;
```

### Color
Initialization
```cpp
mr::Color c1; // same as Vec4f(0, 0, 0, 0)
mr::Color c2 = mr::Color(0.3, 0.47, 0.8); // == (0.3, 0.47, 0.8, 1.0)
mr::Color c3 = mr::Color(76, 119, 204, 255); // == (0.2980392156862745, 0.4666666666666667, 0.8, 1.0)
mr::Color c4 = mr::Color(0x4C'77'CC'FF); // == (0.2980392156862745, 0.4666666666666667, 0.8, 1.0)
mr::Color c5 = 0x4C'77'CC'FF_rgba; // == (0.2980392156862745, 0.4666666666666667, 0.8, 1.0)
```
Formats
```cpp
mr::Color c6 = 0x4C'77'CC'FF_rgba;
mr::Color c7 = color.argb(); // == 0xFF'4C'77'CC_rgba
mr::Color c8 = color.bgra(); // == 0xCC'77'4c'FF_rgba
mr::Color c9 = color.abgr(); // == 0xFF'CC'77'4c_rgba
```
Operations
- comparison
```cpp
mr::Color c10 = 0x4C'77'CC'FF_rgba;
mr::Color copy = c10;

bool is_equal = c10 == copy; // == true
```
- sum
```cpp
mr::Color c11 = mr::Color(1.0, 0.0, 0.5, 1.0) + mr::Color(0.0, 1.0, 0.5, 1.0); // == mr::Color(1.0, 1.0, 1.0, 2.0));
```

### Useful stuff
```cpp
// concept for integers and floating-point types
bool var = mr::ArithmeticT<float> and mr::ArithmeticT<int>; // true

template <mr::ArithmeticT T>
  void print_number(T number);

// shortcut for std::numeric_limits<T>::epsilon()
float eps = mr::epsion<float>();

// copmare primitive or mr-provided types
bool equal = mr::equal(mr::Vec3f{}, mr::Vec3f{0, 0, 0}, 0);
bool near = mr::equal(0.1 + 0.2, 0.3);

// check if a value is within a given range
int x = 47;
if (mr::within_ex(30, 47)(x)) // excluding boundaries
  std::println("X <= 30 or 47 <= X");

auto is_digit = mr::within(0, 9); // including boundaries
if (is_digit(x))
  std::println("X is digit");

// variable with value of pi and type mr::Radiansf
mr::pi = mr::Radiansf(std::numbers::pi_v<float>);

// default axis directions
// can be changed according to your needs - this will affect camera calculations
mr::axis::x = {1, 0, 0};
mr::axis::y = {0, 1, 0};
mr::axis::z = {0, 0, -1};

// literals
using namespace mr::literals;
1_rad == mr::Radiansf(1)
1_deg == mr::Degreesf(1)
1_pi  == 1 * mr::pi
```