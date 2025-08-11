cmake_minimum_required(VERSION 3.20)
project(odbc CXX)

# Настройка Runtime Library (MT/MD)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cpp/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/cpp/src)

# Отключаем создание импортной библиотеки .dll.a
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)  # Автоматический экспорт символов
set(CMAKE_GNUtoMS ON)                     # Игнорировать .dll.a (только для MinGW)

# Настройки для nanodbc
set(NANODBC_DISABLE_INSTALL ON)
set(NANODBC_ENABLE_SHARED OFF)
set(NANODBC_ENABLE_UNICODE ON)
set(NANODBC_DISABLE_EXAMPLES ON)
set(NANODBC_DISABLE_TESTS ON)
set(NANODBC_FORCE_LIBCXX  ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_definitions(-DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

find_package(Threads REQUIRED)
find_package(ODBC REQUIRED)

add_subdirectory(third_party/nanodbc)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/fmt)

# Ищем все исходники в src/
file(GLOB_RECURSE SRC_FILES "${SOURCE_DIR}/*.cpp" )

# Создаём один DLL файл с содержимым src
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Подключаем заголовочные файлы проекта
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Определяем ODBC_EXPORTS только при сборке библиотеки
target_compile_definitions(${PROJECT_NAME} PRIVATE ODBC_EXPORTS)

# Линкуем зависимости
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nanodbc 
    spdlog::spdlog 
    fmt::fmt 
    ODBC::ODBC 
    Threads::Threads
)
