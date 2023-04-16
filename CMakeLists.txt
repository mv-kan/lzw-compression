cmake_minimum_required(VERSION 3.12)

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

project(${TARGET})

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(${TARGET} ${SOURCES})

target_compile_features(${TARGET} PRIVATE cxx_std_11)