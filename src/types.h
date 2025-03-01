#ifndef _TYPES_H_
#define _TYPES_H_
#include "gl_headers.h"
#include <string>
#include <array>
#include <vector>

class Level
{
public:
    virtual bool update(float delta) = 0;
    virtual bool draw(float delta) = 0;
    ~Level() {}
    Level() {}
};

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

typedef float Mat4[16];

typedef float Mat2[4];

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

/** graphics */

typedef struct Bounds
{
    Vec2 pos;
    Vec2 size;
} Bounds;

typedef struct TexInfo
{
    float texPos[2];
    float texSize[2];
    bool flip;
    float texOrigin[2];
} TexInfo;

typedef enum
{
    UP = 0,
    RIGHT,
    LEFT,
    DOWN,
    NUM_DIRECTIONS
} Direction_t;

typedef struct Animation {
    std::array<std::vector<TexInfo>,NUM_DIRECTIONS> frames;
    std::vector<float> deltas;
    int currentFrame;
    float currentDelta;
    Direction_t currentDirection;
} Animation;

class Drawable
{
public:
    //Drawable() {}
    //virtual ~Drawable() = default;
    virtual void draw() = 0;
    virtual void setPosition(Vec2 pos) = 0;
    virtual void updateCamera(Mat4 view, Mat4 proj) = 0;
};

class Object2D : public Drawable
{
public:
    GLuint attribPos;
    GLuint vao;
    GLuint vertexBuffer;
    GLuint tex;
    GLuint program;
    GLuint texOffset;
    Animation animation;
    Vec2 pos;
    virtual void updateAnimation(float delta_s);
    virtual void draw() override;
    virtual void setPosition(Vec2 pos) override;
    virtual void updateCamera(float view[16], float proj[16]) override;
    void setAnimation(Direction_t animDir);
};



#endif // _TYPES_H_