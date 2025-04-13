# configure benchmark executable with maximum optimization possible
cmake --preset=benchmark

# build benchmark executable
cmake --build build/benchmark

# run benchmarks collecting hardware metrics with a "bench-output.csv" output file
./build/benchmark/mr-math-bench \
    --benchmark_counters_tabular=true \
    --benchmark_perf_counters=CYCLES,INSTRUCTIONS,BRANCHES,BRANCH-MISSES,CACHE-MISSES \
    --benchmark_format=csv > bench-output.csv

# run visualization script
python bench-viz.py
