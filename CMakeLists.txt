cmake_minimum_required(VERSION 3.20)
project(nanodbc4j CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel" OR CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)    #link time optimization
endif()

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cpp/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/cpp/src)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(UNIX AND NOT APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(NANODBC_DISABLE_INSTALL ON)
set(NANODBC_ENABLE_SHARED OFF)
set(NANODBC_ENABLE_UNICODE ON)
set(NANODBC_DISABLE_EXAMPLES ON)
set(NANODBC_DISABLE_TESTS ON)
set(NANODBC_FORCE_LIBCXX  ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(SPDLOG_FMT_EXTERNAL ON)

find_package(Threads REQUIRED)
find_package(ODBC REQUIRED)

add_subdirectory(third_party/nanodbc)
add_subdirectory(third_party/fmt)
add_subdirectory(third_party/spdlog)

# Ищем все исходники в src/
file(GLOB_RECURSE SRC_FILES "${SOURCE_DIR}/*.cpp" )

# Создаём один DLL файл с содержимым src
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Подключаем заголовочные файлы проекта
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ODBC_EXPORTS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)
    add_definitions(-DSPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()

# Отключаем вывод имени файла и номера строки в исключениях nanodbc
target_compile_definitions(nanodbc PRIVATE NANODBC_THROW_NO_SOURCE_LOCATION)

# utfcpp
add_library(utfcpp INTERFACE)
target_include_directories(utfcpp INTERFACE third_party/utfcpp/source)

# Линкуем зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nanodbc 
    fmt::fmt
    spdlog::spdlog_header_only
    ODBC::ODBC 
    Threads::Threads
    utfcpp
)

if(UNIX AND NOT APPLE)
    find_library(LIBDL dl)
    if(LIBDL)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBDL})
    endif()
endif()

# === Настройки на Linux: переносимость и статическая линковка ===
if(UNIX AND NOT APPLE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${PROJECT_NAME} PRIVATE   -static-libgcc    -static-libstdc++     -Wl,--gc-sections)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_options(${PROJECT_NAME} PRIVATE   -static-libstdc++   -Wl,--gc-sections)
    endif()
endif()