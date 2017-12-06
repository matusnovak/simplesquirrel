cmake_minimum_required(VERSION 3.1)

# Find libraries
find_library(SQUIRREL_LIBRARIES NAMES squirrel)
find_library(SQSTDLIB_LIBRARIES NAMES sqstdlib)

# Squirrel libraries
set(INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}" CACHE STRING "Additional include directories for examples and tests")
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" CACHE STRING "Additional library directories for examples and tests")

# Add executables
add_executable(TestClasses classes.cpp)
add_executable(TestFunctions functions.cpp)
add_executable(TestHelloWorld hello_world.cpp)
add_executable(TestObjects objects.cpp)

set(TESTS TestClasses TestFunctions TestHelloWorld TestObjects)

# Set properties
foreach(test ${TESTS})
    include_directories(${test} ${INCLUDE_DIRECTORIES})
    link_directories(${test} ${CMAKE_BUILD_DIR})
    target_link_libraries(${test} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${test} ${SQSTDLIB_LIBRARIES})
    target_link_libraries(${test} squirrelbind)
    add_test(NAME ${test} COMMAND ${test})

    if(MSVC)
        set_target_properties(${test} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)
endforeach(test)