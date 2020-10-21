cmake_minimum_required(VERSION 3.14...3.18)

project(FetchExample LANGUAGES CXX)

#include(FetchContent)
include(CTest)

#FetchContent_Declare(
#  Catch2
#  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#  GIT_TAG v2.13.0)

# CMake 3.14+
#FetchContent_MakeAvailable(Catch2)

find_package(Catch2 REQUIRED)

# Define shared libraries
add_library(hdf5lib SHARED IMPORTED)
set_target_properties(hdf5lib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../hdf5-1.10.7-macOS-10.14-x86_64-clang10.0.1/lib/libhdf5.dylib)
add_library(h5hllib SHARED IMPORTED)
set_target_properties(h5hllib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../hdf5-1.10.7-macOS-10.14-x86_64-clang10.0.1/lib/libhdf5_hl.dylib)

# Testing
enable_testing()

# Create the test executable
add_executable(hdf5_test ../test/hdf5test.cpp Hdf5File.cpp)
# Add include directories
target_include_directories(hdf5_test PUBLIC ${CMAKE_SOURCE_DIR}/include)
# Link in the defined libraries to the target
target_link_libraries(hdf5_test PRIVATE hdf5lib)
target_link_libraries(hdf5_test PRIVATE h5hllib)
target_link_libraries(hdf5_test PRIVATE Catch2::Catch2)
add_test(NAME hdf5_test COMMAND hdf5_test)
