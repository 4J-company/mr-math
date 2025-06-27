from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.cmake import CMakeDeps
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import CMake
from conan.tools.build import check_min_cppstd

class MrMath(ConanFile):
    name = "mr-math"

    version = "1.0.0"

    license = "MIT"

    description = "Linear algebra library for computer graphics"

    topics = ("math", "linear algebra", "computer graphics", "SIMD")

    homepage = "https://github.com/4J-company/mr-math"

    package_type = "header-library"

    settings = ("os", "compiler", "build_type", "arch")

    options = {
        "target": ["library", "tests", "benchmark", "all"],
        "profiling": [True, False],
        "extra_optimized": [True, False],
    }
    
    default_options = {
        "target": "library",
        "profiling": False,
        "extra_optimized": False,
    }

    def _tests_enabled(self):
        return self.options.target in ("tests", "all")    
    
    def _benchmark_enabled(self):
        return self.options.target in ("benchmark", "all")

    def _profiling_enabled(self):
        if not self._benchmark_enabled():
            return False
        return self.options.profiling

    def validate(self):
        check_min_cppstd(self, "20")

    def configure(self):
        if not self._benchmark_enabled():
            self.options.rm_safe("profiling")

    def requirements(self):
        self.requires("vc/1.4.3-4j")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.27]")
        self.tool_requires("ninja/[~1]")

        if self._tests_enabled():
            self.test_requires("gtest/1.14.0")
            
        if self._benchmark_enabled():
            self.test_requires("benchmark/1.9.1")

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.presets_prefix = "mr-math"
        toolchain.cache_variables["MR_MATH_ENABLE_TESTING"] = self._tests_enabled()
        toolchain.cache_variables["MR_MATH_ENABLE_BENCHMARK"] = self._benchmark_enabled()
        toolchain.cache_variables["MR_MATH_ENABLE_PROFILING"] = self._profiling_enabled()
        toolchain.cache_variables["MR_MATH_EXTRA_OPTIMIZED"] = self.options.extra_optimized
        toolchain.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        cmake_layout(self)
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
