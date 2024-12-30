cmake_minimum_required (VERSION 3.12)

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}Test
"CustomMatchers.h"
"Maths/Maths.cpp"
"Maths/Vector.cpp" 
"Maths/Matrix.cpp" "Maths/Matrix3x3.cpp" "Maths/Matrix4x4.cpp" 
"Maths/PolarCoordinates.cpp"
"Utility/BitFlags.cpp")

set_target_properties(${PROJECT_NAME}Test PROPERTIES LINKER_LANGUAGE CXX) # CMake will try to infer off file names making this unnecesary oftentimes.
set_target_properties(${PROJECT_NAME}Test PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME}Test PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME}Test PRIVATE GTest::gmock_main GTest::gtest GTest::gmock)
target_link_libraries(${PROJECT_NAME}Test PRIVATE ${PROJECT_NAME}_static)

add_test(${PROJECT_NAME}Test ${PROJECT_NAME}Test)