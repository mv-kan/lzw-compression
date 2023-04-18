cmake_minimum_required(VERSION 3.26)

# kan's lzw implementation (klzw)
set(TARGET 
    klzw
)
set(SOURCES
    src/main.cpp
    src/comptable.h
    src/comptable.cpp
    src/compress.cpp
    src/compress.h
    src/decompress.cpp
    src/decompress.h
    src/klzw.h
    src/utils.h
)

set(TARGET_LIB 
    klzw_shared
)

project(${TARGET})

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Werror")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(${TARGET} ${SOURCES})

add_library(${TARGET_LIB} STATIC ${SOURCES})

target_compile_features(${TARGET} PRIVATE cxx_std_11)

# gtest

# Download and install GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

add_subdirectory(testing)
