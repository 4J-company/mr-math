#include <iostream>

#include "math.hpp"

#define C 1000

template<typename F, typename... Args>
void timer(F f, Args ...args) {
  const auto start = std::chrono::steady_clock::now();
  f(args...);
  const auto end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << elapsed_seconds.count() << std::endl;
}

int main() {
  mr::Vec4f v1 {1.f, 0.f, 0.f, 0.f};
  mr::Vec4f v2 {0.f, 1.f, 0.f, 0.f};
  std::string s = "0123";

  std::cout << v1 % v2;

  auto f = [&]() {
  };

  auto benchmark = [&]() {
    for (int i = 0; i < C; i++)
      for (int j = 0; j < C; j++)
        f();
  };

  timer(benchmark);

  return 0;
}
