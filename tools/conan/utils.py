from tools.conan import sys_constants
import shutil

def cpy(src: str, dst: str):
    try:
        shutil.copy(sys_constants.PWD + src, 
                    sys_constants.PWD + dst)
    except OSError:
        raise Exception(f"Failed to copy ${src} to ${dst}")