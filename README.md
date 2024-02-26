![mr. Math](./mr-math-logo.png) 
### Lightweight and high-performance linear algebra library for 3d graphics

### Comparison
| Library      | Matrix Multiplication (ns) | Vector normalization (ns) | Scalar triple products (ns) |
|--------------|----------------------------|---------------------------|-----------------------------|
| mr. Math     |              4             |             2             |            6.5              |
| DirectXMath  |            4.8             |           4.3             |            2.3              |
| glm          |            5.7             |           3.3             |            1.6              |
| cglm         |            6.2             |          14.6             |            2.2              |
| lin          |            5.7             |           3.3             |            2.3              |

### Usage
#### Vectors
Initialization
```cpp
// alias for mr::Vec<float, 3>
mr::Vec3f v1 {1, 0, 0};
mr::Vec3f v2 {0, 1, 0};
```
Operations
```cpp
// cross product
std::cout << v1.cross(v2) << std::endl; // output: (0, 0, 1)
// alternative
std::cout << v1 % v2 << std::endl; // output: (0, 0, 1)

// dot product
std::cout << v1.dot(v2) << std::endl; // output: (0, 0, 1)
// alternative
std::cout << v1 & v2 << std::endl; // output: 0

// returns normalized copy
mr::Vec3f v3 {2, 0, 0};
mr::Vec3f v4 = v3.normalized();
std::cout << v3 << " " << v4 << std::endl; // output: (2, 0, 0) (1, 0, 0)

// normalizes in place, returns mutable reference
mr::Vec3f v3 {2, 0, 0};
mr::Vec3f v4 = v3.normalize();
std::cout << v3 << " " << v4 << std::endl; // output: (1, 0, 0) (1, 0, 0)

mr::Vec3f v5 {3, 4, 0};
// calculates length (slowest)
float l = v5.length();
std::cout << l << std::endl; // output: 5

// calculates squared length (fastest)
float l = v5.length2();
std::cout << l << std::endl; // output: 25

// etc (+ - [0..N-1])
```

#### Matrices
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
// calculate determinant
float d1 = m1.determinant();
std::cout << d1 << std::endl; // output: 0
// alternative
float d1 = !m1;
std::cout << d1 << std::endl; // output: 0

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
#### Camera
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

#### Useful stuff
```cpp
// variable with value of pi and type mr::Radiansf
mr::pi = mr::Radiansf(std::numbers::pi_v<float>);

// default axis directions (can be changed according to your needs)
mr::axis::x = {1, 0, 0};
mr::axis::y = {0, 1, 0};
mr::axis::z = {0, 0, -1};

// literals
using namespace mr::literals;
1_rad == mr::Radiansf(1)
1_deg == mr::Degreesf(1)
1_pi  == 1 * mr::pi
```

### Installation
#### Using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake):
```cmake
CPMAddPackage(
  NAME mr-math
  GITHUB_REPOSITORY 4J-company/mr-math
  VERSION 1.0.0
)
target_link_libraries(<your-project>
            PUBLIC/INTERFACE/PRIVATE
            mr-math)
```
#### Manually:
##### Clone to your project subdirectory
```bash
git clone https://github.com/4J-company/mr-math
```
##### In cmake script:
```cmake
add_subdirectory(mr-math)
target_link_libraries(<your-project>
            PUBLIC/INTERFACE/PRIVATE
            mr-math)
```

### Benchmarking
##### To build projects benchmarks you have to generate CMake with -DENABLE_BENCHMARK=true argument.
##### Then build the executable (by default it compiles into architechture specific assembly)

