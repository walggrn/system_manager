cmake_minimum_required(VERSION 3.10)
project(Tests)

set(TESTS_SOURCE
    ${CMAKE_SOURCE_DIR}/tests/src/test_parser.cpp
    ${CMAKE_SOURCE_DIR}/tests/src/test_system_manager.cpp
    ${CMAKE_SOURCE_DIR}/tests/src/mock_command.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/SystemManager.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Parser.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/ICommand.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/tests/include)

enable_testing()
add_executable(tests ${TESTS_SOURCE})

target_link_libraries(tests PRIVATE gtest gtest_main)

target_include_directories(tests PRIVATE ${gtest_SOURCE_DIR}/include)

add_test(NAME Tests COMMAND tests)