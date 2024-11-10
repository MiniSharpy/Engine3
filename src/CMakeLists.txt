cmake_minimum_required (VERSION 3.12)

# Copy data folder to build directory. Hard links might be better, then there's less duplicate data. CMAKE_CURRENT_SOURCE_DIR might be better instead of LIST.
add_custom_target(copy_assets COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Data ${CMAKE_CURRENT_BINARY_DIR}/Data)

# Includes
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Create project as static library to link against in testing.
add_library(${PROJECT_NAME}_static STATIC ${ALL_FILES} 
	"Window.h" 
	"Engine.h" "Engine.cpp"
	"Events.h" "Events.cpp" 
	"Renderer.h" "Renderer.cpp" 
	"Maths/Vector.h" 
	"Maths/Matrix.h" 
	"Maths/Maths.h" "Maths/PolarCoordinates.h" 
	"Input/InputManager.h"  
	"Input/Action.h" "Input/Action.cpp" "Input/Conditions/Condition.h" "Input/Conditions/PressedCondition.h" "Input/Conditions/ReleasedCondition.h" "Input/Modifiers/DeadZoneModifier.h" "Input/Modifiers/SwizzleModifier.h" "Input/Modifiers/Modifier.h" )
set_target_properties(${PROJECT_NAME}_static PROPERTIES LINKER_LANGUAGE CXX) # Not strictly speaking neccesary. CMake will infer off the types.

# Linking against static library.
target_link_libraries(${PROJECT_NAME}_static PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME}_static PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME}_static PRIVATE GLEW::GLEW)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} "main.cpp")
add_dependencies(${PROJECT_NAME} copy_assets)

# Linking against executable, strictly speaking only what's included in main is neccesary.
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_static)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)