# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/xunililak/Code/Projects/1-VulkanEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xunililak/Code/Projects/1-VulkanEngine/build

# Include any dependencies generated for this target.
include src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/compiler_depend.make

# Include the progress variables for this target.
include src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/progress.make

# Include the compile flags for this target's objects.
include src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/flags.make

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/flags.make
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o: /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/context.cpp
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xunililak/Code/Projects/1-VulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o -MF CMakeFiles/neko_renderer_basic.dir/context.cpp.o.d -o CMakeFiles/neko_renderer_basic.dir/context.cpp.o -c /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/context.cpp

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/neko_renderer_basic.dir/context.cpp.i"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/context.cpp > CMakeFiles/neko_renderer_basic.dir/context.cpp.i

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/neko_renderer_basic.dir/context.cpp.s"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/context.cpp -o CMakeFiles/neko_renderer_basic.dir/context.cpp.s

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/flags.make
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o: /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/window.cpp
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xunililak/Code/Projects/1-VulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o -MF CMakeFiles/neko_renderer_basic.dir/window.cpp.o.d -o CMakeFiles/neko_renderer_basic.dir/window.cpp.o -c /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/window.cpp

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/neko_renderer_basic.dir/window.cpp.i"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/window.cpp > CMakeFiles/neko_renderer_basic.dir/window.cpp.i

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/neko_renderer_basic.dir/window.cpp.s"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/window.cpp -o CMakeFiles/neko_renderer_basic.dir/window.cpp.s

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/flags.make
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o: /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/instance.cpp
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xunililak/Code/Projects/1-VulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o -MF CMakeFiles/neko_renderer_basic.dir/instance.cpp.o.d -o CMakeFiles/neko_renderer_basic.dir/instance.cpp.o -c /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/instance.cpp

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/neko_renderer_basic.dir/instance.cpp.i"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/instance.cpp > CMakeFiles/neko_renderer_basic.dir/instance.cpp.i

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/neko_renderer_basic.dir/instance.cpp.s"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/instance.cpp -o CMakeFiles/neko_renderer_basic.dir/instance.cpp.s

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/flags.make
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o: /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/surface.cpp
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xunililak/Code/Projects/1-VulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o -MF CMakeFiles/neko_renderer_basic.dir/surface.cpp.o.d -o CMakeFiles/neko_renderer_basic.dir/surface.cpp.o -c /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/surface.cpp

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/neko_renderer_basic.dir/surface.cpp.i"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/surface.cpp > CMakeFiles/neko_renderer_basic.dir/surface.cpp.i

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/neko_renderer_basic.dir/surface.cpp.s"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic/surface.cpp -o CMakeFiles/neko_renderer_basic.dir/surface.cpp.s

# Object files for target neko_renderer_basic
neko_renderer_basic_OBJECTS = \
"CMakeFiles/neko_renderer_basic.dir/context.cpp.o" \
"CMakeFiles/neko_renderer_basic.dir/window.cpp.o" \
"CMakeFiles/neko_renderer_basic.dir/instance.cpp.o" \
"CMakeFiles/neko_renderer_basic.dir/surface.cpp.o"

# External object files for target neko_renderer_basic
neko_renderer_basic_EXTERNAL_OBJECTS =

src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/context.cpp.o
src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/window.cpp.o
src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/instance.cpp.o
src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/surface.cpp.o
src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/build.make
src/renderer/basic/libneko_renderer_basic.a: src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xunililak/Code/Projects/1-VulkanEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libneko_renderer_basic.a"
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && $(CMAKE_COMMAND) -P CMakeFiles/neko_renderer_basic.dir/cmake_clean_target.cmake
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/neko_renderer_basic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/build: src/renderer/basic/libneko_renderer_basic.a
.PHONY : src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/build

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/clean:
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic && $(CMAKE_COMMAND) -P CMakeFiles/neko_renderer_basic.dir/cmake_clean.cmake
.PHONY : src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/clean

src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/depend:
	cd /home/xunililak/Code/Projects/1-VulkanEngine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xunililak/Code/Projects/1-VulkanEngine /home/xunililak/Code/Projects/1-VulkanEngine/src/renderer/basic /home/xunililak/Code/Projects/1-VulkanEngine/build /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic /home/xunililak/Code/Projects/1-VulkanEngine/build/src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/renderer/basic/CMakeFiles/neko_renderer_basic.dir/depend

