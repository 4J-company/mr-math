# configure benchmark executable with maximum optimization possible
cmake -S . -B build \
  -D CMAKE_BUILD_TYPE=Release \
  -D MR_MATH_ENABLE_BENCHMARK=ON \
  -D MR_MATH_PRESET_BENCHMARK=ON \
  -DBENCHMARK_ENABLE_LIBPFM=ON

# build benchmark executable
cmake --build build

# run benchmarks collecting hardware metrics with a "bench-output.csv" output file
./build/mr-math-bench \
    --benchmark_counters_tabular=true \
    --benchmark_perf_counters=CYCLES,INSTRUCTIONS,BRANCHES,BRANCH-MISSES,CACHE-MISSES \
    --benchmark_format=csv > bench-output.csv

# run visualization script
python bench-viz.py
