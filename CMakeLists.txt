cmake_minimum_required(VERSION 3.10)

project(TileEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)


# SFML submodule
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})


add_executable(app

    # All source files
    src/main.cpp

)


target_link_libraries(app sfml-graphics sfml-window sfml-system)
target_compile_options(app PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)