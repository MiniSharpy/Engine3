cmake_minimum_required (VERSION 3.12)

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}_test "Vector.cpp")
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE CXX) # CMake will try to infer off file names making this unnecesary oftentimes.
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main GTest::gtest)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_static)

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)