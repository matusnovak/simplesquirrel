cmake_minimum_required(VERSION 3.1)
project (simplesquirrel)

set (CMAKE_CXX_STANDARD 11)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_INSTALL "Build install" ON)

if(MSVC)
    link_directories(${CMAKE_BINARY_DIR}/Debug)
    link_directories(${CMAKE_BINARY_DIR}/Release)
endif(MSVC)

# Squirrel libraries
set(INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}" CACHE STRING "Additional include directories for examples and tests")
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" CACHE STRING "Additional library directories for examples and tests")

# Find libraries
find_library(SQUIRREL_LIBRARIES NAMES squirrel_static squirrel)
find_library(SQSTDLIB_LIBRARIES NAMES sqstdlib_static sqstdlib)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_LIBRARY_PATH})

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

if(UNIX OR APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")
endif()

AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/source SOURCES) 
FILE(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/simplesquirrel/*.hpp)

message(STATUS ${SOURCES})
  
# Add our library
add_library(${PROJECT_NAME}_static STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE SSQ_EXPORTS=1 SSQ_DLL=1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} ${SQUIRREL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SQSTDLIB_LIBRARIES})

# Install
if(BUILD_INSTALL)
    if(MSVC)
        INSTALL(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)
    endif()
    INSTALL(TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION lib LIBRARY DESTINATION lib COMPONENT library)
    INSTALL(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin COMPONENT library)
    INSTALL(DIRECTORY include/squirrelbind DESTINATION include)
endif()

# Tests and examples 
if(BUILD_TESTS)
    # Enable testing (done by catch.hpp)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()
