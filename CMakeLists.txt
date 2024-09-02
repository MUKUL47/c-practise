cmake_minimum_required(VERSION 3.10)
project(MyProject C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Find SDL2_ttf package
find_package(SDL2_ttf REQUIRED)

# Find OpenGL package (needed by ImGui)
find_package(OpenGL REQUIRED)

# Glob the source files from the src directory
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.h")

# Add executable
add_executable(MyProject ${SOURCES})

# Link SDL2, SDL2_ttf, OpenGL, and other needed libraries to your executable
target_link_libraries(MyProject PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf OpenGL::GL)
