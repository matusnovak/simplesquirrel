cmake_minimum_required(VERSION 3.1)

# Find libraries
find_library(SQUIRREL_LIBRARIES NAMES squirrel_static squirrel)
find_library(SQSTDLIB_LIBRARIES NAMES sqstdlib_static sqstdlib)

# Squirrel libraries
set(INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}" CACHE STRING "Additional include directories for examples and tests")
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" CACHE STRING "Additional library directories for examples and tests")

# Add executables
add_executable(ExampleArrays example_arrays.cpp)
add_executable(ExampleClasses example_classes.cpp)
add_executable(ExampleEnums example_enums.cpp)
add_executable(ExampleFunctions example_functions.cpp)
add_executable(ExampleTables example_tables.cpp)

set(EXAMPLES ExampleArrays ExampleClasses ExampleEnums ExampleFunctions ExampleTables)
set(FILES example_arrays.nut example_classes.nut example_enums.nut example_functions.nut example_tables.nut)

foreach(file ${FILES})
    file(COPY ${file} DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${file} DESTINATION ${CMAKE_BINARY_DIR}/examples)
endforeach(file)

# Set properties
foreach(example ${EXAMPLES})
    include_directories(${example} ${INCLUDE_DIRECTORIES})
    link_directories(${example} ${CMAKE_BUILD_DIR})
    target_link_libraries(${example} squirrelbind)
    target_link_libraries(${example} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${example} ${SQSTDLIB_LIBRARIES})
    add_test(NAME ${example} COMMAND ${example})

    if(MSVC)
        set_target_properties(${example} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)
endforeach(example)