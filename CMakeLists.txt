cmake_minimum_required(VERSION 3.10)
project(cmdcalc VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(calc_lib src/calculator.cpp src/tokenizer.cpp)
 target_include_directories(calc_lib PUBLIC include)

add_executable(cmdcalc src/main.cpp)
 target_link_libraries(cmdcalc PRIVATE calc_lib)

add_executable(calc_tests tests/test_calculator.cpp)
 target_link_libraries(calc_tests PRIVATE calc_lib)

enable_testing()
add_test(NAME calc_tests COMMAND calc_tests)
