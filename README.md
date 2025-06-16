![mr. Math](./mr-math-logo.png) 
# Lightweight and high-performance linear algebra library optimized for 3d graphics workflow

## Features
- **Cross-platform** library with support for GCC, Cland and MSVC.
- **Modern C++23**
- **Competitive performance** (see [library comparisons](README.md#comparison-with-other-libraries)).
- **Linear algebra types:** vectors, matrices, quaternions, and normals.
- **3D graphics suitable types:** bound boxes, projection matrices, camera, color.
- **Type safety**: correct unit conversions, transformation matrices, and normalization.
- **Safe by default** with explicit faster options.
- **Debug visualizers** for all types (MSVC (ready) and GDB (coming soon)).
- **Printing types** using `std::ostream` or `std::format`.
- **Useful utilities**.



## Usage
For functionality and usage examples see [usage](USAGE.md).

## Installation
#### Using [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake):
```cmake
CPMAddPackage("gh:4J-company/mr-math#master")
target_link_libraries(<your-project>
            PUBLIC/INTERFACE/PRIVATE
            mr-math-lib)
```

## Building
We provide comprehensive set of configurations via CMake presets.
To enable presets, use `cmake --preset <configure_preset_name_> ...` or `cmake --build --preset <build_preset_name> ...`.

You can found some ready-to-use configurations (e.g. GCC/MSVC debug/release) in [CMakeUserPresets.json](CMakeUserPresets.json).
This file can be changed to fit your needs. To see basic available configuration options, check [CMakePresets.json](CMakePresets.json).
If you modify [CMakeUserPresets.json](CMakeUserPresets.json) and don't want git to track your changes, use
```bash
 git update-index --assume-unchanged CMakeUserPresets.json
```

## Benchmarking
#### Easiest way:
```bash
bash run-bench-viz.sh
```
NOTE: requires `python`, `pandas`, `matplotlib`, `numpy`

#### With console output:
```bash
cmake --preset=benchmark
cmake --build build/benchmark
./build/benchmark/mr-math-bench
```

#### More advanced benchmark configuration
To build benchmarks, you can generate CMake with `benchmarking` preset. \
If you are interested in specific results, you can disable particular benchmarks with the following options:
- `MR_MATH_ENABLE_VEC_BENCH`
- `MR_MATH_ENABLE_MATR_BENCH`
- `MR_MATH_ENABLE_QUAT_BENCH`
- `MR_MATH_ENABLE_AABB_BENCH`
- `MR_MATH_ENABLE_CAM_BENCH`
- `MR_MATH_ENABLE_COLOR_BENCH`

#### Available options:
- `MR_MATH_PRESET_OPTIMIZED` \
    Adds `-ffast-math`, this hurts accuracy a little but increases perf a lot
- `MR_MATH_PRESET_BENCHMARK` \
    Adds `-march=native`, this makes the library only usable on host PC, but increases perf a lot. \
    Includes `MR_MATH_PRESET_OPTIMIZED`
- `MR_MATH_PRESET_PROFILING` \
    Makes changes suggested by profiling tools used by me \
    Adds `-lprofiler -ltcmalloc` \
    **DOES NOT** include any optimization

## Comparison with other libraries
| Library      | Matrix Multiplication (ns) | Vector normalization (ns) | Scalar triple products (ns) |
|--------------|----------------------------|---------------------------|-----------------------------|
| mr. Math     |              4             |             2             |            6.5              |
| DirectXMath  |            4.8             |           4.3             |            2.3              |
| glm          |            5.7             |           3.3             |            1.6              |
| cglm         |            6.2             |          14.6             |            2.2              |
| lin          |            5.7             |           3.3             |            2.3              |
