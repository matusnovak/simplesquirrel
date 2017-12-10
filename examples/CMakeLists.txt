cmake_minimum_required(VERSION 3.1)

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
    target_link_libraries(${example} squirrelbind_static)
    target_link_libraries(${example} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${example} ${SQSTDLIB_LIBRARIES})
    add_test(NAME ${example} COMMAND ${example})

    if(MSVC)
        set_target_properties(${example} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)
endforeach(example)