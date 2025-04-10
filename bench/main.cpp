#include "mr-math/math.hpp"
#include <benchmark/benchmark.h>
#include <typeinfo>

namespace {
    // Volatile variables to prevent optimization
    alignas(64) volatile float a = 1.0f;
    alignas(64) volatile float b = 2.0f;
    alignas(64) volatile float c = 3.0f;
} // namespace

// ====================== Template Utilities ======================
template <typename T> T get_value(int idx) { return static_cast<T>(0.1f * idx); }
template <> int get_value<int>(int idx) { return idx % 10; }

// ====================== Matrix Benchmark Fixture ======================
template <typename T, int N>
struct MatrixBench {
    using Matrix = mr::Matr<T, N>;
    Matrix m1 = []() {
      Matrix m;
      for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
          m[i]._set_ind(j, get_value<T>(i*N + j));
        }
      }
      return m;
    }();

    Matrix m2 = []() {
      Matrix m;
      for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
          m[i]._set_ind(j, get_value<T>((i*N + (N-j-1))));
        }
      }
      return m;
    }();
};

// ====================== Vector Benchmark Fixture ======================
template <typename T, int N>
struct VectorBench {
    using Vector = mr::Vec<T, N>;
    Vector v1 = []() {
      Vector v;
      for(int i = 0; i < N; ++i) {
        v.set(i, get_value<T>(i + 1));
      }
      return v;
    }();
    Vector v2 = []() {
      Vector v;
      for(int i = 0; i < N; ++i) {
        v.set(i, get_value<T>(N - i));
      }
      return v;
    }();
};

// ====================== Matrix Benchmarks ======================
template <typename T, int N>
void BM_MatrixMultiply(benchmark::State& state) {
    MatrixBench<T, N> fixture;
    for(auto _ : state) {
        auto m3 = fixture.m1 * fixture.m2;
        benchmark::DoNotOptimize(m3);
    }
}

template <typename T, int N>
void BM_MatrixTranspose(benchmark::State& state) {
    MatrixBench<T, N> fixture;
    for(auto _ : state) {
        auto transposed = fixture.m1.transposed();
        benchmark::DoNotOptimize(transposed);
    }
}

