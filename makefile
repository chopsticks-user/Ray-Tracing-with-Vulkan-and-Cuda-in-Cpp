# Makefile for C++

COMPILER = g++
VERSION = -std=c++20
PROJECT_ABSOLUTE_DIR = $(shell pwd)
ENTRY_NAME = Application

CFLAGS = $(VERSION) -g
ASM_FLAGS = $(CFLAGS) -S -fverbose-asm
BIN_FLAGS = $(CFLAGS) -Wall -Wextra -pedantic -Wextra -Wshadow -Wconversion
OBJDUMP_FLAGS = -S --disassemble
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
MDFLAGS =

SHADERS_DIR = resources/shaders
SOURCE_PATHS = $(ENTRY_NAME).cpp \
	$(shell find $(PROJECT_ABSOLUTE_DIR)/src -type f -name "*.cpp")

INCLUDE_DIRS = -I$(PROJECT_ABSOLUTE_DIR)/include -I$(PROJECT_ABSOLUTE_DIR)/src

$(ENTRY_NAME): $(ENTRY_NAME).cpp
	@mkdir -p build
	@$(COMPILER) $(BIN_FLAGS) $(INCLUDE_DIRS) -o build/$(ENTRY_NAME) $(SOURCE_PATHS) $(LDFLAGS) $(MDFLAGS)
	@objdump $(OBJDUMP_FLAGS) build/$(ENTRY_NAME) > build/$(ENTRY_NAME).dump
	@echo "Build successfully."

.PHONY: compile-shaders build-debug build-release run-debug run-release cleans
	
# compile-shaders:
# 	@mkdir -p build/shaders
# 	@glslc $(SHADERS_DIR)/basic.vert -o build/shaders/basic_vert.spv
# 	@glslc $(SHADERS_DIR)/basic.frag -o build/shaders/basic_frag.spv
# 	@echo "Shaders compiled successfully."

build-debug: clean compile-shaders $(ENTRY_NAME)

build-release: MDFLAGS += -DNDEBUG
build-release: CFLAGS += -O3
build-release: clean compile-shaders $(ENTRY_NAME)

run-debug: build-debug
	@./build/$(ENTRY_NAME)

run-release: build-release
	@./build/$(ENTRY_NAME)
	
clean:
	@rm -rf build/*
	@cp MangoHud.conf build/MangoHud.conf