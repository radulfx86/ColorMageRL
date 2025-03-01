set(ColorMage ./main.cpp)

file(GLOB SOURCES
    *.h
    *.cpp
	)

add_executable(ColorMage ${SOURCES} main.cpp)
