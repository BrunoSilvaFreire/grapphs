from conans.model.conan_file import ConanFile
from conans.tools import get_env


class GrapphsTestsConan(ConanFile):
    name = "grapphs"
    exports_sources = [
        "CMakeLists.txt",
        "include/*",
        "modules/",
        "samples/",
        "cmake/*"
    ]

    version = get_env("GRAPPHS_CONAN_VERSION")
    license = "MIT"
    author = "Bruno Ribeiro Braga Silva Freire brunorbsf@gmail.com"
    url = "https://github.com/BrunoSilvaFreire/grapphs"
    description = "Performance oriented header-only C++ graph theory library"
    topics = ("algorithm", "algorithms", "graph", "graph-theory", "graph-algorithms", "data-structures")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package", "cmake_find_package_multi"

    def requirements(self):
        self.requires(self.tested_reference_str)
        self.requires("gtest/1.11.0")
        self.requires("nlohmann_json/3.10.5")
