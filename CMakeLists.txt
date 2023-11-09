cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(SlidingPuzzle)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
add_subdirectory(raylib raylib)
configure_file("fonts/FS Clerkenwell Bold.otf" "FS Clerkenwell Bold.otf" COPYONLY)
configure_file("models/piece.obj" "piece.obj" COPYONLY)
configure_file("shaders/lighting.vs" "shaders/lighting.vs" COPYONLY)
configure_file("shaders/lighting.fs" "shaders/lighting.fs" COPYONLY)
configure_file("words.txt" "words.txt" COPYONLY)

# Our Project

add_subdirectory(src)
