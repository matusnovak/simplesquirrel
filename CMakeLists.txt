cmake_minimum_required(VERSION 3.1)
project (simplesq)

set (CMAKE_CXX_STANDARD 11)
  
# Add our library
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE include/simplesq.hpp)

# Headers
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/simplesq.hpp"
    DESTINATION "include"
)

# Tests and examples >>

# Squirrel libraries
set(INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}" CACHE STRING "Additional include directories for examples and tests")
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" CACHE STRING "Additional library directories for examples and tests")

# Find libraries
find_library(SQUIRREL_LIBRARIES NAMES squirrel)
find_library(SQSTDLIB_LIBRARIES NAMES sqstdlib)

# Add executables
add_executable(simplesq_tests tests.cpp)
add_executable(simplesq_examples examples.cpp)

# Add dependencies
include_directories(simplesq_tests ${INCLUDE_DIRECTORIES})
include_directories(simplesq_examples ${INCLUDE_DIRECTORIES})

target_link_libraries(simplesq_tests ${SQUIRREL_LIBRARIES})
target_link_libraries(simplesq_tests ${SQSTDLIB_LIBRARIES})

target_link_libraries(simplesq_examples ${SQUIRREL_LIBRARIES})
target_link_libraries(simplesq_examples ${SQSTDLIB_LIBRARIES})

# Enable testing (done by catch.hpp)
enable_testing()
add_test(NAME simplesq_tests COMMAND simplesq_tests)
add_test(NAME simplesq_examples COMMAND simplesq_examples)


