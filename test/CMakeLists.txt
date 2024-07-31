cmake_minimum_required (VERSION 3.12)

find_package(GTest CONFIG REQUIRED)

add_executable(MathsTest "Vector.cpp")
set_target_properties(MathsTest PROPERTIES LINKER_LANGUAGE CXX) # CMake will try to infer off file names making this unnecesary oftentimes.
set_target_properties(MathsTest PROPERTIES CXX_STANDARD 23)
set_target_properties(MathsTest PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(MathsTest PRIVATE GTest::gtest_main GTest::gtest)
target_link_libraries(MathsTest PRIVATE ${PROJECT_NAME}_static)

add_test(MathsTest MathsTest)