# Makefile for C++

COMPILER = g++
VERSION = -std=c++17
PROJECT_ABSOLUTE_DIR = $(shell pwd)
ENTRY_NAME = Application

CFLAGS = $(VERSION) -g
ASM_FLAGS = $(CFLAGS) -S -fverbose-asm
BIN_FLAGS = $(CFLAGS) -Wall -Wextra
OBJDUMP_FLAGS = -S --disassemble
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
MDFLAGS =
SOURCE_PATHS = $(ENTRY_NAME).cpp src/*.cpp
INCLUDE_DIRS = -I$(PROJECT_ABSOLUTE_DIR)/include -I$(PROJECT_ABSOLUTE_DIR)/src
SHADERS_DIR = resources/shaders

# $(COMPILER) $(ASM_FLAGS) $(INCLUDE_DIRS) -o build/$(ENTRY_NAME).asm $(SOURCE_DIRS) $(LDFLAGS) $(MDFLAGS)

$(ENTRY_NAME): $(ENTRY_NAME).cpp
	@mkdir -p build
	$(shell clean-binaries)
	$(COMPILER) $(BIN_FLAGS) $(INCLUDE_DIRS) -o build/$(ENTRY_NAME) $(SOURCE_PATHS) $(LDFLAGS) $(MDFLAGS)
	objdump $(OBJDUMP_FLAGS) build/$(ENTRY_NAME) > build/$(ENTRY_NAME).dump
	@echo "Build successfully.\n"

.PHONY: compile-shaders debug release clean

compile-shaders:
	@mkdir -p build/shaders
	$(shell clean-shaders)
	glslc $(SHADERS_DIR)/triangle.vert -o build/shaders/triangle_vert.spv
	glslc $(SHADERS_DIR)/triangle.frag -o build/shaders/triangle_frag.spv
	@echo "Shaders compiled successfully.\n"

debug: clean compile-shaders $(ENTRY_NAME)
	@./build/$(ENTRY_NAME)

release: MDFLAGS += -DNDEBUG
release: CFLAGS += -O3
release: clean compile-shaders $(ENTRY_NAME)
	@./build/$(ENTRY_NAME)
	
clean:
	@rm -rf build/*