##                                                                                                ##
##  File: CMakeLists.txt                                                                          ##
##                                                                                                ##
##  These coded statements, instructions, and computer programs contain unpublished proprietary   ##
##  informations and are protected by copyright law. They may not be disclosed to third parties   ##
##  or copied or duplicated in any form without the written consent of nanoXplore.                ##
##                                                                                                ##



##                                                                                                ##
##  Global options                                                                                ##
##                                                                                                ##

cmake_minimum_required(VERSION 3.22)

project(3D)

if(NOT DEFINED ENV{NX_EXTERNAL_PATH} OR "$ENV{NX_EXTERNAL_PATH}" STREQUAL "")
    message(FATAL_ERROR "Missing environment variable: NX_EXTERNAL_PATH")
endif()
IF(NOT EXISTS "$ENV{NX_EXTERNAL_PATH}")
    message(FATAL_ERROR "NX_EXTERNAL_PATH '$ENV{NX_EXTERNAL_PATH}' is not reachable")
endif()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS external_dependencies.txt)

# Set path to external dependencies
file(STRINGS external_dependencies.txt external_dependencies)
foreach(external_dependency IN LISTS external_dependencies)
    string(REPLACE "|" ";" external_dependency ${external_dependency})
    list(GET external_dependency 0 name)
#    list(GET external_dependency 1 version)
    list(GET external_dependency 2 base)
    list(GET external_dependency 3 cmake)
#    list(GET external_dependency 4 lib)
#    list(GET external_dependency 5 extra)

    string(STRIP "${name}" name)
    string(STRIP "${base}" base)
    string(STRIP "${cmake}" cmake)

    set(lib_path ${base}/${cmake})
    set(CMAKE_PREFIX_PATH "$ENV{NX_EXTERNAL_PATH}/${lib_path};${CMAKE_PREFIX_PATH}")

    # Force find_package() to search again, in case we changed its path
    unset(${name}_DIR CACHE)
endforeach()



if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define GCC specific options here
add_compile_options(-Wall -Wextra)

# include(CTest)
# include(GoogleTest)
# enable_testing()
# find_package(GUnit REQUIRED)

add_executable(3D
    stlcopy.cpp
    cutter.cpp
    stlloader.cpp
)

# target_link_libraries(3D PUBLIC GUnit)
