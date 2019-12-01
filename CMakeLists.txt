cmake_minimum_required(VERSION 3.1)

project(memdag)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(exec src/main.cc src/solve.cc)

set(TEST_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/result.txt)
add_custom_command(
    OUTPUT ${TEST_OUTPUT}
    COMMAND ./run-tcs.sh > ${TEST_OUTPUT}
    COMMAND echo "Result is saved to '${TEST_OUTPUT}'"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS exec
)
add_custom_target(run_tcs DEPENDS ${TEST_OUTPUT})
