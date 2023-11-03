add_subdirectory(backend)
# add_executable(SlidingPuzzle frontend3d.c)
add_executable(${PROJECT_NAME} frontend3d.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib backend)
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_SOURCE_DIR}/src/backend"
                          )

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
