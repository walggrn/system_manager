cmake_minimum_required(VERSION 3.31)
project(SystemManager)
#cmake
set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Copy.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Create.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Delete.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Dir.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Find.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/ICommand.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Insert.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Modify.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/Read.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Parser.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Log.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/SystemManager.cpp
) 

set(HEADERS ${CMAKE_SOURCE_DIR}/include)

set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist)

set(DOCS_DIR ${CMAKE_SOURCE_DIR}/docs)
set(DOXYFILE_PATH ${CMAKE_SOURCE_DIR}/Doxyfile)

set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wpedantic -Iinclude -MMD -MP")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(SystemManager ${SOURCES})
target_include_directories(SystemManager PRIVATE ${HEADERS})

target_link_libraries(SystemManager gtest gtest_main)
enable_testing()
add_subdirectory(tests)

add_custom_target(run
    COMMAND SystemManager
    DEPENDS SystemManager
    COMMENT "Project was running"
)

add_custom_target(installe
    COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove -f ${INSTALL_DIR}/${EXE_FILE}.exe
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/SystemManager.exe ${INSTALL_DIR}/SystemManager.exe
    COMMENT "Executable file was copied at '/bin'"
    DEPENDS SystemManager
)

add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DIST_DIR}/${EXE_FILE}.exe
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/SystemManager.exe ${DIST_DIR}/SystemManager.exe
    COMMAND ${CMAKE_COMMAND} -E tar -czvf ${DIST_DIR}/${EXE_FILE}.tar.gz -- ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DIST_DIR}/SystemManager.exe
    DEPENDS SystemManager
    COMMENT "Distribution was created at '/dist'"
)


add_custom_target(docs
    COMMAND doxygen ${DOXYFILE_PATH}
    COMMENT "Documentation was generated at '/docs'"
)

add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TEMP_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${INSTALL_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOCS_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DIST_DIR}
    COMMENT "All generated files was removed"
)

add_custom_target(guide
    COMMAND ${CMAKE_COMMAND} -E echo "Usage:"
    COMMAND ${CMAKE_COMMAND} -E echo "make                           Build the project"
    COMMAND ${CMAKE_COMMAND} -E echo "make run                       Build and run the project"
    COMMAND ${CMAKE_COMMAND} -E echo "make installe               Install the executable file"
    COMMAND ${CMAKE_COMMAND} -E echo "make dist                      Create distribution package"
    COMMAND ${CMAKE_COMMAND} -E echo "make docs                      Generate documentation"
    COMMAND ${CMAKE_COMMAND} -E echo "make distclean                  Reset project"
    COMMAND ${CMAKE_COMMAND} -E echo "make guide                     Show guide"
)