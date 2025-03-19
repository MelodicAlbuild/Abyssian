cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Abyssian LANGUAGES CXX VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
        src/main.cpp
        src/lexer.cpp
        src/parser.cpp
        src/interpreter.cpp
        src/utils.cpp
)

# Add header files
set(HEADERS
        src/lexer.h
        src/parser.h
        src/interpreter.h
        src/utils.h
        src/ast.h
)

# Add the executable
add_executable(Abyssian ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(Abyssian PRIVATE src)

# Enable warnings
if (MSVC)
    target_compile_options(Abyssian PRIVATE /W4 /WX)
else()
    target_compile_options(Abyssian PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Add subdirectory for tests
add_subdirectory(tests)