cmake_minimum_required(VERSION 3.1)
project (simplesquirrel)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# Some required properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_INSTALL "Install library" ON)

# Add third party libraries
if(NOT DEFINED SQUIRREL_LIBRARIES AND NOT DEFINED SQSTDLIB_LIBRARIESRARIES)
  ExternalProject_Add(SQUIRREL
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/squirrel
    CMAKE_ARGS -DDISABLE_STATIC=OFF -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
    BUILD_COMMAND cmake --build . --config ${CMAKE_BUILD_TYPE}
    INSTALL_COMMAND ""
    TEST_COMMAND ""
  )

  if(MSVC)
    set(SQUIRREL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/squirrel/include)
    set(SQUIRREL_LIBRARIES ${CMAKE_CURRENT_BINARY_DIR}/SQUIRREL-prefix/src/SQUIRREL-build/squirrel/${CMAKE_BUILD_TYPE}/squirrel_static.lib)
    set(SQSTDLIB_LIBRARIESRARIES ${CMAKE_CURRENT_BINARY_DIR}/SQUIRREL-prefix/src/SQUIRREL-build/sqstdlib/${CMAKE_BUILD_TYPE}/sqstdlib_static.lib)
  else()
    set(SQUIRREL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/squirrel/include)
    set(SQUIRREL_LIBRARIES ${CMAKE_CURRENT_BINARY_DIR}/SQUIRREL-prefix/src/SQUIRREL-build/squirrel/libsquirrel_static.a)
    set(SQSTDLIB_LIBRARIESRARIES ${CMAKE_CURRENT_BINARY_DIR}/SQUIRREL-prefix/src/SQUIRREL-build/sqstdlib/libsqstdlib_static.a)
  endif()
endif()

# Grab the files
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/simplesquirrel/*.hpp)

include_directories(${INCLUDE_DIRECTORIES} ${SQUIRREL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_LIBRARY_PATH})

# Add our library
add_library(${PROJECT_NAME}_static STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
add_dependencies(${PROJECT_NAME} SQUIRREL)
add_dependencies(${PROJECT_NAME}_static SQUIRREL)
target_compile_definitions(${PROJECT_NAME} PRIVATE SSQ_EXPORTS=1 SSQ_DLL=1)
target_link_libraries(${PROJECT_NAME} ${SQUIRREL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SQSTDLIB_LIBRARIESRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "simplesquirrel/lib")
set_property(TARGET ${PROJECT_NAME}_static PROPERTY FOLDER "simplesquirrel/lib")

# Install
if(BUILD_INSTALL)
  if(MSVC)
      INSTALL(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)
  endif()
  INSTALL(TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION lib LIBRARY DESTINATION lib COMPONENT library)
  INSTALL(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin COMPONENT library)
  INSTALL(DIRECTORY include/simplesquirrel DESTINATION include)
endif()

# Build Tests
if(BUILD_TESTS)
    # Enable testing (done by catch.hpp)
    enable_testing()
    add_subdirectory(tests)
endif()

# Build Examples 
if(BUILD_TESTS)
    add_subdirectory(examples)
endif()
