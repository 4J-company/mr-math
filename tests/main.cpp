#include <chrono>
#include <iostream>

#include "math.hpp"

#define C 1

template<typename F, typename... Args>
void timer(F f, Args ...args) {
  const auto start = std::chrono::steady_clock::now();
  f(args...);
  const auto end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << elapsed_seconds.count() << std::endl;
}

int main() {
  using namespace mr;

  mr::Matr4f m;
  // {
  //   mr::Matr4f::Row{1.f, 2.f, 3.f, 4.f},
  //   mr::Matr4f::Row{2.f, 3.f, 4.f, 5.f},
  //   mr::Matr4f::Row{3.f, 4.f, 5.f, 6.f},
  //   mr::Matr4f::Row{4.f, 5.f, 6.f, 7.f}
  // };


  auto f = [&]() {
    std::cout << m << std::endl;
  };

  auto benchmark = [&]() {
    for (int i = 0; i < C; i++)
      for (int j = 0; j < C; j++)
        f();
  };

  timer(benchmark);

  return 0;
}
