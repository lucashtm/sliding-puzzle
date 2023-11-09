cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(SlidingPuzzle)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file("resources/fonts/FS Clerkenwell Bold.otf" "resources/fonts/FS Clerkenwell Bold.otf" COPYONLY)
configure_file("resources/models/piece.obj" "resources/models/piece.obj" COPYONLY)
configure_file("resources/shaders/lighting.vs" "resources/shaders/lighting.vs" COPYONLY)
configure_file("resources/shaders/lighting.fs" "resources/shaders/lighting.fs" COPYONLY)
configure_file("words.txt" "words.txt" COPYONLY)
if (EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ../resources -s TOTAL_MEMORY=67108864 -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(PLATFORM Web CACHE STRING "" FORCE) # force web platform
  # set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
endif ()
# Dependencies
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE) # build shared libraries
add_subdirectory(raylib raylib)

# Our Project

add_subdirectory(src)
