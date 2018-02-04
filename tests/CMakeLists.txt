cmake_minimum_required(VERSION 3.1)

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
    target_link_libraries(${test} simplesquirrel_static)
    target_link_libraries(${test} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${test} ${SQSTDLIB_LIBRARIES})
    add_test(NAME ${test} COMMAND ${test})

    if(MSVC)
        set_target_properties(${test} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)
endforeach(test)