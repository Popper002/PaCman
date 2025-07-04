#**************************************************************************************************
#
#   PaCman Makefile using raylib
#
#**************************************************************************************************

.PHONY: all clean main

# Define required environment variables
#------------------------------------------------------------------------------------------------
# Define target platform: PLATFORM_DESKTOP
PLATFORM              ?= PLATFORM_DESKTOP

# Define required raylib variables
PROJECT_NAME          ?= pacman
RAYLIB_VERSION        ?= 5.5.0
RAYLIB_PATH           ?= src/utils/raylib

# Define raylib source code path
RAYLIB_SRC_PATH       ?= src/utils/raylib/src

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# Library type compilation: STATIC (.a)
RAYLIB_LIBTYPE        ?= STATIC

# Determine PLATFORM_OS when required
# No uname.exe on MinGW!, but OS=Windows_NT on Windows!
ifeq ($(OS),Windows_NT)
    PLATFORM_OS = WINDOWS
else
    UNAMEOS = $(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS = LINUX
    endif
    ifeq ($(UNAMEOS),Darwin)
        PLATFORM_OS = OSX
    endif
endif

# Define default C compiler: CC
#------------------------------------------------------------------------------------------------
CC = gcc

ifeq ($(PLATFORM_OS),OSX)
    # OSX default compiler
    CC = clang
endif

# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
else
    CFLAGS += -O2
endif

# Define include paths for required headers: INCLUDE_PATHS
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS = -I. -I$(RAYLIB_SRC_PATH) -I$(RAYLIB_SRC_PATH)/external

# Define library paths containing required libs: LDFLAGS
#------------------------------------------------------------------------------------------------
LDFLAGS = -L. -L$(RAYLIB_SRC_PATH)

ifeq ($(PLATFORM_OS),OSX)
    # Libraries for OSX desktop compiling
    LDLIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif

ifeq ($(PLATFORM_OS),LINUX)
    # Libraries for Linux desktop compiling
    LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic
endif

ifeq ($(PLATFORM_OS),WINDOWS)
    # Libraries for Windows desktop compilation
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
endif

# Define source files
#------------------------------------------------------------------------------------------------
SOURCE_FILES = src/main.c src/pacman.c

# Define processes to execute
#------------------------------------------------------------------------------------------------
# Default target entry
all: $(PROJECT_NAME)

# Main target (alias for all)
main: $(PROJECT_NAME)

# Build raylib static library if it doesn't exist
$(RAYLIB_SRC_PATH)/libraylib.a:
	$(MAKE) -C $(RAYLIB_SRC_PATH)

# Build project
$(PROJECT_NAME): $(RAYLIB_SRC_PATH)/libraylib.a $(SOURCE_FILES)
	$(CC) -o $(PROJECT_NAME) $(SOURCE_FILES) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# Clean everything
clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	del *.o *.exe
else
	rm -fv $(PROJECT_NAME) *.o
endif
	@echo Cleaning done

# Install raylib (optional)
install-raylib:
	$(MAKE) -C $(RAYLIB_SRC_PATH) install

.PHONY: install-raylib