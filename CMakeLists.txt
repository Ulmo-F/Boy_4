cmake_minimum_required(VERSION 3.22)

project(Boy)

include(external_dependencies.cmake)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Define GCC specific options here
add_compile_options(-Wall -Wextra)

include(CTest)
include(GoogleTest)
include(CTest)
include(GoogleTest)
enable_testing()
# find_package(GUnit REQUIRED)

add_executable(copy
    stlcopy.cpp
    # cutter.cpp
)

target_link_libraries(copy PRIVATE 3D)

add_subdirectory(3D)

# target_link_libraries(3D PUBLIC GUnit)
