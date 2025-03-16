set(ColorMage ./main.cpp)

file(GLOB SOURCES
    ./*.h
    ./*.cpp
    ./scenes/*.h
    ./scenes/*.cpp
    ./ecs/*.h
    ./ecs/*.cpp
	)

IF (WIN32)
add_library(StormEngine STATIC
../glad/glad.c
./ecs/ecs.cpp
./ecs/ecs.h
./tools.cpp
./level.cpp
./io.h
./display.cpp
./object.cpp
./types.h
./tools.h
./log.h
./object.h
./level.h
./shaders.h
./state_machine.h
./object_factory.h
./display.h
./scenes
./scenes/gameover_scene.cpp
./scenes/dummy_scene.h
./scenes/scene_manager.cpp
./scenes/gameover_scene.h
./scenes/title_scene.cpp
./scenes/scene_manager.h
./scenes/title_scene.h
./scenes/dummy_scene.cpp
./io.cpp
./object_factory.cpp
./gl_headers.h
./main.cpp
./shaders.cpp
./controller.h
)

ELSE ()
#add_executable(ColorMage ${SOURCES} main.cpp)

add_library(StormEngine STATIC
./ecs/ecs.cpp
./ecs/ecs.h
./tools.cpp
./level.cpp
./io.h
./display.cpp
./object.cpp
./types.h
./tools.h
./log.h
./object.h
./level.h
./shaders.h
./state_machine.h
./object_factory.h
./display.h
./scenes
./scenes/gameover_scene.cpp
./scenes/dummy_scene.h
./scenes/scene_manager.cpp
./scenes/gameover_scene.h
./scenes/title_scene.cpp
./scenes/scene_manager.h
./scenes/title_scene.h
./scenes/dummy_scene.cpp
./io.cpp
./object_factory.cpp
./gl_headers.h
./main.cpp
./shaders.cpp
./controller.h
)

ENDIF ()

#set_target_properties(StormEngine PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(StormEngine PROPERTIES PUBLIC_HEADER ecs/ecs.h display.h scenes/scene_manager.h)

