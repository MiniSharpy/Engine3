cmake_minimum_required(VERSION 3.12)

include(FetchContent)
FetchContent_Declare(vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg/
    GIT_TAG 2023.11.20
)
FetchContent_MakeAvailable(vcpkg)

# NOTE: This must be defined before the first project call
set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project ("Engine" LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pass debug symbol for Linux. Applies to sub directories.
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_DEBUG")
endif()

add_subdirectory("src")
add_subdirectory("test")