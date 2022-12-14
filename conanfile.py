import os
import shutil
from conans import ConanFile, CMake

class NekoEngineConan(ConanFile):
    requires = "nlohmann_json/3.11.2"
    generators = "cmake"
    
    def imports(self):
        pass
    
    def build(self):
        try:
            shutil.copy("scripts/MangoHud.conf", "build/MangoHud.conf")
        except OSError:
            pass
        cmake = CMake(self)
        cmake.configure()
        cmake.build()