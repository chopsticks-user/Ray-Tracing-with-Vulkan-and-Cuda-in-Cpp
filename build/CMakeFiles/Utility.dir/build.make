# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xunililak/Code/Projects/4-RTVulkan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xunililak/Code/Projects/4-RTVulkan/build

# Include any dependencies generated for this target.
include CMakeFiles/Utility.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Utility.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Utility.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Utility.dir/flags.make

CMakeFiles/Utility.dir/src/Utility.cpp.o: CMakeFiles/Utility.dir/flags.make
CMakeFiles/Utility.dir/src/Utility.cpp.o: /home/xunililak/Code/Projects/4-RTVulkan/src/Utility.cpp
CMakeFiles/Utility.dir/src/Utility.cpp.o: CMakeFiles/Utility.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xunililak/Code/Projects/4-RTVulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Utility.dir/src/Utility.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Utility.dir/src/Utility.cpp.o -MF CMakeFiles/Utility.dir/src/Utility.cpp.o.d -o CMakeFiles/Utility.dir/src/Utility.cpp.o -c /home/xunililak/Code/Projects/4-RTVulkan/src/Utility.cpp

CMakeFiles/Utility.dir/src/Utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Utility.dir/src/Utility.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xunililak/Code/Projects/4-RTVulkan/src/Utility.cpp > CMakeFiles/Utility.dir/src/Utility.cpp.i

CMakeFiles/Utility.dir/src/Utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Utility.dir/src/Utility.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xunililak/Code/Projects/4-RTVulkan/src/Utility.cpp -o CMakeFiles/Utility.dir/src/Utility.cpp.s

# Object files for target Utility
Utility_OBJECTS = \
"CMakeFiles/Utility.dir/src/Utility.cpp.o"

# External object files for target Utility
Utility_EXTERNAL_OBJECTS =

libUtility.a: CMakeFiles/Utility.dir/src/Utility.cpp.o
libUtility.a: CMakeFiles/Utility.dir/build.make
libUtility.a: CMakeFiles/Utility.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xunililak/Code/Projects/4-RTVulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libUtility.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Utility.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Utility.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Utility.dir/build: libUtility.a
.PHONY : CMakeFiles/Utility.dir/build

CMakeFiles/Utility.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Utility.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Utility.dir/clean

CMakeFiles/Utility.dir/depend:
	cd /home/xunililak/Code/Projects/4-RTVulkan/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xunililak/Code/Projects/4-RTVulkan /home/xunililak/Code/Projects/4-RTVulkan /home/xunililak/Code/Projects/4-RTVulkan/build /home/xunililak/Code/Projects/4-RTVulkan/build /home/xunililak/Code/Projects/4-RTVulkan/build/CMakeFiles/Utility.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Utility.dir/depend

