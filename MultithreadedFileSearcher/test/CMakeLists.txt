# This file is processed because of the 'add_subdirectory(test)' in the parent CMakeLists.txt.

# Define your test executable. It includes your test source file.
add_executable(test_search test_search.cpp)

# Link the test executable with required libraries:
# - core_lib: Your project's core logic.
# - GTest::gtest_main: Provides the main() function for Google Test, simplifying your test code.
# - GTest::gtest: The main Google Test library with assertions.
# - stdc++fs: For C++17 filesystem usage in your tests (e.g., FileSearcherTest fixture).
# - Threads::Threads: For threading support (e.g., ThreadPool tests).
target_link_libraries(test_search PRIVATE
    core_lib
    GTest::gtest_main
    GTest::gtest
    Threads::Threads
)

# Add the test to CTest.
# CTest uses this definition to know which executable to run as a test.
# $<TARGET_FILE:test_search> is a generator expression that resolves to the full path
# of the 'test_search' executable at build time, ensuring CTest finds it correctly.
add_test(NAME run_unit_tests COMMAND $<TARGET_FILE:test_search>)