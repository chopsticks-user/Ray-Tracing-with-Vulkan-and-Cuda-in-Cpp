import sys
import os

AVAILABLE_FLAGS = {
    "--shared": {"-DBUILD_SHARED_LIBS": ["ON", "OFF"]}
}

class Flags(object):
    __flags = DEFAULT_FLAGS
    
    def __init__(self):
        pass
    
    def add(self, flag: str, value: str):
        if(AVAILABLE_FLAGS[flag].count(value) == 0):
            raise Exception("Unknown flag value "+ value)    
        self.__flags[flag] = value
        return self
        

def default_build():
    pass

def parse_argv(argv):
    pass

def build(argv):
    flags = parse_argv(argv)

def main(argv=sys.argv[1:]):
    arg_count = len(argv)
    if arg_count == 0:
        default_build()
    else:
        # build(argv)
        flag_dict[argv[0]]

if __name__ == "__main__":
    main()