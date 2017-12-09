cmake_minimum_required(VERSION 3.1)
project (squirrelbind)

set (CMAKE_CXX_STANDARD 11)
option(BUILD_TESTS "Build tests" ON)

if(MSVC)
    link_directories(${CMAKE_BINARY_DIR}/Debug)
    link_directories(${CMAKE_BINARY_DIR}/Release)
endif(MSVC)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_LIBRARY_PATH})

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/source SOURCES) 
FILE(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/squirrelbind/*.hpp)

message(STATUS ${SOURCES})
  
# Add our library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# Headers
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/squirrelbind.hpp"
    DESTINATION "include"
)

# Tests and examples 
if(BUILD_TESTS)
    # Enable testing (done by catch.hpp)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()
