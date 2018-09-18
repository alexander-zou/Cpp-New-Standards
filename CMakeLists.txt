
cmake_minimum_required( VERSION 3.0)

project( Cpp-New-Standards)

include_directories( include/*.h)

set( CPP_STD "-std=c++17")
set( CMAKE_CXX_FLAGS "${CPP_STD} -Wall")
set( CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set( CMAKE_CXX_FLAGS_RELEASE "-O2")
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set( CMAKE_VERBOSE_MAKEFILE ON)


file( GLOB tools_src "tools/*.cpp")

add_subdirectory( src/syntax)


