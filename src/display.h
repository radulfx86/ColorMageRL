#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "types.h"

void mainloop(void *userData);

void move(const Vec2i &dir);

void initGL(const char *title = "");

uint64_t getNow();

void startMainLoop(Scene2D &scene);

void initScene(Scene2D &scene);

void cleanScene(Scene2D &scene);

#endif // _DISPLAY_H_
