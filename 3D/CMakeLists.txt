
add_library(3D SHARED
    # ${sources}
    src/Facet.cpp
    src/FileUtilities.cpp
    src/Normal.cpp
    src/Plane.cpp
    src/Transformation.cpp
)

target_include_directories(3D PRIVATE include/3D)
target_include_directories(3D INTERFACE include)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
