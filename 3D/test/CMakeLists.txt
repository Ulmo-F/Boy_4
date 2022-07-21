add_executable(3D_test
    cut_test.cpp
)

find_package(GUnit REQUIRED)

target_link_libraries(3D_test PUBLIC 3D GUnit)

gtest_discover_tests(3D_test)
