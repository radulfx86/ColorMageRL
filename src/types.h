#ifndef _TYPES_H_
#define _TYPES_H_
#include "gl_headers.h"
#include <string>

class Level;
class Controller;

enum SceneTypes {
    SCENE_TITLE = 0,
    SCENE_LEVEL,
    SCENE_GAMEOVER,
    NUM_SCENES
};

typedef struct Scene2D
{
    std::string name;
    bool running;
    bool paused;
    #ifndef __EMSCRIPTEN__
    SDL_Window *window;
    #endif
    uint64_t last;
    Level *currentLevel;
    Controller *controller;
    uint64_t tick;
} Scene2D;

#define MAX_WIDTH 100

typedef struct Vec2i
{
    int x;
    int y;
    friend bool operator==(const Vec2i &a, const Vec2i &b)
    {
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator<(const Vec2i &a, const Vec2i &b)
    {
        return (a.x * MAX_WIDTH + a.y) < (b.x * MAX_WIDTH + b.y);
        //return a.x * a.x + a.y * a.y < b.x * b.x + b.y * b.y;
    }
} Vec2i;

typedef struct Vec2
{
    float x;
    float y;
 } Vec2;

typedef struct Action
{
    enum ActionType {
        MOTION,
        ATTACK,
        INTERACT,
        SPECIAL,
        NUM_ACTIONS
    } type;
    Vec2i value_i;
    Vec2 value_f;
    bool value_b;
} Action;

#endif // _TYPES_H_