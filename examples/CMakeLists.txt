cmake_minimum_required(VERSION 3.1)

# Add executables
add_executable(example_arrays example_arrays.cpp)
add_executable(example_classes example_classes.cpp)
add_executable(example_enums example_enums.cpp)
add_executable(example_functions example_functions.cpp)
add_executable(example_tables example_tables.cpp)

set(EXAMPLES example_arrays example_classes example_enums example_functions example_tables)
set(FILES example_arrays.nut example_classes.nut example_enums.nut example_functions.nut example_tables.nut)

foreach(file ${FILES})
    file(COPY ${file} DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${file} DESTINATION ${CMAKE_BINARY_DIR}/examples)
endforeach(file)

# Set properties
foreach(example ${EXAMPLES})
    include_directories(${example} ${INCLUDE_DIRECTORIES} ${SQUIRREL_INCLUDE_DIR})
    link_directories(${example} ${CMAKE_BUILD_DIR})
    target_link_libraries(${example} simplesquirrel_static)
    target_link_libraries(${example} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${example} ${SQSTDLIB_LIBRARIESRARIES})
    add_dependencies(${example} ${PROJECT_NAME})
    add_test(NAME ${example} COMMAND ${example})

    if(MSVC)
        set_target_properties(${example} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)

    set_property(TARGET ${example} PROPERTY FOLDER "simplesquirrel/examples")
endforeach(example)