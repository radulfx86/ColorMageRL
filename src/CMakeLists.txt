set(ColorMage ./main.cpp)

file(GLOB SOURCES
    ./*.h
    ./*.cpp
    ./scenes/*.h
    ./scenes/*.cpp
    ./ecs/*.h
    ./ecs/*.cpp
	)

add_executable(ColorMage ${SOURCES} main.cpp)
