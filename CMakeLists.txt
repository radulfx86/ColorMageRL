cmake_minimum_required(VERSION 3.5)
project(ColorMage)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -g")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -fno-omit-frame-pointer -fdiagnostics-show-option -fno-optimize-sibling-calls")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -fno-omit-frame-pointer -fdiagnostics-show-option")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

link_libraries(m)
IF (WIN32)
link_libraries(opengl32)
ELSE ()
link_libraries(GL)
ENDIF ()
link_libraries(SDL2)
link_libraries(SDL2_mixer)
link_libraries(SDL2_image)
include_directories(src
    src/scenes
    src/ecs
    glad
)

add_subdirectory(src)
