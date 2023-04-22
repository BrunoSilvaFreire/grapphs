from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conans.model.conan_file import ConanFile
from conans.tools import get_env


class GrapphsConan(ConanFile):
    name = "grapphs"
    exports = "scripts/detect_version.py"
    exports_sources = [
        "CMakeLists.txt",
        "include/*",
        "cmake/**"
    ]
    version = get_env("GRAPPHS_CONAN_VERSION")
    license = "MIT"
    author = "Bruno Ribeiro Braga Silva Freire brunorbsf@gmail.com"
    url = "https://github.com/BrunoSilvaFreire/grapphs"
    description = "Performance oriented header-only C++ graph theory library"
    topics = ("algorithm", "algorithms", "graph", "graph-theory", "graph-algorithms", "data-structures")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package", "cmake_find_package_multi"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        ver = self.version.split('+')[0]
        cmake.configure(variables={
            "GRAPPHS_VERSION": ver,
            "GRAPPHS_COMPILE_SAMPLES": "Off",
            "GRAPPHS_COMPILE_SVG": "Off",
            "GRAPPHS_COMPILE_TESTS": "Off",
            "GRAPPHS_COMPILE_GRAPHVIZ": "Off"
        }
        )

    def package(self):
        cmake = CMake(self)
        cmake.install()
