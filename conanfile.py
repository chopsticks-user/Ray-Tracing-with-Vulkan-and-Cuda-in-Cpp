from tools.conan import prebuild
from conans import ConanFile, CMake

class NekoEngineConan(ConanFile):
    requires = "nlohmann_json/3.11.2"
    generators = "cmake"
    
    def imports(self):
        pass
    
    def build(self):
        prebuild.setup_monitoring_config("data/configs/MangoHud.conf")
        cmake = CMake(self)
        cmake.definitions["BUILD_SHARED_LIBS"] = "OFF"
        cmake.definitions["CMAKE_BUILD_TYPE"] = "Debug"
        cmake.configure()
        cmake.build()
        