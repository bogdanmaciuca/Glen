cmake_minimum_required(VERSION 3.30)
project(Glen LANGUAGES C CXX)

# Clangd support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CXX version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SRCS
    src/*.cpp
    vendor/imgui/*.cpp
)

# Third party libraries through git submodules
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)

# Createting the target
add_executable(${PROJECT_NAME} ${SRCS})

# Debug preprocessor definition
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLEN_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

# Compile options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    GL
    glm::glm
)

