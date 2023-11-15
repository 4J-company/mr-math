#include "math.hpp"

#include <benchmark/benchmark.h>

volatile float a = 1; // to disable constexpr calculations

mr::Vec3f v1 {a, 0, 0};
mr::Vec3f v2 {0, a, 0};
mr::Vec3f v3 {0, 0, a};

mr::Matr4f m1 {
  mr::Matr4f::RowT{a, 2, 3, 4},
  mr::Matr4f::RowT{a, 3, 4, 5},
  mr::Matr4f::RowT{a, 4, 5, 6},
  mr::Matr4f::RowT{a, 5, 6, 7}
};
mr::Matr4f m2 {
  mr::Matr4f::RowT{a, 6, 5, 4},
  mr::Matr4f::RowT{a, 5, 4, 3},
  mr::Matr4f::RowT{a, 4, 3, 2},
  mr::Matr4f::RowT{a, 3, 2, 1}
};

static void BM_normalized(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1.normalized();
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_normalized);
BENCHMARK(BM_normalized);
BENCHMARK(BM_normalized);

static void BM_normalized_fast(benchmark::State& state) {
  for (auto _ : state) {
    auto v3 = v1.normalized_fast_unsafe();
    benchmark::DoNotOptimize(v3);
  }
}
BENCHMARK(BM_normalized_fast);
BENCHMARK(BM_normalized_fast);
BENCHMARK(BM_normalized_fast);

static void BM_dot(benchmark::State& state) {
  for (auto _ : state) {
    auto v4 = v1.dot(v3);
    benchmark::DoNotOptimize(v4);
  }
}
BENCHMARK(BM_dot);
BENCHMARK(BM_dot);
BENCHMARK(BM_dot);

static void BM_cross(benchmark::State& state) {
  for (auto _ : state) {
    auto v4 = v1.cross(v2);
    benchmark::DoNotOptimize(v4);
  }
}
BENCHMARK(BM_cross);
BENCHMARK(BM_cross);
BENCHMARK(BM_cross);

static void BM_scalar_triple_product(benchmark::State& state) {
  for (auto _ : state) {
    auto v4 = v1.cross(v2).dot(v3);
    benchmark::DoNotOptimize(v4);
  }
}
BENCHMARK(BM_scalar_triple_product);
BENCHMARK(BM_scalar_triple_product);
BENCHMARK(BM_scalar_triple_product);

static void BM_matrix_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1 * m2;
    benchmark::DoNotOptimize(m3);
  }
}

BENCHMARK(BM_matrix_multiplication);
BENCHMARK(BM_matrix_multiplication);
BENCHMARK(BM_matrix_multiplication);

BENCHMARK_MAIN();

