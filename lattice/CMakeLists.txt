cmake_minimum_required(VERSION 3.16)
project(lattice_commitment LANGUAGES CXX)

# ------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address")

# ------------------------------------------------------------
# Compiler warnings (strong but sane)
# ------------------------------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wno-sign-conversion
    )
endif()

# ------------------------------------------------------------
# Source files
# ------------------------------------------------------------
set(SOURCES
    main.cpp
    commitment.cpp
)

# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------
add_executable(lattice_test ${SOURCES})

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------
target_include_directories(lattice_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------
# Optimization flags (Debug vs Release)
# ------------------------------------------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(lattice_test PRIVATE -O3)
else()
    target_compile_options(lattice_test PRIVATE -O0 -g)
endif()
