cmake_minimum_required(VERSION 3.26)

set(TARGET_TEST
    klzwtest
)
set(SOURCES_TEST
    compress.cpp
    decompress.cpp
)

# Add the test executable
add_executable(${TARGET_TEST} ${SOURCES_TEST})

# Link against GoogleTest
target_link_libraries(${TARGET_TEST} PUBLIC gtest gtest_main klzw_shared)

# Add the tests to CTest
add_test(NAME ${TARGET_TEST} COMMAND ${TARGET_TEST})