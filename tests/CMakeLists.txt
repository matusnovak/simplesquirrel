cmake_minimum_required(VERSION 3.1)

# Add executables
add_executable(test_classes classes.cpp)
add_executable(test_functions functions.cpp)
add_executable(test_helloworld hello_world.cpp)
add_executable(test_objects objects.cpp)

set(TESTS test_classes test_functions test_helloworld test_objects)

# Set properties
foreach(test ${TESTS})
    include_directories(${test} ${INCLUDE_DIRECTORIES} ${SQUIRREL_INCLUDE_DIR})
    link_directories(${test} ${CMAKE_BUILD_DIR})
    target_link_libraries(${test} simplesquirrel_static)
    target_link_libraries(${test} ${SQUIRREL_LIBRARIES})
    target_link_libraries(${test} ${SQSTDLIB_LIBRARIESRARIES})
    add_dependencies(${test} ${PROJECT_NAME})
    add_test(NAME ${test} COMMAND ${test})

    if(MSVC)
        set_target_properties(${test} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    endif(MSVC)

    if(MSVC)
        set_target_properties(${test} PROPERTIES COMPILE_FLAGS "/bigobj")
    endif()
    if(MINGW)
        set_target_properties(${test} PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj")
    endif()

    set_property(TARGET ${test} PROPERTY FOLDER "simplesquirrel/tests")
endforeach(test)