#ifndef _TYPES_H_
#define _TYPES_H_
#include "gl_headers.h"
#include <string>
#include <array>
#include <vector>
#include <queue>

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

    uint64_t last;
    Level *currentLevel;
    Controller *controller;
    uint64_t tick;
    uint64_t round;
    float bg_color[4];
} Scene2D;

typedef float Mat4[16];

typedef float Mat2[4];

#define MAX_WIDTH 1000

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

template<>
struct std::hash<Vec2i>
{
    std::size_t operator()(const Vec2i &v) const noexcept
    {
        return v.x * MAX_WIDTH + v.y;
    }
};

typedef struct Vec2
{
    float x;
    float y;
    friend Vec2 operator+(const Vec2 &a, const Vec2 &b)
    {
        return Vec2{a.x + b.x, a.y + b.y};
    }
    friend Vec2 operator-(const Vec2 &a, const Vec2 &b)
    {
        return Vec2{a.x - b.x, a.y - b.y};
    }
    friend Vec2 operator*(const Vec2 &a, const float &p)
    {
        return Vec2{a.x * p, a.y * p};
    }
    friend Vec2 operator*(const Vec2 &a, const Vec2 &b)
    {
        return Vec2{a.x * b.x, a.y * b.y};
    }
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

typedef struct ActionQueue
{
    bool skip;
    int maxActions;
    std::queue<Action> elements;
} ActionQueue;

/** graphics */

typedef struct Bounds
{
    Vec2 offset;
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

typedef struct {
    Vec2 speed;
    Vec2 acc;
    bool onGround;
    bool onWall;
} MotionParameters_t;

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



#endif // _TYPES_H_