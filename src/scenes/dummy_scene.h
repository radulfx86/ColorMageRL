#ifndef _DUMMY_SCENE_H_
#define _DUMMY_SCENE_H_
#include "types.h"
#include "object.h"
#include "controller.h"
#include "state_machine.h"
#include "io.h"
#include "shaders.h"
#include "level.h"

typedef int CameraID;

class Camera : public System
{
public:
    constexpr static Mat4 identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    Mat4 view;
    Mat4 proj;
    Vec2 pos;
    Camera();

    void move(Vec2 pos);

    void zoom(float level);

    Bounds getViewCone();

    virtual void update(float delta) override;

    void setTarget(EntityID target)
    {
        this->target = target;
        center = true;
    }

    void unsetTarget()
    {
        center = false;
    }

    void setCameraID(CameraID cid);
    
private:
    EntityID target;
    bool center;
    CameraID cameraId;
};


class DummyLevel : public Level
{
public:
    DummyLevel();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
    EntityID getTarget() { return player; }
    void handleAction(EntityID eid, Action action);
private:
    EntityID player;
    EntityID background;
    Text2D *info;
    float deltaSum;
    Text2D *info2;
    int frames;
    StateMachineManager statesManager;
    SystemID drawingSystem;
    SystemID actionsSystem;
    Camera camera;
    int activeLevelData;
    LevelData data;
    int numTiles;
};

class GameController : public Controller
{
public:
    GameController(EntityID target) : target(target)
    {
    }
    virtual void addAction(Action action) override
    {
        ActionQueue &queue = EntityManager::getInstance().getComponent<ActionQueue>(target);
        queue.elements.push(action);
    }

private:
    EntityID target;
};

#endif // _DUMMY_SCENE_H_