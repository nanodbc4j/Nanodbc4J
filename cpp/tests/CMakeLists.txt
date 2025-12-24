cmake_minimum_required(VERSION 3.20)
project(nanodbc4j_tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Include GoogleTest from submodule
set(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/third_party/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest ${GTEST_BINARY_DIR} EXCLUDE_FROM_ALL)

# Find test source files
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/cpp/tests/*.cpp")

if(NOT TEST_SOURCES)
    message(WARNING "No test sources found in cpp/tests/")
    return()
endif()

# Create test executable
add_executable(nanodbc4j_tests ${TEST_SOURCES})

# Include project headers
target_include_directories(nanodbc4j_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/cpp/include
)

if(WIN32)
    add_custom_command(TARGET nanodbc4j_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:nanodbc4j>
            $<TARGET_FILE_DIR:nanodbc4j_tests>
    )
endif()

# Inherit all dependencies from main library
# (alternative: link explicitly, but easier — through interface)
target_link_libraries(nanodbc4j_tests PRIVATE
    nanodbc4j                 # main library name from root CMakeLists.txt
    GTest::gtest_main
    nanodbc
    fmt::fmt
    spdlog::spdlog_header_only
    ODBC::ODBC
    Threads::Threads
    utfcpp
    bimap
)

# Repeat compilation flags as in main project
if(WIN32)
    target_compile_definitions(nanodbc4j_tests PRIVATE NOMINMAX)
    target_compile_definitions(nanodbc4j_tests PRIVATE SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()

if(UNIX AND NOT APPLE)
    find_library(LIBDL dl REQUIRED)
    target_link_libraries(nanodbc4j_tests PRIVATE ${LIBDL})
endif()

# Register test for ctest
add_test(NAME nanodbc4j_native_tests COMMAND nanodbc4j_tests)

enable_testing()