from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conans.model.conan_file import ConanFile
from conans.tools import get_env


def as_cmake_option(flagged):
    if flagged:
        return "On"
    else:
        return "Off"


class GrapphsConan(ConanFile):
    name = "grapphs"
    exports = "scripts/detect_version.py"
    exports_sources = [
        "CMakeLists.txt",
        "include/*",
        "cmake/**",
        "modules/**/*"
    ]
    version = get_env("GRAPPHS_CONAN_VERSION")
    license = "MIT"
    author = "Bruno Ribeiro Braga Silva Freire brunorbsf@gmail.com"
    url = "https://github.com/BrunoSilvaFreire/grapphs"
    description = "Performance oriented header-only C++ graph theory library"
    topics = ("algorithm", "algorithms", "graph", "graph-theory", "graph-algorithms", "data-structures")

    options = {
        "svg_module": [False, True],
        "graphviz_module": [False, True]
    }
    default_options = {
        "svg_module": True,
        "graphviz_module": True
    }

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

        build_svg = self.options.svg_module
        build_graphviz = self.options.graphviz_module

        needs_build = build_svg or build_graphviz

        cmake.configure(
            variables={
                "GRAPPHS_VERSION": ver,
                "GRAPPHS_COMPILE_SAMPLES": "Off",
                "GRAPPHS_COMPILE_TESTS": "Off",
                "GRAPPHS_COMPILE_SVG": as_cmake_option(build_svg),
                "GRAPPHS_COMPILE_GRAPHVIZ": as_cmake_option(build_graphviz),
                "CONAN_EXPORTED": "TRUE"
            }
        )
        if needs_build:
            cmake.build()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "Grapphs")
        self.cpp_info.set_property("cmake_target_name", "grapphs::grapphs")
        self.cpp_info.set_property("cmake_target_aliases", ["grapphs"])
        self.cpp_info.set_property("pkg_config_name", f"grapphs")

        libs = []
        if self.options.svg_module:
            libs.append("graphviz/lib/libgrapphs-svg")

        if self.options.graphviz_module:
            libs.append("graphviz/lib/libgrapphs-graphviz")

        self.cpp_info.libs = libs

        # TODO: to remove in conan v2 once cmake_find_package* & pkg_config generators removed
        self.cpp_info.build_modules["cmake_find_package"] = "Grapphs"
        self.cpp_info.build_modules["cmake_find_package_multi"] = "Grapphs"

    def package(self):
        cmake = CMake(self)
        cmake.install()
