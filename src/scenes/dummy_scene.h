#ifndef _DUMMY_SCENE_H_
#define _DUMMY_SCENE_H_
#include "types.h"
#include "object.h"
#include "controller.h"
#include "state_machine.h"
#include "io.h"
#include "shaders.h"

class DummyLevel : public Level
{
public:
    DummyLevel();
    virtual bool update(float delta) override;
    virtual bool draw(float delta) override;
    Object2D *getTarget() { return info; }
private:
    Text2D *info;
    float deltaSum;
    Text2D *info2;
    int frames;
    StateMachineManager statesManager;
    SystemID drawingSystem;
};

class GameController : public Controller
{
public:
    GameController(Object2D *obj) : target(obj)
    {
    }
    virtual void addAction(Action action) override
    {
        switch (action.type)
        {
        case Action::MOTION:
        {
            Vec2 delta{(float)action.value_i.x, (float)action.value_i.y};
            target->setPosition(target->pos + delta);
            break;
        }
        case Action::ATTACK:
            break;
        case Action::INTERACT:
            break;
        case Action::SPECIAL:
            break;
        default:
            break;
        }
    }

private:
    Object2D *target;
};

#endif // _DUMMY_SCENE_H_