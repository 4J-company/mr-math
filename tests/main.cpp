#include "math.hpp"

#include <benchmark/benchmark.h>

mr::Vec3f v1 {1, 0, 0};
mr::Vec3f v2 {0, 1, 0};

mr::Matr4f m1 {
  mr::Matr4f::Row_t{1, 2, 3, 4},
  mr::Matr4f::Row_t{2, 3, 4, 5},
  mr::Matr4f::Row_t{3, 4, 5, 6},
  mr::Matr4f::Row_t{4, 5, 6, 7}
};
mr::Matr4f m2 {
  mr::Matr4f::Row_t{7, 6, 5, 4},
  mr::Matr4f::Row_t{6, 5, 4, 3},
  mr::Matr4f::Row_t{5, 4, 3, 2},
  mr::Matr4f::Row_t{4, 3, 2, 1}
};

// int main() {
  // std::cout << v1 % v2 << std::endl;
  // std::cout << m1 * m2 << std::endl;
  // std::cout << m1 * m2 * m1 << std::endl;
// }

// constexpr mr::Matr4f::Row_t r1 {1, 2, 3, 4};
// constexpr std::array<mr::Matr4f::Row_t, 4> r2 {2, 3, 4, 5};
// constexpr std::array<mr::Matr4f::Row_t, 4> r3 {3, 4, 5, 6};
// constexpr std::array<mr::Matr4f::Row_t, 4> r4 {4, 5, 6, 7};


//
// static void BM_length(benchmark::State& state) {
//   for (auto _ : state) {
//     auto v3 = v1.length();
//     benchmark::DoNotOptimize(v3);
//   }
// }
// BENCHMARK(BM_length);
//
// static void BM_length2(benchmark::State& state) {
//   for (auto _ : state) {
//     auto v3 = v1.length2();
//     benchmark::DoNotOptimize(v3);
//   }
// }
// BENCHMARK(BM_length2);
//
// static void BM_normalized(benchmark::State& state) {
//   for (auto _ : state) {
//     auto v3 = v1.normalized();
//     benchmark::DoNotOptimize(v3);
//   }
// }
// BENCHMARK(BM_normalized);
//
// static void BM_cross(benchmark::State& state) {
//   for (auto _ : state) {
//     auto v3 = v1 % v2;
//     benchmark::DoNotOptimize(v3);
//   }
// }
// BENCHMARK(BM_cross);
//
// static void BM_dot(benchmark::State& state) {
//   for (auto _ : state) {
//     auto v3 = v1 & v2;
//     benchmark::DoNotOptimize(v3);
//   }
// }
// BENCHMARK(BM_dot);
//
// static void BM_multiplication(benchmark::State& state) {
//   for (auto _ : state) {
//     auto m3 = m1 * m2;
//     benchmark::DoNotOptimize(m3);
//   }
// }
//
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
// BENCHMARK(BM_multiplication);
//
// static void BM_addition(benchmark::State& state) {
//   for (auto _ : state) {
//     auto m3 = m1 + m2;
//     benchmark::DoNotOptimize(m3);
//   }
// }
// BENCHMARK(BM_addition);
//
// static void BM_inverse(benchmark::State& state) {
//   for (auto _ : state) {
//     auto m3 = m1.inversed_safe();
//     benchmark::DoNotOptimize(m3);
//   }
// }
// BENCHMARK(BM_inverse);
//
// // static void BM_inverse_safe(benchmark::State& state) {
// //   for (auto _ : state) {
// //     auto m3 = m1.inversed_safe();
// //     benchmark::DoNotOptimize(m3);
// //   }
// // }
// // BENCHMARK(BM_inverse_safe);
//
// static void BM_transpose(benchmark::State& state) {
//   for (auto _ : state) {
//     auto m3 = m1.transposed();
//     benchmark::DoNotOptimize(m3);
//   }
// }
// BENCHMARK(BM_transpose);

// static void BM_init_copy(benchmark::State& state) {
//   for (auto _ : state) {
//     mr::Matr4f m3 {
//       mr::Matr4f::Row_t{1, 2, 3, 4},
//       mr::Matr4f::Row_t{2, 3, 4, 5},
//       mr::Matr4f::Row_t{3, 4, 5, 6},
//       mr::Matr4f::Row_t{4, 5, 6, 7}
//     };
//     // auto m4 = m3;
//     benchmark::DoNotOptimize(m3);
//   }
// }
// BENCHMARK(BM_init_copy);

static void BM_identity(benchmark::State& state) {
  auto m = mr::Matr4f::Id;
  for (auto _ : state) {
    auto m3 = m;
    benchmark::DoNotOptimize(m3);
  }
}
BENCHMARK(BM_identity);

static void BM_multiplication(benchmark::State& state) {
  for (auto _ : state) {
    auto m3 = m1 * m2;
    benchmark::DoNotOptimize(m3);
  }
}

BENCHMARK(BM_multiplication);
BENCHMARK(BM_multiplication);
BENCHMARK(BM_multiplication);
BENCHMARK(BM_multiplication);
BENCHMARK(BM_multiplication);

BENCHMARK_MAIN();

