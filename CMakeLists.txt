cmake_minimum_required(VERSION 3.10)

project(TileEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ERROR_DEPRECATED True)


# Set the build type if it's not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler optimization flags
if(apple OR unix)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -march=native")
  set(OUTPUT_DIR "Release")

elseif(WIN32)
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /GL")
  set(OUTPUT_DIR "")
endif()



# SFML submodule
add_subdirectory(libs/SFML)
include_directories(${SFML_INCLUDE_DIR})

add_executable(app
    # All source files
    src/main.cpp
)

# Enable Link-Time Optimization if supported
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)
if(lto_supported)
  set_property(TARGET app PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(app sfml-graphics sfml-window sfml-system)

set_target_properties(app PROPERTIES
    EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../${OUTPUT_DIR}"
)