import os
import shutil
from conans import ConanFile, CMake

PWD = os.getcwd() + '/'

class NekoEngineConan(ConanFile):
    requires = "nlohmann_json/3.11.2"
    generators = "cmake"
    
    def imports(self):
        pass
    
    def build(self):
        try:
            shutil.copy(PWD + "configs/MangoHud.conf", 
                        PWD + "build/MangoHud.conf")
        except OSError:
            raise Exception("shutil.copy() failed")
        cmake = CMake(self)
        cmake.configure()
        cmake.build()