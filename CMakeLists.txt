cmake_minimum_required(VERSION 3.20)
project(OS_Lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(Main main.cpp marker.cpp event.cpp)
target_link_libraries(Main PRIVATE pthread)

add_executable(Tests tests.cpp marker.cpp event.cpp)
target_link_libraries(Tests PRIVATE pthread)

enable_testing()
add_test(NAME ThreadLogicTests COMMAND Tests)