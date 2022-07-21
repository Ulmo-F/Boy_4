if(NOT DEFINED ENV{NX_EXTERNAL_PATH} OR "$ENV{NX_EXTERNAL_PATH}" STREQUAL "")
    message(FATAL_ERROR "Missing environment variable: NX_EXTERNAL_PATH")
endif()
IF(NOT EXISTS "$ENV{NX_EXTERNAL_PATH}")
    message(FATAL_ERROR "NX_EXTERNAL_PATH '$ENV{NX_EXTERNAL_PATH}' is not reachable")
endif()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS external_dependencies.txt)

set(EXTERNAL_LIBRARIES "")

# Set path to external dependencies
file(STRINGS external_dependencies.txt external_dependencies)
foreach(external_dependency IN LISTS external_dependencies)
    string(REPLACE "|" ";" external_dependency ${external_dependency})
    list(GET external_dependency 0 name)
    list(GET external_dependency 1 base)
    list(GET external_dependency 2 cmake)
    list(GET external_dependency 3 lib)

    string(STRIP "${name}" name)
    string(STRIP "${base}" base)
    string(STRIP "${cmake}" cmake)
    string(STRIP "${lib}" lib)

    set(lib_path $ENV{NX_EXTERNAL_PATH}/${base}/${lib})
    set(EXTERNAL_LIBRARIES "${lib_path}:${EXTERNAL_LIBRARIES}")
    set(CMAKE_PREFIX_PATH "${lib_path};${CMAKE_PREFIX_PATH}")

    # Force find_package() to search again, in case we changed its path
    unset(${name}_DIR CACHE)
endforeach()

configure_file( launch.sh.in ${CMAKE_BINARY_DIR}/launch.sh @ONLY )
