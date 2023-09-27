# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (hello)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)

# Include the wxWidgets use file
include(${wxWidgets_USE_FILE})

add_custom_target(
    format
    COMMAND clang-format
    -style=file
    -i
    ${CMAKE_SOURCE_DIR}/source/*.cpp
    ${CMAKE_SOURCE_DIR}/include/*.h
)


# Create an executable target
add_executable(${PROJECT_NAME} main.cpp)

# Link the wxWidgets libraries to the target
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})