# Add test files
set(TEST_SOURCES
        test_main.cpp
)

# Add the sources from the main project
set(MAIN_SOURCES
        ../src/lexer.cpp
        ../src/parser.cpp
        ../src/interpreter.cpp
        ../src/utils.cpp
)

# Add the headers from the main project
set(MAIN_HEADERS
        ../src/lexer.h
        ../src/parser.h
        ../src/interpreter.h
        ../src/utils.h
        ../src/ast.h
)

# Add test executable
add_executable(runTests ${TEST_SOURCES} ${MAIN_SOURCES} ${MAIN_HEADERS})

# Include directories
target_include_directories(runTests PRIVATE ../src)

# Enable warnings
if (MSVC)
    target_compile_options(runTests PRIVATE /W4 /WX)
else()
    target_compile_options(runTests PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-variable)
endif()

add_custom_target(copy-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/tests/test_cases
        ${CMAKE_BINARY_DIR}/test_cases
)