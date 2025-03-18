#include "mr-math/math.hpp"

#include <benchmark/benchmark.h>

namespace {
  alignas(64) volatile float a = 1.0f;  // Separate cache line
  alignas(64) volatile float b = 2.0f;
  alignas(64) volatile float c = 3.0f;

  mr::Vec3f v1 {a, b, c};
  mr::Vec3f v2 {b, c, a};
  mr::Vec3f v3 {c, a, b};

  mr::Matr4f m1 = []{
    mr::Matr4f m;
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        m[i]._set_ind(j, 0.1f * (i*4 + j) + a);
    return m;
  }();

  mr::Matr4f m2 = []{
    mr::Matr4f m;
    for(int i=0; i<4; ++i)
      for(int j=0; j<4; ++j)
        m[i]._set_ind(j, 0.1f * (i*4 + (3-j)) + b);
    return m;
  }();

  mr::Camera<float> cam = []{
    mr::Camera<float> res {
      mr::Vec3f(a, b, c),
      mr::Vec3f(b, c, a)
    };
    return res;
  }();
} // namespace

static void BM_camera_perspective(benchmark::State& state) {
  for (auto _ : state) {
    auto m = cam.calculate_perspective();
    benchmark::DoNotOptimize(m);
  }
}
BENCHMARK(BM_camera_perspective);

static void BM_camera_ortholinear(benchmark::State& state) {
  for (auto _ : state) {
    auto m = cam.calculate_orthographic();
    benchmark::DoNotOptimize(m);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_camera_ortholinear);

static void BM_camera_frustum(benchmark::State& state) {
  for (auto _ : state) {
    auto m = cam.calculate_frustum();
    benchmark::DoNotOptimize(m);
  }
}
BENCHMARK(BM_camera_frustum);

static void BM_camera_rotation(benchmark::State& state) {
  for (auto _ : state) {
    cam += mr::Yaw(mr::pi);
  }
}
BENCHMARK(BM_camera_rotation);

static void BM_vector_rotation(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1 * mr::Matr4f::rotate_z(mr::Radiansf(mr::pi));
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_vector_rotation);

static void BM_vector_scale(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1 * mr::ScaleMatr3f({1, 2, 3});
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_vector_scale);

static void BM_vector_const_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1 * 3;
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_vector_const_multiplication);

static void BM_vector_matrix_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1 * m1;
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_vector_matrix_multiplication);

static void BM_normalized(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1.normalized();
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_normalized);

static void BM_normalized_fast(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1.normalized_fast_unchecked();
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_normalized_fast);

static void BM_dot(benchmark::State& state) {
  for (auto _ : state) {
    auto v4 = v1.dot(v3);
    benchmark::DoNotOptimize(v4);
  }
}
BENCHMARK(BM_dot);

static void BM_cross(benchmark::State& state) {
  for (auto _ : state) {
    auto v4 = v1.cross(v2);
    benchmark::DoNotOptimize(v4);
  }
}
BENCHMARK(BM_cross);

static void BM_matrix_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1 * m2;
    benchmark::DoNotOptimize(m3);
  }
}

BENCHMARK(BM_matrix_multiplication);

static void BM_matrix_transposed(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = m1.transposed();
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_matrix_transposed);


// ====================== Benchmark Fixture ======================
class QuaternionBench : public benchmark::Fixture {
  protected:
    mr::Quatf q_rot = mr::Quatf(mr::Degreesf(45.0f),
        mr::Vec3f(1.0f, 0.0f, 0.0f)).normalize();
    mr::Quatf q1 = mr::Quatf(0.707f, 0.0f, 0.707f, 0.0f).normalize();
    mr::Quatf q2 = mr::Quatf(0.5f, 0.5f, 0.5f, 0.5f).normalize();
    mr::Vec3f vec = {1.0f, 2.0f, 3.0f};

    void SetUp(const benchmark::State&) {
      // Warm up cache
      auto temp = vec * q_rot;
      benchmark::DoNotOptimize(temp);
    }
};

// ====================== Vector Rotation Benchmarks ======================
BENCHMARK_F(QuaternionBench, VectorRotation)(benchmark::State& state) {
  for (auto _ : state) {
    auto rotated = vec * q_rot;
    benchmark::DoNotOptimize(rotated);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_F(QuaternionBench, VectorRotationChain)(benchmark::State& state) {
  for (auto _ : state) {
    auto temp = vec;
    for (int i = 0; i < 10; ++i) { // Chain 10 rotations
      temp *= q_rot;
    }
    benchmark::DoNotOptimize(temp);
    benchmark::ClobberMemory();
  }
}

// ====================== Quaternion Multiplication Benchmarks ======================
BENCHMARK_F(QuaternionBench, QuaternionMultiplication)(benchmark::State& state) {
  for (auto _ : state) {
    auto res = q1 * q2;
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_F(QuaternionBench, QuaternionMultiplicationChain)(benchmark::State& state) {
  for (auto _ : state) {
    auto temp = q1;
    for (int i = 0; i < 10; ++i) { // Chain 10 multiplications
      temp *= q2;
    }
    benchmark::DoNotOptimize(temp);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_F(QuaternionBench, QuaternionSlerp)(benchmark::State& state) {
  const auto q1 = mr::Quatf(mr::Radiansf(a), mr::axis::y);
  const auto q2 = mr::Quatf(mr::Radiansf(b), mr::axis::x);

  for (auto _ : state) {
    auto q = mr::slerp(q1, q2, 0.5f);
    benchmark::DoNotOptimize(q);
    benchmark::ClobberMemory();
  }
}

BENCHMARK_MAIN();

