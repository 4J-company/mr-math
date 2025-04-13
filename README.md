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
We provide comprehensive set of configurations via CMake presets (see [CMakePresets.json](CMakePresets.json)).
To enable presets use `cmake --preset <configure_preset_name_> ...` or `cmake --build --preset <build_preset_name> ...`

You can also create custom presets with additional settings in `CMakeUserPresets.json`.
Custom presets for GCC & Ninja may look like this:
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "custom-base",
      "hidden": true,
      "inherits": [ "base" ],
      "architecture": {
        "value": "x64",
        "strategy": "external"
      },
      "generator": "Ninja",
      "cacheVariables": {
        "CPM_SOURCE_CACHE": "D:/Cache/CPM"
      }
    },
    {
      "name": "gcc-base",
      "hidden": true,
      "inherits": [ "custom-base" ],
      "cacheVariables": {
        "CMAKE_C_COMPILER": "gcc",
        "CMAKE_CXX_COMPILER": "g++"
      }
    },
    {
      "name": "gcc-release",
      "displayName": "GCC Release",
      "inherits": [ "gcc-base", "release", "build-all" ]
    },
    {
      "name": "gcc-debug",
      "displayName": "GCC Debug",
      "inherits": [ "gcc-base", "debug", "build-all" ]
    }
  ],
  "buildPresets": [
    {
      "name": "gcc-debug",
      "displayName": "GCC Debug",
      "configurePreset": "gcc-debug"
    },
    {
      "name": "gcc-release",
      "displayName": "GCC Release",
      "configurePreset": "gcc-release"
    }
  ]
}

```
```bash

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
To build benchmarks you have to generate CMake with `-D MR_MATH_ENABLE_BENCHMARK=ON`. \
Note that this will not modify `CMAKE_BUILD_TYPE` or add any optimizations to compiler flags. \
In order to add tested compiler flags please specify the preset. \
**Note that specifying preset doesn't affect `CMAKE_BUILD_TYPE` either**.
#### Available presets:
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
