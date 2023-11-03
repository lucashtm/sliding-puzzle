cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(SlidingPuzzle)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
# set(RAYLIB_VERSION 4.5.0)
# find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
# if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  # include(FetchContent)
  # FetchContent_Declare(
    # raylib
    # DOWNLOAD_EXTRACT_TIMESTAMP OFF
    # URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  # )
  # FetchContent_GetProperties(raylib)
  # if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    # set(FETCHCONTENT_QUIET NO)
    # FetchContent_Populate(raylib)
    # set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    # add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  # endif()
# endif()
configure_file("fonts/FS Clerkenwell Bold.otf" "FS Clerkenwell Bold.otf" COPYONLY)
configure_file("models/piece.obj" "piece.obj" COPYONLY)
# configure_file("FS Clerkenwell Bold.ttf" "FS Clerkenwell Bold.ttf" COPYONLY)
# configure_file("Montserrat-Medium.otf" "Montserrat-Medium.otf" COPYONLY)
configure_file("words.txt" "words.txt" COPYONLY)

# Our Project

add_subdirectory(src)