// ====================== Vector Benchmarks ======================
template <typename T, int N>
void BM_VectorDotProduct(benchmark::State& state) {
    VectorBench<T, N> fixture;
    for(auto _ : state) {
        auto result = fixture.v1.dot(fixture.v2);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T, int N>
void BM_VectorNormalize(benchmark::State& state) {
    VectorBench<T, N> fixture;
    for(auto _ : state) {
        auto norm = fixture.v1.normalized();
        benchmark::DoNotOptimize(norm);
    }
}

// ====================== Edge Case Benchmarks ======================
template <typename T, int N>
void BM_ZeroVectorNormalize(benchmark::State& state) {
    typename VectorBench<T, N>::Vector v{};
    for(auto _ : state) {
        auto norm = v.normalized_fast_unchecked();
        benchmark::DoNotOptimize(norm);
    }
}

template <typename T, int N>
void BM_IdentityMatrixMultiply(benchmark::State& state) {
    typename MatrixBench<T, N>::Matrix m1 = MatrixBench<T, N>::Matrix::identity();
    typename MatrixBench<T, N>::Matrix m2 = MatrixBench<T, N>::Matrix::identity();
    for(auto _ : state) {
        auto m3 = m1 * m2;
        benchmark::DoNotOptimize(m3);
    }
}

// ====================== Combination Benchmarks ======================
template <typename T, int N>
void BM_MatrixMultiplyTranspose(benchmark::State& state) {
    MatrixBench<T, N> fixture;
    for(auto _ : state) {
        auto m3 = fixture.m1 * fixture.m2;
        auto transposed = m3.transposed();
        benchmark::DoNotOptimize(transposed);
    }
}

// ====================== Camera Benchmark Fixture ======================
template <typename T>
struct CameraBench {
    mr::Vec<T,3> eye = {static_cast<T>(a), static_cast<T>(b), static_cast<T>(c)};
    mr::Vec<T,3> target = {static_cast<T>(b), static_cast<T>(c), static_cast<T>(a)};
    mr::Camera<T> cam = mr::Camera<T>(eye, target);
};

// Camera Benchmarks
template <typename T>
void BM_CameraPerspective(benchmark::State& state) {
    CameraBench<T> fixture;
    for (auto _ : state) {
        auto m = fixture.cam.calculate_perspective();
        benchmark::DoNotOptimize(m);
    }
}

template <typename T>
void BM_CameraOrthographic(benchmark::State& state) {
    CameraBench<T> fixture;
    for (auto _ : state) {
        auto m = fixture.cam.calculate_orthographic();
        benchmark::DoNotOptimize(m);
    }
}

template <typename T>
void BM_CameraFrustum(benchmark::State& state) {
    CameraBench<T> fixture;
    for (auto _ : state) {
        auto m = fixture.cam.calculate_frustum();
        benchmark::DoNotOptimize(m);
    }
}

template <typename T>
void BM_CameraRotation(benchmark::State& state) {
    CameraBench<T> fixture;
    for (auto _ : state) {
        fixture.cam += mr::Yaw(mr::Radians<T>(std::numbers::pi_v<T>));
    }
}

// ====================== Quaternion Benchmark Fixture ======================
template <typename T>
struct QuaternionBench {
  mr::Quat<T> q_rot = mr::Quat<T>(mr::Degrees<T>(45.0),
                      mr::Vec<T,3>(1, 0, 0)).normalize();
  mr::Quat<T> q1 = mr::Quat<T>(static_cast<T>(0.707), 0, static_cast<T>(0.707), 0).normalize();
  mr::Quat<T> q2 = mr::Quat<T>(0.5, 0.5, 0.5, 0.5).normalize();
  mr::Vec<T,3> vec = {1, 2, 3};
};

// Quaternion Benchmarks
template <typename T>
void BM_QuaternionVectorRotation(benchmark::State& state) {
    QuaternionBench<T> fixture;
    for (auto _ : state) {
        auto rotated = fixture.vec * fixture.q_rot;
        benchmark::DoNotOptimize(rotated);
    }
}

template <typename T>
void BM_QuaternionMultiplication(benchmark::State& state) {
    QuaternionBench<T> fixture;
    for (auto _ : state) {
        auto res = fixture.q1 * fixture.q2;
        benchmark::DoNotOptimize(res);
    }
}

template <typename T>
void BM_QuaternionSlerp(benchmark::State& state) {
    QuaternionBench<T> fixture;
    const auto q1 = mr::Quat<T>(mr::Radians<T>(a), mr::axis::y);
    const auto q2 = mr::Quat<T>(mr::Radians<T>(b), mr::axis::x);

    for (auto _ : state) {
        auto q = mr::slerp(q1, q2, static_cast<T>(0.5));
        benchmark::DoNotOptimize(q);
    }
}
// ====================== AABB Benchmark Fixture ======================
template <typename T>
struct AABBBench {
    mr::AABB<T> main_aabb = {mr::Vec3<T>(T(0)), mr::Vec3<T>(T(10))};
    mr::AABB<T> contained_aabb = {mr::Vec3<T>(T(2)), mr::Vec3<T>(T(8))};
    mr::AABB<T> overlapping_aabb = {mr::Vec3<T>(T(5)), mr::Vec3<T>(T(15))};
    mr::AABB<T> non_overlapping_aabb = {mr::Vec3<T>(T(11)), mr::Vec3<T>(T(20))};
    mr::AABB<T> empty_aabb = {mr::Vec3<T>(T(5)), mr::Vec3<T>(T(5))};
    mr::Vec3<T> inside_point = mr::Vec3<T>(T(5));
    mr::Vec3<T> outside_point = mr::Vec3<T>(T(11));
};

// ====================== AABB Benchmarks ======================
template <typename T>
void BM_AABBDimensions(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        auto dims = fixture.main_aabb.dimensions();
        benchmark::DoNotOptimize(dims);
    }
}

template <typename T>
void BM_AABBContainsPointInside(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.contains(fixture.inside_point);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBContainsPointOutside(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.contains(fixture.outside_point);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBContainsAABBInside(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.contains(fixture.contained_aabb);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBContainsAABBOutside(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.contains(fixture.non_overlapping_aabb);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBIntersectsOverlapping(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.intersects(fixture.overlapping_aabb);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBIntersectsNonOverlapping(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.main_aabb.intersects(fixture.non_overlapping_aabb);
        benchmark::DoNotOptimize(result);
    }
}

template <typename T>
void BM_AABBEmptyContains(benchmark::State& state) {
    AABBBench<T> fixture;
    for (auto _ : state) {
        bool result = fixture.empty_aabb.contains(fixture.inside_point);
        benchmark::DoNotOptimize(result);
    }
}

// ====================== Color Benchmark Fixture ======================
struct ColorBench : public benchmark::Fixture {
    // Volatile inputs to prevent optimization
    alignas(64) volatile float fr = 0.1f;
    alignas(64) volatile float fg = 0.2f;
    alignas(64) volatile float fb = 0.3f;
    alignas(64) volatile float fa = 0.4f;
    alignas(64) volatile uint8_t ir = 128;
    alignas(64) volatile uint8_t ig = 129;
    alignas(64) volatile uint8_t ib = 130;
    alignas(64) volatile uint8_t ia = 131;
    alignas(64) volatile uint32_t packed = 0x808080FF;

    mr::Color base_color{0.2f, 0.3f, 0.4f, 0.5f};
    mr::Color other_color{0.5f, 0.6f, 0.7f, 0.8f};
};

struct ColorConstructionBench {
    // Volatile inputs to prevent optimization
    alignas(64) volatile float fr = 0.1f;
    alignas(64) volatile float fg = 0.2f;
    alignas(64) volatile float fb = 0.3f;
    alignas(64) volatile float fa = 0.4f;
    alignas(64) volatile uint8_t ir = 128;
    alignas(64) volatile uint8_t ig = 129;
    alignas(64) volatile uint8_t ib = 130;
    alignas(64) volatile uint8_t ia = 131;
    alignas(64) volatile uint32_t packed = 0x808080FF;

    mr::Color base_color{0.2f, 0.3f, 0.4f, 0.5f};
    mr::Color other_color{0.5f, 0.6f, 0.7f, 0.8f};
};

// ====================== Construction Benchmarks ======================
static void BM_ColorConstructFromFloats(benchmark::State& state) {
    ColorConstructionBench fixture;
    for (auto _ : state) {
        mr::Color c(fixture.fr, fixture.fg, fixture.fb, fixture.fa);
        benchmark::DoNotOptimize(c);
    }
}

static void BM_ColorConstructFromIntegers(benchmark::State& state) {
    ColorConstructionBench fixture;
    for (auto _ : state) {
        mr::Color c(fixture.ir, fixture.ig, fixture.ib, fixture.ia);
        benchmark::DoNotOptimize(c);
    }
}

static void BM_ColorConstructFromPacked(benchmark::State& state) {
    ColorConstructionBench fixture;
    for (auto _ : state) {
        mr::Color c(fixture.packed);
        benchmark::DoNotOptimize(c);
    }
}

// ====================== Arithmetic Benchmarks ======================
BENCHMARK_DEFINE_F(ColorBench, OperatorPlus)(benchmark::State& state) {
    for (auto _ : state) {
        auto result = base_color + other_color;
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(ColorBench, OperatorPlusAssign)(benchmark::State& state) {
    for (auto _ : state) {
        auto temp = base_color;
        temp += other_color;
        benchmark::DoNotOptimize(temp);
        benchmark::ClobberMemory();
    }
}

// ====================== Conversion Benchmarks ======================
BENCHMARK_DEFINE_F(ColorBench, ConvertToARGB)(benchmark::State& state) {
    for (auto _ : state) {
        auto converted = base_color.argb();
        benchmark::DoNotOptimize(converted);
    }
}

BENCHMARK_DEFINE_F(ColorBench, ConvertToBGRA)(benchmark::State& state) {
    for (auto _ : state) {
        auto converted = base_color.bgra();
        benchmark::DoNotOptimize(converted);
    }
}

BENCHMARK_DEFINE_F(ColorBench, ConvertToABGR)(benchmark::State& state) {
    for (auto _ : state) {
        auto converted = base_color.abgr();
        benchmark::DoNotOptimize(converted);
    }
}

// ====================== Accessor Benchmarks ======================
BENCHMARK_DEFINE_F(ColorBench, ComponentAccess)(benchmark::State& state) {
    volatile float sum = 0;
    for (auto _ : state) {
        sum += base_color.r();
        sum += base_color.g();
        sum += base_color.b();
        sum += base_color.a();
    }
    benchmark::DoNotOptimize(sum);
}

BENCHMARK_DEFINE_F(ColorBench, ComponentSet)(benchmark::State& state) {
    for (auto _ : state) {
        mr::Color temp = base_color;
        temp.r(fr);
        temp.g(fg);
        temp.b(fb);
        temp.a(fa);
        benchmark::DoNotOptimize(temp);
        benchmark::ClobberMemory();
    }
}

// ====================== Comparison Benchmarks ======================
BENCHMARK_DEFINE_F(ColorBench, OperatorEqual)(benchmark::State& state) {
    bool result;
    for (auto _ : state) {
        result = (base_color == other_color);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(ColorBench, EqualWithEpsilon)(benchmark::State& state) {
    bool result;
    for (auto _ : state) {
        result = base_color.equal(other_color, 0.01f);
        benchmark::DoNotOptimize(result);
    }
}

// ====================== Edge Case Benchmarks ======================
BENCHMARK_DEFINE_F(ColorBench, MaxValueColor)(benchmark::State& state) {
    for (auto _ : state) {
        mr::Color c(255, 255, 255, 255);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK_DEFINE_F(ColorBench, ZeroValueColor)(benchmark::State& state) {
    for (auto _ : state) {
        mr::Color c(0.0f, 0.0f, 0.0f, 0.0f);
        benchmark::DoNotOptimize(c);
    }
}

// ====================== Literal Benchmark ======================
static void BM_ColorLiteral(benchmark::State& state) {
    using namespace mr::math::literals;
    for (auto _ : state) {
        auto color = 0xFF00FF00_rgba;
        benchmark::DoNotOptimize(color);
    }
}

// ====================== Registration ======================
#define REGISTER_BENCHMARKS(Type, Size) \
    BENCHMARK_TEMPLATE(BM_MatrixMultiply, Type, Size); \
    BENCHMARK_TEMPLATE(BM_MatrixTranspose, Type, Size); \
    BENCHMARK_TEMPLATE(BM_VectorDotProduct, Type, Size); \
    BENCHMARK_TEMPLATE(BM_VectorNormalize, Type, Size); \
    BENCHMARK_TEMPLATE(BM_ZeroVectorNormalize, Type, Size); \
    BENCHMARK_TEMPLATE(BM_IdentityMatrixMultiply, Type, Size); \
    BENCHMARK_TEMPLATE(BM_MatrixMultiplyTranspose, Type, Size)

#define REGISTER_CAMERA_BENCHMARKS(Type) \
    BENCHMARK_TEMPLATE(BM_CameraPerspective, Type); \
    BENCHMARK_TEMPLATE(BM_CameraOrthographic, Type); \
    BENCHMARK_TEMPLATE(BM_CameraFrustum, Type); \
    BENCHMARK_TEMPLATE(BM_CameraRotation, Type)

#define REGISTER_QUATERNION_BENCHMARKS(Type) \
    BENCHMARK_TEMPLATE(BM_QuaternionVectorRotation, Type); \
    BENCHMARK_TEMPLATE(BM_QuaternionMultiplication, Type); \
    BENCHMARK_TEMPLATE(BM_QuaternionSlerp, Type)

#define REGISTER_AABB_BENCHMARKS(Type) \
    BENCHMARK_TEMPLATE(BM_AABBDimensions, Type); \
    BENCHMARK_TEMPLATE(BM_AABBContainsPointInside, Type); \
    BENCHMARK_TEMPLATE(BM_AABBContainsPointOutside, Type); \
    BENCHMARK_TEMPLATE(BM_AABBContainsAABBInside, Type); \
    BENCHMARK_TEMPLATE(BM_AABBContainsAABBOutside, Type); \
    BENCHMARK_TEMPLATE(BM_AABBIntersectsOverlapping, Type); \
    BENCHMARK_TEMPLATE(BM_AABBIntersectsNonOverlapping, Type); \
    BENCHMARK_TEMPLATE(BM_AABBEmptyContains, Type)

// Register benchmarks for different types and sizes
REGISTER_BENCHMARKS(int, 2);
REGISTER_BENCHMARKS(int, 3);
REGISTER_BENCHMARKS(int, 4);
REGISTER_BENCHMARKS(float, 2);
REGISTER_BENCHMARKS(float, 3);
REGISTER_BENCHMARKS(float, 4);
REGISTER_BENCHMARKS(double, 2);
REGISTER_BENCHMARKS(double, 3);
REGISTER_BENCHMARKS(double, 4);

REGISTER_CAMERA_BENCHMARKS(float);
REGISTER_CAMERA_BENCHMARKS(double);

REGISTER_QUATERNION_BENCHMARKS(float);
REGISTER_QUATERNION_BENCHMARKS(double);

REGISTER_AABB_BENCHMARKS(float);
REGISTER_AABB_BENCHMARKS(double);
REGISTER_AABB_BENCHMARKS(int);
REGISTER_AABB_BENCHMARKS(uint32_t);

BENCHMARK_REGISTER_F(ColorBench, OperatorPlus);
BENCHMARK_REGISTER_F(ColorBench, OperatorPlusAssign);
BENCHMARK_REGISTER_F(ColorBench, ConvertToARGB);
BENCHMARK_REGISTER_F(ColorBench, ConvertToBGRA);
BENCHMARK_REGISTER_F(ColorBench, ConvertToABGR);
BENCHMARK_REGISTER_F(ColorBench, ComponentAccess);
BENCHMARK_REGISTER_F(ColorBench, ComponentSet);
BENCHMARK_REGISTER_F(ColorBench, OperatorEqual);
BENCHMARK_REGISTER_F(ColorBench, EqualWithEpsilon);
BENCHMARK_REGISTER_F(ColorBench, MaxValueColor);
BENCHMARK_REGISTER_F(ColorBench, ZeroValueColor);

BENCHMARK(BM_ColorConstructFromFloats);
BENCHMARK(BM_ColorConstructFromIntegers);
BENCHMARK(BM_ColorConstructFromPacked);
BENCHMARK(BM_ColorLiteral);

BENCHMARK_MAIN();
