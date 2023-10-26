#include "math.hpp"

#include <benchmark/benchmark.h>

mr::Matr4f m1 {
  mr::Matr4f::Row_t(1, 2, 3, 4),
  mr::Matr4f::Row_t(2, 3, 4, 5),
  mr::Matr4f::Row_t(3, 4, 5, 6),
  mr::Matr4f::Row_t(4, 5, 6, 7)
};
mr::Matr4f m2 {
  mr::Matr4f::Row_t(7, 6, 5, 4),
  mr::Matr4f::Row_t(6, 5, 4, 3),
  mr::Matr4f::Row_t(5, 4, 3, 2),
  mr::Matr4f::Row_t(4, 3, 2, 1)
};

static void BM_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1 * m2;
    benchmark::DoNotOptimize(m3);
  }
}
BENCHMARK(BM_multiplication);

static void BM_addition(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1 + m2;
    benchmark::DoNotOptimize(m3);
  }
}
BENCHMARK(BM_addition);

static void BM_inverse(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1.inversed_safe();
    benchmark::DoNotOptimize(m3);
  }
}
BENCHMARK(BM_inverse);

BENCHMARK_MAIN();

